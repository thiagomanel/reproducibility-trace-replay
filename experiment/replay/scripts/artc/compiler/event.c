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

#define _GNU_SOURCE /* for vasprintf() */
#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>
#include <limits.h>
#include <assert.h>
#ifdef ARTC
#include <sys/mman.h>
#else
#include <fcntl.h>
#endif

#include "trace.h"
#include "misc.h"
#include "event.h"
#include "flags.h"

#ifdef ARTC
#include "fsmodel.h" /* for struct pginfo definition */
#include "codegen.h"
#endif

#ifdef ARTC
static void free_symflags(struct symflag* flags)
{
	struct symflag* f;
	struct symflag* next;

	for (f = flags; f; f = next) {
		next = f->next;
		xfree(f);
	}
}

/* Generate a free function for an *_attrs struct */
#define MAKE_DUMMY_ATTRFREE(name) \
	static void free_##name##_attrs(struct event* ev) { }

#define MAKE_ATTRFREE(name, member) \
	static void free_##name##_attrs(struct event* ev) \
	{ \
		struct name##_attrs* a = &ev->attrs.name; \
		xfree(a->member); \
	}

#define MAKE_ATTRFREE2(name, member1, member2) \
	static void free_##name##_attrs(struct event* ev) \
	{ \
		struct name##_attrs* a = &ev->attrs.name; \
		xfree(a->member1); \
		xfree(a->member2); \
	}

#define MAKE_ATTRFREE3(name, member1, member2, member3) \
	static void free_##name##_attrs(struct event* ev) \
	{ \
		struct name##_attrs* a = &ev->attrs.name; \
		xfree(a->member1); \
		xfree(a->member2); \
		xfree(a->member3); \
	}

MAKE_ATTRFREE(shm_open, name);
MAKE_ATTRFREE(read, dummy1);
MAKE_ATTRFREE(write, dummy1);
MAKE_ATTRFREE(pread, dummy1);
MAKE_ATTRFREE(pwrite, dummy1);
MAKE_ATTRFREE(aio_read, dummy1);
MAKE_ATTRFREE(fsync, dummy1);
MAKE_ATTRFREE(fdatasync, dummy1);
MAKE_ATTRFREE(ftruncate, dummy1);
MAKE_ATTRFREE2(link, oldpath, newpath);
MAKE_ATTRFREE2(symlink, oldpath, newpath);
MAKE_ATTRFREE(rmdir, pathname);
MAKE_ATTRFREE(unlink, pathname);
MAKE_ATTRFREE2(readlink, path, dummy1);
MAKE_ATTRFREE3(rename, oldpath, newpath, dummy1);
MAKE_ATTRFREE2(exchangedata, path1, path2);
MAKE_ATTRFREE(mkdir, pathname);
MAKE_ATTRFREE(flock, dummy1);
MAKE_ATTRFREE(statfs, path);
MAKE_ATTRFREE(statfs64, path);
MAKE_ATTRFREE(lstat, path);
MAKE_ATTRFREE(stat, path);
MAKE_ATTRFREE(lstat64, path);
MAKE_ATTRFREE(stat64, path);
MAKE_ATTRFREE(setattrlist, path);
MAKE_ATTRFREE(getattrlist, path);
MAKE_ATTRFREE3(getxattr, path, name, dummy1);
MAKE_ATTRFREE2(listxattr, path, dummy1);
MAKE_ATTRFREE(chown, path);
MAKE_ATTRFREE(chmod, path);
MAKE_ATTRFREE2(fsetxattr, name, dummy1);
MAKE_ATTRFREE(chdir, path);
MAKE_ATTRFREE2(removexattr, path, name);
MAKE_ATTRFREE(fremovexattr, name);
MAKE_ATTRFREE3(setxattr, path, name, dummy1);
MAKE_ATTRFREE(controller, msg);
MAKE_ATTRFREE(hfs_vnop_pagein, name);
MAKE_ATTRFREE(syscall_map, dummy1);
MAKE_ATTRFREE(posix_spawn, path);
MAKE_ATTRFREE(flistxattr, dummy1);
MAKE_ATTRFREE2(fgetxattr, name, dummy1);
MAKE_ATTRFREE(getcwd, dummy1);

MAKE_ATTRFREE(utimensat, pathname);
MAKE_ATTRFREE(faccessat, pathname);
MAKE_ATTRFREE(newfstatat, pathname);
MAKE_ATTRFREE(fchownat, pathname);
MAKE_ATTRFREE(unlinkat, pathname);
MAKE_ATTRFREE(fadvise64, advice);

static void free_open_attrs(struct event* ev)
{
	struct open_attrs* a = &ev->attrs.open;

	xfree(a->dummy1);
	xfree(a->dummy2);

	xfree(a->path);
	free_symflags(a->flags);
}

static void free_fcntl_attrs(struct event* ev)
{
	struct fcntl_attrs* a = &ev->attrs.fcntl;

	if (!strcmp(a->cmdname, "F_SETFL"))
		free_symflags(a->arg.flags);

	xfree(a->cmdname);
	xfree(a->dummy1);
}

static void free_mmap_attrs(struct event* ev)
{
	struct mmap_attrs* a = &ev->attrs.mmap;

	free_symflags(a->flags);
	free_symflags(a->prot);
}

static void free_access_attrs(struct event* ev)
{
	struct access_attrs* a = &ev->attrs.access;

	xfree(a->pathname);
	if (traceinfo.type == TT_STRACE)
		free_symflags(a->flags);
}

static void free_io_submit_attrs(struct event* ev)
{
	struct artc_iocb* i;
	struct artc_iocb* tmp;
	struct io_submit_attrs* a = &ev->attrs.io_submit;

	for (i = a->iocbs; i; i = tmp) {
		tmp = i->next;
		xfree(i->dummy);
		xfree(i);
	}
}

static void free_io_getevents_attrs(struct event* ev)
{
	struct artc_ioevent* i;
	struct artc_ioevent* tmp;
	struct io_getevents_attrs* a = &ev->attrs.io_getevents;

	for (i = a->ioevents; i; i = tmp) {
		tmp = i->next;
		xfree(i);
	}
}

static void output_symflags(const struct symflag* s, FILE* out)
{
	struct symflag* f;

	if (!s) {
		fprintf(out, "0");
		return;
	}

	if (!strcmp(s->name, "MAP_FIXED")
	    || !strcmp(s->name, "O_EXLOCK"))
		fprintf(out, "0");
	else
		fprintf(out, "%s", s->name);

	for (f = s->next; f; f = f->next) {
		if (!strcmp(f->name, "MAP_FIXED"))
			fprintf(out, "|0");
		else
			fprintf(out, "|%s", f->name);
	}
}

static inline char hexchar(unsigned char n)
{
	return n < 10 ? ('0' + n) : ('A' + (n - 0xA));
}

/*
 * Could be potentially a bit more efficient by realloc()ing
 * 'formatted' and doing an end-back copy converting non-printing
 * chars to escape sequences.
 *
 * All this crap because some doofus at Apple apparently decided to
 * put a carriage return in a file name (WTF??)...sigh.
 */
static __attribute__((format(printf, 1, 2))) char* fmt_esc_string(const char* fmt, ...)
{
	char* formatted;
	char* escaped;
	char* dst;
	char* src;
	int esc_chars = 0, i;
	va_list va;

	va_start(va, fmt);
	if (vasprintf(&formatted, fmt, va) < 0) {
		fprintf(stderr, "vasprintf failed\n");
		exit(1);
	}
	va_end(va);

	if (!formatted) {
		fprintf(stderr, "malloc failed\n");
		exit(1);
	}

	for (i = 0; formatted[i]; i++) {
		switch (formatted[i]) {
		case '\r':
		case '\n':
		case '\v':
		case '\f':
		case '\t':
			esc_chars++;
			break;

		default:
			break;
		}
	}

	escaped = xmalloc(strlen(formatted) + esc_chars + 1);

	for (dst = escaped, src = formatted; *src; src++) {
		switch (*src) {
		case '\r':
			*dst++ = '\\';
			*dst++ = 'r';
			break;

		case '\n':
			*dst++ = '\\';
			*dst++ = 'n';
			break;

		case '\v':
			*dst++ = '\\';
			*dst++ = 'v';
			break;

		case '\f':
			*dst++ = '\\';
			*dst++ = 'f';
			break;

		case '\t':
			*dst++ = '\\';
			*dst++ = 't';
			break;

		default:
			*dst++ = *src;
		}
	}
	*dst = '\0';

	free(formatted);
	return escaped;
}

/*
 * OSX traces use shm names like "/tmp/foobar", but this doesn't work
 * on Linux unless /dev/shm/tmp exists (which it generally doesn't).
 * Further, Linux's man page for shm_open() specifies that for
 * portability, shm names should take the form "/somename", where
 * somename does not include any additional slashes, so here we
 * overwrite slashes after the first character with underscores.  Note
 * that this doesn't necessarily conform to the "/somename" format if
 * the original name didn't start with a slash, but it works on both
 * systems anyway, so out of laziness I'm leaving it like that, for
 * now at least.  Note also that it modifies the string in place and
 * then returns it (for convenience in the MAKE_ARGGEN() macros
 * below), but it's idempotent, so it shouldn't really matter anyway.
 */
static char* sanitize_shm_name(char* name)
{
	int i;

	if (!*name)
		return name;

	for (i = 1; name[i]; i++) {
		if (name[i] == '/')
			name[i] = '_';
	}

	return name;
}

/* FIXME: this should probably go somewhere else. */
static int fd_is_regular_file(const struct fduse* fd)
{
	struct pginfo* pgi;

	if (fd->rtidx < 0)
		return 0;

	pgi = ip_assoc_lookup(&traceinfo.fd_pgi_map, fd->rtidx);
	assert(pgi);
	return pgi != NONFILE_PGINFO;
}

#define FDFMT(name) "."name" = { .rtidx = %d, .provider = "EVPTR_FMT", .lastuse = " \
	EVPTR_FMT", .nonfile = %d, }"
/*
 * HACK: this is pretty gross (generated code should have symbolic AT_FDCWD,
 * not integer literal.
 */
#define FDARGS(ev, fd) (((fd).traceval == AT_FDCWD) ? AT_FDCWD : (fd).rtidx), \
		EVDEP_ARGS((ev), (fd).provider), EVDEP_ARGS((ev), (fd).lastuse), \
		!fd_is_regular_file(&(fd))

static void open_genargs(const struct event* ev, FILE* output)
{
	const struct open_attrs* a = &ev->attrs.open;

	fprintf(output, " .path = \"%s\", .flags = ", relpath_from_abs(a->path));
	output_symflags(a->flags, output);
	fprintf(output, ", .mode = 0700, ");
}

static void mmap_genargs(const struct event* ev, FILE* output)
{
	const struct mmap_attrs* a = &ev->attrs.mmap;

	fprintf(output, " .addr = (void*)%#lx, .length = %zd, .prot = ",
	        (unsigned long)a->addr, a->length);
	output_symflags(a->prot, output);
	fprintf(output, ", .flags = ");
	output_symflags(a->flags, output);
	fprintf(output, ", "FDFMT("fd")", .offset = %jd, ", FDARGS(ev, a->fd),
	        (intmax_t)a->offset);
}

static void access_genargs(const struct event* ev, FILE* output)
{
	char* tmp;
	const struct access_attrs* a = &ev->attrs.access;

	tmp = fmt_esc_string(".pathname = \"%s\", .mode = ",
	                     relpath_from_abs(a->pathname));
	fprintf(output, " %s ", tmp);
	xfree(tmp);
	if (traceinfo.type == TT_IBENCH)
		fprintf(output, "%d", a->mode);
	else
		output_symflags(a->flags, output);
}

static void faccessat_genargs(const struct event* ev, FILE* output)
{
	char* tmp;
	const struct faccessat_attrs* a = &ev->attrs.faccessat;

	tmp = fmt_esc_string(FDFMT("dirfd")", .pathname = \"%s\", "
	                     ".mode = ", FDARGS(ev, a->dirfd),
	                     relpath_from_abs(a->pathname));
	fprintf(output, " %s ", tmp);
	xfree(tmp);
	output_symflags(a->mode, output);
}

static void symlink_genargs(const struct event* ev, FILE* output)
{
	char* tmp;
	const char* oldrel;
	int i;
	const struct symlink_attrs* a = &ev->attrs.symlink;

	fprintf(output, " .oldpath = \"");
	/*
	 * Prepend a "../" for each separator ('/') in the path
	 * (relativize it).
	 */
	oldrel = relpath_from_abs(a->oldpath);
	for (i = 0; oldrel[i]; i++) {
		if (oldrel[i] == '/')
			fprintf(output, "../");
	}
	tmp = fmt_esc_string("%s\", .newpath = \"%s\" ", oldrel,
	                     relpath_from_abs(a->newpath));
	fprintf(output, "%s", tmp);
	xfree(tmp);
}

static void utimensat_genargs(const struct event* ev, FILE* output)
{
	char* escapedargs;
	const struct utimensat_attrs* a = &ev->attrs.utimensat;

	/* This can legitimately be NULL (see Linux utimensat(2) man page). */
	if (a->pathname)
		escapedargs = fmt_esc_string(FDFMT("dirfd")",  .pathname = \"%s\", "
		                             ".times = &(struct timespec[2]){}, "
		                             ".flags = %d", FDARGS(ev, a->dirfd),
		                             relpath_from_abs(a->pathname), a->flags);
	else
		escapedargs = fmt_esc_string(FDFMT("dirfd")",  .pathname = NULL, "
		                             ".times = &(struct timespec[2]){}, "
		                             ".flags = %d", FDARGS(ev, a->dirfd), a->flags);

	fprintf(output, " %s ", escapedargs);
	xfree(escapedargs);
}

static void fcntl_genargs(const struct event* ev, FILE* output)
{
	const struct fcntl_attrs* a = &ev->attrs.fcntl;
	fprintf(output, " "FDFMT("fd")", .cmd = %s, .arg = ", FDARGS(ev, a->fd),
	        a->cmdname);
	if (!strcmp(a->cmdname, "F_SETFL"))
		output_symflags(a->arg.flags, output);
	else
		fprintf(output, "%ld", a->arg.l);
	fprintf(output, " ");
}

#define MAKE_ARGGEN(name, fmt, ...) \
	static void name##_genargs(const struct event* e, FILE* output) \
	{ \
		char* escapedargs; \
		const struct name##_attrs* a = &e->attrs.name; \
		(void)a; /* avoid unused-variable warning */ \
		escapedargs = fmt_esc_string(fmt, ##__VA_ARGS__); \
		fprintf(output, " %s ", escapedargs); \
		xfree(escapedargs); \
	}

#define a2r relpath_from_abs
MAKE_ARGGEN(shm_open, ".name = \"%s\", .oflag = O_CREAT|%d", sanitize_shm_name(a->name),
            a->oflag);
MAKE_ARGGEN(close, FDFMT("fd"), FDARGS(e, a->fd));
MAKE_ARGGEN(dup, FDFMT("fd"), FDARGS(e, a->fd));
MAKE_ARGGEN(kqueue, "");
MAKE_ARGGEN(socket, ".domain = %d, .type = %d, .protocol = %d", a->domain, a->type,
            a->protocol);
MAKE_ARGGEN(socketpair, ".domain = %d, .type = %d, .protocol = %d, "FDFMT("fd1")", "FDFMT("fd2"),
            a->domain, a->type, a->protocol, FDARGS(e, a->fd1), FDARGS(e, a->fd2));
MAKE_ARGGEN(read, FDFMT("fd")", .iobuf = thread%d_iobuf, .count = %zd", FDARGS(e, a->fd),
            e->thread, a->count);
MAKE_ARGGEN(write, FDFMT("fd")", .iobuf = thread%d_iobuf, .count = %zd", FDARGS(e, a->fd),
            e->thread, a->count);
MAKE_ARGGEN(pread, FDFMT("fd")", .iobuf = thread%d_iobuf, .count = %zd, .offset = %jd",
            FDARGS(e, a->fd), e->thread, a->count, (intmax_t)a->offset);
MAKE_ARGGEN(pwrite, FDFMT("fd")", .iobuf = thread%d_iobuf, .count = %zd, .offset = %jd",
            FDARGS(e, a->fd), e->thread, a->count, (intmax_t)a->offset);
MAKE_ARGGEN(aio_read, FDFMT("fd")", .infoidx = %d", FDARGS(e, a->fd), a->aiocbnum);
MAKE_ARGGEN(aio_error, FDFMT("fd")", .infoidx = %d", FDARGS(e, a->fd), a->aiocbnum);
MAKE_ARGGEN(aio_return, FDFMT("fd")", .infoidx = %d", FDARGS(e, a->fd), a->aiocbnum);
MAKE_ARGGEN(aio_suspend, "");
MAKE_ARGGEN(lseek, FDFMT("fd")", .offset = %jd, .whence = %d", FDARGS(e, a->fd),
            (intmax_t)a->offset, a->whence);
MAKE_ARGGEN(fsync, FDFMT("fd"), FDARGS(e, a->fd));
MAKE_ARGGEN(fdatasync, FDFMT("fd"), FDARGS(e, a->fd));
MAKE_ARGGEN(ftruncate, FDFMT("fd")", .length = %jd", FDARGS(e, a->fd), (intmax_t)a->length);
MAKE_ARGGEN(link, ".oldpath = \"%s\", .newpath = \"%s\"", a2r(a->oldpath), a2r(a->newpath));
MAKE_ARGGEN(rmdir, ".pathname = \"%s\"", a2r(a->pathname));
MAKE_ARGGEN(unlink, ".pathname = \"%s\"", a2r(a->pathname));
MAKE_ARGGEN(readlink, ".path = \"%s\", .bufsiz = %zd, .iobuf = thread%d_iobuf",
            a2r(a->path), a->bufsiz, e->thread);
MAKE_ARGGEN(rename, ".oldpath = \"%s\", .newpath = \"%s\"", a2r(a->oldpath),
            a2r(a->newpath));
MAKE_ARGGEN(exchangedata, ".path1 = \"%s\", .path2 = \"%s\"", a2r(a->path1),
            a2r(a->path2));
MAKE_ARGGEN(mkdir, ".pathname = \"%s\", .mode = 0%o", a2r(a->pathname), a->mode);
MAKE_ARGGEN(flock, FDFMT("fd")", .operation = %d", FDARGS(e, a->fd), a->operation);
MAKE_ARGGEN(futimes, FDFMT("fd"), FDARGS(e, a->fd));
MAKE_ARGGEN(utimes, "");
MAKE_ARGGEN(statfs, ".path = \"%s\"", a2r(a->path));
MAKE_ARGGEN(statfs64, ".path = \"%s\"", a2r(a->path));
MAKE_ARGGEN(fstatfs64, FDFMT("fd"), FDARGS(e, a->fd));
MAKE_ARGGEN(fstat, FDFMT("fd"), FDARGS(e, a->fd));
MAKE_ARGGEN(lstat, ".path = \"%s\"", a2r(a->path));
MAKE_ARGGEN(stat, ".path = \"%s\"", a2r(a->path));
MAKE_ARGGEN(fstat64, FDFMT("fd"), FDARGS(e, a->fd));
MAKE_ARGGEN(lstat64, ".path = \"%s\"", a2r(a->path));
MAKE_ARGGEN(stat64, ".path = \"%s\"", a2r(a->path));
MAKE_ARGGEN(setattrlist, ".path = \"%s\"", a2r(a->path));
MAKE_ARGGEN(getattrlist, ".path = \"%s\"", a2r(a->path));
MAKE_ARGGEN(getxattr, ".path = \"%s\"", a2r(a->path));
MAKE_ARGGEN(fgetxattr, FDFMT("fd"), FDARGS(e, a->fd));
MAKE_ARGGEN(listxattr, ".path = \"%s\"", a2r(a->path));
MAKE_ARGGEN(flistxattr, FDFMT("fd"), FDARGS(e, a->fd));
MAKE_ARGGEN(chown, ".path = \"%s\", .owner = %d, .group = %d", a2r(a->path), a->owner,
            a->group);
MAKE_ARGGEN(chmod, ".path = \"%s\", .mode = 0%o", a2r(a->path), a->mode);
MAKE_ARGGEN(fsetxattr, FDFMT("fd")", .name = \""xattr("%s")"\"", FDARGS(e, a->fd), a->name);
MAKE_ARGGEN(fsgetpath, ".inum = %jd", (intmax_t)a->inum);
MAKE_ARGGEN(getdirentries64, FDFMT("fd")", .iobuf = thread%d_iobuf, .iobufsize = "
            "sizeof(thread%d_iobuf)", FDARGS(e, a->fd), e->thread, e->thread);
MAKE_ARGGEN(getdirentriesattr, FDFMT("fd"), FDARGS(e, a->fd));
MAKE_ARGGEN(getfsstat64, "");
MAKE_ARGGEN(getdtablesize, "");
MAKE_ARGGEN(chdir, ".path = \"%s\"", a2r(a->path));
MAKE_ARGGEN(fchdir, FDFMT("fd"), FDARGS(e, a->fd));
MAKE_ARGGEN(getcwd, ".buf = thread%d_iobuf, .size = %zd", e->thread, a->size);
MAKE_ARGGEN(kill, ".pid = %d, .sig = %d", a->pid, a->sig);
MAKE_ARGGEN(fchflags, FDFMT("fd")", .flags = %d", FDARGS(e, a->fd), a->flags);
MAKE_ARGGEN(fchmod, FDFMT("fd")", .mode = 0%o", FDARGS(e, a->fd), a->mode);
MAKE_ARGGEN(fchown, FDFMT("fd")", .owner = %d, .group = %d", FDARGS(e, a->fd), a->owner, a->group);
MAKE_ARGGEN(removexattr, ".path = \"%s\", .name = \""xattr("%s")"\"", a2r(a->path), a->name);
MAKE_ARGGEN(fremovexattr, FDFMT("fd")", .name = \""xattr("%s")"\"", FDARGS(e, a->fd), a->name);
MAKE_ARGGEN(setxattr, ".path = \"%s\", .name = \""xattr("%s")"\"", a2r(a->path), a->name);
MAKE_ARGGEN(hfs_vnop_pagein, ".offset = %jd, .size = %zd, .name = \"%s\"", (intmax_t)a->offset,
            a->size, a->name);
MAKE_ARGGEN(posix_spawn, ".path = \"%s\"", a2r(a->path));

MAKE_ARGGEN(fadvise64, FDFMT("fd")", .offset = %jd, .len = %jd, .advice = %s", FDARGS(e, a->fd),
            (intmax_t)a->offset, (intmax_t)a->len, a->advice);
MAKE_ARGGEN(epoll_create, ".size = %d", a->size);
MAKE_ARGGEN(fstatfs, FDFMT("fd"), FDARGS(e, a->fd));
MAKE_ARGGEN(getdents, FDFMT("fd")", .count = %u", FDARGS(e, a->fd), a->count);
MAKE_ARGGEN(fallocate, FDFMT("fd")", .mode = %d, .offset = %jd, .len = %jd", FDARGS(e, a->fd),
            a->mode, (intmax_t)a->offset, (intmax_t)a->len);
MAKE_ARGGEN(pipe, FDFMT("rd")", "FDFMT("wr"), FDARGS(e, a->rd), FDARGS(e, a->wr));
MAKE_ARGGEN(newfstatat, FDFMT("dirfd")", .pathname = \"%s\"", FDARGS(e, a->dirfd), a2r(a->pathname));
MAKE_ARGGEN(dup2, FDFMT("old")", "FDFMT("new"), FDARGS(e, a->old), FDARGS(e, a->new));
MAKE_ARGGEN(fchownat, FDFMT("dirfd")", .pathname = \"%s\"", FDARGS(e, a->dirfd), a2r(a->pathname));
MAKE_ARGGEN(unlinkat, FDFMT("dirfd")", .pathname = \"%s\"", FDARGS(e, a->dirfd), a2r(a->pathname));

MAKE_ARGGEN(io_setup, ".nr_events = %u, .ctx_id = %d", a->nr_events, a->ctx_id);
MAKE_ARGGEN(io_destroy, ".ctx_id = %d", a->ctx_id);

/*
 * !!!HACK!!!
 *
 * These only output attrs for their first iocb/ioevent, relying on callers to
 * split up multi-request events into multiple events.
 */
MAKE_ARGGEN(io_submit, ".infoidx = %d, "FDFMT("fd")", .ctx_id = %d",
            a->iocbs->aiocbnum, FDARGS(e, a->iocbs->fd), a->ctx_id);
MAKE_ARGGEN(io_getevents, ".infoidx = %d, "FDFMT("fd")", .ctx_id = %d",
            a->ioevents->aiocbnum, FDARGS(e, a->ioevents->fd), a->ctx_id);

MAKE_ARGGEN(threadspawn, ".thread = &thread%d", a->tid);

#undef a2r
#undef FDFMT
#undef FDARGS

#define MAKE_EVGETTER(type, attrname, evname, member) \
	static type get##attrname##_##evname(const struct event* ev) \
	{ \
		const struct evname##_attrs* a = &ev->attrs.evname; \
		return a->member; \
	}

#define MAKE_GETPATH(name, member) \
	MAKE_EVGETTER(const char*, path, name, member)

MAKE_GETPATH(link, oldpath);
MAKE_GETPATH(rmdir, pathname);
MAKE_GETPATH(unlink, pathname);
MAKE_GETPATH(readlink, path);
MAKE_GETPATH(rename, oldpath);
MAKE_GETPATH(mkdir, pathname);
MAKE_GETPATH(statfs, path);
MAKE_GETPATH(statfs64, path);
MAKE_GETPATH(lstat, path);
MAKE_GETPATH(stat, path);
MAKE_GETPATH(lstat64, path);
MAKE_GETPATH(stat64, path);
MAKE_GETPATH(setattrlist, path);
MAKE_GETPATH(getattrlist, path);
MAKE_GETPATH(getxattr, path);
MAKE_GETPATH(listxattr, path);
MAKE_GETPATH(access, pathname);
MAKE_GETPATH(chown, path);
MAKE_GETPATH(chmod, path);
MAKE_GETPATH(chdir, path);
MAKE_GETPATH(removexattr, path);
MAKE_GETPATH(setxattr, path);
MAKE_GETPATH(posix_spawn, path);
MAKE_GETPATH(utimensat, pathname);
MAKE_GETPATH(faccessat, pathname);
MAKE_GETPATH(newfstatat, pathname);
MAKE_GETPATH(fchownat, pathname);
MAKE_GETPATH(unlinkat, pathname);

#define MAKE_GETIOBUFSIZE(name, member) \
	MAKE_EVGETTER(size_t, iobufsize, name, member)

MAKE_GETIOBUFSIZE(read, count);
MAKE_GETIOBUFSIZE(write, count);
MAKE_GETIOBUFSIZE(pread, count);
MAKE_GETIOBUFSIZE(pwrite, count);
MAKE_GETIOBUFSIZE(readlink, bufsiz);
MAKE_GETIOBUFSIZE(hfs_vnop_pagein, size);
MAKE_GETIOBUFSIZE(aio_read, nbytes);
MAKE_GETIOBUFSIZE(getdents, count);
MAKE_GETIOBUFSIZE(getcwd, size);

static size_t getiobufsize_fcntl(const struct event* ev)
{
	const struct fcntl_attrs* a = &ev->attrs.fcntl;

	if (!strcmp(a->cmdname, "F_GETPATH"))
		return PATH_MAX; /* FIXME: this should be of the target system */
	else
		return 0;
}

/*
 * HACK: getdirentries is special-cased, since the ibench traces don't
 * have the size argument for it recorded in the trace, and hence in
 * its 'attrs' struct.  However, it returns the number of bytes read
 * on success, so if we have a non-negative return value we know it
 * had a buffer of at least that size.
 */
static size_t getiobufsize_getdirentries64(const struct event* ev)
{
	if (traceinfo.type == TT_IBENCH)
		return ev->ret.i < 0 ? 0 : ev->ret.i;
	else {
		fprintf(stderr, "getdirentries64 in non-iBench trace??\n");
		abort();
	}
}

static size_t getiobufsize_io_submit(const struct event* ev)
{
	const struct artc_iocb* i;
	size_t max = 0;
	const struct io_submit_attrs* a = &ev->attrs.io_submit;

	for (i = a->iocbs; i; i = i->next)
		max = i->nbytes > max ? i->nbytes : max;

	return max;
}
#endif

int event_returns_fd(const struct event* ev)
{
	switch (ev->type) {
	case EV_OPEN:
	case EV_SHM_OPEN:
	case EV_DUP:
	case EV_DUP2:
	case EV_KQUEUE:
	case EV_SOCKET:
	case EV_EPOLL_CREATE:
		return 1;

	case EV_FCNTL:
#ifdef ARTC
		return !strcmp(ev->attrs.fcntl.cmdname, "F_DUPFD");
#else
		return ev->attrs.fcntl.cmd == F_DUPFD;
#endif

	default:
		return 0;
	}
}

/*
 * For async I/O, only "submit" calls are ordered.
 */
int event_unordered(const struct event* e)
{
	switch (e->type) {
	case EV_IO_GETEVENTS:
		return 1;

	default:
		return 0;
	}
}

#define MAKE_EVPTRGETTER(type, attrname, evname, member) \
	static runtime_const type* get##attrname##_##evname(runtime_const struct event* ev) \
	{ \
		runtime_const struct evname##_attrs* a = &ev->attrs.evname; \
		return &a->member; \
	}

#define MAKE_GETFD_NAMED(name, fdname) \
	MAKE_EVPTRGETTER(struct fduse, fd, name, fdname)

/* For most things, where fds are named 'fd' */
#define MAKE_GETFD(name) MAKE_GETFD_NAMED(name, fd)

/* For *at() calls, with fds named 'dirfd' */
#define MAKE_AT_GETFD(name) MAKE_GETFD_NAMED(name, dirfd)

MAKE_GETFD(close);
MAKE_GETFD(dup);
MAKE_GETFD(read);
MAKE_GETFD(write);
MAKE_GETFD(pread);
MAKE_GETFD(pwrite);
MAKE_GETFD(lseek);
MAKE_GETFD(fsync);
MAKE_GETFD(fdatasync);
MAKE_GETFD(fcntl);
MAKE_GETFD(ftruncate);
MAKE_GETFD(flock);
MAKE_GETFD(mmap);
MAKE_GETFD(futimes);
MAKE_GETFD(fstatfs64);
MAKE_GETFD(fstat);
MAKE_GETFD(fstat64);
MAKE_GETFD(fgetxattr);
MAKE_GETFD(flistxattr);
MAKE_GETFD(fsetxattr);
MAKE_GETFD(getdirentries64);
MAKE_GETFD(getdirentriesattr);
MAKE_GETFD(fchdir);
MAKE_GETFD(fchflags);
MAKE_GETFD(fchmod);
MAKE_GETFD(fchown);
MAKE_GETFD(fremovexattr);
MAKE_GETFD(fadvise64);
MAKE_GETFD(fstatfs);
MAKE_GETFD(getdents);
MAKE_GETFD(fallocate);

MAKE_GETFD(aio_read);
MAKE_GETFD(aio_error);
MAKE_GETFD(aio_return);

MAKE_AT_GETFD(utimensat);
MAKE_AT_GETFD(faccessat);
MAKE_AT_GETFD(newfstatat);
MAKE_AT_GETFD(fchownat);
MAKE_AT_GETFD(unlinkat);

static runtime_const struct fduse* getfd_dup2(runtime_const struct event* ev)
{
	abort();
}

#ifdef ARTC
/*
 * These should never be called.  They exist only so we can fill in the evinfo
 * table without having to #ifdef ARTC individual member elements.
 */
static runtime_const struct fduse* getfd_io_submit(runtime_const struct event* ev) { abort(); }
static runtime_const struct fduse* getfd_io_getevents(runtime_const struct event* ev) { abort(); }

#else

MAKE_GETFD(io_submit);
MAKE_GETFD(io_getevents);

#endif /* ARTC */

#ifdef ARTC
static int iret_gez(const struct event* ev)
{
	return ev->ret.i >= 0;
}

static int iret_gtz(const struct event* ev)
{
	return ev->ret.i > 0;
}

static int iret_eqz(const struct event* ev)
{
	return ev->ret.i == 0;
}

static int iret_01(const struct event* ev)
{
	return ev->ret.i == 0 || ev->ret.i == 1;
}

static int always_succeeds(const struct event* ev)
{
	return 1;
}

static int fdret_succeeds(const struct event* ev)
{
	return ev->ret.fd.traceval >= 0;
}

static int oret_valid(const struct event* ev)
{
	/* FIXME: original/replay host incompatibility */
	return ev->ret.o != (off_t)-1;
}

static int pret_valid(const struct event* ev)
{
	return ev->ret.p != MAP_FAILED;
}

static int fcntl_succeeded(const struct event* ev)
{
	if (event_returns_fd(ev))
		return fdret_succeeds(ev);

	/*
	 * FIXME: how, if at all, is this even remotely correct??  I have no
	 * recollection at all of how this ended up this way.
	 */
	return 1;
}

/*
 * In strace output, when a syscall occurs while another one is outstanding in
 * another thread, the outstanding one gets split into an entry half and an
 * exit half, with some (depending on the syscall) parameters in each half.
 * These merge functions take the exit half and merge the required parameters
 * back in to the entry half (only used by the strace parser).
 */
#define MAKE_MERGEFN1(name, member) \
	static void merge_##name(struct event* entry, const struct event* exit) \
	{ \
		entry->attrs.name.member = exit->attrs.name.member; \
	}

#define MAKE_MERGEFN2(name, member1, member2) \
	static void merge_##name(struct event* entry, const struct event* exit) \
	{ \
		entry->attrs.name.member1 = exit->attrs.name.member1; \
		entry->attrs.name.member2 = exit->attrs.name.member2; \
	}

#define MAKE_MERGEFN3(name, member1, member2, member3) \
	static void merge_##name(struct event* entry, const struct event* exit) \
	{ \
		entry->attrs.name.member1 = exit->attrs.name.member1; \
		entry->attrs.name.member2 = exit->attrs.name.member2; \
		entry->attrs.name.member3 = exit->attrs.name.member3; \
	}

MAKE_MERGEFN1(getdents, count);
MAKE_MERGEFN2(read, count, dummy1);
MAKE_MERGEFN1(fgetxattr, size);
MAKE_MERGEFN1(readlink, bufsiz);
MAKE_MERGEFN1(listxattr, size);
MAKE_MERGEFN1(getxattr, size);
MAKE_MERGEFN1(flistxattr, size);
MAKE_MERGEFN2(getcwd, size, dummy1);
MAKE_MERGEFN3(pread, count, offset, dummy1);
MAKE_MERGEFN1(io_getevents, ioevents);
MAKE_MERGEFN1(io_setup, ctx);
#endif

struct event_type_info {
	const char* name;
#ifdef ARTC
	const char* enumname;
	void (*attrfree)(struct event*);
	void (*arggen)(const struct event*, FILE*);

	size_t (*getiobufsize)(const struct event*);
	const char* (*getpath)(const struct event*);

	int (*should_succeed)(const struct event*);

	void (*merge)(struct event*, const struct event*);
#endif
	runtime_const struct fduse* (*getfd)(runtime_const struct event*);
};

#ifdef ARTC
#define EVINFO(bigname, smallname, afree, agen, giobs, gfd, ss, gp, mergefn) \
	[EV_##bigname] = { \
		.enumname = "EV_" #bigname, \
		.name = smallname, \
		.attrfree = afree, \
		.arggen = agen, \
		.getiobufsize = giobs, \
		.getfd = gfd, \
		.should_succeed = ss, \
		.getpath = gp, \
		.merge = mergefn, \
	}
#else
#define EVINFO(bigname, smallname, afree, agen, giobs, gfd, ss, gp, mergefn) \
	[EV_##bigname] = { \
		.name = smallname, \
		.getfd = gfd, \
	}
#endif

static const struct event_type_info evinfo[] = {
	EVINFO(OPEN, "open", free_open_attrs, open_genargs, NULL, NULL,
	       fdret_succeeds, NULL, NULL),
	EVINFO(SHM_OPEN, "shm_open", free_shm_open_attrs, shm_open_genargs, NULL,
	       NULL, fdret_succeeds, NULL, NULL),
	EVINFO(CLOSE, "close", NULL, close_genargs, NULL, getfd_close, iret_eqz,
	       NULL, NULL),
	EVINFO(DUP, "dup", NULL, dup_genargs, NULL, getfd_dup, fdret_succeeds,
	       NULL, NULL),
	EVINFO(KQUEUE, "kqueue", NULL, kqueue_genargs, NULL, NULL, fdret_succeeds,
	       NULL, NULL),
	EVINFO(SOCKET, "socket", NULL, socket_genargs, NULL, NULL, fdret_succeeds,
	       NULL, NULL),
	EVINFO(SOCKETPAIR, "socketpair", NULL, socketpair_genargs, NULL, NULL,
	       iret_eqz, NULL, NULL),
	EVINFO(READ, "read", free_read_attrs, read_genargs, getiobufsize_read,
	       getfd_read, iret_gez, NULL, merge_read),
	EVINFO(WRITE, "write", free_write_attrs, write_genargs, getiobufsize_write,
	       getfd_write, iret_gez, NULL, NULL),
	EVINFO(PREAD, "pread", free_pread_attrs, pread_genargs, getiobufsize_pread,
	       getfd_pread, iret_gez, NULL, merge_pread),
	EVINFO(PWRITE, "pwrite", free_pwrite_attrs, pwrite_genargs, getiobufsize_pwrite,
	       getfd_pwrite, iret_gez, NULL, NULL),
	EVINFO(AIO_READ, "aio_read", free_aio_read_attrs, aio_read_genargs,
	       getiobufsize_aio_read, getfd_aio_read, iret_eqz, NULL, NULL),
	EVINFO(AIO_ERROR, "aio_error", NULL, aio_error_genargs, NULL, getfd_aio_error,
	       always_succeeds, NULL, NULL),
	EVINFO(AIO_RETURN, "aio_return", NULL, aio_return_genargs, NULL, getfd_aio_return,
	       iret_gez, NULL, NULL),
	EVINFO(AIO_SUSPEND, "aio_suspend", NULL, aio_suspend_genargs, NULL, NULL,
	       iret_eqz, NULL, NULL),
	EVINFO(LSEEK, "lseek", NULL, lseek_genargs, NULL, getfd_lseek, oret_valid,
	       NULL, NULL),
	EVINFO(FSYNC, "fsync", free_fsync_attrs, fsync_genargs, NULL, getfd_fsync,
	       iret_eqz, NULL, NULL),
	EVINFO(FDATASYNC, "fdatasync", free_fdatasync_attrs, fdatasync_genargs, NULL,
	       getfd_fdatasync, iret_eqz, NULL, NULL),
	EVINFO(FCNTL, "fcntl", free_fcntl_attrs, fcntl_genargs, getiobufsize_fcntl,
	       getfd_fcntl, fcntl_succeeded, NULL, NULL),
	EVINFO(FTRUNCATE, "ftruncate", free_ftruncate_attrs, ftruncate_genargs, NULL,
	       getfd_ftruncate, iret_eqz, NULL, NULL),
	EVINFO(LINK, "link", free_link_attrs, link_genargs, NULL, NULL, iret_eqz,
	       getpath_link, NULL),
	EVINFO(SYMLINK, "symlink", free_symlink_attrs, symlink_genargs, NULL, NULL,
	       iret_eqz, NULL, NULL),
	EVINFO(RMDIR, "rmdir", free_rmdir_attrs, rmdir_genargs, NULL, NULL, iret_eqz,
	       getpath_rmdir, NULL),
	EVINFO(UNLINK, "unlink", free_unlink_attrs, unlink_genargs, NULL, NULL, iret_eqz,
	       getpath_unlink, NULL),
	EVINFO(READLINK, "readlink", free_readlink_attrs, readlink_genargs,
	       getiobufsize_readlink, NULL, iret_gez, getpath_readlink, merge_readlink),
	EVINFO(RENAME, "rename", free_rename_attrs, rename_genargs, NULL, NULL, iret_eqz,
	       getpath_rename, NULL),
	EVINFO(EXCHANGEDATA, "exchangedata", free_exchangedata_attrs, exchangedata_genargs,
	       NULL, NULL, iret_eqz, NULL, NULL),
	EVINFO(MKDIR, "mkdir", free_mkdir_attrs, mkdir_genargs, NULL, NULL, iret_eqz,
	       getpath_mkdir, NULL),
	EVINFO(FLOCK, "flock", free_flock_attrs, flock_genargs, NULL, getfd_flock,
	       iret_eqz, NULL, NULL),
	EVINFO(MMAP, "mmap", free_mmap_attrs, mmap_genargs, NULL, getfd_mmap, pret_valid,
	       NULL, NULL),
	EVINFO(FUTIMES, "futimes", NULL, futimes_genargs, NULL, getfd_futimes, iret_eqz,
	       NULL, NULL),
	EVINFO(UTIMES, "utimes", NULL, utimes_genargs, NULL, NULL, iret_eqz, NULL, NULL),
	EVINFO(STATFS, "statfs", free_statfs_attrs, statfs_genargs, NULL, NULL, iret_eqz,
	       getpath_statfs, NULL),
	EVINFO(STATFS64, "statfs64", free_statfs64_attrs, statfs64_genargs, NULL, NULL,
	       iret_eqz, getpath_statfs64, NULL),
	EVINFO(FSTATFS64, "fstatfs64", NULL, fstatfs64_genargs, NULL, getfd_fstatfs64,
	       iret_eqz, NULL, NULL),
	EVINFO(FSTAT, "fstat", NULL, fstat_genargs, NULL, getfd_fstat, iret_eqz, NULL,
	       NULL),
	EVINFO(LSTAT, "lstat", free_lstat_attrs, lstat_genargs, NULL, NULL, iret_eqz,
	       getpath_lstat, NULL),
	EVINFO(STAT, "stat", free_stat_attrs, stat_genargs, NULL, NULL, iret_eqz,
	       getpath_stat, NULL),
	EVINFO(FSTAT64, "fstat64", NULL, fstat64_genargs, NULL, getfd_fstat64, iret_eqz,
	       NULL, NULL),
	EVINFO(LSTAT64, "lstat64", free_lstat64_attrs, lstat64_genargs, NULL, NULL,
	       iret_eqz, getpath_lstat64, NULL),
	EVINFO(STAT64, "stat64", free_stat64_attrs, stat64_genargs, NULL, NULL, iret_eqz,
	       getpath_stat64, NULL),
	EVINFO(SETATTRLIST, "setattrlist", free_setattrlist_attrs, setattrlist_genargs,
	       NULL, NULL, iret_eqz, getpath_setattrlist, NULL),
	EVINFO(GETATTRLIST, "getattrlist", free_getattrlist_attrs, getattrlist_genargs,
	       NULL, NULL, iret_eqz, getpath_getattrlist, NULL),
	EVINFO(GETXATTR, "getxattr", free_getxattr_attrs, getxattr_genargs, NULL, NULL,
	       iret_gtz, getpath_getxattr, merge_getxattr),
	EVINFO(FGETXATTR, "fgetxattr", free_fgetxattr_attrs, fgetxattr_genargs, NULL,
	       getfd_fgetxattr, iret_gtz, NULL, merge_fgetxattr),
	EVINFO(LISTXATTR, "listxattr", free_listxattr_attrs, listxattr_genargs, NULL, NULL,
	       iret_gtz, getpath_listxattr, merge_listxattr),
	EVINFO(FLISTXATTR, "flistxattr", free_flistxattr_attrs, flistxattr_genargs, NULL,
	       getfd_flistxattr, iret_gez, NULL, merge_flistxattr),
	EVINFO(ACCESS, "access", free_access_attrs, access_genargs, NULL, NULL, iret_eqz,
	       getpath_access, NULL),
	EVINFO(CHOWN, "chown", free_chown_attrs, chown_genargs, NULL, NULL, iret_eqz,
	       getpath_chown, NULL),
	EVINFO(CHMOD, "chmod", free_chmod_attrs, chmod_genargs, NULL, NULL, iret_eqz,
	       getpath_chmod, NULL),
	EVINFO(FSETXATTR, "fsetxattr", free_fsetxattr_attrs, fsetxattr_genargs, NULL,
	       getfd_fsetxattr, iret_eqz, NULL, NULL),
	EVINFO(FSGETPATH, "fsgetpath", NULL, fsgetpath_genargs, NULL, NULL, iret_gtz,
	       NULL, NULL),
	EVINFO(GETDIRENTRIES64, "getdirentries64", NULL, getdirentries64_genargs,
	       getiobufsize_getdirentries64, getfd_getdirentries64, iret_gez, NULL, NULL),
	EVINFO(GETDIRENTRIESATTR, "getdirentriesattr", NULL, getdirentriesattr_genargs,
	       NULL, getfd_getdirentriesattr, iret_01, NULL, NULL),
	EVINFO(GETFSSTAT64, "getfsstat64", NULL, getfsstat64_genargs, NULL, NULL,
	       iret_gez, NULL, NULL),
	EVINFO(GETDTABLESIZE, "getdtablesize", NULL, getdtablesize_genargs, NULL, NULL,
	       always_succeeds, NULL, NULL),
	EVINFO(CHDIR, "chdir", free_chdir_attrs, chdir_genargs, NULL, NULL, iret_eqz,
	       getpath_chdir, NULL),
	EVINFO(FCHDIR, "fchdir", NULL, fchdir_genargs, NULL, getfd_fchdir, iret_eqz,
	       NULL, NULL),
	EVINFO(GETCWD, "getcwd", free_getcwd_attrs, getcwd_genargs, getiobufsize_getcwd,
	       NULL, iret_gez, NULL, merge_getcwd),
	EVINFO(KILL, "kill", NULL, kill_genargs, NULL, NULL, iret_eqz, NULL, NULL),
	EVINFO(FCHFLAGS, "fchflags", NULL, fchflags_genargs, NULL, getfd_fchflags,
	       iret_eqz, NULL, NULL),
	EVINFO(FCHMOD, "fchmod", NULL, fchmod_genargs, NULL, getfd_fchmod, iret_eqz,
	       NULL, NULL),
	EVINFO(FCHOWN, "fchown", NULL, fchown_genargs, NULL, getfd_fchown, iret_eqz,
	       NULL, NULL),
	EVINFO(REMOVEXATTR, "removexattr", free_removexattr_attrs, removexattr_genargs,
	       NULL, NULL, iret_eqz, getpath_removexattr, NULL),
	EVINFO(FREMOVEXATTR, "fremovexattr", free_fremovexattr_attrs,
	       fremovexattr_genargs, NULL, getfd_fremovexattr, iret_eqz, NULL, NULL),
	EVINFO(SETXATTR, "setxattr", free_setxattr_attrs, setxattr_genargs, NULL, NULL,
	       iret_eqz, getpath_setxattr, NULL),
#ifdef ARTC
	EVINFO(CONTROLLER, "controller", free_controller_attrs, NULL, NULL, NULL, NULL,
	       NULL, NULL),
	EVINFO(RESOLVER, "resolver", NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	EVINFO(ALL_CALLS, "all_calls", NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	EVINFO(SETTER, "setter", NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	EVINFO(BUILD_PATH, "build_path", NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	EVINFO(NAMEI, "namei", NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	EVINFO(VN_CREATE, "vn_create", NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	EVINFO(VNODE_PAGEIN, "vnode_pagein", NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	EVINFO(SYSCALL_MAP, "syscall_map", free_syscall_map_attrs, NULL, NULL, NULL,
	       NULL, NULL, NULL),
#endif
	EVINFO(HFS_VNOP_PAGEIN, "hfs_vnop_pagein", free_hfs_vnop_pagein_attrs,
	       hfs_vnop_pagein_genargs, getiobufsize_hfs_vnop_pagein, NULL, always_succeeds,
	       NULL, NULL),
	EVINFO(POSIX_SPAWN, "posix_spawn", free_posix_spawn_attrs, posix_spawn_genargs,
	       NULL, NULL, iret_eqz, getpath_posix_spawn, NULL),
	EVINFO(EXECVE, "execve", NULL, NULL, NULL, NULL, NULL, NULL, NULL),

	EVINFO(FADVISE64, "fadvise64", free_fadvise64_attrs, fadvise64_genargs, NULL,
	       getfd_fadvise64, iret_eqz, NULL, NULL),
	EVINFO(EPOLL_CREATE, "epoll_create", NULL, epoll_create_genargs, NULL, NULL,
	       fdret_succeeds, NULL, NULL),
	EVINFO(FSTATFS, "fstatfs", NULL, fstatfs_genargs, NULL, getfd_fstatfs, iret_eqz,
	       NULL, NULL),
	EVINFO(GETDENTS, "getdents", NULL, getdents_genargs, getiobufsize_getdents,
	       getfd_getdents, iret_gez, NULL, merge_getdents),
	EVINFO(FALLOCATE, "fallocate", NULL, fallocate_genargs, NULL, getfd_fallocate,
	       iret_eqz, NULL, NULL),
	EVINFO(UTIMENSAT, "utimensat", free_utimensat_attrs, utimensat_genargs, NULL,
	       getfd_utimensat, iret_eqz, getpath_utimensat, NULL),
	EVINFO(PIPE, "pipe", NULL, pipe_genargs, NULL, NULL, iret_eqz, NULL, NULL),
	EVINFO(FACCESSAT, "faccessat", free_faccessat_attrs, faccessat_genargs, NULL,
	       getfd_faccessat, iret_eqz, getpath_faccessat, NULL),
	EVINFO(NEWFSTATAT, "newfstatat", free_newfstatat_attrs, newfstatat_genargs, NULL,
	       getfd_newfstatat, iret_eqz, getpath_newfstatat, NULL),
	EVINFO(DUP2, "dup2", NULL, dup2_genargs, NULL, getfd_dup2, fdret_succeeds,
	       NULL, NULL),
	EVINFO(FCHOWNAT, "fchownat", free_fchownat_attrs, fchownat_genargs, NULL,
	       getfd_fchownat, iret_eqz, getpath_fchownat, NULL),
	EVINFO(UNLINKAT, "unlinkat", free_unlinkat_attrs, unlinkat_genargs, NULL,
	       getfd_unlinkat, iret_eqz, getpath_unlinkat, NULL),

	EVINFO(IO_SETUP, "io_setup", NULL, io_setup_genargs, NULL, NULL, iret_eqz, NULL,
	       merge_io_setup),
	EVINFO(IO_SUBMIT, "io_submit", free_io_submit_attrs, io_submit_genargs,
	       getiobufsize_io_submit, getfd_io_submit, iret_gez, NULL, NULL),
	EVINFO(IO_GETEVENTS, "io_getevents", free_io_getevents_attrs, io_getevents_genargs,
	       NULL, getfd_io_getevents, iret_gez, NULL, merge_io_getevents),
	EVINFO(IO_DESTROY, "io_destroy", NULL, io_destroy_genargs, NULL, NULL, iret_eqz,
	       NULL, NULL),

	EVINFO(_THREADSPAWN, "threadspawn", NULL, threadspawn_genargs, NULL, NULL, NULL,
	       NULL, NULL),
	EVINFO(_THREADCONT, "threadcont", NULL, NULL, NULL, NULL, NULL, NULL, NULL),
	EVINFO(_THREADEXIT, "threadexit", NULL, NULL, NULL, NULL, NULL, NULL, NULL),
};

#ifdef ARTC

void free_event_temporal_deps(struct event* ev)
{
	struct event_dep* dep;
	struct event_dep* tmp;

	/*
	 * Yes, this kind of falls in the "for-loop abuse" category, but I
	 * kind of like it.
	 */
	for (dep = ev->temporal_deps;
	     dep && (tmp = dep->next, xfree(dep), tmp);
	     dep = tmp);
}

void free_event(struct event* ev)
{
	if (evinfo[ev->type].attrfree)
		evinfo[ev->type].attrfree(ev);

	if (ev->paths.consumed)
		xfree(ev->paths.consumed);
	if (ev->paths.removed)
		xfree(ev->paths.removed);
	if (ev->paths.produced)
		xfree(ev->paths.produced);
	if (ev->files)
		xfree(ev->files);

	free_event_temporal_deps(ev);

	xfree(ev);
}

/* Name-shadowing of exit(3) is ugly here. */
void merge_event(struct event* entry, const struct event* exit)
{
	assert(entry->type == exit->type);
	if (evinfo[entry->type].merge)
		evinfo[entry->type].merge(entry, exit);
}

void event_genattrs(const struct event* ev, FILE* output)
{
	evinfo[ev->type].arggen(ev, output);
}

size_t event_getiobufsize(const struct event* ev)
{
	if (evinfo[ev->type].getiobufsize)
		return evinfo[ev->type].getiobufsize(ev);
	else
		return 0;
}

const char* event_get_accessed_path(const struct event* ev)
{
	if (evinfo[ev->type].getpath)
		return evinfo[ev->type].getpath(ev);
	else
		return NULL;
}

const char* event_type_enumname(evid_t t)
{
	return evinfo[t].enumname;
}

int event_should_succeed(const struct event* ev)
{
	if (evinfo[ev->type].should_succeed)
		return evinfo[ev->type].should_succeed(ev);

	fprintf(stderr, "no should_succeed() for %s [%d]\n",
	        event_type_enumname(ev->type), ev->type);
	abort();
}

static char* parent_dir(const char* path)
{
	char* slash;
	char* parent;

	slash = strrchr(path, '/');

	/* If there are no slashes in path, parent is '.' */
	if (!slash)
		return xstrdup(".");

	/*
	 * I don't think this should ever actually arise, but skip repeated
	 * trailing slashes if they do pop up for some reason.
	 */
	while (slash > path && slash[-1] == '/')
		slash--;

	parent = xmalloc(slash - path + 1);
	memcpy(parent, path, slash - path);
	parent[slash-path] = '\0';

	return parent;
}

/*
 * Remove all instances of flags named 'name' from 'flags', returning the
 * number removed (zero if 'name' wasn't found anywhere in 'flags').
 */
unsigned int remove_flag(struct symflag** flags, const char* name)
{
	struct symflag* f;
	struct symflag* prev;
	struct symflag* next;
	unsigned int count = 0;

	for (prev = NULL, f = *flags; f; f = next) {
		next = f->next;
		if (!strcmp(f->name, name)) {
			*(prev ? &prev->next : flags) = f->next;
			xfree(f);
			count++;
			/*
			 * Not returning here so as to handle the possibility
			 * of duplicate flags.
			 */
		}
	}

	return count;
}

int flags_include(const struct symflag* flags, const char* s)
{
	const struct symflag* f;

	for (f = flags; f; f = f->next) {
		if (!strcmp(f->name, s))
			return 1;
	}

	return 0;
}

#define FOLLOW_ASSUME 0

static int at_follows_symlink(const struct event* ev)
{
	int nofollow;
	switch (ev->type) {
	case EV_UTIMENSAT:
		nofollow = evattr(utimensat, ev, flags) & AT_SYMLINK_NOFOLLOW;
		break;
	case EV_FACCESSAT:
		nofollow = evattr(faccessat, ev, flags) & AT_SYMLINK_NOFOLLOW;
		break;
	case EV_NEWFSTATAT:
		nofollow = flags_include(evattr(newfstatat, ev, flags),
		                         "AT_SYMLINK_NOFOLLOW");
		break;
	case EV_FCHOWNAT:
		nofollow = flags_include(evattr(fchownat, ev, flags),
		                         "AT_SYMLINK_NOFOLLOW");
		break;
	case EV_UNLINKAT:
		nofollow = flags_include(evattr(unlinkat, ev, flags),
		                         "AT_SYMLINK_NOFOLLOW");
		break;
	default:
		abort();
	}

	return traceinfo.type == TT_STRACE ? !nofollow : FOLLOW_ASSUME;
}

/*
 * Writes malloc()ed strings into 'used' of the paths required to exist for
 * 'ev' to execute successfully.  The array is terminated by a NULL pointer.
 * Note that this is only the *directly* used paths, so if one is a symlink,
 * only the path of the link itself will be included.  Also returns a
 * malloc()ed string into '*provided' of the path created by 'ev' (or at least
 * that should exist when it returns if it executes successfully) if there is
 * such a path, NULL otherwise.  Whether or not each used path uses a symlink
 * directly or follows it is written into the entry of 'follows' that
 * corresponds to each entry in 'used'.  For exchangedata(),
 * {get,set}attrlist(), {get,set,remove,list}xattr() this is determined by an
 * flag argument to the call, but we don't have those recorded in the iBench
 * traces, so we use the value of FOLLOW_ASSUME.
 */
char* event_get_paths(const struct event* ev, char* used[EVENT_MAXPATHS],
                      int follows[EVENT_MAXPATHS], char** removed, char** negdep)
{
	unsigned int nused = 0;
	struct symflag* flags;
	char* provided = NULL;

	*removed = NULL;
	*negdep = NULL;

#define USEPATH(type, p, f) do { \
		used[nused] = xstrdup(evattr(type, ev, p)); \
		follows[nused++] = (f); \
	} while (0)
#define USEPARENT(type, p) do { \
		used[nused] = parent_dir(evattr(type, ev, p)); \
		follows[nused++] = 1; \
	} while (0)
#define BASIC_USE(captype, type, p, f) case EV_##captype: USEPATH(type, p, (f)); break
#define PROVIDE(type, p, neg) do { \
		provided = xstrdup(evattr(type, ev, p)); \
		if (neg) \
			*negdep = xstrdup(provided); \
	} while (0)
#define REMOVE(type, p) *removed = xstrdup(evattr(type, ev, p))

	switch (ev->type) {
	case EV_OPEN:
		flags = evattr(open, ev, flags);
		if (flags_include(flags, "O_CREAT")) {
			USEPARENT(open, path);
			PROVIDE(open, path, flags_include(flags, "O_EXCL"));
		} else
			USEPATH(open, path, 1);
		break;

	case EV_LINK:
		USEPATH(link, oldpath, 0);
		USEPARENT(link, newpath);
		PROVIDE(link, newpath, 1);
		break;

	case EV_SYMLINK:
		USEPARENT(symlink, newpath);
		PROVIDE(symlink, newpath, 1);
		break;

	case EV_RMDIR:
		USEPATH(rmdir, pathname, 0);
		REMOVE(rmdir, pathname);
		break;

	case EV_UNLINK:
		USEPATH(unlink, pathname, 0);
		REMOVE(unlink, pathname);
		break;

	case EV_RENAME:
		/*
		 * This code is roughly "correct", but process_paths() handles
		 * renames specially, so this function should never be called
		 * on a rename.  (Hence the abort.)
		 */
		abort();

		USEPARENT(rename, newpath);
		PROVIDE(rename, newpath, 0);
		REMOVE(rename, oldpath);
		break;

	case EV_EXCHANGEDATA:
		USEPATH(exchangedata, path1, FOLLOW_ASSUME);
		USEPATH(exchangedata, path2, FOLLOW_ASSUME);
		break;

	case EV_MKDIR:
		USEPARENT(mkdir, pathname);
		PROVIDE(mkdir, pathname, 1);
		break;

	case EV_UTIMENSAT:
		/* pathname can be NULL, in which case we don't count it. */
		if (evattr(utimensat, ev, pathname))
			USEPATH(utimensat, pathname, at_follows_symlink(ev));
		break;

	case EV_FACCESSAT:
		USEPATH(faccessat, pathname, at_follows_symlink(ev));
		break;

	case EV_NEWFSTATAT:
		USEPATH(newfstatat, pathname, at_follows_symlink(ev));
		break;

	case EV_FCHOWNAT:
		USEPATH(fchownat, pathname, at_follows_symlink(ev));
		break;

	case EV_UNLINKAT:
		USEPATH(unlinkat, pathname, at_follows_symlink(ev));
		REMOVE(unlinkat, pathname);
		break;

	BASIC_USE(STATFS, statfs, path, 1);
	BASIC_USE(STATFS64, statfs64, path, 1);
	BASIC_USE(LSTAT, lstat, path, 0);
	BASIC_USE(STAT, stat, path, 1);
	BASIC_USE(LSTAT64, lstat64, path, 0);
	BASIC_USE(STAT64, stat64, path, 1);
	BASIC_USE(SETATTRLIST, setattrlist, path, FOLLOW_ASSUME);
	BASIC_USE(GETATTRLIST, getattrlist, path, FOLLOW_ASSUME);
	BASIC_USE(GETXATTR, getxattr, path, FOLLOW_ASSUME);
	BASIC_USE(ACCESS, access, pathname, 1);
	BASIC_USE(CHOWN, chown, path, 1);
	BASIC_USE(CHMOD, chmod, path, 1);
	BASIC_USE(REMOVEXATTR, removexattr, path, FOLLOW_ASSUME);
	BASIC_USE(SETXATTR, setxattr, path, FOLLOW_ASSUME);
	BASIC_USE(POSIX_SPAWN, posix_spawn, path, 1);
	BASIC_USE(READLINK, readlink, path, 0);
	BASIC_USE(LISTXATTR, listxattr, path, FOLLOW_ASSUME);
	BASIC_USE(CHDIR, chdir, path, 1);

	default:
		break;
	}

#undef USEPATH
#undef USEPARENT
#undef PROVIDE
#undef BASIC_USE
#undef REMOVE

	used[nused] = NULL;

	return provided;
}

/*
 * A much dumber, more raw version of the above.  Writes pointers to
 * locations within the given event struct where explicit paths are
 * named.
 */
void event_get_raw_paths(struct event* ev, char*** p1, char*** p2)
{
	switch (ev->type) {

#define RAWPATH(utype, ltype, name) \
	case EV_##utype: \
		*p1 = &ev->attrs.ltype.name; \
		*p2 = NULL; \
		break

#define RAWPATH2(utype, ltype, name1, name2) \
	case EV_##utype: \
		*p1 = &ev->attrs.ltype.name1; \
		*p2 = &ev->attrs.ltype.name2; \
		break

		RAWPATH(OPEN, open, path);
		RAWPATH2(LINK, link, oldpath, newpath);
		RAWPATH2(SYMLINK, symlink, oldpath, newpath);
		RAWPATH(RMDIR, rmdir, pathname);
		RAWPATH(UNLINK, unlink, pathname);
		RAWPATH(READLINK, readlink, path);
		RAWPATH2(RENAME, rename, oldpath, newpath);
		RAWPATH2(EXCHANGEDATA, exchangedata, path1, path2);
		RAWPATH(MKDIR, mkdir, pathname);
		RAWPATH(STATFS, statfs, path);
		RAWPATH(STATFS64, statfs64, path);
		RAWPATH(LSTAT, lstat, path);
		RAWPATH(STAT, stat, path);
		RAWPATH(LSTAT64, lstat64, path);
		RAWPATH(STAT64, stat64, path);
		RAWPATH(SETATTRLIST, setattrlist, path);
		RAWPATH(GETATTRLIST, getattrlist, path);
		RAWPATH(GETXATTR, getxattr, path);
		RAWPATH(SETXATTR, setxattr, path);
		RAWPATH(REMOVEXATTR, removexattr, path);
		RAWPATH(LISTXATTR, listxattr, path);
		RAWPATH(ACCESS, access, pathname);
		RAWPATH(CHOWN, chown, path);
		RAWPATH(CHMOD, chmod, path);
		RAWPATH(POSIX_SPAWN, posix_spawn, path);
		RAWPATH(UTIMENSAT, utimensat, pathname);
		RAWPATH(FACCESSAT, faccessat, pathname);
		RAWPATH(NEWFSTATAT, newfstatat, pathname);
		RAWPATH(FCHOWNAT, fchownat, pathname);
		RAWPATH(UNLINKAT, unlinkat, pathname);

		RAWPATH(CHDIR, chdir, path);

#undef RAWPATH
#undef RAWPATH2

	default:
		*p1 = *p2 = NULL;
		break;
	}
}

static char disabled_events[EV__MAX_+1] = {
	/* Not sure why this is in the iBench traces to begin with... */
	[EV_KILL] = 1,

	/*
	 * utimes entries in the iBench traces seem to have reported the
	 * literal address of the path passed as the first argument instead of
	 * dereferencing the pointer and printing the string, so it's kind of
	 * useless and un-replayable (though it only ever appears in
	 * iphoto_import400, so it's not exactly ubiquitous).
	 *
	 * FIXME: this depends on utimes as such only appearing in iBench
	 * traces (strace on linux yields utimensat, which is not disabled).
	 */
	[EV_UTIMES] = 1,

	/*
         * posix_spawn is only used in a handful of traces, and only ever for
         * nmblookup and smbclient.  I think it's safe to ignore.
	 */
	[EV_POSIX_SPAWN] = 1,

	/*
	 * We're only aiming for explicit system calls here, so we'll skip
	 * paging activity.
	 */
	[EV_HFS_VNOP_PAGEIN] = 1,
};

#define DYLD_CACHE_PFX "dyld_shared_cache_"

int event_elidable(const struct event* ev)
{
	if (disabled_events[ev->type])
		return 1;
	else if (ev->type == EV_HFS_VNOP_PAGEIN)
		return !strncmp(evattr(hfs_vnop_pagein, ev, name),
		                DYLD_CACHE_PFX, strlen(DYLD_CACHE_PFX));
	else if (ev->type == EV_IO_GETEVENTS)
		return !evattr(io_getevents, ev, ioevents);
	else
		return 0;
}

#else /* ARTC */

int event_type_uses_fd(evid_t t)
{
	return !!evinfo[t].getfd;
}
#endif /* ARTC */

const char* event_type_name(evid_t t)
{
	return evinfo[t].name;
}

void event_visit_fds(runtime_const struct event* ev, fd_visitor_t fn, void* arg)
{
#ifdef ARTC
	struct artc_iocb* cb;
	struct artc_ioevent* ioe;
#endif

	switch (ev->type) {
#ifdef ARTC
	case EV_IO_SUBMIT:
		for (cb = ev->attrs.io_submit.iocbs; cb; cb = cb->next) {
			if (fn(ev, &cb->fd, arg))
				return;
		}
		break;

	case EV_IO_GETEVENTS:
		for (ioe = ev->attrs.io_getevents.ioevents; ioe; ioe = ioe->next) {
			if (fn(ev, &ioe->fd, arg))
				return;
		}
		break;
#endif

	case EV_DUP2:
		if (fn(ev, &ev->attrs.dup2.new, arg))
			return;
		fn(ev, &ev->attrs.dup2.old, arg);
		break;

	default:
		if (evinfo[ev->type].getfd)
			fn(ev, evinfo[ev->type].getfd(ev), arg);
		break;
	}
}
