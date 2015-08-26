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
/*
 * Generate a benchmark from an ibench trace.
 */
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <getopt.h>
#include <limits.h>
#include <stdarg.h>

#include "trace.h"
#include "misc.h"
#include "fsmodel.h"
#include "event.h"
#include "assoc.h"
#include "codegen.h"
#include "flags.h"

#include "ibench-parse.tab.h"
#include "strace-parse.tab.h"
#include "initparse.tab.h"
#include "ibench-initparse.tab.h"
#include "iostat-parse.tab.h"

struct traceinfo traceinfo = {
	.path = NULL,
	.type = TT_STRACE,
	.initfile_path = NULL,
	.threadassoc = ASSOC_INIT,
	.pathidx_remap = ASSOC_INIT,
	.mystery_fds = ASSOC_INIT,
	.aioctx_assoc = ASSOC_INIT,
	.mystery_aioctxs = ASSOC_INIT,
	.setupthread = {
		.id = -1,
		.evlist = { .head = NULL, .tail = NULL, },
		.maxbuf = 0,
		.length = 0,
		.lastreturn = 0,
	},
	.numthreads = 0,
	.num_fds = 0,
	.num_mystery_fds = 0,
	.num_mystery_aioctxs = 0,
	.num_aio_infos = 0,
	.num_aio_ctxs = 0,
	.fdidx_evcounts = NULL,
	.fdidx_evcounts_slots = 0,
	.fd_pgi_map = ASSOC_INIT,
	.event_type_counts = { 0, },
	.fsm = NULL,
	.cpucount = 0,
};

int all_init_files = 0;
int include_intrathread_deps = 0;
static int statsonly = 0;
static struct {
	int summary, log;
} stats_enabled = {
	.summary = 0,
	.log = 0,
};
static int dump_depgraph = 0;

/* For CPU utilization numbers parsed from optional iostat file */
static struct {
	float* samples;
	int num_samples;
	float avg;
} cpu = {
	.samples = NULL,
	.num_samples = 0,
	.avg = 0.0,
};

static void diagnostic(const char* filename, int line, const char* tag, const char* fmt, va_list va)
{
	fprintf(stderr, "%s:%d: %s: ", filename, line, tag);
	vfprintf(stderr, fmt, va);
	fputc('\n', stderr);
}

static __attribute__((format(printf, 2, 3))) void twarn(const struct event* ev, const char* fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	diagnostic(traceinfo.path, ev->traceline, "warning", fmt, va);
	va_end(va);
}

static __attribute__((format(printf, 2, 3))) void iwarn(const struct initent* ie, const char* fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	diagnostic(traceinfo.initfile_path, ie->initline, "warning", fmt, va);
	va_end(va);
}

/* Used by trace parsers */
void append_event(struct event* ev, struct parser_state* state)
{
	ev->predelays = DEPDELAY_INIT;
	ev->paths.consumed = NULL;
	ev->temporal_deps = NULL;
	ev->next = NULL;

	/* Fix up events that return FDs (which are treated as ints during most of parsing) */
	if (event_returns_fd(ev)) {
		ev->ret.fd.traceval = ev->ret.i;
		ev->ret.fd.rtidx = -1;
		ev->ret.fd.lastuse = ev->ret.fd.provider = NULL;
	}

	if (!state->events) {
		state->last = state->events = ev;
		ev->prev = NULL;
	} else {
		state->last->next = ev;
		ev->prev = state->last;
		state->last = ev;
	}
}


static void compute_avg_cpu(void)
{
	int i;
	float s = 0.0;

	for (i = 0; i < cpu.num_samples; i++)
		s += cpu.samples[i];

	cpu.avg = s / cpu.num_samples;
}

static void set_event_cpu_util(struct event* ev)
{
	int second;

	if (ev->time.entry < traceinfo.time.start) {
		ev->cpu_util = 0.0;
		return;
	}

	second = (ev->time.entry - traceinfo.time.start) / BILLION;

	if (second >= cpu.num_samples)
		ev->cpu_util = cpu.avg;
	else
		ev->cpu_util = cpu.samples[second];
}

static struct threadtrace* find_thread(int tid)
{
	struct threadtrace* trace;

	if (tid == SETUP_TID)
		return &traceinfo.setupthread;
	else if (tid < 0)
		return NULL;

	trace = it_assoc_lookup(&traceinfo.threadassoc, tid);

	if (!trace) {
		trace = xmalloc(sizeof(*trace));
		*trace = (struct threadtrace) {
			.id = tid,
			.evlist = { .head = NULL, .tail = NULL, },
			.maxbuf = 0,
			.lastreturn = -1,
			.thread_temporal_deps = ASSOC_INIT,
		};
		it_assoc_insert(&traceinfo.threadassoc, tid, trace);
		traceinfo.numthreads += 1;
	}

	return trace;
}

struct prevthread_args {
	struct threadtrace* thread;
};

static void prev_thread_test(int tid, struct threadtrace* thread, void* arg)
{
	struct prevthread_args* pta = arg;

	if (thread->lastreturn > 0) {
		if (!pta->thread || pta->thread->lastreturn < thread->lastreturn)
			pta->thread = thread;
	}
}

/*
 * Find the (non-setup) thread whose last event's exit timestamp is the most
 * recent.
 */
static struct threadtrace* prev_thread(void)
{
	struct prevthread_args args = {
		.thread = NULL,
	};

	it_assoc_visit(&traceinfo.threadassoc, prev_thread_test, &args);

	return args.thread;
}

static unsigned int inc_fdidx_evcount(unsigned int idx)
{
	unsigned prev_slots;
	size_t size;

	if (idx >= traceinfo.fdidx_evcounts_slots) {
		prev_slots = traceinfo.fdidx_evcounts_slots;
		traceinfo.fdidx_evcounts_slots = idx * 1.1 + 1;

		size = traceinfo.fdidx_evcounts_slots * sizeof(*traceinfo.fdidx_evcounts);
		traceinfo.fdidx_evcounts = xrealloc(traceinfo.fdidx_evcounts, size);

		size -= prev_slots * sizeof(*traceinfo.fdidx_evcounts);
		memset(&traceinfo.fdidx_evcounts[prev_slots], 0, size);
	}

	return ++traceinfo.fdidx_evcounts[idx];
}

static inline nstime_t ev_interval(const struct event* from, const struct event* to)
{
	return to->time.entry - from->time.exit;
}

MAKE_ASSOC(fe_assoc, struct pginfo*, NULL, struct event*, NULL);
MAKE_ASSOC(pe_assoc, uintptr_t, 0, struct event*, NULL);
MAKE_ASSOC(ge_assoc, struct pathgen*, NULL, struct event*, NULL);

/*
 * Tracking what previous events did what with/to what resources so we
 * can compute ordering-constraint-specific predelays.
 */
static struct {
	/* Maps an fdidx to the event that created it */
	ie_assoc_t fddep;

	/* Maps an fdidx to the last event that used it (or created it) */
	ie_assoc_t fdorder;

	/* Maps a pginfo to the last event that used it */
	fe_assoc_t fileorder;

	/* Maps an aiocb tag to the event that submitted it */
	pe_assoc_t aiodep;

	struct {
		/*
		 * Maps a pathgen to the event that created it (or NULL if
		 * it's there from the start)
		 */
		ge_assoc_t creator;

		/*
		 * Maps a pathgen to the event that destroyed it (or NULL if
		 * it hasn't yet existed)
		 */
		ge_assoc_t destroyer;
	} pathdep;
} deptrack = {
	.fddep = ASSOC_INIT,
	.fdorder = ASSOC_INIT,
	.fileorder = ASSOC_INIT,
	.aiodep = ASSOC_INIT,
	.pathdep = {
		.creator = ASSOC_INIT,
		.destroyer = ASSOC_INIT,
	},
};

/*
 * Minor hack: u64 used to store two ints.
 * Upper half is aiocb index, lower half is fd index.
 */
MAKE_ASSOC(pip_assoc, uintptr_t, 0, uint64_t, UINT64_MAX);

static pip_assoc_t aiocb_assoc = ASSOC_INIT;

static void compute_aiodep_predelay(struct event* e, uintptr_t tag)
{
	nstime_t delay;
	struct event* submitter = pe_assoc_lookup(&deptrack.aiodep, tag);

	assert(submitter);

	/* I doubt we'll ever have AIO requests in the setup-thread, but... */
	if (submitter->thread == SETUP_TID)
		return;

	delay = ev_interval(submitter, e);
	assert(delay > 0);

	if (e->predelays.aiodep < 0 || delay < e->predelays.aiodep)
		e->predelays.aiodep = delay;
}

/*
 * Associates an aio_*() call with an index into the global aiocbs[] array,
 * whose content is generated (piece by piece) by this function.
 */
static void process_posix_aio(struct event* e)
{
	uintptr_t tag;
	uint64_t u64;
	int aiocbnum;
	int fdidx;
	struct fduse* fduse;
	struct aio_read_attrs* a;

	switch (e->type) {
	case EV_AIO_READ:
		tag = evattr(aio_read, e, tag);
		break;
	case EV_AIO_RETURN:
		tag = evattr(aio_return, e, tag);
		break;
	case EV_AIO_ERROR:
		tag = evattr(aio_error, e, tag);
		break;
	default:
		abort();
	}

	u64 = pip_assoc_lookup(&aiocb_assoc, tag);

	if (e->type == EV_AIO_READ) {
		if (u64 != UINT64_MAX) {
			fprintf(stderr, "aio_read got re-used aiocbnum! (line %d)\n",
			        e->traceline);
			exit(1);
		}

		aiocbnum = traceinfo.num_aio_infos++;
		a = &e->attrs.aio_read;
		pip_assoc_insert(&aiocb_assoc, tag,
		                 ((uint64_t)aiocbnum << 32 | a->fd.rtidx));
		pe_assoc_delete(&deptrack.aiodep, tag);
		pe_assoc_insert(&deptrack.aiodep, tag, e);

		a->aiocbnum = aiocbnum;

		if (!statsonly)
			output_posix_aiocb(e);
	} else {
		if (u64 == UINT64_MAX) {
			fprintf(stderr, "%s did not find associated aiocbnum! "
			        "(line %d)\n", event_type_name(e->type), e->traceline);
			exit(1);
		}

		compute_aiodep_predelay(e, tag);

		aiocbnum = u64 >> 32;
		fdidx = u64 & 0xFFFFFFFFULL;

		if (e->type == EV_AIO_RETURN) {
			/* End of aiocb live range, disassociate */
			pip_assoc_delete(&aiocb_assoc, tag);
			evattr(aio_return, e, aiocbnum) = aiocbnum;
			fduse = &evattr(aio_return, e, fd);
		} else {
			evattr(aio_error, e, aiocbnum) = aiocbnum;
			fduse = &evattr(aio_error, e, fd);
		}

		assert(fduse->rtidx == fdidx);
	}
}

/*
 * Get the AIO context index that 'ctx' is remapped to, providing a new one if
 * none exists.  If a new one is created, writes a zero to '*found', otherwise
 * writes a one to '*found'
 */
static int remap_aioctx(uintptr_t ctx, int* found)
{
	int id = pi_assoc_lookup(&traceinfo.aioctx_assoc, ctx);

	*found = id != -1;

	if (!*found) {
		id = traceinfo.num_aio_ctxs++;
		pi_assoc_insert(&traceinfo.aioctx_assoc, ctx, id);
	}

	return id;
}

static void record_mystery_aioctx(int ctx_id)
{
	ii_assoc_insert(&traceinfo.mystery_aioctxs, ctx_id, 1);
	traceinfo.num_mystery_aioctxs++;
}

static void process_linux_iocb(struct event* ev, struct artc_iocb* cb)
{
	int aiocbnum;
	uint64_t tag = cb->data;
	uint64_t u64 = pip_assoc_lookup(&aiocb_assoc, tag);

	if (u64 != UINT64_MAX) {
		fprintf(stderr, "io_submit got reused iocb data! (line %d)\n",
		        ev->traceline);
		fprintf(stderr, "(Not supported, may require strace patched)\n");
		exit(1);
	}

	aiocbnum = traceinfo.num_aio_infos++;
	pip_assoc_insert(&aiocb_assoc, tag,
	                 ((uint64_t)aiocbnum << 32) | cb->fd.rtidx);
	cb->aiocbnum = aiocbnum;

	if (!statsonly)
		output_linux_aiocb(cb);
}

static void process_linux_aio_submit(struct event* e)
{
	int found;
	struct artc_iocb* i;
	uintptr_t ctx = evattr(io_submit, e, ctx);

	for (i = e->attrs.io_submit.iocbs; i; i = i->next)
		process_linux_iocb(e, i);

	evattr(io_submit, e, ctx_id) = remap_aioctx(ctx, &found);

	if (!found) {
		twarn(e, "mystery AIO context %"PRIuPTR, ctx);
		record_mystery_aioctx(evattr(io_submit, e, ctx_id));
	}
}

static void process_linux_ioevent(struct event* ev, struct artc_ioevent* ioev)
{
	int fdidx, aiocbnum;
	uintptr_t tag = ioev->data;
	uint64_t u64 = pip_assoc_lookup(&aiocb_assoc, tag);

	if (u64 == UINT64_MAX) {
		fprintf(stderr, "io_getevents() did not find assiociated aiocbnum! "
		        "(line %d)\n", ev->traceline);
		fprintf(stderr, "(Malformed trace?)\n");
		exit(1);
	}

	aiocbnum = u64 >> 32;
	fdidx = u64 & 0xFFFFFFFFULL;

	/* The AIOCB live range ends here, so we disassociate it */
	pip_assoc_delete(&aiocb_assoc, tag);
	ioev->aiocbnum = aiocbnum;
	assert(ioev->fd.rtidx == fdidx);
}

static void process_linux_aio_getevents(struct event* e)
{
	int found;
	struct artc_ioevent* i;
	uintptr_t ctx = evattr(io_getevents, e, ctx);

	for (i = e->attrs.io_getevents.ioevents; i; i = i->next)
		process_linux_ioevent(e, i);

	evattr(io_getevents, e, ctx_id) = remap_aioctx(ctx, &found);

	if (!found) {
		twarn(e, "mystery AIO context %"PRIuPTR, ctx);
		record_mystery_aioctx(evattr(io_getevents, e, ctx_id));
	}
}

static void process_linux_aio_setup(struct event* e)
{
	int found;

	evattr(io_setup, e, ctx_id) = remap_aioctx(evattr(io_setup, e, ctx), &found);

	if (found)
		twarn(e, "io_setup() of already-set-up AIO context %"PRIuPTR,
		      evattr(io_setup, e, ctx));
}

static void process_linux_aio_destroy(struct event* e)
{
	int found;
	uintptr_t ctx = evattr(io_destroy, e, ctx);

	evattr(io_destroy, e, ctx_id) = remap_aioctx(ctx, &found);

	pi_assoc_delete(&traceinfo.aioctx_assoc, ctx);

	if (!found) {
		twarn(e, "mystery AIO context %"PRIuPTR, ctx);
		record_mystery_aioctx(evattr(io_destroy, e, ctx_id));
	}
}

static void record_init_paths(struct initent* initpaths)
{
	int i;
	struct initent* ie;
	struct pathent* pe;
	struct pathgen* pg;

	static const struct {
		const char* path;
		const char* target;
	} hacks[] = {
		/*
		 * /dev/random blocks on linux, so we symlink it to
		 * /dev/urandom, which does not.  On OSX they're
		 * indistinguishable anyway.
		 */
		{ "/dev/zero", "/dev/zero", },
		{ "/dev/random", "/dev/urandom", },
		{ "/dev/urandom", "/dev/urandom", },
		{ "/dev/fd", "/dev/fd", },
		{ "/dev/dtracehelper", "/dev/zero", },
		{ "/dev/autofs_nowait", "/dev/zero", },
		{ "/dev/tty", "/dev/zero", },
	};

	for (ie = initpaths; ie; ie = ie->next) {
		/* Hack. */
		if (!strcmp(ie->path, "/"))
			continue;

		if ((pe = fsm_lookup(traceinfo.fsm, ie->path, 0))) {
			iwarn(ie, "duplicated path '%s'", ie->path);
			pg = path_getgen(pe, 0);
			if (ie->type != pg->info->type) {
				fprintf(stderr, "Error: conflicting types for '%s' "
				        "in init file\n", ie->path);
				exit(1);
			}

			switch (ie->type) {
			case FT_DIR:
				/* should be fine to ignore */
				continue;

			case FT_SYMLINK:
				/* check that they're pointing at the same thing */
				if (strcmp(pg->info->target, ie->target)) {
					fprintf(stderr, "Error: conflicting symlink "
					        "targets for '%s' in init file\n",
					        ie->path);
					exit(1);
				}
				continue;

			case FT_FILE:
				/* use the bigger one */
				if (ie->size > pg->info->size)
					pg->info->size = ie->size;
				continue;

			default:
				abort();
			}
		}

		switch (ie->type) {
		case FT_FILE:
			pe = fsm_creat(traceinfo.fsm, ie->path, ie->size, FSM_FROMINIT);
			break;

		case FT_DIR:
			pe = fsm_mkdir(traceinfo.fsm, ie->path, FSM_FROMINIT);
			break;

		case FT_SYMLINK:
			pe = fsm_symlink(traceinfo.fsm, xstrdup(ie->target), ie->path,
			                 FSM_FROMINIT);
			break;

		default:
			abort();
		}
	}

	/* Add /dev/ */
	if (!fsm_lookup(traceinfo.fsm, "/dev", 0))
		fsm_mkdir(traceinfo.fsm, "/dev", FSM_FROMINIT);

	for (i = 0; i < sizeof(hacks) / sizeof(hacks[0]); i++) {
		pe = fsm_symlink(traceinfo.fsm, xstrdup(hacks[i].target), hacks[i].path,
		                 FSM_FROMINIT);
		path_lastgen(pe)->info->type = FT_HOSTLINK;
	}
}

static ii_assoc_t fdremap_assoc = ASSOC_INIT;

struct gfdu_arg {
	int rtidx;
	struct fduse* fd;
};

static int get_fduse(struct event* ev, struct fduse* fd, void* arg)
{
	struct gfdu_arg* args = arg;

	if (fd->rtidx == args->rtidx) {
		args->fd = fd;
		return 1;
	} else
		return 0;
}

/*
 * Find a given event's fduse struct that refers to a file descriptor we're
 * remapping to the given rtidx.
 */
static struct fduse* event_get_fduse(struct event* ev, int rtidx)
{
	struct gfdu_arg arg = {
		.rtidx = rtidx,
		.fd = NULL,
	};

	event_visit_fds(ev, get_fduse, &arg);

	return arg.fd;
}

/*
 * Find the last event that used the fd that we're remapping to the given
 * rtidx.  If the fd's entry in deptrack.fdorder overlaps the given event,
 * scan back through that fd's use chain until we reach one that doesn't.
 */
static struct event* previous_fd_user(struct event* ev, int rtidx)
{
	struct event* lastuser;
	struct fduse* fd;

	lastuser = ie_assoc_lookup(&deptrack.fdorder, rtidx);

	while (lastuser && lastuser->time.exit > ev->time.entry) {
		fd = event_get_fduse(lastuser, rtidx);
		assert(fd);
		lastuser = fd->lastuse;
	}

	return lastuser;
}

static struct filemd* event_get_filemd(struct event* ev, struct pginfo* pgi)
{
	int i;

	for (i = 0; ev->files[i].pgi; i++) {
		if (ev->files[i].pgi == pgi)
			return &ev->files[i];
	}

	return NULL;
}

/* Analogous to previous_fd_user(), but for files rather than fds. */
static struct event* previous_file_user(struct event* ev, struct pginfo* pgi)
{
	struct event* lastuser;
	struct filemd* file;

	lastuser = fe_assoc_lookup(&deptrack.fileorder, pgi);

	while (lastuser && lastuser->time.exit > ev->time.entry) {
		file = event_get_filemd(lastuser, pgi);
		assert(file);
		lastuser = file->lastuse;
	}

	return lastuser;
}

/*
 * Remap a recorded file descriptor number from the trace to an index into the
 * fd_info[] array used at runtime.  This doesn't specify what the runtime fd
 * number will be, but determines a location where it will be stored (and
 * where its availability status will be tracked).  Records the remapped
 * runtime index ('rtidx') and the 'provider' dependency information into 'fd'.
 */
static void remap_fduse(struct fduse* fd, int new, struct event* ev)
{
	/* Ugly kludge: overwriting original trace FD value */
	if (!event_should_succeed(ev))
		fd->traceval = -1;

	if (fd->traceval < 0) {
		fd->rtidx = -1;
		fd->provider = NULL;
		fd->lastuse = NULL;
		return;
	}

	fd->rtidx = ii_assoc_lookup(&fdremap_assoc, fd->traceval);

	if (new) {
		if (fd->rtidx != INT_MIN) {
			twarn(ev, "missed close on fd %d", fd->traceval);
			/* Continue anyway, just reassociate */
			ii_assoc_delete(&fdremap_assoc, fd->traceval);
		}
		fd->rtidx = traceinfo.num_fds++;
		ii_assoc_insert(&fdremap_assoc, fd->traceval, fd->rtidx);
		fd->provider = fd->lastuse = NULL;
	} else {
		if (fd->rtidx == INT_MIN
		    && !(ev->type == EV_DUP2 && fd == &ev->attrs.dup2.new)) {
			/* Don't bother warning on stdio descriptors */
			if (fd->traceval > 2)
				twarn(ev, "fd %d of mysterious origin", fd->traceval);

			/* Continue anyway */
			fd->rtidx = traceinfo.num_fds++;
			ii_assoc_insert(&fdremap_assoc, fd->traceval, fd->rtidx);
			ie_assoc_insert(&traceinfo.mystery_fds, fd->rtidx, ev);
			ip_assoc_insert(&traceinfo.fd_pgi_map, fd->rtidx,
			                NONFILE_PGINFO);
			++traceinfo.num_mystery_fds;
		}

		fd->provider = ie_assoc_lookup(&deptrack.fddep, fd->rtidx);
		fd->lastuse = previous_fd_user(ev, fd->rtidx);

		if (ev->type == EV_CLOSE
		    || (ev->type == EV_DUP2 && fd == &ev->attrs.dup2.new))
			/* End of fd live range, disassociate */
			ii_assoc_delete(&fdremap_assoc, fd->traceval);

		if (fd->rtidx >= 0 && !event_unordered(ev))
			inc_fdidx_evcount(fd->rtidx);
	}
}

static pi_assoc_t aio_origfds = ASSOC_INIT;

/*
 * This is gross.  What happens when between process_fds() and
 * process_posix_aio() is confusing, messy, and very much in need of
 * reorganization.
 */
static void posix_aio_fd_hack(struct event* e, struct fduse* fd)
{
	uintptr_t tag;
	int origfd;

	tag = e->type == EV_AIO_ERROR ? evattr(aio_error, e, tag)
		: evattr(aio_return, e, tag);

	origfd = pi_assoc_lookup(&aio_origfds, tag);
	if (origfd == -1) {
		fprintf(stderr, "posix_aio_fd_hack: internal error\n");
		abort();
	}

	fd->traceval = origfd;
}

static void linux_aio_fd_hack(struct event* e, struct fduse* fd)
{
	int origfd;

	origfd = pi_assoc_lookup(&aio_origfds, container_of(fd, struct artc_ioevent, fd)->data);
	if (origfd == -1) {
		fprintf(stderr, "linux_aio_fd_hack: internal error\n");
		abort();
	}

	fd->traceval = origfd;
}

/*
 * Compute the elapsed time between the use of fdidx by e and when
 * that fd was created, setting it as e's fddep predelay (unless e
 * already has a lower fddep predelay).
 */
static void compute_fddep_predelay(struct event* e, int fdidx)
{
	nstime_t delay;
	struct event* fdprovider = ie_assoc_lookup(&deptrack.fddep, fdidx);

	/* If we have a mystery-FD, treat its first use as its provider */
	if (!fdprovider) {
		fdprovider = ie_assoc_lookup(&traceinfo.mystery_fds, fdidx);

		/* This should only occur in the event of a mystery-FD */
		if (fdprovider == e)
			return;
	}

	assert(fdprovider);

	if (fdprovider->thread == SETUP_TID)
		return;

	delay = ev_interval(fdprovider, e);
	assert(delay > 0);

	if (e->predelays.fddep < 0 || delay < e->predelays.fddep)
		e->predelays.fddep = delay;
}

/*
 * Like compute_fddep_predelay(), but for FD ordering, so the time
 * delay since the last event to use fdidx.
 *
 * Also marks 'e' as the last use of 'fdidx'.
 */
static void compute_fdorder_predelay(struct event* e, int fdidx)
{
	nstime_t delay;
	struct event* lastuse = ie_assoc_delete(&deptrack.fdorder, fdidx);

	ie_assoc_insert(&deptrack.fdorder, fdidx, e);

	if (!lastuse) {
		lastuse = ie_assoc_lookup(&traceinfo.mystery_fds, fdidx);
		assert(e == lastuse);
		return;
	}

	assert(lastuse);

	if (lastuse->thread == SETUP_TID)
		return;

	delay = ev_interval(lastuse, e);

	/*
	 * We can have overlapping FD uses, so 'delay' could be
	 * negative here.  If it is, we just won't count it.
	 */
	if (delay < 0)
		return;

	if (e->predelays.fdorder < 0 || delay < e->predelays.fdorder)
		e->predelays.fdorder = delay;
}

static int process_used_fd(struct event* e, struct fduse* fd, void* dummy)
{
	assert(fd->provider == NULL);

	if (e->type == EV_AIO_RETURN || e->type == EV_AIO_ERROR)
		posix_aio_fd_hack(e, fd);
	else if (e->type == EV_IO_GETEVENTS)
		linux_aio_fd_hack(e, fd);

	remap_fduse(fd, 0, e);

	if (fd->rtidx >= 0) {
		compute_fddep_predelay(e, fd->rtidx);

		if (!event_unordered(e))
			compute_fdorder_predelay(e, fd->rtidx);
	}

	if (e->type == EV_AIO_READ)
		pi_assoc_insert(&aio_origfds,
		                evattr(aio_read, e, tag), fd->traceval);
	else if (e->type == EV_AIO_RETURN)
		pi_assoc_delete(&aio_origfds,
		                evattr(aio_return, e, tag));
	else if (e->type == EV_IO_SUBMIT)
		pi_assoc_insert(&aio_origfds,
		                container_of(fd, struct artc_iocb, fd)->data,
		                fd->traceval);
	else if (e->type == EV_IO_GETEVENTS)
		pi_assoc_delete(&aio_origfds,
		                container_of(fd, struct artc_ioevent, fd)->data);

	return 0;
}

static void process_new_fds(struct event* e)
{
	struct fduse* fd1 = NULL;
	struct fduse* fd2 = NULL;

	if (e->type == EV_SOCKETPAIR) {
		fd1 = &e->attrs.socketpair.fd1;
		fd2 = &e->attrs.socketpair.fd2;
	} else if (e->type == EV_PIPE) {
		fd1 = &e->attrs.pipe.rd;
		fd2 = &e->attrs.pipe.wr;
	}

	if (fd1 || fd2) {
		remap_fduse(fd1, 1, e);
		remap_fduse(fd2, 1, e);

		/* Record fddep tracking info... */
		ie_assoc_insert(&deptrack.fddep, fd1->rtidx, e);
		ie_assoc_insert(&deptrack.fddep, fd2->rtidx, e);

		/* ...and fdorder tracking info. */
		ie_assoc_insert(&deptrack.fdorder, fd1->rtidx, e);
		ie_assoc_insert(&deptrack.fdorder, fd2->rtidx, e);

		/*
		 * We should only be in this block EV_SOCKETPAIR & EV_PIPE,
		 * which shouldn't be unordered, but inserting unordered
		 * events into deptrack.fdorder is bad news, so we'll just
		 * make sure.
		 */
		assert(!event_unordered(e));
		return;
	}

	if (event_returns_fd(e)) {
		remap_fduse(&e->ret.fd, 1, e);

		/* Record fddep and fdorder tracking info. */
		if (e->ret.fd.rtidx >= 0) {
			ie_assoc_insert(&deptrack.fddep, e->ret.fd.rtidx, e);
			ie_assoc_insert(&deptrack.fdorder, e->ret.fd.rtidx, e);

			/*
			 * Like above with socketpair & pipe, fd-returning
			 * events should never be unordered, but we're
			 * asserting it anyway because we've added the event
			 * to deptrack.fdorder.
			 */
			assert(!event_unordered(e));
		}
	}
}

/*
 * Like compute_fdorder_predelay(), but for *files* as opposed to FDs.
 */
static void compute_fileorder_predelay(struct event* e, struct pginfo* f)
{
	nstime_t delay;
	struct event* lastuse = fe_assoc_delete(&deptrack.fileorder, f);

	fe_assoc_insert(&deptrack.fileorder, f, e);

	/* This might be the first access to the file */
	if (!lastuse)
		return;

	if (lastuse->thread == SETUP_TID)
		return;

	delay = ev_interval(lastuse, e);

	if (delay < 0)
		return;

	if (e->predelays.fileorder < 0 || delay < e->predelays.fileorder)
		e->predelays.fileorder = delay;
}

static int find_fd_pgi(struct event* ev, struct fduse* fd, void* dummy)
{
	struct pginfo* pgi;
	int* idx = dummy;

	if (fd->rtidx >= 0) {
		pgi = ip_assoc_lookup(&traceinfo.fd_pgi_map, fd->rtidx);
		if (!pgi) {
			fprintf(stderr, "no pginfo for fdidx %d\n", fd->rtidx);
			abort();
		}

		if (pgi && pgi != NONFILE_PGINFO) {
			ev->files[*idx].pgi = pgi;
			if (event_unordered(ev)) {
				ev->files[*idx].lastuse = NULL;
			} else {
				ev->files[*idx].lastuse = previous_file_user(ev, pgi);
			}
		} else {
			ev->files[*idx].pgi = NULL;
			ev->files[*idx].lastuse = NULL;
		}

		if (!event_unordered(ev))
			compute_fileorder_predelay(ev, pgi);

		*idx += 1;
	}

	return 0;
}

static void process_used_files(struct event* ev)
{
	struct pathent* pe;
	struct pathent* pe2;
	struct pginfo* pgi;
	struct pginfo* pgi2;
	const char* path;
	int flags, numdone = 0;

	if (!event_should_succeed(ev))
		return;

	/* The only event that actually accesses more than one file. */
	if (ev->type == EV_EXCHANGEDATA) {
		pe = fsm_lookup(traceinfo.fsm, evattr(exchangedata, ev, path1),
		                FSM_FOLLOW);
		pe2 = fsm_lookup(traceinfo.fsm, evattr(exchangedata, ev, path2),
		                 FSM_FOLLOW);
		assert(pe && pe2);
		pgi = path_lastgen(pe)->info;
		pgi2 = path_lastgen(pe2)->info;

		ev->files[0].lastuse = previous_file_user(ev, pgi);
		ev->files[1].lastuse = previous_file_user(ev, pgi2);

		compute_fileorder_predelay(ev, pgi);
		ev->files[0].pgi = pgi;

		compute_fileorder_predelay(ev, pgi2);
		ev->files[1].pgi = pgi2;
		return;
	}

	event_visit_fds(ev, find_fd_pgi, &numdone);
	if (numdone)
		return;

	/* Otherwise we're dealing with path-based, non-fd-returning file access. */
	path = event_get_accessed_path(ev);
	if (!path)
		return;

	switch (ev->type) {
	case EV_RMDIR:
	case EV_UNLINK:
	case EV_UNLINKAT:
	case EV_RENAME:
		/*
		 * Because process_paths() will have already removed the path
		 * from our fsmodel by this point, looking up a dead pathent
		 * is fine (and expected).
		 */
		flags = FSM_DEADOK;
		break;

	case EV_LSTAT:
	case EV_LSTAT64:
	case EV_LINK:
	case EV_READLINK:
	/*
	 * Hack: setattrlist has a flag argument indicating whether or not to
	 * follow symlinks, but it isn't in the ibench traces.  Line 689269 of
	 * iphoto_import400 depends on it (a successful setattrlist on a
	 * broken symlink), so we'll kludge it in as a special case.
	 */
	case EV_SETATTRLIST:
		flags = 0;
		break;

	case EV_UTIMENSAT:
		flags = (traceinfo.type == TT_STRACE) ?
			((ev->attrs.utimensat.flags & AT_SYMLINK_NOFOLLOW) ?
			 0 : FSM_FOLLOW)
			: FSM_FOLLOW;
		break;

	case EV_FACCESSAT:
		/* FIXME: duplicates utimensat code above. */
		flags = (traceinfo.type == TT_STRACE) ?
			((ev->attrs.faccessat.flags & AT_SYMLINK_NOFOLLOW) ?
			 0 : FSM_FOLLOW)
			: FSM_FOLLOW;
		break;

	default:
		flags = FSM_FOLLOW;
		break;
	}

	pe = fsm_lookup(traceinfo.fsm, path, flags);
	assert(pe);
	assert(pathent_alive(pe) == !(ev->type == EV_RENAME
	                              || ev->type == EV_UNLINK
	                              || ev->type == EV_UNLINKAT
	                              || ev->type == EV_RMDIR));
	pgi = path_lastgen(pe)->info;
	ev->files[0].pgi = pgi;
	if (event_unordered(ev)) {
		ev->files[0].lastuse = NULL;
	} else {
		ev->files[0].lastuse = previous_file_user(ev, pgi);
		compute_fileorder_predelay(ev, pgi);
	}
}

static int handle_duped_fd(struct event* ev, struct fduse* fd, void* dummy)
{
	struct pginfo* pgi;

	if (ev->type == EV_DUP2 && fd != &ev->attrs.dup2.old)
		return 0;

	pgi = ip_assoc_lookup(&traceinfo.fd_pgi_map, fd->rtidx);
	assert(pgi);
	ip_assoc_insert(&traceinfo.fd_pgi_map, ev->ret.fd.rtidx, pgi);

	return 0;
}

static void process_new_files(struct event* ev)
{
	struct pathent* pe;
	struct pginfo* pgi;
	struct fduse* fd1 = NULL;
	struct fduse* fd2 = NULL;

	if (!event_should_succeed(ev))
		return;

	if (ev->type == EV_SOCKETPAIR) {
		fd1 = &ev->attrs.socketpair.fd1;
		fd2 = &ev->attrs.socketpair.fd2;
	} else if (ev->type == EV_PIPE) {
		fd1 = &ev->attrs.pipe.rd;
		fd2 = &ev->attrs.pipe.wr;
	}

	if (fd1 || fd2) {

		if (fd1->rtidx >= 0 && fd2->rtidx >= 0) {
			assert(!ip_assoc_lookup(&traceinfo.fd_pgi_map, fd1->rtidx));
			assert(!ip_assoc_lookup(&traceinfo.fd_pgi_map, fd2->rtidx));
			ip_assoc_insert(&traceinfo.fd_pgi_map, fd1->rtidx,
			                NONFILE_PGINFO);
			ip_assoc_insert(&traceinfo.fd_pgi_map, fd2->rtidx,
			                NONFILE_PGINFO);
		}

		assert(!ev->files[0].pgi);

		return;
	}

	if (event_returns_fd(ev) && ev->ret.fd.traceval >= 0) {
		assert(!ip_assoc_lookup(&traceinfo.fd_pgi_map,
		                        ev->ret.fd.rtidx));

		switch (ev->type) {
		case EV_SOCKET:
		case EV_SHM_OPEN:
		case EV_KQUEUE:
		case EV_EPOLL_CREATE:
			ip_assoc_insert(&traceinfo.fd_pgi_map, ev->ret.fd.rtidx,
			                NONFILE_PGINFO);
			assert(!ev->files[0].pgi);
			break;

		case EV_DUP:
		case EV_DUP2:
		case EV_FCNTL: /* F_DUPFD */
			event_visit_fds(ev, handle_duped_fd, NULL);
			/*
			 * ev->files[0] already set above in generic fd-use
			 * handling code, no need to do so here.
			 */
			break;

		case EV_OPEN:
			pe = fsm_lookup(traceinfo.fsm, evattr(open, ev, path),
			                FSM_FOLLOW);
			assert(pe);
			pgi = path_lastgen(pe)->info;
			ip_assoc_insert(&traceinfo.fd_pgi_map, ev->ret.fd.rtidx, pgi);
			compute_fileorder_predelay(ev, pgi);
			ev->files[0].pgi = pgi;
			ev->files[0].lastuse = NULL;
			break;

		default:
			abort();
		}
	}
}

static void ev_alloc_files(struct event* ev)
{
	struct artc_iocb* cb;
	struct artc_ioevent* ioe;
	int i, num = 0;

	if (!event_should_succeed(ev)) {
		ev->files = NULL;
		return;
	}

	if (ev->type == EV_IO_SUBMIT) {
		for (cb = ev->attrs.io_submit.iocbs; cb; cb = cb->next)
			num += 1;
	} else if (ev->type == EV_IO_GETEVENTS) {
		for (ioe = ev->attrs.io_getevents.ioevents; ioe; ioe = ioe->next)
			num += 1;
	} else
		num = 2;

	ev->files = xmalloc((num+1) * sizeof(*ev->files));
	for (i = 0; i <= num; i++) {
		ev->files[i].pgi = NULL;
		ev->files[i].lastuse = NULL;
	}
}

static filetype_t provided_path_type(const struct event* ev)
{
	struct pathent* pe;
	char* tmp;

	switch (ev->type) {
	case EV_OPEN:
		return FT_FILE;

	case EV_MKDIR:
		return FT_DIR;

	case EV_SYMLINK:
		return FT_SYMLINK;

	case EV_RENAME:
	case EV_LINK:
		tmp = (ev->type == EV_LINK) ? evattr(link, ev, oldpath)
			: evattr(rename, ev, oldpath);
		pe = fsm_lookup(traceinfo.fsm, tmp, 0);
		if (!pe) {
			twarn(ev, "unknown provided-path type for '%s'", tmp);
			abort();
		}
		if (!pathent_alive(pe))
			twarn(ev, "use of dead path '%s'", tmp);
		return path_lastgen(pe)->info->type;

	default:
		fprintf(stderr, "%s does not provide a path\n", event_type_enumname(ev->type));
		abort();
	}
}

/*
 * Dinky little helper for the common case of events
 * producing/consuming/removing a single path.
 */
static int* singleton_list(int val)
{
	int* l = xmalloc(2 * sizeof(int));
	l[0] = val;
	l[1] = -1;
	return l;
}

struct aita_arg {
	int* list;
	unsigned int pop, size;
};

static void add_id_to_array(const char* path, struct pathent* pe, struct pathgen* pg,
                            void* varg)
{
	struct aita_arg* args = varg;
	if (pathent_alive(pe) && pg == path_lastgen(pe)
	    && (all_init_files || pathent_used(pe))) {
		if (args->pop == args->size) {
			args->size *= 2;
			args->size += 8;
			args->list = xrealloc(args->list, args->size * sizeof(int));
		}
		args->list[args->pop++] = pg->id;
	}
}

/*
 * Returns a malloc()ed array (-1 terminated) of pathgen IDs for every
 * currently-live subpath under 'path'.  This includes the ID of whatever's
 * currently at 'path'.
 */
static int* child_pathidxs(const char* path)
{
	struct aita_arg arg = {
		.list = NULL,
		.pop = 0,
		.size = 0,
	};
	struct pathent* pe = fsm_lookup(traceinfo.fsm, path, 0);

	if (!pe)
		return NULL;

	fsm_visit_subtree(pe, add_id_to_array, &arg);

	arg.list = xrealloc(arg.list, (arg.pop + 1) * sizeof(int));
	arg.list[arg.pop] = -1;

	return arg.list;
}

static void inc_pg_usecnt(const char* path, struct pathent* pe, struct pathgen* pg,
                          void* arg)
{
	if (pathent_alive(pe) && pg == path_lastgen(pe))
		pg->usecnt++;
}

/*
 * rename() on a directory is kind of a special case, since it can potentially
 * remove and produce large numbers of paths.
 *
 * FIXME: creating or unlinking a symlink (transitively) to a directory would
 * also exhibit this behavior.
 */
static void process_rename_paths(struct event* ev)
{
	const char* oldpath;
	const char* newpath;
	struct pathent* pe;

	assert(ev->type == EV_RENAME);

	newpath = evattr(rename, ev, newpath);
	oldpath = evattr(rename, ev, oldpath);

	pe = fsm_lookup(traceinfo.fsm, oldpath, 0);
	if (!pe || !pathent_alive(pe)) {
		fprintf(stderr, "can't find live oldpath (%s) for rename\n", oldpath);
		abort();
	}

	ev->paths.removed = child_pathidxs(oldpath);
	ev->paths.consumed = child_pathidxs(oldpath);

	fsm_visit_subtree(pe, inc_pg_usecnt, NULL);

	fsm_rename(traceinfo.fsm, oldpath, newpath);
	ev->paths.produced = child_pathidxs(newpath);
	ev->paths.negdep = NULL;
}

static void compute_pathnegdep_predelay(struct event* ev, struct pathgen* pg)
{
	nstime_t delay;
	struct event* destruction = ge_assoc_lookup(&deptrack.pathdep.destroyer, pg);

	/* The path may not have existed since before the trace began. */
	if (!destruction || destruction->thread == SETUP_TID)
		return;

	delay = ev_interval(destruction, ev);
	assert(delay > 0);

	if (ev->predelays.pathdep < 0 || delay < ev->predelays.pathdep)
		ev->predelays.pathdep = delay;
}

/*
 * Returns the event that destroys the pathgen on which 'ev' has a negdep.
 */
static struct event* process_negdep(struct event* ev, const char* negdep)
{
	struct pathent* pe;
	struct pathgen* pg;
	int flags = FSM_DEADOK;

	if (ev->type == EV_OPEN
	    && flags_include(evattr(open, ev, flags), "O_EXCL"))
		flags |= FSM_FOLLOW;

	pe = fsm_lookup(traceinfo.fsm, negdep, flags);
	if (pe && pathent_alive(pe)) {
		/* The trace is exhibiting some bogosity. */
		twarn(ev, "negdep on existing path '%s'", negdep);
		if (ev->type == EV_OPEN) {
			/* open()s we can paper over by omitting O_EXCL. */
			if (!remove_flag(&evattr(open, ev, flags), "O_EXCL")) {
				fprintf(stderr, "no O_EXCL on bogus open() "
				        "negdep?\n");
				abort();
			}
		} else {
			/* other calls...shrug and give up. */
			fprintf(stderr, "can't handle bogus negdep on %s\n",
			        event_type_enumname(ev->type));
			exit(1);
		}
		return NULL;
	} else {
		if (!pe)
			return NULL;
		else {
			pg = path_lastgen(pe);
			compute_pathnegdep_predelay(ev, pg);
			return ge_assoc_lookup(&deptrack.pathdep.destroyer, pg);
		}
	}
}

/*
 * Some special-case hacks for known trace screwiness that we detect and
 * prepare for (successfully-accessed paths that aren't in the init listings
 * or created during the trace).
 */
static const struct initent ibench_hackpaths[] = {
	{ .path = "/_CodeSignature", .type = FT_DIR, },
	{ .path = "/Resources", .type = FT_DIR, },
	{ .path = "/var/folders/hK/hKG7P-KrGc8iIPvFZEL0Qk+++TI/-Tmp-/iPhoto",
	  .type = FT_DIR, },
	{ .path = "/usr/share/zoneinfo/UTC", .type = FT_FILE, { .size = 56, }, },
};

static struct pathent* ibench_hack_path(const char* path)
{
	int i;

	for (i = 0; i < ARR_SIZE(ibench_hackpaths); i++) {
		if (!strcmp(path, ibench_hackpaths[i].path)) {
			switch (ibench_hackpaths[i].type) {
			case FT_FILE:
				return fsm_creat(traceinfo.fsm, path,
				                 ibench_hackpaths[i].size,
				                 FSM_CREATEPARENTS|FSM_FROMINIT);
				break;

			case FT_DIR:
				return fsm_mkdir(traceinfo.fsm, path,
				                 FSM_CREATEPARENTS|FSM_FROMINIT);
				break;

			default:
				abort();
			}
		}
	}

	return NULL;
}

static void compute_pathdep_predelay(struct event* ev, struct pathgen* pg)
{
	nstime_t delay;
	struct event* creation = ge_assoc_lookup(&deptrack.pathdep.creator, pg);

	/* The path may have existed since before the trace started. */
	if (!creation || creation->thread == SETUP_TID)
		return;

	delay = ev_interval(creation ,ev);
	assert(delay > 0);

	if (ev->predelays.pathdep < 0 || delay < ev->predelays.pathdep)
		ev->predelays.pathdep = delay;
}

/*
 * Return the ID of the current pathgen at 'path' used by 'ev' (following
 * symlinks if 'follow' is non-zero).
 */
static int process_path_use(struct event* ev, const char* path, int follow)
{
	struct pathent* pe;
	struct pathgen* pg;

	pe = fsm_lookup(traceinfo.fsm, path, follow ? FSM_FOLLOW : 0);
	if (!pe) {
		twarn(ev, "use of mystery path '%s'", path);

		if (traceinfo.type == TT_IBENCH)
			pe = ibench_hack_path(path);

		/* If it's something not in our special-case hackpaths set */
		if (!pe)
			pe = fsm_creat(traceinfo.fsm, path, 0, FSM_CREATEPARENTS|FSM_FROMINIT);

		/* FIXME: unify with identical statement below */
		path_lastgen(pe)->usecnt++;
	} else {
		if (!pathent_alive(pe)) {
			twarn(ev, "use of dead path '%s'", path);
			/* What to do? */
			abort();
		} else {
			pg = path_lastgen(pe);
			pg->usecnt++;
			compute_pathdep_predelay(ev, pg);
		}
	}

	return path_lastgen(pe)->id;
}

/*
 * Returns the ID of the pathgen produced by 'ev' (since this won't be
 * handling renames, there will be at most one).
 */
static int process_provided_path(struct event* ev, const char* provided)
{
	struct pathent* pe;
	struct pathgen* pg;
	struct pathent* existing;

	/*
	 * FIXME: link(2) should re-use pginfo and bump refcount, not create a
	 * new file.
	 */

	switch (provided_path_type(ev)) {
	case FT_DIR:
		existing = fsm_lookup(traceinfo.fsm, provided, 0);
		if (existing) {
			twarn(ev, "%s provides existing directory",
			      event_type_name(ev->type));
			pe = existing;
		} else
			pe = fsm_mkdir(traceinfo.fsm, provided, 0);
		break;

	case FT_FILE:
		existing = fsm_lookup(traceinfo.fsm, provided, FSM_FOLLOW);
		if (existing)
			pe = existing;
		else {
			/* FIXME: open(O_CREAT) follows symlinks */
			pe = fsm_creat(traceinfo.fsm, provided, 0, 0);
		}
		break;

	case FT_SYMLINK:
		existing = fsm_lookup(traceinfo.fsm, provided, 0);
		if (existing) {
			twarn(ev, "%s provides existing symlink",
			      event_type_name(ev->type));
			pe = existing;
		} else
			pe = fsm_symlink(traceinfo.fsm,
			                 xstrdup(evattr(symlink, ev, oldpath)),
			                 provided, 0);
		break;

	default:
		abort();
	}

	pg = path_lastgen(pe);

	ge_assoc_insert(&deptrack.pathdep.creator, pg, ev);

	return pg->id;
}

static char* trace_cwd = NULL;

/*
 * Fix up the path at *p, which may be relative to the trace's CWD, to
 * be absolute.
 */
static void fix_relative_path(const struct event* ev, char** p)
{
	char* newpath;

	/*
	 * "Relative to the root directory, let it be..."
	 * (With apologies to John Lennon.)
	 */
	if (**p == '/')
		return;

	if (!trace_cwd) {
		fprintf(stderr, "relative path with unknown CWD! (line %d)\n",
		        ev->traceline);
		exit(1);
	}

	if (asprintf(&newpath, "%s/%s", trace_cwd, *p) < 1) {
		perror("asprintf");
		abort();
	}

	xfree(*p);
	*p = newpath;
}

static void fix_event_relative_paths(struct event* ev)
{
	char** p1;
	char** p2;

	/* HACK: symlink special-cased */
	if (ev->type == EV_SYMLINK)
		fix_relative_path(ev, &evattr(symlink, ev, newpath));
	else {
		event_get_raw_paths(ev, &p1, &p2);
		if (p1)
			fix_relative_path(ev, p1);
		if (p2)
			fix_relative_path(ev, p2);
	}
}

static void process_paths(struct event* ev)
{
	char* usedpaths[EVENT_MAXPATHS];
	int follows[EVENT_MAXPATHS];
	char* removedpath;
	char* providedpath;
	char* negdep;
	struct pathent* pe;
	struct pathgen* pg;
	int i, id;

	/*
	 * If it failed in the original execution, we don't care if it fails
	 * when we replay it too.  (In fact, it probably *should* fail.)
	 */
	if (!event_should_succeed(ev)) {
		ev->paths.consumed = NULL;
		ev->paths.produced = NULL;
		ev->paths.removed = NULL;
		ev->paths.negdep = NULL;
		return;
	}

	/*
	 * rename is special-cased to handle the possibility of a directory
	 * rename affecting a large number of paths.  FIXME: symlinks to
	 * directories could do the same thing, should be handled similarly.
	 */
	if (ev->type == EV_RENAME) {
		process_rename_paths(ev);
		return;
	}

	providedpath = event_get_paths(ev, usedpaths, follows, &removedpath, &negdep);

	if (negdep) {
		ev->paths.negdep = process_negdep(ev, negdep);
		xfree(negdep);
	} else
		ev->paths.negdep = NULL;

	ev->paths.consumed = xmalloc(EVENT_MAXPATHS * sizeof(*ev->paths.consumed));
	for (i = 0; i < EVENT_MAXPATHS && usedpaths[i]; i++) {
		ev->paths.consumed[i] = process_path_use(ev, usedpaths[i],
		                                         follows[i]);
		xfree(usedpaths[i]);
	}
	ev->paths.consumed[i] = -1;

	if (providedpath) {
		/*
		 * In cases where an event creates a path that did not
		 * previously exist but does not have a true negdep on it
		 * (e.g. open(O_CREAT & ~O_EXCL)), we insert an artificial
		 * negdep on it.  This makes playback truer to the trace
		 * (though not necessarily the original application), but
		 * perhaps more importantly allows certain event patterns to
		 * play back *correctly*: for example, in many of the iphoto_*
		 * ibench traces, lots of threads have matched pairs of
		 * open(O_CREAT) and unlink() calls, without O_EXCL.  If
		 * things get ordered such that these pairs get interleaved,
		 * we'll end up with two consecutive creates (both of which
		 * succeed, including the second, since there's no O_EXCL),
		 * followed by two unlinks, the second of which will fail.  By
		 * pretending it actually *did* specify O_EXCL, we can prevent
		 * this problem from happening.
		 */
		if (!ev->paths.negdep
		    && (pe = fsm_lookup(traceinfo.fsm, providedpath, FSM_DEADOK))
		    && !pathent_alive(pe))
			ev->paths.negdep = ge_assoc_lookup(&deptrack.pathdep.destroyer,
			                                   path_lastgen(pe));

		id = process_provided_path(ev, providedpath);
		ev->paths.produced = singleton_list(id);
		xfree(providedpath);
	} else
		ev->paths.produced = NULL;

	if (removedpath) {
		pe = fsm_remove(traceinfo.fsm, removedpath);
		xfree(removedpath);
		pg = path_lastgen(pe);
		ev->paths.removed = singleton_list(pg->id);
		ge_assoc_insert(&deptrack.pathdep.destroyer, pg, ev);
	} else
		ev->paths.removed = NULL;
}

static void append_threadtrace_event(struct threadtrace* thread, struct event* ev)
{
	ev->thread_evidx = thread->length++;
	ev->next = NULL;

	if (!thread->evlist.head) {
		ev->prev = NULL;
		thread->evlist.head = thread->evlist.tail = ev;
	} else {
		thread->evlist.tail->next = ev;
		ev->prev = thread->evlist.tail;
		thread->evlist.tail = ev;
	}
}

static void add_spawnevent(struct threadtrace* parent, int childtid, int serial)
{
	struct event* se;

	se = xmalloc(sizeof(*se));

	se->traceline = -1;
	se->serial = serial;
	se->predelays = DEPDELAY_INIT;
	se->cpu_util = 0.0;
	se->type = EV__THREADSPAWN;
	se->ret.i = 0;
	se->time.entry = se->time.exit = parent->lastreturn;
	se->thread = parent->id;
	se->subtype = ST__INVALID_;
	se->next = NULL;
	se->attrs.threadspawn.tid = childtid;
	se->paths.consumed = NULL;
	se->paths.produced = NULL;
	se->paths.removed = NULL;
	se->paths.negdep = NULL;
	se->files = NULL;
	se->temporal_deps = NULL;

	append_threadtrace_event(parent, se);

	traceinfo.event_type_counts[EV__THREADSPAWN] += 1;
}

/*
 * Record, explicitly, that 'second' depends (temporally) on 'first'.
 */
static void record_event_dep(struct event* second, struct event* first)
{
	struct event_dep* dep = xmalloc(sizeof(*dep));

	dep->event = first;
	dep->next = second->temporal_deps;
	second->temporal_deps = dep;
}

struct event_temporal_dep_info {
	struct event* event;
	struct threadtrace* thread;
};

static void check_event_dep(int tid, struct threadtrace* otherthread, void* varg)
{
	struct event* otherevent;
	struct event* otherthread_lastseen;
	struct event_temporal_dep_info* args = varg;
	struct threadtrace* ourthread = args->thread;
	struct event* ev = args->event;

	/* Find the last event in otherthread that completed before 'ev'. */
	for (otherevent = otherthread->evlist.tail;
	     otherevent && otherevent->time.exit >= ev->time.entry;
	     otherevent = otherevent->prev);

	/* I don't *think* this should ever occur...assert? */
	if (!otherevent)
		return;

	/*
	 * Look up the last event from otherthread that a thread in ourthread
	 * temporally depended on.
	 */
	otherthread_lastseen = ie_assoc_lookup(&ourthread->thread_temporal_deps, tid);

	/*
	 * If the most recent temporal dependency in otherthread is *not* the
	 * last one depended upon in that thread by this thread, we need to
	 * record it and update the our threadtrace's info on that.
	 */
	if (otherevent != otherthread_lastseen && otherthread != ourthread) {
		if (otherthread_lastseen) {
			//TODO: MANEL
			//printf ("tid=<%d> otherevent.evidx=<%d> otherthread_lastseen.evidx=<%d> ev.evidx=<%d>\n",
			//		tid, otherevent->thread_evidx, otherthread_lastseen->thread_evidx,
			//		ev->thread_evidx);

			//printf ("tid=<%d> otherevent.time.entry=<%ld> otherthread_lastseen.time.exit=<%ld>\n",
			//		tid, otherevent->time.entry, otherthread_lastseen->time.exit);

			//TODO: MANEL
			assert(otherevent->time.entry > otherthread_lastseen->time.exit);
			//assert(otherevent->time.entry >= otherthread_lastseen->time.exit);
		}
		record_event_dep(ev, otherevent);
		if (otherthread_lastseen)
			ie_assoc_delete(&ourthread->thread_temporal_deps, tid);
		ie_assoc_insert(&ourthread->thread_temporal_deps, tid, otherevent);
	}
}

static void record_event_deps(struct event* ev, struct threadtrace* thread)
{
	struct event_temporal_dep_info arg = {
		.event = ev,
		.thread = thread,
	};

	it_assoc_visit(&traceinfo.threadassoc, check_event_dep, &arg);
}

static void process_event(struct event* ev, struct threadtrace* thread)
{
	size_t bufsize;

	set_event_cpu_util(ev);

	if (ev->predelays.thread < 0)
		ev->predelays.thread = 0;

	bufsize = event_getiobufsize(ev);
	if (bufsize > thread->maxbuf)
		thread->maxbuf = bufsize;

	fix_event_relative_paths(ev);
	if (ev->type == EV_CHDIR && event_should_succeed(ev))
		trace_cwd = ev->attrs.chdir.path;
	else if (ev->type == EV_FCHDIR && event_should_succeed(ev))
		twarn(ev, "unhandled fchdir()");

	/*
	 * Updates our fsmodel, which is used later in
	 * tracking what fds refer to what files.
	 */
	process_paths(ev);

	/*
	 * Doing this before process_posix_aio() is important,
	 * because the fd number gets copied into the
	 * generated aio_info struct.  If we did this after,
	 * it wouldn't get updated with the renamed fd number.
	 */
	event_visit_fds(ev, process_used_fd, NULL);

	ev_alloc_files(ev);

	process_used_files(ev);

	switch (ev->type) {
	case EV_AIO_READ:
	case EV_AIO_RETURN:
	case EV_AIO_ERROR:
		process_posix_aio(ev);
		break;

	case EV_IO_SETUP:
		process_linux_aio_setup(ev);
		break;

	case EV_IO_SUBMIT:
		process_linux_aio_submit(ev);
		break;

	case EV_IO_GETEVENTS:
		process_linux_aio_getevents(ev);
		break;

	case EV_IO_DESTROY:
		process_linux_aio_destroy(ev);
		break;

	default:
		ev->predelays.aiodep = -1;
		break;
	}

	record_event_deps(ev, thread);

	traceinfo.event_type_counts[ev->type] += 1;

	if (event_returns_fd(ev) || ev->type == EV_SOCKETPAIR
	    || ev->type == EV_PIPE) {
		process_new_fds(ev);
		process_new_files(ev);
	}
}

static void output_thread_log(int tid, struct threadtrace* thread, void* arg)
{
	const struct event* e;

	printf("thread_log(%d):", tid);

	for (e = thread->evlist.head; e; e = e->next) {
		if (e->type == EV__THREADSPAWN)
			continue;
		printf(" %s:%"PRInst":%"PRInst, event_type_name(e->type),
		       e->time.entry - traceinfo.time.start,
		       e->time.exit - e->time.entry);
	}

	printf("\n");
}

static void print_dep_edge(struct event* ev, struct event* dep, const char* style)
{
	printf("\ts%d -> s%d [%s];\n", dep->serial, ev->serial, style);
}

static void print_dep(struct event* ev, struct event* dep, const char* style)
{
	if (dep->thread != SETUP_TID
	    && (dep->thread != ev->thread || include_intrathread_deps))
		print_dep_edge(ev, dep, style);
}

#define TEMPORAL_EDGESTYLE "style=dashed"
#define ARTC_EDGESTYLE "style=bold"
#define THREAD_EDGESTYLE "color=grey"

static int print_event_fd_deps(struct event* ev, struct fduse* fd, void* arg)
{
	if (fd->provider)
		print_dep(ev, fd->provider, ARTC_EDGESTYLE);

	if (fd->lastuse)
		print_dep(ev, fd->lastuse, ARTC_EDGESTYLE);

	return 0;
}

static void print_depgraph_deps(struct event* ev)
{
	int i;
	struct event_dep* dep;

	if (ev->thread == SETUP_TID)
		return;

	printf("\ts%d [label=\"%s\\n%d\",pos=\"%.1f,%.1f\"];\n", ev->serial,
	       event_type_name(ev->type), ev->traceline,
	       ((double)ev->serial * 125.0), (double)ev->thread * 100.0);

	event_visit_fds(ev, print_event_fd_deps, NULL);

	if (ev->files) {
		for (i = 0; ev->files[i].pgi; i++) {
			if (ev->files[i].lastuse)
				print_dep(ev, ev->files[i].lastuse, ARTC_EDGESTYLE);
		}
	}

	if (ev->paths.negdep)
		print_dep(ev, ev->paths.negdep, ARTC_EDGESTYLE);

	for (dep = ev->temporal_deps; dep; dep = dep->next)
		print_dep(ev, dep->event, TEMPORAL_EDGESTYLE);

	if (ev->prev)
		print_dep_edge(ev, ev->prev, THREAD_EDGESTYLE);
}

static void start_depgraph(void)
{
	printf("digraph deps {\n");
	printf("\tsplines=\"spline\";\n");
}

static void finish_depgraph(void)
{
	printf("}\n");
}

static struct threadtrace* splitpass(struct event* evlist)
{
	struct event* ev;
	struct event* next;
	struct threadtrace* thread;
	struct threadtrace* prevthread;
	struct threadtrace* firstthread = NULL;
	int callcount = 0, num_elided = 0, serial = 0;

	if (dump_depgraph)
		start_depgraph();

	for (ev = evlist; ev; ev = next) {
		next = ev->next;

		thread = find_thread(ev->thread);

		if (ev->time.entry > 0 && traceinfo.time.start == 0)
			traceinfo.time.start = ev->time.entry;

		if (ev->time.exit > traceinfo.time.end)
			traceinfo.time.end = ev->time.exit;

		switch (ev->subtype) {
		case ST_CALL:
			callcount += 1;

			if (event_elidable(ev)) {
				if (thread->evlist.head)
					thread->lastreturn = ev->time.exit;
				free_event(ev);
				num_elided++;
				break;
			}

			ev->serial = ++serial;

			if (!firstthread && thread != &traceinfo.setupthread)
				firstthread = thread;

			prevthread = prev_thread();

			if (!thread->evlist.head && prevthread) {
				/* This is the first event of a new thread */
				ev->predelays.thread = ev->time.entry \
					- prevthread->lastreturn;
				add_spawnevent(prevthread, ev->thread, ++serial);
			} else {
				/* MANEL - hack here */
				assert(ev->time.entry > thread->lastreturn
				       || ev->thread == SETUP_TID);
				//assert(ev->time.entry >= thread->lastreturn
				//       || ev->thread == SETUP_TID);
				ev->predelays.thread = ev->time.entry \
					- thread->lastreturn;
			}

			process_event(ev, thread);

			/* assign the event to its specific thread */
			append_threadtrace_event(thread, ev);

			thread->lastreturn = ev->time.exit;

			if (dump_depgraph)
				print_depgraph_deps(ev);

			break;

		default:
			abort(); /* FIXME: die more gracefully */
		}
	}

	if (dump_depgraph)
		finish_depgraph();

	/* Don't bother predelaying the very first event */
	firstthread->evlist.head->predelays = DEPDELAY_INIT;

	if (stats_enabled.log)
		it_assoc_visit(&traceinfo.threadassoc, output_thread_log, NULL);

	if (stats_enabled.summary) {
		printf("orig_syscalls: %d\n", callcount);
		printf("elided_calls: %d\n", num_elided);
		printf("num_threads: %d\n", traceinfo.numthreads);
		printf("num_fds: %d\n", traceinfo.num_fds);
		if (traceinfo.cpucount)
			printf("num_cpus: %lu\n", traceinfo.cpucount);
		printf("elapsed_time: %"PRInst"\n",
		       traceinfo.time.end - traceinfo.time.start);
	}

	return firstthread;
}

static void free_initents(struct initent* ents)
{
	struct initent* ie;
	struct initent* next;

	for (ie = ents; ie; ie = next) {
		next = ie->next;
		xfree(ie->path);
		if (ie->type == FT_SYMLINK)
			xfree(ie->target);
		xfree(ie);
	}
}

static void free_evlist(struct event* list)
{
	struct event* ev;
	struct event* next;

	for (ev = list; ev; ev = next) {
		next = ev->next;
		free_event(ev);
	}
}

static void free_threadtrace(int tid, struct threadtrace* t, void* arg)
{
	free_evlist(t->evlist.head);
	ie_assoc_dealloc(&t->thread_temporal_deps, NULL, NULL);
	xfree(t);
}

static void free_events(void)
{
	it_assoc_visit(&traceinfo.threadassoc, free_threadtrace, NULL);
	if (traceinfo.setupthread.evlist.head)
		free_evlist(traceinfo.setupthread.evlist.head);
}

/* Parser debugging options */
enum {
	PD_TRACE = 0,
	PD_INITLIST,
	PD_IOSTAT,
};

static char* const parser_debug_opts[] = {
	[PD_TRACE]    = "trace",
	[PD_INITLIST] = "initlist",
	[PD_IOSTAT]   = "iostat",
	NULL,
};

/* Info-extraction-mode options */
enum {
	EX_DEPGRAPH = 0,
	EX_THREADLOGS,
	EX_SUMMARY,
};

static char* const extract_opts[] = {
	[EX_DEPGRAPH]   = "depgraph",
	[EX_THREADLOGS] = "thread_logs",
	[EX_SUMMARY]    = "summary",
	NULL,
};

static void usage(FILE* out, int status)
{
	int i;

	fprintf(out, "Usage: artc [FLAGS] TRACEFILE INITFILE OUTDIR\n"
	        "\t-A, --extract=INFO: just extract various information, no codegen (omit OUTDIR):\n");

	for (i = 0; extract_opts[i]; i++)
		fprintf(out, "\t\t%s\n", extract_opts[i]);

	fprintf(out,
	        "\t-a, --all-initfiles: include all files from initlist in output\n"
	        "\t-d, --debug=PARSERS: enable parser debugging for:\n");

	for (i = 0; parser_debug_opts[i]; i++)
		fprintf(out, "\t\t%s\n", parser_debug_opts[i]);

	fprintf(out,
	        "\t-h, --help: display this help message\n"
	        "\t-I, --ibench: iBench trace mode\n"
	        "\t-i, --iostat=IOSTAT: parse IOSTAT (iostat output file) for CPU information\n"
	        "\t-S, --strace: strace mode (default)\n"
	        "\t-t, --intra-thread: include intra-thread dependencies in output\n");

	exit(status);
}

static void set_parser_debug_flags(char* optstr)
{
	char* value;

	while (*optstr) {
		switch (getsubopt(&optstr, parser_debug_opts, &value)) {
		case PD_TRACE:
			stracedebug = 1;
			ibenchdebug = 1;
			break;

		case PD_INITLIST:
			initdebug = 1;
			ibench_initdebug = 1;
			break;

		case PD_IOSTAT:
			iostatdebug = 1;
			break;

		default:
			fprintf(stderr, "Unrecognized parser-debug option: %s\n", value);
			usage(stderr, 1);
		}
	}
}

static void set_extract_debug_flags(char* optstr)
{
	char* value;

	while (*optstr) {
		switch (getsubopt(&optstr, extract_opts, &value)) {
		case EX_DEPGRAPH:
			dump_depgraph = 1;
			break;

		case EX_THREADLOGS:
			stats_enabled.log = 1;
			break;

		case EX_SUMMARY:
			stats_enabled.summary = 1;
			break;

		default:
			fprintf(stderr, "Unrecognized static analysis option: %s\n", value);
			usage(stderr, 1);
		}
	}
}

static struct {
	struct event* (*parse)(FILE* tracefile);
	struct initent* (*initparse)(FILE* initfile);
	void (*teardown)(void);
} frontends[] = {
	[TT_IBENCH] = { .parse = parse_ibench_trace,
	                .initparse = parse_ibench_init,
	                .teardown = ibench_parser_teardown, },
	[TT_STRACE] = { .parse = parse_strace,
	                .initparse = parse_init,
	                .teardown = strace_parser_teardown, },
};

static const struct option long_opts[] = {
	{ "extract",       required_argument, NULL, 'A', },
	{ "all-initfiles", no_argument,       NULL, 'a', },
	{ "debug",         required_argument, NULL, 'd', },
	{ "help",          no_argument,       NULL, 'h', },
	{ "ibench",        no_argument,       NULL, 'I', },
	{ "iostat",        required_argument, NULL, 'i', },
	{ "strace",        no_argument,       NULL, 'S', },
	{ "intra-thread",  no_argument,       NULL, 't', },
	{ NULL, 0, NULL, 0, },
};

int main(int argc, char** argv)
{
	int opt, status;
	struct event* evlist;
	FILE* tracefile;
	FILE* initfile;
	const char* outdir;
	const struct threadtrace* firstthread;
	FILE* iostatfile = NULL;
	const char* iostatpath = NULL;
	struct initent* initpaths = NULL;

	while ((opt = getopt_long(argc, argv, "A:ad:hIi:St", long_opts, NULL)) != -1) {
		switch (opt) {
		case 'A':
			statsonly = 1;
			set_extract_debug_flags(optarg);
			break;

		case 'a':
			all_init_files = 1;
			break;

		case 'd':
			set_parser_debug_flags(optarg);
			break;

		case 'h':
			usage(stdout, 0);

		case 'I':
			traceinfo.type = TT_IBENCH;
			break;

		case 'i':
			iostatpath = optarg;
			break;

		case 'S':
			traceinfo.type = TT_STRACE;
			break;

		case 't':
			include_intrathread_deps = 1;
			break;

		case '?':
			usage(stderr, 1);
		}
	}

	if ((!statsonly && optind != argc - 3)
	    || (statsonly && optind != argc - 2))
		usage(stderr, 1);

	traceinfo.path = argv[optind];
	tracefile = xfopen(traceinfo.path, "r");

	traceinfo.initfile_path = argv[optind+1];
	initfile = xfopen(traceinfo.initfile_path, "r");

	if (iostatpath)
		iostatfile = xfopen(iostatpath, "r");

	if (!statsonly) {
		outdir = argv[optind+2];

		status = mkdir(outdir, 0755);
		if (status == -1 && errno != EEXIST) {
			fprintf(stderr, "failed to create output directory '%s': %s\n",
			        outdir, strerror(errno));
			exit(1);
		}

		if (chdir(outdir)) {
			fprintf(stderr, "failed to chdir() to '%s': %s\n",
			        outdir, strerror(errno));
			exit(1);
		}
	}

	initpaths = frontends[traceinfo.type].initparse(initfile);
	fclose(initfile);
	if (!initpaths) {
		fprintf(stderr, "error reading init file %s\n",
		        traceinfo.initfile_path);
		exit(1);
	}

	evlist = frontends[traceinfo.type].parse(tracefile);
	frontends[traceinfo.type].teardown();

	fclose(tracefile);
	if (!evlist) {
		fprintf(stderr, "error reading trace file %s\n", traceinfo.path);
		exit(1);
	}

	if (iostatfile) {
		cpu.samples = parse_iostat(iostatfile, &cpu.num_samples, &traceinfo.cpucount);
		if (!cpu.samples) {
			fprintf(stderr, "error reading iostat file %s\n", iostatpath);
			exit(1);
		}
		compute_avg_cpu();
		fclose(iostatfile);
	}

	traceinfo.fsm = new_fsmodel();
	traceinfo.fsm->record_uses = 0;
	record_init_paths(initpaths);
	free_initents(initpaths);

	traceinfo.fsm->record_uses = 1;
	firstthread = splitpass(evlist);
	traceinfo.fsm->record_uses = 0;

	if (!statsonly)
		generate_code(firstthread);

	xfree(cpu.samples);
	xfree(traceinfo.fdidx_evcounts);
	ii_assoc_dealloc(&fdremap_assoc, NULL, NULL);
	ie_assoc_dealloc(&traceinfo.mystery_fds, NULL, NULL);
	pip_assoc_dealloc(&aiocb_assoc, NULL, NULL);

	free_events();
	fsm_free(traceinfo.fsm);
	it_assoc_dealloc(&traceinfo.threadassoc, NULL, NULL);
	ip_assoc_dealloc(&traceinfo.fd_pgi_map, NULL, NULL);
	pi_assoc_dealloc(&aio_origfds, NULL, NULL);
	ii_assoc_dealloc(&traceinfo.pathidx_remap, NULL, NULL);
	pi_assoc_dealloc(&traceinfo.aioctx_assoc, NULL, NULL);
	ii_assoc_dealloc(&traceinfo.mystery_aioctxs, NULL, NULL);

	ie_assoc_dealloc(&deptrack.fddep, NULL, NULL);
	ie_assoc_dealloc(&deptrack.fdorder, NULL, NULL);
	fe_assoc_dealloc(&deptrack.fileorder, NULL, NULL);
	pe_assoc_dealloc(&deptrack.aiodep, NULL, NULL);
	ge_assoc_dealloc(&deptrack.pathdep.creator, NULL, NULL);
	ge_assoc_dealloc(&deptrack.pathdep.destroyer, NULL, NULL);

	return 0;
}
