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

#ifndef MISC_H
#define MISC_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <pthread.h>

#ifdef ARTC
#include <errno.h>
#endif

#include "system.h"

#ifdef LINUX
#define XATTR_PREFIX "user."
#else
#define XATTR_PREFIX ""
#endif

//MANEL
//#define BILLION 1000000000L
#define BILLION 1000000000ULL

/* Type used for nanosecond timestamps and intervals. */
//MANEL
typedef int64_t nstime_t;
//typedef uint64_t nstime_t;
//MANEL
#define PRInst PRIu64
//#define PRInst PRId64

/* Just an optional string concatenation */
#define xattr(name) XATTR_PREFIX name

#define ARR_SIZE(a) (sizeof(a)/sizeof((a)[0]))

#ifndef offsetof
#define offsetof(t, m) __builtin_offsetof(t, m)
#endif

/* Borrowed from the Linux kernel's include/linux/kernel.h */
#define container_of(ptr, type, member) ({ \
		const typeof(((type*)0)->member) *__mptr = (ptr); \
		(type*)((char*)__mptr - offsetof(type, member)); \
	})

static inline void* xmalloc(size_t s)
{
	void* p = malloc(s);

	if (!p) {
		fprintf(stderr, "malloc failed\n");
		exit(1);
	}

	return p;
}

static inline void* xcalloc(size_t s)
{
	void* p = calloc(1, s);

	if (!p) {
		fprintf(stderr, "malloc failed\n");
		exit(1);
	}

	return p;
}

static inline void* xrealloc(void* p, size_t s)
{
	void* r = realloc(p, s);

	if (!r) {
		fprintf(stderr, "malloc failed\n");
		exit(1);
	}

	return r;
}

static inline void xfree(void* p)
{
	free(p);
}

static inline char* xstrdup(const char* s)
{
	char* p = strdup(s);

	if (!p) {
		fprintf(stderr, "malloc failed\n");
		exit(1);
	}

	return p;
}

#ifdef ARTC
/*
 * Give the relative form of an absolute path, assuming '.' is '/' and
 * p is in some reasonably canonical form.
 */
static inline const char* relpath_from_abs(const char* p)
{
	if (!*p)
		return ".";
	else if (*p != '/')
		return p;
	else
		return p[1] ? p+1 : ".";
}

/*
 * Do some basic path normalization.  For now, this consists of simply
 * stripping off trailing slashes.  Modifies its argument in place and returns
 * it.
 */
static inline char* normalize_path(char* path)
{
	int i = strlen(path) - 1;

	if (strspn(path, "/") == i+1)
		return path;

	while (path[i] == '/')
		path[i--] = '\0';

	return path;
}

static inline FILE* xfopen(const char* path, const char* mode)
{
	FILE* f = fopen(path, mode);

	if (!f) {
		fprintf(stderr, "Failed to open '%s': %s\n", path, strerror(errno));
		exit(1);
	}

	return f;
}

#else

static inline void xlock(pthread_mutex_t* l)
{
	int status = pthread_mutex_lock(l);
	if (status) {
		fprintf(stderr, "pthread_mutex_lock: %s\n", strerror(status));
		abort();
	}
}

static inline void xunlock(pthread_mutex_t* l)
{
	int status = pthread_mutex_unlock(l);
	if (status) {
		fprintf(stderr, "pthread_mutex_unlock: %s\n", strerror(status));
		abort();
	}
}

static inline void xcondwait(pthread_cond_t* restrict cond,
                             pthread_mutex_t* restrict lock)
{
	int status = pthread_cond_wait(cond, lock);
	if (status) {
		fprintf(stderr, "pthread_cond_wait: %s\n", strerror(status));
		abort();
	}
}

static inline void xcondbcast(pthread_cond_t* cond)
{
	int status = pthread_cond_broadcast(cond);
	if (status) {
		fprintf(stderr, "pthread_cond_broadcast: %s\n", strerror(status));
		abort();
	}
}

#endif /* ARTC */

#endif /* MISC_H */
