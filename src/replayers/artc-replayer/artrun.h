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

#ifndef ARTRUN_H
#define ARTRUN_H

#include "system.h"

#include <pthread.h>

#ifdef OSX
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#include "trace.h"
#include "flags.h"

#ifdef OSX
#define FSYNC_MAX 2.0
#else
#define FSYNC_MAX 1.0
#endif

struct artrun_options {
	/* Initialization modes */
	int union_init;
	int lazy_init;

	/* Constraint enforcement */
	struct {
		int aio;
		int fdstage;
		int fdseq;
		int file;
		int path;

		int temporalorder;
		int specialfileorder;
	} constraints;

	int singlethread;

	/* Predelay adjustment */
	int simplepredelay;
	int ignore_cpu_util;
	int use_cpucount;
	double speed_factor;

	/* Debug tracing */
	struct {
		int fds;
		int paths;
		int aio;
		int threads;
	} debug;

	/* Miscellaneous */
	int nosyscalls;
	int randomize_file_content;
	int eager_threadspawn;

	/*
	 * fsync_pt determines the point along the "fsync spectrum" we're
	 * operating at.  The integral part specifies a base index into the
	 * fsync_fns array; the fractional part specifies the probability that
	 * the index will be increased by one before being used to select what
	 * function is going to be called to implement fsync() for this call.
	 *
	 * So, for example, a value of 0.0 would always call fsync_nop
	 * (i.e. skip all fsyncs), 0.75 would skip fsync()s about 1/4 of the
	 * time, 1.0 would specify normal fsync() playback.  On OSX, values up
	 * to 2.0 are allowed, so 1.25 would specify that fsync()s have a 75%
	 * chance of being played back normally and a 25% chance of being
	 * played back as fcntl(F_FULLFSYNC).
	 */
	double fsync_pt;

	int ignore_version_mismatch;
};

extern struct artrun_options options;

#define noreturn __attribute__((noreturn))

noreturn void* threadfunc(void* args);

extern const struct benchmark* bench;

static inline int get_fd(int idx)
{
	/*
	 * The idx < 0 case handles AT_FDCWD, which must be negative or all
	 * hope is lost.  Hence the assert.
	 *
	 * This is something of an ugly hack to handle *at() syscalls
	 * (e.g. openat(2) and friends).  For events that wish to use
	 * AT_FDCWD, codegen outputs them specially, with AT_FDCWD as their
	 * rtidx value (hideously, not even symbolic!), which is then returned
	 * unmodified as the file-descriptor value to use for replay.
	 */
	assert(AT_FDCWD < 0);

	return idx < 0 ? idx : bench->fds[idx].fd;
}

void assign_fd(int idx, int val, resstate_t status);
nstime_t fd_wait_lastevent(int idx);
void resstatus_change(struct resstatus* r, resstate_t s);
resstate_t resstatus_waitlive(struct resstatus* r);

#define aio_debug(...) do { if (options.debug.aio) debug("aio", __VA_ARGS__); } while (0)
#define fd_debug(...) do { if (options.debug.fds) debug("fd", __VA_ARGS__); } while (0)
#define path_debug(...) do { if (options.debug.paths) debug("path", __VA_ARGS__); } while (0)
#define thread_debug(...) do { if (options.debug.threads) debug("thread", __VA_ARGS__); } while (0)

extern pthread_mutex_t warn_lock;

void warn(const char* fmt, ...) __attribute__((format(printf, 1, 2)));
void debug(const char* pfx, const char* fmt, ...) __attribute__((format(printf, 2, 3)));

extern uid_t uid;
extern gid_t gid;

#ifdef OSX
extern mach_timebase_info_data_t mach_timebase;
#endif

/*
 * Return a nanosecond timestamp since some point in the past at
 * whatever the highest resolution the system has available is.
 */
static inline nstime_t getnanos(void)
{
#if defined(LINUX) || defined(FREEBSD) || defined(SUN)
	struct timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC, &ts)) {//MANEL
		perror("clock_gettime");
		exit(1);
	}
	return BILLION * ts.tv_sec + ts.tv_nsec;
#elif defined(OSX)
	uint64_t t = mach_absolute_time();
	return (t * mach_timebase.numer) / mach_timebase.denom;
#else
	struct timeval tv;
	if (gettimeofday(&tv, NULL)) {
		perror("gettimeofday");
		exit(1);
	}
	return BILLION * tv.tv_sec + 1000 * tv.tv_usec;
#endif
}

#endif /* ARTRUN_H */
