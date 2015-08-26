/*
 * This file is part of artc, the Approximate-Replay Trace Compiler.
 *
 * Copyright (C) 2012, 2013 Zev Weiss <zev@cs.wisc.edu>
 *
 * artc is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 2 of the License, or (at your
 * option) any later version.
 *
 * artc is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with artc.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "system.h"

#ifdef LINUX
#define _GNU_SOURCE 1 /* RUSAGE_THREAD, ... */
#define _XOPEN_SOURCE 500 /* nftw, ... */
#define _BSD_SOURCE /* timersub, ... */
#endif

#include <stdio.h>
#include <unistd.h>
#include <ftw.h>
#include <pthread.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <fcntl.h>
#include <limits.h>
#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/resource.h>

#if defined(LINUX) || defined(OSX)
#include <sys/syscall.h>
#endif

#include "trace.h"
#include "misc.h"
#include "event.h"
#include "fsmodel.h"
#include "replay.h"
#include "artrun.h"

struct artrun_options options = {
	.union_init = 0,
	.lazy_init = 0,

	.constraints = {
		.aio = 1,
		.fdstage = 1,
		.fdseq = 1,
		.file = 1,
		.path = 1,

		.temporalorder = 0,
		.specialfileorder = 0,
	},

	.singlethread = 0,

	.simplepredelay = 0,
	.ignore_cpu_util = 0,
	.use_cpucount = 0,
	.speed_factor = 1.0,

	.debug = {
		.fds = 0,
		.paths = 0,
		.aio = 0,
		.threads = 0,
	},

	.nosyscalls = 0,
	.randomize_file_content = 0,
	.eager_threadspawn = 0,

	.fsync_pt = 1.0,

	.ignore_version_mismatch = 0,
};

extern const char version_string[];

pthread_mutex_t warn_lock = PTHREAD_MUTEX_INITIALIZER;

uid_t uid;
gid_t gid;

nstime_t replay_start;

#define strictmode (options.constraints.aio && options.constraints.fdstage && \
                    options.constraints.fdseq && options.constraints.file && \
                    options.constraints.path)

void warn(const char* fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	xlock(&warn_lock);
	fputs("Warning: ", stderr);
	vfprintf(stderr, fmt, va);
	fputc('\n', stderr);
	xunlock(&warn_lock);
	va_end(va);
}

void debug(const char* pfx, const char* fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	xlock(&warn_lock);
	fprintf(stderr, "<%s>: ", pfx);
	vfprintf(stderr, fmt, va);
	fputc('\n', stderr);
	xunlock(&warn_lock);
	va_end(va);
}

static __attribute__((format(printf, 1, 2))) noreturn void diehard(const char* fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	fputs("Internal error: ", stderr);
	vfprintf(stderr, fmt, va);
	fputc('\n', stderr);
	va_end(va);

	abort();
}

static __attribute__((format(printf, 1, 2))) noreturn void die(const char* fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	fputs("Error: ", stderr);
	vfprintf(stderr, fmt, va);
	fputc('\n', stderr);
	va_end(va);

	exit(1);
}


/* Minimum number of nanoseconds we bother predelaying for */
#define DELAY_FUDGE_NANOS 100

static inline void xnanosleep(nstime_t nanos)
{
	int status;
	struct timespec ts = {
		.tv_sec = nanos / BILLION,
		.tv_nsec = nanos % BILLION,
	};

	do {
		status = nanosleep(&ts, NULL);
		if (status && status != EINTR)
			diehard("nanosleep failed??");
	} while (status == EINTR);
}

static inline nstime_t get_predelay(const struct depdelays* d, float cpu_util)
{
	nstime_t delay = -1;

	if (options.simplepredelay)
		delay = d->thread < 0 ? 0 : d->thread;
	else {
#define CHECK(cond, name) if (cond && d->name > 0 && (d->name < delay || delay == -1)) delay = d->name
		CHECK(1, thread);
		CHECK(options.constraints.aio, aiodep);
		CHECK(options.constraints.fdstage, fddep);
		CHECK(options.constraints.fdseq, fdorder);
		CHECK(options.constraints.file, fileorder);
		CHECK(options.constraints.path, pathdep);
#undef CHECK
	}

	delay = delay < 0 ? 0 : delay;

	delay *= options.speed_factor;

	if (options.use_cpucount)
		delay *= bench->cpucount;

	if (!options.ignore_cpu_util)
		delay *= cpu_util;

	return delay;
}

static inline void predelay_sleep(const struct depdelays* d, float cpu_util)
{
	nstime_t nanos = get_predelay(d, cpu_util);

	if (nanos < DELAY_FUDGE_NANOS)
		return;

	xnanosleep(nanos);
}

/* Wait for a resource to become available. */
resstate_t resstatus_waitlive(struct resstatus* r)
{
	resstate_t s;

	if (options.singlethread)
		return RES_AVAIL;

	xlock(&r->lock);
	while (r->state == RES_FUTURE)
		xcondwait(&r->cond, &r->lock);
	s = r->state;
	xunlock(&r->lock);

	assert(s == RES_AVAIL || s == RES_ERR);

	return s;
}

/*
 * Update a resource's status to 's' (presumably not RES_FUTURE).
 */
static void __resstatus_change(struct resstatus* r, resstate_t s)
{
	resstate_t prev;
	prev = r->state;
	r->state = s;

	if ((prev == RES_FUTURE || prev == RES_DEAD)
	    && (s == RES_AVAIL || s == RES_ERR))
		xcondbcast(&r->cond);
	else if ((prev == RES_AVAIL || prev == RES_ERR) && s == RES_DEAD)
		xcondbcast(&r->cond);
	else if (strictmode)
		diehard("invalid resource state transition %d -> %d", prev, s);
}

void resstatus_change(struct resstatus* r, resstate_t s)
{
	xlock(&r->lock);
	__resstatus_change(r, s);
	xunlock(&r->lock);
}

static void* bench_handle;
const struct benchmark* bench;

static void bench_init(const char* libpath)
{
	const char* err;

	bench_handle = dlopen(libpath, RTLD_LAZY);

	if (!bench_handle) {
		fprintf(stderr, "%s\n", dlerror());
		exit(1);
	}

	dlerror();
	bench = (struct benchmark*)dlsym(bench_handle, "benchmark");
	err = dlerror();

	if (err) {
		fprintf(stderr, "%s:\"benchmark\": %s\n", libpath, err);
		exit(1);
	}

	if (strcmp(version_string, bench->version_string)) {
		if (options.ignore_version_mismatch)
			fprintf(stderr, "Warning: %s: version mismatch ignored, "
			        "expect segfaults...\n", libpath);
		else {
			fprintf(stderr, "%s: version mismatch\n", libpath);
			fprintf(stderr, "Please recompile %s with an artc version "
			        "that matches this artrun version\n", libpath);
			exit(1);
		}
	}
}

static nstime_t resstatus_wait_lastevent(struct resstatus* r)
{
	nstime_t before, after;
	unsigned int cur;

	if (options.singlethread)
		return 0;

	before = getnanos();

	xlock(&r->lock);
	while (r->evcount > 1)
		xcondwait(&r->cond, &r->lock);
	xunlock(&r->lock);

	after = getnanos();

	return after - before;
}

static void resstatus_mark_event(struct resstatus* r)
{
	xlock(&r->lock);
	assert(r->evcount > 0);
	r->evcount -= 1;
	if (r->evcount == 1)
		xcondbcast(&r->cond);
	xunlock(&r->lock);
}

nstime_t fd_wait_lastevent(int idx)
{
	if (idx < 0)
		return 0;

	assert(idx < bench->num_fds);

	return resstatus_wait_lastevent(&bench->fds[idx].status);
}

static void wait_for_pathgen(int idx)
{
	assert(idx >= 0);
	resstatus_waitlive(&bench->pathgens[idx].status);
}

static void wait_for_pathgen_list(int* list)
{
	unsigned int i;
	if (!list)
		return;
	for (i = 0; list[i] >= 0; i++)
		wait_for_pathgen(list[i]);
}

static void pathgen_list_wait_lastevent(int* list)
{
	unsigned int i;
	if (!list)
		return;

	for (i = 0; list[i] >= 0; i++) {
		assert(list[i] < bench->num_pathgens);
		resstatus_wait_lastevent(&bench->pathgens[list[i]].status);
	}
}

void assign_fd(int idx, int val, resstate_t status)
{
	if (idx < 0)
		return;

	assert(idx < bench->num_fds);

	xlock(&bench->fds[idx].status.lock);
	bench->fds[idx].fd = val;
	__resstatus_change(&bench->fds[idx].status, status);
	xunlock(&bench->fds[idx].status.lock);
}

static void mark_pathgen_available(int idx)
{
	assert(idx >= 0);
	assert(idx < bench->num_pathgens);

	/*
	 * In the case of successive open(O_CREAT&~O_EXCL) calls, the
	 * pathgen could already be available, in which case we won't
	 * try to (re-)mark it as such.
	 */
	xlock(&bench->pathgens[idx].status.lock);
	if (bench->pathgens[idx].status.state != RES_AVAIL)
		__resstatus_change(&bench->pathgens[idx].status, RES_AVAIL);
	xunlock(&bench->pathgens[idx].status.lock);

	path_debug("pathgen %d available", idx);
}

static void mark_pathgen_list_available(int* list)
{
	unsigned int i;

	if (!list)
		return;

	for (i = 0; list[i] >= 0; i++)
		mark_pathgen_available(list[i]);
}

static void mark_pathgen_dead(int idx)
{
	assert(idx >= 0);
	assert(idx < bench->num_pathgens);
	resstatus_change(&bench->pathgens[idx].status, RES_DEAD);
	path_debug("pathgen %d dead", idx);
}

static void mark_pathgen_list_dead(int* list)
{
	unsigned int i;

	if (!list)
		return;

	for (i = 0; list[i] >= 0; i++)
		mark_pathgen_dead(list[i]);
}

static void mark_fd_use(int idx)
{
	if (idx < 0)
		return;

	assert(idx < bench->num_fds);

	resstatus_mark_event(&bench->fds[idx].status);
}

static void mark_pathgen_use(int idx)
{
	assert(idx >= 0);
	assert(idx < bench->num_pathgens);

	resstatus_mark_event(&bench->pathgens[idx].status);
}

struct thread_report {
	pthread_t pthread;
	int orig_tid;
	uint64_t attempted, successful;
	nstime_t total_time;
	nstime_t syscall_ns, sleep_ns, wait_ns;

#ifdef HAVE_RUSAGE_THREAD
	struct timeval utime, stime;
	long inblock, oublock;
#endif
};

/* The write end of the thread-done-reporting pipe */
static int thread_done_w;
/* And a lock to arbitrate write access to it */
static pthread_mutex_t reportlock = PTHREAD_MUTEX_INITIALIZER;

static void writeall(int fd, const void* buf, size_t len)
{
	ssize_t ret;
	size_t done = 0, todo = len;

	do {
		ret = write(fd, buf + done, todo);
		if (ret < 0) {
			perror("write");
			exit(1);
		} else {
			done += ret;
			todo -= ret;
		}
	} while (todo > 0);
}

static void readall(int fd, void* buf, size_t len)
{
	ssize_t ret;
	ssize_t done = 0, todo = len;

	do {
		ret = read(fd, buf + done, todo);
		if (ret < 0) {
			perror("read");
			exit(1);
		} else {
			done += ret;
			todo -= ret;
		}
	} while (todo > 0);
}

struct tracestat {
	nstime_t systime, sleeptime, waittime;
	uint64_t attempted, succeeded;
};

static noreturn void finalize_thread(struct tracestat* ts,
#ifdef HAVE_RUSAGE_THREAD
                                     struct timeval* utime, struct timeval* stime,
                                     long inblock, long oublock,
#endif
                                     nstime_t elapsed, int orig_tid)
{
	struct thread_report tr;

	tr.pthread = pthread_self();
	tr.successful = ts->succeeded;
	tr.attempted = ts->attempted;
	tr.total_time = elapsed;
	tr.orig_tid = orig_tid;
	tr.syscall_ns = ts->systime;
	tr.sleep_ns = ts->sleeptime;
	tr.wait_ns = ts->waittime;

#ifdef HAVE_RUSAGE_THREAD
	tr.utime = *utime;
	tr.stime = *stime;
	tr.inblock = inblock;
	tr.oublock = oublock;
#endif

	xlock(&reportlock);
	writeall(thread_done_w, &tr, sizeof(tr));
	xunlock(&reportlock);

	pthread_exit(NULL);
}

static int fd_predeps(const struct event* ev, const struct fduse* fd, void* arg)
{
	if (fd->rtidx < 0)
		return 0;

	if (options.constraints.fdstage && fd->provider)
		resstatus_waitlive(&fd->provider->status);

	if (options.constraints.fdseq
	    && !(fd->nonfile && !options.constraints.specialfileorder)
	    && fd->lastuse)
		resstatus_waitlive(&fd->lastuse->status);

	return 0;
}

static void event_predeps(const struct event* ev)
{
	int i;
	const struct event_dep* dep;
	const struct filemd* f;

	event_visit_fds(ev, fd_predeps, NULL);

	if (options.constraints.path) {
		wait_for_pathgen_list(ev->paths.consumed);
		wait_for_pathgen_list(ev->paths.removed);
		pathgen_list_wait_lastevent(ev->paths.removed);
	}

	if (options.constraints.path && ev->paths.negdep)
		resstatus_waitlive(&ev->paths.negdep->status);

	if (options.constraints.file) {
		for (f = ev->files;
		     f < ev->files + 2 && (f->idx != -1 || f->lastuse);
		     f++) {
			if (f->lastuse)
				resstatus_waitlive(&f->lastuse->status);
		}
	}

	if (options.constraints.temporalorder) {
		for (dep = ev->temporal_deps; dep->event; dep++)
			resstatus_waitlive(&dep->event->status);
	}
}

static int fd_postdeps(const struct event* ev, const struct fduse* fd, void* arg)
{
	if (fd->rtidx < 0)
		return 0;

	if (options.constraints.fdstage || options.constraints.fdseq)
		mark_fd_use(fd->rtidx);

	return 0;
}

static void event_postdeps(struct event* ev)
{
	int i;

	if (!event_unordered(ev)) {
		event_visit_fds(ev, fd_postdeps, NULL);

		if (options.constraints.path && ev->paths.consumed) {
			for (i = 0; ev->paths.consumed[i] >= 0; i++)
				mark_pathgen_use(ev->paths.consumed[i]);
		}

		if (options.constraints.path) {
			if (ev->paths.produced >= 0)
				mark_pathgen_list_available(ev->paths.produced);
			if (ev->paths.removed >= 0)
				mark_pathgen_list_dead(ev->paths.removed);
		}

		if (options.constraints.file) {
			for (i = 0; i < 2 && ev->files[i].idx != -1; i++)
				resstatus_mark_event(&bench->files[ev->files[i].idx]);
		}
	}

	resstatus_change(&ev->status, RES_AVAIL);
}

#if defined(LINUX)
#define GETTID_SYSCALL SYS_gettid
#elif defined(OSX)
#define GETTID_SYSCALL SYS_thread_selfid
#endif

static void play_trace(struct thread* t, struct tracestat* ts)
{
	struct event* e;
	int ret;
	nstime_t before, after, remaining, systime;
	nstime_t totsystime = 0, totsleeptime = 0, totwaittime = 0;
	uint64_t succeeded = 0, attempted = 0;

#ifdef GETTID_SYSCALL
	thread_debug("trace tid %d replaying on tid %ld", t->tid,
	             (long)syscall(GETTID_SYSCALL));
#endif

	for (e = t->evtable; e->type != EV__THREADEXIT; e++) {
		if (e->type == EV__THREADCONT)
			e = e->attrs.threadcont.next;

		before = getnanos();
		predelay_sleep(&e->predelays, e->cpu_util);
		after = getnanos();
		totsleeptime += after - before;

		before = getnanos();
		if (!event_unordered(e))
			event_predeps(e);
		after = getnanos();
		totwaittime += after - before;

		if (options.nosyscalls) {
			if (e->type == EV__THREADSPAWN)
				ret = replay_fns[e->type](e);
			else
				ret = 0;

			if (event_returns_fd(e) && e->ret.i >= 0)
				assign_fd(e->ret.i, 0, RES_AVAIL);

			if (e->type == EV_SOCKETPAIR && e->ret.i == 0) {
				if (e->attrs.socketpair.fd1.rtidx >= 0)
					assign_fd(e->attrs.socketpair.fd1.rtidx, 0, RES_AVAIL);
				if (e->attrs.socketpair.fd2.rtidx >= 0)
					assign_fd(e->attrs.socketpair.fd2.rtidx, 0, RES_AVAIL);
			}

			if (e->type == EV_CLOSE)
				assign_fd(e->attrs.close.fd.rtidx, -1, RES_DEAD);
			else if (e->type == EV_DUP2 && e->attrs.dup2.new.rtidx >= 0)
				assign_fd(e->attrs.dup2.new.rtidx, -1, RES_DEAD);

			after = before = getnanos();
			systime = 0;
		} else if (replay_fns[e->type]) {
			before = getnanos();
			ret = replay_fns[e->type](e);

			after = getnanos();
			systime = after - before;
		} else
			diehard("No playback function for event type %d", e->type);

		event_postdeps(e);

		totsystime += systime;

		attempted += 1;
		succeeded += !ret;
		e->time.entry = before - replay_start;
		e->time.exit = after - replay_start;
	}

	if (ts) {
		ts->systime = totsystime;
		ts->sleeptime = totsleeptime;
		ts->waittime = totwaittime;
		ts->succeeded = succeeded;
		ts->attempted = attempted;
	}

	thread_debug("trace thread %d complete", t->tid);
}

noreturn void* threadfunc(void* args)
{
	struct thread* t;
	nstime_t start, end;
	struct tracestat ts;
#ifdef HAVE_RUSAGE_THREAD
	struct rusage pre, post;
	long iblk, oblk;
	struct timeval utime, stime;

	if (getrusage(RUSAGE_THREAD, &pre))
		diehard("getrusage() failed: %s", strerror(errno));
#endif

	t = args;

	start = getnanos();
	play_trace(t, &ts);
	end = getnanos();

#ifdef HAVE_RUSAGE_THREAD
	if (getrusage(RUSAGE_THREAD, &post))
		diehard("getrusage() failed: %s", strerror(errno));
	timersub(&post.ru_utime, &pre.ru_utime, &utime);
	timersub(&post.ru_stime, &pre.ru_stime, &stime);
	iblk = post.ru_inblock - pre.ru_inblock;
	oblk = post.ru_oublock - pre.ru_oublock;
#endif

	finalize_thread(&ts,
#ifdef HAVE_RUSAGE_THREAD
	                &utime, &stime, iblk, oblk,
#endif
	                end - start, t->tid);
}

/*
 * Spawn all threads in the benchmark.  This causes the EV__THREADSPAWN events
 * used when not in eager_threadspawn mode to become noops.  With all
 * dependencies enforced, this should still result in an equally-correct replay.
 */
static void spawn_all_bench_threads(void)
{
	int i, status;
	pthread_t dummy;

	for (i = bench->num_threads-1; i >= 0; i--) {
		status = pthread_create(&dummy, NULL, threadfunc, (void*)bench->threads[i]);
		if (status)
			die("pthread_create: %s", strerror(status));
	}
}

/*
 * Returns the total number of events on which playback was (or should have
 * been) attempted, writes the number that did so successfully into
 * *successful.  Writes each thread_report (in the order they arrive) into the
 * array 'trs'.
 */
static uint64_t run(uint64_t* successful, struct thread_report* trs)
{
	int i, status, threadpipe[2];
	pthread_t dummy;
	uint64_t total;

	if (pipe(threadpipe)) {
		perror("pipe");
		exit(1);
	}
	thread_done_w = threadpipe[1];

	replay_start = getnanos();

	if (options.eager_threadspawn)
		spawn_all_bench_threads();
	else {
		status = pthread_create(&dummy, NULL, threadfunc, bench->first_thread);
		if (status)
			die("pthread_create: %s", strerror(status));
	}

	*successful = total = 0;

	for (i = 0; i < bench->num_threads; i++) {
		readall(threadpipe[0], &trs[i], sizeof(*trs));
		status = pthread_join(trs[i].pthread, NULL);
		if (status)
			die("pthread_join: %s", strerror(status));
		total += trs[i].attempted;
		*successful += trs[i].successful;
	}

	return total;
}

/*
 * Same as above, but runs through all thread traces with a single
 * playback thread, effectively merge-sorting them on the fly.
 */
static uint64_t run_singlethread(uint64_t* successful)
{
	int i, ret, tidx;
	struct event* e;
	const int* fdptr;
	nstime_t before, after;
	uint64_t total = 0;
	struct event** tptrs = xcalloc(bench->num_threads * sizeof(*tptrs));

	*successful = 0;

	for (i = 0; i < bench->num_threads; i++)
		tptrs[i] = bench->threads[i]->evtable;

	for (;;) {
		e = NULL;
		tidx = -1;

		/* scan all threads for the one with the lowest-numbered event next */
		for (i = 0; i < bench->num_threads; i++) {
			if (tptrs[i]->type != EV__THREADEXIT
			    && (!e || tptrs[i]->serial < e->serial)) {
				e = tptrs[i];
				tidx = i;
			}
		}

		/* If we haven't found one, we're done. */
		if (!e)
			break;

		tptrs[tidx] += 1;

		switch (e->type) {
		case EV__THREADEXIT:
			/* Should never happen. */
			abort();

		case EV__THREADCONT:
			tptrs[tidx] = e->attrs.threadcont.next;
			break;

		case EV__THREADSPAWN:
			/*
			 * threadspawns are freebies in single-thread
			 * mode...not so much "successful" as "not failed".
			 */
			*successful += 1;
			++total;
			break;

		default:
			if (options.nosyscalls) {
				if (event_returns_fd(e) && e->ret.i >= 0)
					assign_fd(e->ret.i, 0, RES_AVAIL);

				if (e->type == EV_SOCKETPAIR && e->ret.i == 0) {
					if (e->attrs.socketpair.fd1.rtidx >= 0)
						assign_fd(e->attrs.socketpair.fd1.rtidx,
						          0, RES_AVAIL);
					if (e->attrs.socketpair.fd2.rtidx >= 0)
						assign_fd(e->attrs.socketpair.fd2.rtidx,
						          0, RES_AVAIL);
				}

				if (e->type == EV_CLOSE)
					assign_fd(e->attrs.close.fd.rtidx, -1, RES_DEAD);
				else if (e->type == EV_DUP2 && e->attrs.dup2.new.rtidx >= 0)
					assign_fd(e->attrs.dup2.new.rtidx, -1, RES_DEAD);

				ret = 0;
				after = before = getnanos();
			} else if (replay_fns[e->type]) {
				before = getnanos();
				ret = replay_fns[e->type](e);
				after = getnanos();
			} else
				diehard("No playback function for event type %d", e->type);

			*successful += !ret;
			e->time.entry = before - replay_start;
			e->time.exit = after - replay_start;
			++total;

			break;
		}
	}

	xfree(tptrs);

	return total;
}

char filler[8192];

static void randomize_filler(void)
{
	int* ip = (int*)&filler;

	assert(sizeof(filler) % sizeof(int) == 0);

	while (ip < (int*)(filler + sizeof(filler)))
		*ip++ = rand();
}

static void size_file(int fd, off_t size)
{
	struct stat st;
	off_t todo;
	size_t towrite;

	if (!options.lazy_init || options.union_init) {
		if (fstat(fd, &st)) {
			perror("fstat");
			exit(1);
		}
	}

	if ((options.lazy_init && options.union_init && size > st.st_size)
	    || (!options.union_init && st.st_size > size)) {
		if (ftruncate(fd, size)) {
			perror("ftruncate");
			exit(1);
		} else
			return;
	}

	if (lseek(fd, st.st_size, SEEK_SET) != st.st_size) {
		perror("lssek");
		exit(1);
	}

	todo = size - st.st_size;
	while (todo > 0) {
		if (options.randomize_file_content)
			randomize_filler();
		towrite = todo > sizeof(filler) ? sizeof(filler) : todo;
		writeall(fd, filler, towrite);
		todo -= towrite;
	}
}

static void pginit_status(struct pathgen* pg)
{
	pg->status.state = RES_AVAIL;
}

/*
 * Recurse through the subtree at '.', deleting and resizing things to match
 * what the state of the tree should be.
 */
static int phase1_visitfn(const char* path, const struct stat* st, int type, struct FTW* info)
{
	struct initpath* ip;
	int fd, will_remove;
	char targbuf[PATH_MAX];
	ssize_t targlen;

	if (!strcmp(path, "."))
		return 0;

	if (path[0] != '.' || path[1] != '/')
		die("unexpected path from nftw(): %s", path);

	/* skip over the initial "./" */
	const char* relpath = path + 2;
	size_t rellen = strlen(relpath);

	switch (type) {
	case FTW_F:
		/*
		 * FIXME: non-lazy init should (maybe?) check for and fix
		 * sparse files
		 */
		ip = bench->get_initpath(relpath, rellen);

		if (!ip)
			will_remove = !options.union_init;
		else if (bench->pathgens[ip->idx].info.type != FT_FILE) {
			will_remove = 1;
			if (options.union_init)
				fprintf(stderr, "Warning: %s is a regular file but "
				        "should not be, removing despite -U\n", relpath);
		} else
			will_remove = 0;

		if (will_remove && unlink(path))
			die("unlink(%s): %s", relpath, strerror(errno));
		else if (ip && st->st_size != bench->pathgens[ip->idx].info.size) {
			fd = open(path, O_WRONLY);
			if (fd < 0)
				die("open(%s): %s", relpath, strerror(errno));
			size_file(fd, bench->pathgens[ip->idx].info.size);
			close(fd);
		} else if (ip)
			pginit_status(&bench->pathgens[ip->idx]);
		break;

	case FTW_D:
		/*
		 * Only check directories on the post-order visit.  This way a
		 * directory that shouldn't be present will already have had
		 * all its children unlinked, so it will be empty and can be
		 * safely rmdir()ed.
		 */
		break;

	case FTW_DP:
		ip = bench->get_initpath(relpath, rellen);

		if (!ip)
			will_remove = !options.union_init;
		else if (bench->pathgens[ip->idx].info.type != FT_DIR) {
			will_remove = 1;
			if (options.union_init)
				fprintf(stderr, "Warning: %s is a directory but should"
				        " not be, removing despite -U\n", relpath);
		} else
			will_remove = 0;

		if (will_remove && rmdir(path))
			die("rmdir(%s): %s%s", relpath, strerror(errno),
			    (options.union_init && errno == ENOTEMPTY) ?
			    "\n(-U may not be possible with this tree.)" : "");
		else if (ip)
			pginit_status(&bench->pathgens[ip->idx]);
		break;

	case FTW_SL:
	case FTW_SLN:
		ip = bench->get_initpath(relpath, rellen);

		if (!ip)
			will_remove = !options.union_init;
		else if (bench->pathgens[ip->idx].info.type != FT_SYMLINK) {
			will_remove = 1;
			if (options.union_init)
				fprintf(stderr, "Warning: %s is a symlink but should "
				        "not be, removing despite -U\n", relpath);
		} else
			will_remove = 0;

		if (ip && !will_remove) {
			targlen = readlink(path, targbuf, sizeof(targbuf));
			if (targlen < 0)
				die("readlink(%s): %s", relpath, strerror(errno));
			will_remove = !!strncmp(bench->pathgens[ip->idx].info.target,
			                        targbuf, targlen);
			if (will_remove && options.union_init)
				fprintf(stderr, "Warning: %s is a symlink but has the"
				        " wrong target, redirecting despite -U\n",
				        relpath);
		}

		if (will_remove) {
			if (unlink(path))
				die("unlink(%s): %s", relpath, strerror(errno));
		} else if (ip)
			pginit_status(&bench->pathgens[ip->idx]);
		break;

	case FTW_DNR:
		die("unreadable directory: %s", path);

	case FTW_NS:
		die("stat(\"%s\") failed", path);

	default:
		diehard("bogus type from nftw()??");
	}

	return 0;
}

static void prep_phase1(void)
{
	if (nftw(".", phase1_visitfn, 128, FTW_DEPTH|FTW_PHYS))
		die("error: prep_phase1 nftw() failed\n");
}

static void prep_path(const struct initpath* ip, void* again)
{
	int fd, status;
	struct stat st;
	struct pathgen* pg = &bench->pathgens[ip->idx];
	const char* path = ip->path;

	if (pg->status.state == RES_AVAIL)
		return;

	switch (pg->info.type) {
	case FT_FILE:
		fd = open(path, O_WRONLY|O_CREAT, 0644);
		if (fd < 0) {
			if (errno == ENOENT)
				*(int*)again = 1;
			else {
				perror(path);
				exit(1);
			}
			/* Might just have not created parent dirs yet */
		} else {
			/* Initialize it, mark it as such, and close the fd */
			size_file(fd, pg->info.size);
			pginit_status(pg);
			close(fd);
		}
		break;

	case FT_DIR:
		status = mkdir(path, 0700);
		if (status && errno == EEXIST) {
			if (stat(path, &st)) {
				perror("%s");
				exit(1);
			}
			if (!S_ISDIR(st.st_mode))
				diehard("prep_phase1 misbehaved: !S_ISDIR(%s)", path);
			else
				pginit_status(pg);
		} else if (status) {
			if (errno == ENOENT)
				*(int*)again = 1;
			else {
				perror(path);
				exit(1);
			}
			/* Might just have not created parent dirs yet */
		} else
			pginit_status(pg);
		break;

	case FT_SYMLINK:
		status = symlink(pg->info.target, path);
		if (status) {
			if (errno == ENOENT)
				*(int*)again = 1;
			else {
				perror(path);
				exit(1);
			}
			/* Might just have not created parent dirs yet */
		} else
			pginit_status(pg);
		break;

	default:
		abort();
	}
}

/*
 * Iterate over init entries that weren't seen in phase1, instantiating them
 * as required.
 */
static void prep_phase2(void)
{
	int again, count = 0;

	do {
		again = 0;
		bench->visit_initpaths(prep_path, &again);
		count += 1;
	} while (again);

	fprintf(stderr, "init inefficiency rating: %d\n", count);
}

static void dummy_prep_path(const struct initpath* ip, void* arg)
{
	pginit_status(&bench->pathgens[ip->idx]);
}

static void xchdir(const char* path)
{
	if (chdir(path)) {
		fprintf(stderr, "chdir(%s): %s\n", path, strerror(errno));
		exit(1);
	}
}

#define ROOTSUBDIR "bench_root"

static void prep_root(const char* path, int noprep)
{
	int ret, cwdfd;
	struct stat st;

	if ((cwdfd = open(".", O_RDONLY)) < 0)
		die("open(\".\"): %s", strerror(errno));

	xchdir(path);

	ret = stat(ROOTSUBDIR, &st);
	if (ret && errno == ENOENT) {
		if (mkdir(ROOTSUBDIR, 0755))
			die("mkdir(%s): %s", path, strerror(errno));
	} else if (ret)
		die("stat(%s): %s", path, strerror(errno));
	else if (!S_ISDIR(st.st_mode))
		die("%s exists but is not a directory", path);

	xchdir(ROOTSUBDIR);

	if (!noprep) {
		memset(filler, '.', sizeof(filler));

		prep_phase1();
		prep_phase2();
	} else
		bench->visit_initpaths(dummy_prep_path, NULL);

	if (fchdir(cwdfd)) {
		perror("fchdir");
		exit(1);
	}

	if (close(cwdfd))
		perror("close");
}

static void clear_cache(const char* root)
{
	sync();

#if defined(OSX)
	/* Who knows how this works. */
	system("/usr/bin/purge");
#elif defined(LINUX)
	int fd, status;
	pid_t pid;

	if ((pid = fork()) >= 0) {
		if (!pid) {
			execlp("clearcache", "clearcache", root, NULL);
			perror("exec(\"clearcache\")");
			exit(1);
		} else if (waitpid(pid, &status, 0) == pid && !WEXITSTATUS(status))
			return;
	}

	warn("'clearcache %s' exited %d\n", root, WEXITSTATUS(status));

	/*
	 * Otherwise make a vain attempt at doing it ourselves...vain, that
	 * is, unless someone is foolish enough to run this as root.
	 */
#define CACHEDROP "/proc/sys/vm/drop_caches"
	fd = open(CACHEDROP, O_WRONLY);
	if (fd < 0) {
		warn("insufficient permissions on "CACHEDROP", pagecache not cleared");
	} else {
		if (write(fd, "3", 1) != 1)
			warn("writing to "CACHEDROP" failed, pagecache not cleared");
		close(fd);
	}
#undef CACHEDROP
#else
	warn("don't know how to drop caches on this system");
#endif
}

struct io_stats {
#ifdef LINUX
	int64_t rchar, wchar;
	int64_t syscr, syscw;
	int64_t read_bytes, write_bytes, cancelled_write_bytes;
#endif
};

static void get_io_stats(struct io_stats* s)
{
#ifdef LINUX
	int n;
	FILE* f = fopen("/proc/self/io", "r");

	memset(s, 0, sizeof(*s));

	if (!f) {
		warn("failed to open /proc/self/io -- kernel too old?");
		return;
	}

	n = fscanf(f, "rchar: %"PRId64"\n"
	           "wchar: %"PRId64"\n"
	           "syscr: %"PRId64"\n"
	           "syscw: %"PRId64"\n"
	           "read_bytes: %"PRId64"\n"
	           "write_bytes: %"PRId64"\n"
	           "cancelled_write_bytes: %"PRIu64"\n",
	           &s->rchar, &s->wchar, &s->syscr, &s->syscw, &s->read_bytes, &s->write_bytes,
	           &s->cancelled_write_bytes);

	if (n != 7)
		warn("only got %d items from /proc/self/io", n);

	fclose(f);
#endif
}

static void io_stats_sub(const struct io_stats* a, const struct io_stats* b, struct io_stats* res)
{
#ifdef LINUX
	res->rchar = a->rchar - b->rchar;
	res->wchar = a->wchar - b->wchar;
	res->syscr = a->syscr - b->syscr;
	res->syscw = a->syscw - b->syscw;
	res->read_bytes = a->read_bytes - b->read_bytes;
	res->write_bytes = a->write_bytes - b->write_bytes;
	res->cancelled_write_bytes = a->cancelled_write_bytes - b->cancelled_write_bytes;
#endif
}

static void print_io_stats(FILE* out, const char* pfx, const struct io_stats* s)
{
#ifdef LINUX
	fprintf(out, "%s_rchar: %"PRId64"\n", pfx, s->rchar);
	fprintf(out, "%s_wchar: %"PRId64"\n", pfx, s->wchar);
	fprintf(out, "%s_syscr: %"PRId64"\n", pfx, s->syscr);
	fprintf(out, "%s_syscw: %"PRId64"\n", pfx, s->syscw);
	fprintf(out, "%s_read_bytes: %"PRId64"\n", pfx, s->read_bytes);
	fprintf(out, "%s_write_bytes: %"PRId64"\n", pfx, s->write_bytes);
	fprintf(out, "%s_cancelled_write_bytes: %"PRId64"\n", pfx, s->cancelled_write_bytes);
#endif
}

static void seed_rand(int use_provided, long int s)
{
	int fd;
	long int seed;

	if (use_provided)
		seed = s;
	else {
		fd = open("/dev/urandom", O_RDONLY);
		if (fd < 0) {
			perror("open(/dev/urandom)");
			exit(1);
		}
		readall(fd, &seed, sizeof(seed));
		close(fd);
	}

	printf("prng_seed: %ld\n", seed);
	srand48(seed);
	srand(seed);
}

static void init_locks(void)
{
	int i, status;
	struct aio_info* ai;
	pthread_mutexattr_t lockattrs;

	status = pthread_mutexattr_init(&lockattrs);
	if (status)
		die("pthread_mutexattr_init: %s", strerror(status));

	status = pthread_mutexattr_settype(&lockattrs, PTHREAD_MUTEX_ERRORCHECK);
	if (status)
		die("pthread_mutexattr_settype: %s", strerror(status));

	for (i = 0; i < bench->num_aiocbs; i++) {
		ai = get_aio_info(i);
		status = pthread_mutex_init(&ai->status.lock, &lockattrs);
		if (status)
			die("pthread_mutex_init: %s", strerror(status));
	}

	status = pthread_mutexattr_destroy(&lockattrs);
	if (status)
		die("pthread_mutexattr_destroy: %s", strerror(status));
}

static void init_files(void)
{
	int i;

	for (i = 0; i < bench->num_files; i++)
		bench->files[i] = (struct resstatus) RESSTATUS_INIT(RES_AVAIL, INT_MAX);
}

static void init_mystery_fds(void)
{

	int i, copy, dummy_fd = open("/dev/zero", O_RDWR);

	if (dummy_fd < 0) {
		warn("failed to open /dev/zero read-write");
		exit(1);
	}

	for (i = 0; i < bench->num_mystery_fds; i++) {
		copy = dup(dummy_fd);
		if (copy < 0) {
			perror("dup");
			exit(1);
		}
		assign_fd(bench->mystery_fds[i], copy, RES_AVAIL);
	}

	if (close(dummy_fd)) {
		perror("close");
		exit(1);
	}
}

/*
 * The number of outstanding events we request that our mystery AIO contexts
 * be able to handle.  This is a pretty arbitrarily selected number.
 */
#define AIO_NR_EVENTS 256

static void init_mystery_aioctxs(void)
{
#ifdef LINUX
	int i, status;

	for (i = 0; i < bench->num_mystery_aioctxs; i++) {
		status = io_setup(AIO_NR_EVENTS, &bench->aioctxs[bench->mystery_aioctxs[i]]);
		if (status) {
			fprintf(stderr, "io_setup: %s\n", strerror(-status));
			exit(1);
		}
	}
#endif
}

static void destroy_remaining_aioctxs(void)
{
#ifdef LINUX
	int i, status;

	for (i = 0; i < bench->num_aioctxs; i++) {
		if (!bench->aioctxs[i])
			continue;

		/* We'll just reuse our convenient wrapper here... */
		status = io_destroy(bench->aioctxs[i]);
		if (status) {
			status = -status;
			perror("io_destroy");
		}
	}
#endif
}

static void print_aiostats(void)
{
	int i;
	struct aiotimes* t;

	printf("aio_latencies:");
	for (i = 0; i < bench->num_aiocbs; i++) {
		t = &get_aio_info(i)->times;

		/*
		 * Output format for each AIO request:
		 *   SUB:CHECK:RET
		 *
		 * All are elapsed times in nanoseconds between the time at
		 * which we entered io_submit() to the time of another event.
		 * Those events are:
		 *
		 * SUB: io_submit() return
		 * CHECK: entering io_getevents() (may be negative)
		 * RET: io_getenvents() return
		 */
		printf(" %"PRId64":%"PRId64":%"PRId64, t->subdone - t->sub,
		       t->check - t->sub, t->ret - t->sub);
	}
	printf("\n");
}

static void print_thread_log(const struct thread* t)
{
	const struct event* e;

	printf("thread_log(%d):", t->tid);

	for (e = t->evtable; e->type != EV__THREADEXIT; e++) {
		switch (e->type) {
		case EV__THREADCONT:
			e = e->attrs.threadcont.next;
			break;

		case EV__THREADSPAWN:
			if (options.eager_threadspawn)
				continue;
			break;

		default:
			break;
		}

		printf(" %s:%"PRInst":%"PRInst, event_type_name(e->type),
		       e->time.entry, e->time.exit - e->time.entry);
	}

	printf("\n");
}

/* Ordering constraint options */
enum {
	OC_ALL = 0,
	OC_AIO,
	OC_FDSTAGE,
	OC_FDSEQ,
	OC_FILE,
	OC_PATH,
	OC_TEMPORAL,
	OC_SPFDSEQ,
};

static char* const constraint_opts[] = {
	[OC_ALL]      = "all",
	[OC_AIO]      = "aio",
	[OC_FDSTAGE]  = "fdstage",
	[OC_FDSEQ]    = "fdseq",
	[OC_FILE]     = "file",
	[OC_PATH]     = "path",
	[OC_TEMPORAL] = "temporal",
	[OC_SPFDSEQ]  = "specialfdseq",
	NULL,
};

/* Debug options */
enum {
	DEBUG_FDS = 0,
	DEBUG_PATHS,
	DEBUG_AIO,
	DEBUG_THREADS,
};

static char* const debug_opts[] = {
	[DEBUG_FDS]     = "fds",
	[DEBUG_PATHS]   = "paths",
	[DEBUG_AIO]     = "aio",
	[DEBUG_THREADS] = "threads",
	NULL,
};

static void usage(FILE* f, int status)
{
	int i;

	fprintf(f, "Usage: artrun [FLAGS] BENCH.SO RUNDIR\n"
	        "FLAGS may include:\n"
	        "\t-C, --ignore-cpu: ignore CPU utilization in predelay\n"
	        "\t-c, --predelay-multiplier=N: multiply timing delays by N\n"
	        "\t-d, --debug=THINGS: enable debugging of (comma-separated list):\n");

	for (i = 0; debug_opts[i]; i++)
		fprintf(f, "\t\t%s\n", debug_opts[i]);

	fprintf(f,
	        "\t-E, --eager-threadspawn: eager thread-spawning (ignore explicit threadspawn events)\n"
	        "\t-F, --fsync-point=P: set fsync point to P (see README)\n"
	        "\t-k, --disable-constraints=CONSTRAINTS: disable enforcement for CONSTRAINTS\n"
	        "\t-K, --enable-constraints=CONSTRAINTS: enable enforcement for CONSTRAINTS\n"
	        "\t\tCONSTRAINTS is comma-separated list of:\n");

	for (i = 0; constraint_opts[i]; i++)
		fprintf(f, "\t\t\t%s\n", constraint_opts[i]);

	fprintf(f,
	        "\t-h, --help: display this help message\n"
	        "\t-I, --ignore-version-mismatch: ignore version mismatch (bad idea, expect segfaults)\n"
	        "\t-M, --predelay-multi-cpu: multiply predelays by trace-time CPU count\n"
	        "\t-n, --no-clear-cache: don't attempt to clear cache before replay\n"
	        "\t-p, --prep-only: just prepare the filesystem tree, don't replay benchmark\n"
	        "\t-P, --skip-prep: skip tree preparation, assume it's been done\n"
	        "\t-q, --skip-syscalls: skip actually making replay system calls\n"
	        "\t-R, --use-urandom: seed PRNG from /dev/urandom\n"
	        "\t-r, --prng-seed=SEED: seed PRNG with SEED\n"
	        "\t-S, --simple-predelay: use simple (thread-ordering-only) predelay model\n"
	        "\t-s, --single-thread: replay with single thread (implies -c 0)\n"
	        "\t-U, --union-init: union init mode (don't delete or shrink existing files)\n"
	        "\t-w, --randomize-init-data: randomize data written to files\n"
	        "\t-z, --lazy-init: lazy init (AVOID)\n");
	exit(status);
}

static const struct option long_opts[] = {
	{ "ignore-cpu",              no_argument,       NULL, 'C', },
	{ "predelay-multiplier",     required_argument, NULL, 'c', },
	{ "debug",                   required_argument, NULL, 'd', },
	{ "eager-threadspawn",       no_argument,       NULL, 'E', },
	{ "fsync-point",             required_argument, NULL, 'F', },
	{ "disable-constraints",     required_argument, NULL, 'k', },
	{ "enable-constraints",      required_argument, NULL, 'K', },
	{ "help",                    no_argument,       NULL, 'h', },
	{ "ignore-version-mismatch", no_argument,       NULL, 'I', },
	{ "predelay-multi-cpu",      no_argument,       NULL, 'M', },
	{ "no-clear-cache",          no_argument,       NULL, 'n', },
	{ "prep-only",               no_argument,       NULL, 'p', },
	{ "skip-prep",               no_argument,       NULL, 'P', },
	{ "skip-syscalls",           no_argument,       NULL, 'q', },
	{ "use-urandom",             no_argument,       NULL, 'R', },
	{ "prng-seed",               required_argument, NULL, 'r', },
	{ "simple-predelay",         no_argument,       NULL, 'S', },
	{ "single-thread",           no_argument,       NULL, 's', },
	{ "union-init",              no_argument,       NULL, 'U', },
	{ "randomize-init-data",     no_argument,       NULL, 'w', },
	{ "lazy-init",               no_argument,       NULL, 'z', },
	{ NULL, 0, NULL, 0, },
};

static void set_constraints(char* optstr, int enabled)
{
	char* value;

	while (*optstr) {
		switch (getsubopt(&optstr, constraint_opts, &value)) {
		case OC_ALL:
			options.constraints.aio = enabled;
			options.constraints.fdstage = enabled;
			options.constraints.fdseq = enabled;
			options.constraints.path = enabled;
			options.constraints.file = enabled;
			options.constraints.temporalorder = enabled;
			options.constraints.specialfileorder = enabled;
			break;

#define OPT_CASE(ENUMNAME, optmember) \
		case OC_##ENUMNAME: \
			options.constraints.optmember = enabled; \
			break

		OPT_CASE(AIO, aio);
		OPT_CASE(FDSTAGE, fdstage);
		OPT_CASE(FDSEQ, fdseq);
		OPT_CASE(FILE, file);
		OPT_CASE(PATH, path);
		OPT_CASE(TEMPORAL, temporalorder);
		OPT_CASE(SPFDSEQ, specialfileorder);
#undef OPT_CASE

		default:
			fprintf(stderr, "Unrecognized constraint type: %s\n", value);
			usage(stderr, 1);
		}
	}
}

static void set_debugflags(char* optstr)
{
	char* value;

	while (*optstr) {
		switch (getsubopt(&optstr, debug_opts, &value)) {
#define OPT_CASE(ENUMNAME, optmember) \
		case DEBUG_##ENUMNAME: \
			options.debug.optmember = 1; \
			break

		OPT_CASE(FDS, fds);
		OPT_CASE(PATHS, paths);
		OPT_CASE(AIO, aio);
		OPT_CASE(THREADS, threads);
#undef OPT_CASE

		default:
			fprintf(stderr, "Unrecognized debug option: %s\n", value);
			usage(stderr, 1);
		}
	}
}

#define WANT_FDS 1024

#ifdef OSX
mach_timebase_info_data_t mach_timebase;
#endif

int main(int argc, char** argv)
{
	int i, opt;
	char* argend;
	nstime_t start, end, elapsed;
	struct rlimit rlim;
	uint64_t total, succeeded;
	struct io_stats preinit_io, postinit_io, prerun_io, postrun_io, init_io, bench_io;
	struct rusage prerusage, postrusage;
	struct timeval utime, stime;
	const char* rootpath = NULL;
	struct thread_report* trs = NULL;
	long int seed = 0;
	int preponly = 0, noprep = 0, noclearcache = 0, randseed = 0;

#ifdef OSX
	kern_return_t kr = mach_timebase_info(&mach_timebase);
	if (kr != KERN_SUCCESS)
		die("mach_timebase_info() failed: %s", mach_error_string(kr));
#endif

	if (setvbuf(stderr, NULL, _IOLBF, 0))
		die("Failed to line-buffer stderr");

	while ((opt = getopt_long(argc, argv, ":Cc:d:EF:hiIk:K:MnpPqRr:Sst:Uwz",
	                          long_opts, NULL)) != -1) {
		switch (opt) {

		case 'C':
			options.ignore_cpu_util = 1;
			break;

		case 'c':
			options.speed_factor = strtod(optarg, &argend);
			if (!*optarg || *argend)
				die("-c: invalid argument '%s'", optarg);
			break;

		case 'd':
			set_debugflags(optarg);
			break;

		case 'E':
			options.eager_threadspawn = 1;
			break;

		case 'F':
			options.fsync_pt = strtod(optarg, &argend);
			if (!*optarg || *argend
			    || options.fsync_pt < 0.0 || options.fsync_pt > FSYNC_MAX)
				die("-F: invalid argument '%s'", optarg);
			break;

		case 'h':
			usage(stdout, 0);
			break;

		case 'I':
			options.ignore_version_mismatch = 1;
			break;

		case 'k':
			set_constraints(optarg, 0);
			break;

		case 'K':
			set_constraints(optarg, 1);
			break;

		case 'M':
			options.use_cpucount = 1;
			break;

		case 'n':
			noclearcache = 1;
			break;

		case 'p':
			preponly = 1;
			break;

		case 'P':
			noprep = 1;
			break;

		case 'q':
			options.nosyscalls = 1;
			break;

		case 'R':
			/* -R: seed PRNG from /dev/urandom */
			randseed = 1;
			break;

		case 'r':
			/* -r VAL: seed PRNG with VAL */
			seed = strtol(optarg, &argend, 0);
			if (!*optarg || *argend)
				die("-r: invalid argument '%s'", optarg);
			randseed = 0;
			break;

		case 'S':
			options.simplepredelay = 1;
			break;

		case 's':
			/*
			 * -s implies -c0, since it's really only intended for
			 * debugging, and the meaningfulness of the actual
			 * timing delays when replaying a multithreaded trace
			 * with a single replay thread is pretty questionable.
			 *
			 * (Though a -cX argument after the -s flag could
			 * override this if you really want the sleeps in
			 * there for some reason.)
			 */
			options.singlethread = 1;
			options.speed_factor = 0;
			break;

		case 'U':
			options.union_init = 1;
			break;

		case 'w':
			options.randomize_file_content = 1;
			break;

		case 'z':
			options.lazy_init = 1;
			break;

		case ':':
			fprintf(stderr, "-%c flag requires an argument\n", optopt);
			usage(stderr, 1);

		default:
			fprintf(stderr, "invalid flag: %c\n", optopt);
			usage(stderr, 1);
			break;
		}
	}

	if (optind == argc - 2) {
		bench_init(argv[optind]);
		rootpath = argv[optind+1];
	} else
		usage(stderr, 1);

	get_io_stats(&preinit_io);
	prep_root(rootpath, noprep);
	get_io_stats(&postinit_io);

	io_stats_sub(&postinit_io, &preinit_io, &init_io);

	if (preponly)
		exit(0);

	/*
	 * Try to ensure we don't run out of file descriptors (OSX
	 * apparently defaults to 256, which is kind of wimpy).
	 */
	if (getrlimit(RLIMIT_NOFILE, &rlim)) {
		warn("failed to determine RLIMIT_NOFILE, pressing on...");
	} else if (rlim.rlim_cur < WANT_FDS) {
		if (rlim.rlim_max < WANT_FDS) {
			warn("RLIMIT_NOFILE lower than preferred, pressing on...");
			rlim.rlim_cur = rlim.rlim_max;
		} else
			rlim.rlim_cur = WANT_FDS;
		if (setrlimit(RLIMIT_NOFILE, &rlim))
			warn("failed to adjust RLIMIT_NOFILE, pressing on...");
	}

	init_locks();
	init_files();

	init_mystery_fds();
	init_mystery_aioctxs();

	if (!noclearcache)
		clear_cache(rootpath);

	seed_rand(!randseed, seed);

	gid = getgid();
	uid = getuid();

	printf("command_line:");
	for (i = 0; i < argc; i++)
		printf(" %s", argv[i]);
	printf("\n");

	xchdir(rootpath);
	xchdir(ROOTSUBDIR);

	if (bench->setup)
		play_trace(bench->setup, NULL);

	get_io_stats(&prerun_io);
	if (getrusage(RUSAGE_SELF, &prerusage))
		diehard("getrusage() failed: %s", strerror(errno));
	start = getnanos();
	if (options.singlethread)
		total = run_singlethread(&succeeded);
	else {
		trs = xcalloc(bench->num_threads * sizeof(*trs));
		total = run(&succeeded, trs);
	}
	end = getnanos();
	if (getrusage(RUSAGE_SELF, &postrusage))
		diehard("getrusage() failed: %s", strerror(errno));
	get_io_stats(&postrun_io);

	io_stats_sub(&postrun_io, &prerun_io, &bench_io);

	elapsed = end - start;

	destroy_remaining_aioctxs();

	if (!options.nosyscalls)
		print_aiostats();

	if (!options.singlethread) {
		for (i = 0; i < bench->num_threads; i++) {
			printf("thread_time(%d): %"PRIu64"\n", trs[i].orig_tid,
			       trs[i].total_time);
			printf("thread_syscall_ns(%d): %"PRIu64"\n", trs[i].orig_tid,
			       trs[i].syscall_ns);
			printf("thread_wait_ns(%d): %"PRIu64"\n", trs[i].orig_tid,
			       trs[i].wait_ns);
			printf("thread_sleep_ns(%d): %"PRIu64"\n", trs[i].orig_tid,
			       trs[i].sleep_ns);
#ifdef HAVE_RUSAGE_THREAD
			printf("thread_utime(%d): %ji.%06ld\n", trs[i].orig_tid,
			       (intmax_t)trs[i].utime.tv_sec, trs[i].utime.tv_usec);
			printf("thread_stime(%d): %ji.%06ld\n", trs[i].orig_tid,
			       (intmax_t)trs[i].stime.tv_sec, trs[i].stime.tv_usec);
			printf("thread_inblock(%d): %ld\n", trs[i].orig_tid,
			       trs[i].inblock);
			printf("thread_oublock(%d): %ld\n", trs[i].orig_tid,
			       trs[i].oublock);
#endif
		}
	}

	for (i = 0; i < bench->num_threads; i++)
		print_thread_log(bench->threads[i]);

	timersub(&postrusage.ru_utime, &prerusage.ru_utime, &utime);
	timersub(&postrusage.ru_stime, &prerusage.ru_stime, &stime);
	printf("total_utime: %ji.%06ld\n", (intmax_t)utime.tv_sec, (long)utime.tv_usec);
	printf("total_stime: %ji.%06ld\n", (intmax_t)stime.tv_sec, (long)stime.tv_usec);
	printf("total_inblock: %ld\n", postrusage.ru_inblock - prerusage.ru_inblock);
	printf("total_oublock: %ld\n", postrusage.ru_oublock - prerusage.ru_oublock);

	printf("total_events: %"PRIu64"\n", total);
	printf("successful_events: %"PRIu64"\n", succeeded);
	printf("failed_events: %"PRIu64"\n", total - succeeded);
	printf("success_rate: %f\n", (double)succeeded / (double)total);
	printf("elapsed_time: %"PRIu64".%09"PRIu64"\n", elapsed / BILLION,
	       elapsed % BILLION);

	print_io_stats(stdout, "init", &init_io);
	print_io_stats(stdout, "bench", &bench_io);

	dlerror();
	if (dlclose(bench_handle)) {
		fprintf(stderr, "dlclose: %s\n", dlerror());
		exit(1);
	}

	return 0;
}
