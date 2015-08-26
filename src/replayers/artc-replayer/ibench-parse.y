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
%code requires {

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>

#include "trace.h"
#include "event.h"
#include "misc.h"
#include "assoc.h"

extern int ibenchdebug;

struct event* parse_ibench_trace(FILE* tracefile);
void ibench_parser_teardown(void);

}

%union {
	int64_t i;
	struct event ev;
	evsubtype_t subtype;
	char* str;
	struct symflag* flags;

	struct open_attrs open;
	struct shm_open_attrs shm_open;
	struct close_attrs close;
	struct dup_attrs dup;
	struct kqueue_attrs kqueue;
	struct socket_attrs socket;
	struct socketpair_attrs socketpair;
	struct read_attrs read;
	struct write_attrs write;
	struct pread_attrs pread;
	struct pwrite_attrs pwrite;
	struct aio_read_attrs aio_read;
	struct aio_error_attrs aio_error;
	struct aio_return_attrs aio_return;
	struct aio_suspend_attrs aio_suspend;
	struct lseek_attrs lseek;
	struct fsync_attrs fsync;
	struct fcntl_attrs fcntl;
	struct ftruncate_attrs ftruncate;
	struct link_attrs link;
	struct symlink_attrs symlink;
	struct rmdir_attrs rmdir;
	struct unlink_attrs unlink;
	struct readlink_attrs readlink;
	struct rename_attrs rename;
	struct exchangedata_attrs exchangedata;
	struct mkdir_attrs mkdir;
	struct flock_attrs flock;
	struct mmap_attrs mmap;
	struct futimes_attrs futimes;
	struct utimes_attrs utimes;
	struct statfs_attrs statfs;
	struct statfs64_attrs statfs64;
	struct fstatfs64_attrs fstatfs64;
	struct fstat_attrs fstat;
	struct lstat_attrs lstat;
	struct stat_attrs stat;
	struct fstat64_attrs fstat64;
	struct lstat64_attrs lstat64;
	struct stat64_attrs stat64;
	struct setattrlist_attrs setattrlist;
	struct getattrlist_attrs getattrlist;
	struct getxattr_attrs getxattr;
	struct fgetxattr_attrs fgetxattr;
	struct listxattr_attrs listxattr;
	struct flistxattr_attrs flistxattr;
	struct access_attrs access;
	struct chown_attrs chown;
	struct chmod_attrs chmod;
	struct fsetxattr_attrs fsetxattr;
	struct fsgetpath_attrs fsgetpath;
	struct getdirentries64_attrs getdirentries64;
	struct getdirentriesattr_attrs getdirentriesattr;
	struct getfsstat64_attrs getfsstat64;
	struct getdtablesize_attrs getdtablesize;
	struct chdir_attrs chdir;
	struct fchdir_attrs fchdir;
	struct kill_attrs kill;
	struct fchflags_attrs fchflags;
	struct fchmod_attrs fchmod;
	struct fchown_attrs fchown;
	struct removexattr_attrs removexattr;
	struct fremovexattr_attrs fremovexattr;
	struct setxattr_attrs setxattr;
	struct controller_attrs controller;
	struct resolver_attrs resolver;
	struct all_calls_attrs all_calls;
	struct setter_attrs setter;
	struct build_path_attrs build_path;
	struct namei_attrs namei;
	struct vn_create_attrs vn_create;
	struct vnode_pagein_attrs vnode_pagein;
	struct syscall_map_attrs syscall_map;
	struct hfs_vnop_pagein_attrs hfs_vnop_pagein;
	struct posix_spawn_attrs posix_spawn;
	struct execve_attrs execve;
};

%code {
#include "ibench-lex.yy.h"

MAKE_ASSOC(pe_assoc, uint64_t, UINT64_MAX, struct event*, NULL);

/*
 * This maps (thread-number, event-type) pairs to the last event seen of that
 * type from that thread.  The tuples are packed into a u64 to use a single key.
 */
static pe_assoc_t thread_prev_assoc = ASSOC_INIT;

static inline uint64_t threadevkey(const struct event* ev)
{
	return ((uint64_t)ev->type << 32) | (uint64_t)(ev->thread);
}

static inline struct event* thread_prev_lookup(const struct event* ev)
{
	return pe_assoc_lookup(&thread_prev_assoc, threadevkey(ev));
}

static inline void thread_prev_insert(const struct event* kev, struct event* vev)
{
	pe_assoc_insert(&thread_prev_assoc, threadevkey(kev), vev);
}

static inline struct event* thread_prev_delete(const struct event* ev)
{
	return pe_assoc_delete(&thread_prev_assoc, threadevkey(ev));
}

static void ibencherror(struct parser_state*, char const*);

static struct symflag* newflag(const char* s)
{
	struct symflag* f = xmalloc(sizeof(struct symflag) + strlen(s) + 1);
	f->next = NULL;
	strcpy(f->name, s);
	return f;
}

#define RSRC_FORK_SFX "/..namedfork/rsrc"
#define VOLFS_PFX "/.vol/"

/*
 * Multi-hack.
 *   Part 1: sweep resource forks under the rug by renaming them to /dev/zero.
 *   Part 2: exploit the fact that "/dev/zero" is shorter than anything that
 *     ends with "/..namedfork/rsrc" and overwrite the path string in place.
 *   Part 3: bypass volfs references by renaming them to '/'.
 */
static char* hack_path(char* path)
{
	size_t plen = strlen(path);
	size_t sfxlen = strlen(RSRC_FORK_SFX);
	size_t pfxlen = strlen(VOLFS_PFX);

	if (plen >= sfxlen && !strcmp(RSRC_FORK_SFX, path + plen - sfxlen))
		strcpy(path, "/dev/zero");
	else if (plen >= pfxlen && !memcmp(VOLFS_PFX, path, pfxlen))
		strcpy(path, "/");

	return path;
}

#define evstruct_i(t, n, a, r) (struct event) { \
		.type = EV_##t, \
		.attrs.n = a, \
		.ret = { \
			.i = r, \
		}, \
	}

#define evstruct_o(t, n, a, r) (struct event) { \
		.type = EV_##t, \
		.attrs.n = a, \
		.ret = { \
			.o = r, \
		}, \
	}

#define evstruct_p(t, n, a, r) (struct event) { \
		.type = EV_##t, \
		.attrs.n = a, \
		.ret = { \
			.p = r, \
		}, \
	}

#define FD(f) (struct fduse) { .traceval = (f), .rtidx = -1, .provider = NULL, .lastuse = NULL, }

}

%{

%}

%token <i> DECNUM
%token <i> HEXNUM
%token <str> FLAG_O
%token <str> FLAG_PROT
%token <str> FLAG_MAP
%token <str> FCNTL_CMD
%token <str> STRING

/* start-of-record, field delimiter, end-of-record */
%token SOR DELIM EOR

%token SUBTYPE_ENTRY
%token SUBTYPE_RETURN
%token SUBTYPE_COMMENT
%token THREAD_NA

%token <i> RSRC_INUM

%token ET_OPEN
%token ET_SHM_OPEN
%token ET_CLOSE
%token ET_DUP
%token ET_KQUEUE
%token ET_SOCKET
%token ET_SOCKETPAIR
%token ET_READ
%token ET_WRITE
%token ET_PREAD
%token ET_PWRITE
%token ET_AIO_READ
%token ET_AIO_ERROR
%token ET_AIO_RETURN
%token ET_AIO_SUSPEND
%token ET_LSEEK
%token ET_FSYNC
%token ET_FCNTL
%token ET_FTRUNCATE
%token ET_LINK
%token ET_SYMLINK
%token ET_RMDIR
%token ET_UNLINK
%token ET_READLINK
%token ET_RENAME
%token ET_EXCHANGEDATA
%token ET_MKDIR
%token ET_FLOCK
%token ET_MMAP
%token ET_FUTIMES
%token ET_UTIMES
%token ET_STATFS
%token ET_STATFS64
%token ET_FSTATFS64
%token ET_FSTAT
%token ET_LSTAT
%token ET_STAT
%token ET_FSTAT64
%token ET_LSTAT64
%token ET_STAT64
%token ET_SETATTRLIST
%token ET_GETATTRLIST
%token ET_GETXATTR
%token ET_FGETXATTR
%token ET_LISTXATTR
%token ET_FLISTXATTR
%token ET_ACCESS
%token ET_CHOWN
%token ET_CHMOD
%token ET_FSETXATTR
%token ET_FSGETPATH
%token ET_GETDIRENTRIES64
%token ET_GETDIRENTRIESATTR
%token ET_GETFSSTAT64
%token ET_GETDTABLESIZE
%token ET_CHDIR
%token ET_FCHDIR
%token ET_KILL
%token ET_FCHFLAGS
%token ET_FCHMOD
%token ET_FCHOWN
%token ET_REMOVEXATTR
%token ET_FREMOVEXATTR
%token ET_SETXATTR
%token ET_CONTROLLER
%token ET_RESOLVER
%token ET_ALL_CALLS
%token ET_SETTER
%token ET_BUILD_PATH
%token ET_NAMEI
%token ET_VN_CREATE
%token ET_HFS_VNOP_PAGEIN
%token ET_VNODE_PAGEIN
%token ET_SYSCALL_MAP
%token ET_POSIX_SPAWN
%token ET_EXECVE

%token END 0 "EOF"

%type <ev> event
%type <ev> evprefix
%type <ev> evinfo

%type <subtype> subtype

%type <open> open_attrs
%type <shm_open> shm_open_attrs
%type <close> close_attrs
%type <dup> dup_attrs
%type <kqueue> kqueue_attrs
%type <socket> socket_attrs
%type <socketpair> socketpair_attrs
%type <read> read_attrs
%type <write> write_attrs
%type <pread> pread_attrs
%type <pwrite> pwrite_attrs
%type <aio_read> aio_read_attrs
%type <aio_error> aio_error_attrs
%type <aio_return> aio_return_attrs
%type <aio_suspend> aio_suspend_attrs
%type <lseek> lseek_attrs
%type <fsync> fsync_attrs
%type <fcntl> fcntl_attrs
%type <ftruncate> ftruncate_attrs
%type <link> link_attrs
%type <symlink> symlink_attrs
%type <rmdir> rmdir_attrs
%type <unlink> unlink_attrs
%type <readlink> readlink_attrs
%type <rename> rename_attrs
%type <exchangedata> exchangedata_attrs
%type <mkdir> mkdir_attrs
%type <flock> flock_attrs
%type <mmap> mmap_attrs
%type <futimes> futimes_attrs
%type <utimes> utimes_attrs
%type <statfs> statfs_attrs
%type <statfs64> statfs64_attrs
%type <fstatfs64> fstatfs64_attrs
%type <fstat> fstat_attrs
%type <lstat> lstat_attrs
%type <stat> stat_attrs
%type <fstat64> fstat64_attrs
%type <lstat64> lstat64_attrs
%type <stat64> stat64_attrs
%type <setattrlist> setattrlist_attrs
%type <getattrlist> getattrlist_attrs
%type <getxattr> getxattr_attrs
%type <fgetxattr> fgetxattr_attrs
%type <listxattr> listxattr_attrs
%type <flistxattr> flistxattr_attrs
%type <access> access_attrs
%type <chown> chown_attrs
%type <chmod> chmod_attrs
%type <fsetxattr> fsetxattr_attrs
%type <fsgetpath> fsgetpath_attrs
%type <getdirentries64> getdirentries64_attrs
%type <getdirentriesattr> getdirentriesattr_attrs
%type <getfsstat64> getfsstat64_attrs
%type <getdtablesize> getdtablesize_attrs
%type <chdir> chdir_attrs
%type <fchdir> fchdir_attrs
%type <kill> kill_attrs
%type <fchflags> fchflags_attrs
%type <fchmod> fchmod_attrs
%type <fchown> fchown_attrs
%type <removexattr> removexattr_attrs
%type <fremovexattr> fremovexattr_attrs
%type <setxattr> setxattr_attrs
%type <hfs_vnop_pagein> hfs_vnop_pagein_attrs
%type <posix_spawn> posix_spawn_attrs

%type <syscall_map> syscall_map_attrs
%type <controller> controller_attrs

%type <vnode_pagein> vnode_pagein_attrs
%type <resolver> resolver_attrs
%type <all_calls> all_calls_attrs
%type <setter> setter_attrs
%type <build_path> build_path_attrs
%type <namei> namei_attrs
%type <vn_create> vn_create_attrs
%type <execve> execve_attrs

%type <flags> o_flags
%type <flags> prot_flags
%type <flags> map_flags

%type <i> inum

%debug

%parse-param {struct parser_state* state}

%name-prefix "ibench"

%error-verbose

%%

input: /* empty */
| input event {
	struct event* entry;
	struct event* ev;

	if ($2.subtype == ST_COMMENT) {
		xfree($2.attrs.syscall_map.dummy1);
		goto ignore;
	}

	ev = xmalloc(sizeof(struct event));
	*ev = $2;

	if (ev->type == EV_HFS_VNOP_PAGEIN) {
		free_event(ev);
	} else if (ev->subtype == ST_ENTRY) {
		assert(!thread_prev_lookup(ev));
		thread_prev_insert(ev, ev);

		if (ev->type == EV_CLOSE)
			append_event(ev, state);
	} else if (ev->subtype == ST_RETURN) {
		entry = thread_prev_delete(ev);
		if (!entry) {
			fprintf(stderr, "error: no entry event for return on line %d\n",
			        ev->traceline);
			exit(1);
		}

		if (ev->type == EV_CLOSE) {
			entry->time.exit = ev->time.exit;
			entry->subtype = ST_CALL;
			free_event(ev);
		} else {
			ev->time.entry = entry->time.entry;
			ev->subtype = ST_CALL;
			/* Use the line number from the entry event */
			ev->traceline = entry->traceline;

			append_event(ev, state);

			free_event(entry);
		}
	} else {
		fprintf(stderr, "bad subtype (%d), aborting\n", ev->subtype);
		abort();
	}

ignore:
	(void)0; /* avoid a warning from bison */
};

event: SOR evprefix DELIM evinfo EOR {
	$$ = $2;
	$$.type = $4.type;
	$$.attrs = $4.attrs;
	$$.ret = $4.ret;
	$$.next = NULL;
	$$.traceline = state->linenum++;
};

evprefix: DECNUM DELIM DECNUM DELIM subtype {
	$$.thread = $3;
	$$.subtype = $5;
	if ($$.subtype == ST_RETURN)
		$$.time.exit = $1;
	else
		$$.time.entry = $1;
}
| DECNUM DELIM THREAD_NA DELIM subtype {
	$$.thread = -1;
	$$.subtype = $5;
	if ($$.subtype == ST_RETURN)
		$$.time.exit = $1;
	else
		$$.time.exit = $1;
}
;

subtype: SUBTYPE_ENTRY { $$ = ST_ENTRY; }
| SUBTYPE_RETURN { $$ = ST_RETURN; }
| SUBTYPE_COMMENT { $$ = ST_COMMENT; }
;

evinfo: ET_OPEN        DELIM open_attrs              DECNUM { $$ = evstruct_i(OPEN, open, $3, $4); }
| ET_SHM_OPEN          DELIM shm_open_attrs          DECNUM { $$ = evstruct_i(SHM_OPEN, shm_open, $3, $4); }
| ET_CLOSE             DELIM close_attrs             DECNUM { $$ = evstruct_i(CLOSE, close, $3, $4); }
| ET_DUP               DELIM dup_attrs               DECNUM { $$ = evstruct_i(DUP, dup, $3, $4); }
| ET_KQUEUE            DELIM kqueue_attrs            DECNUM { $$ = evstruct_i(KQUEUE, kqueue, $3, $4); }
| ET_SOCKET            DELIM socket_attrs            DECNUM { $$ = evstruct_i(SOCKET, socket, $3, $4); }
| ET_SOCKETPAIR        DELIM socketpair_attrs        DECNUM { $$ = evstruct_i(SOCKETPAIR, socketpair, $3, $4); }
| ET_READ              DELIM read_attrs              DECNUM { $$ = evstruct_i(READ, read, $3, $4); }
| ET_WRITE             DELIM write_attrs             DECNUM { $$ = evstruct_i(WRITE, write, $3, $4); }
| ET_PREAD             DELIM pread_attrs             DECNUM { $$ = evstruct_i(PREAD, pread, $3, $4); }
| ET_PWRITE            DELIM pwrite_attrs            DECNUM { $$ = evstruct_i(PWRITE, pwrite, $3, $4); }
| ET_AIO_READ          DELIM aio_read_attrs          DECNUM { $$ = evstruct_i(AIO_READ, aio_read, $3, $4); }
| ET_AIO_ERROR         DELIM aio_error_attrs         DECNUM { $$ = evstruct_i(AIO_ERROR, aio_error, $3, $4); }
| ET_AIO_RETURN        DELIM aio_return_attrs        DECNUM { $$ = evstruct_i(AIO_RETURN, aio_return, $3, $4); }
| ET_AIO_SUSPEND       DELIM aio_suspend_attrs       DECNUM { $$ = evstruct_i(AIO_SUSPEND, aio_suspend, $3, $4); }
| ET_LSEEK             DELIM lseek_attrs             DECNUM { $$ = evstruct_o(LSEEK, lseek, $3, $4); }
| ET_FSYNC             DELIM fsync_attrs             DECNUM { $$ = evstruct_i(FSYNC, fsync, $3, $4); }
| ET_FCNTL             DELIM fcntl_attrs             DECNUM { $$ = evstruct_i(FCNTL, fcntl, $3, $4); }
| ET_FTRUNCATE         DELIM ftruncate_attrs         DECNUM { $$ = evstruct_i(FTRUNCATE, ftruncate, $3, $4); }
| ET_LINK              DELIM link_attrs              DECNUM { $$ = evstruct_i(LINK, link, $3, $4); }
| ET_SYMLINK           DELIM symlink_attrs           DECNUM { $$ = evstruct_i(SYMLINK, symlink, $3, $4); }
| ET_RMDIR             DELIM rmdir_attrs             DECNUM { $$ = evstruct_i(RMDIR, rmdir, $3, $4); }
| ET_UNLINK            DELIM unlink_attrs            DECNUM { $$ = evstruct_i(UNLINK, unlink, $3, $4); }
| ET_READLINK          DELIM readlink_attrs          DECNUM { $$ = evstruct_i(READLINK, readlink, $3, $4); }
| ET_RENAME            DELIM rename_attrs            DECNUM { $$ = evstruct_i(RENAME, rename, $3, $4); }
| ET_EXCHANGEDATA      DELIM exchangedata_attrs      DECNUM { $$ = evstruct_i(EXCHANGEDATA, exchangedata, $3, $4); }
| ET_MKDIR             DELIM mkdir_attrs             DECNUM { $$ = evstruct_i(MKDIR, mkdir, $3, $4); }
| ET_FLOCK             DELIM flock_attrs             DECNUM { $$ = evstruct_i(FLOCK, flock, $3, $4); }
| ET_MMAP              DELIM mmap_attrs              DECNUM { $$ = evstruct_p(MMAP, mmap, $3, (void*)(intptr_t)$4); }
| ET_FUTIMES           DELIM futimes_attrs           DECNUM { $$ = evstruct_i(FUTIMES, futimes, $3, $4); }
| ET_UTIMES            DELIM utimes_attrs            DECNUM { $$ = evstruct_i(UTIMES, utimes, $3, $4); }
| ET_STATFS            DELIM statfs_attrs            DECNUM { $$ = evstruct_i(STATFS, statfs, $3, $4); }
| ET_STATFS64          DELIM statfs64_attrs          DECNUM { $$ = evstruct_i(STATFS64, statfs64, $3, $4); }
| ET_FSTATFS64         DELIM fstatfs64_attrs         DECNUM { $$ = evstruct_i(FSTATFS64, fstatfs64, $3, $4); }
| ET_FSTAT             DELIM fstat_attrs             DECNUM { $$ = evstruct_i(FSTAT, fstat, $3, $4); }
| ET_LSTAT             DELIM lstat_attrs             DECNUM { $$ = evstruct_i(LSTAT, lstat, $3, $4); }
| ET_STAT              DELIM stat_attrs              DECNUM { $$ = evstruct_i(STAT, stat, $3, $4); }
| ET_FSTAT64           DELIM fstat64_attrs           DECNUM { $$ = evstruct_i(FSTAT64, fstat64, $3, $4); }
| ET_LSTAT64           DELIM lstat64_attrs           DECNUM { $$ = evstruct_i(LSTAT64, lstat64, $3, $4); }
| ET_STAT64            DELIM stat64_attrs            DECNUM { $$ = evstruct_i(STAT64, stat64, $3, $4); }
| ET_SETATTRLIST       DELIM setattrlist_attrs       DECNUM { $$ = evstruct_i(SETATTRLIST, setattrlist, $3, $4); }
| ET_GETATTRLIST       DELIM getattrlist_attrs       DECNUM { $$ = evstruct_i(GETATTRLIST, getattrlist, $3, $4); }
| ET_GETXATTR          DELIM getxattr_attrs          DECNUM { $$ = evstruct_i(GETXATTR, getxattr, $3, $4); }
| ET_FGETXATTR         DELIM fgetxattr_attrs         DECNUM { $$ = evstruct_i(FGETXATTR, fgetxattr, $3, $4); }
| ET_LISTXATTR         DELIM listxattr_attrs         DECNUM { $$ = evstruct_i(LISTXATTR, listxattr, $3, $4); }
| ET_FLISTXATTR        DELIM flistxattr_attrs        DECNUM { $$ = evstruct_i(FLISTXATTR, flistxattr, $3, $4); }
| ET_ACCESS            DELIM access_attrs            DECNUM { $$ = evstruct_i(ACCESS, access, $3, $4); }
| ET_CHOWN             DELIM chown_attrs             DECNUM { $$ = evstruct_i(CHOWN, chown, $3, $4); }
| ET_CHMOD             DELIM chmod_attrs             DECNUM { $$ = evstruct_i(CHMOD, chmod, $3, $4); }
| ET_FSETXATTR         DELIM fsetxattr_attrs         DECNUM { $$ = evstruct_i(FSETXATTR, fsetxattr, $3, $4); }
| ET_FSGETPATH         DELIM fsgetpath_attrs         DECNUM { $$ = evstruct_i(FSGETPATH, fsgetpath, $3, $4); }
| ET_GETDIRENTRIES64   DELIM getdirentries64_attrs   DECNUM { $$ = evstruct_i(GETDIRENTRIES64, getdirentries64, $3, $4); }
| ET_GETDIRENTRIESATTR DELIM getdirentriesattr_attrs DECNUM { $$ = evstruct_i(GETDIRENTRIESATTR, getdirentriesattr, $3, $4); }
| ET_GETFSSTAT64       DELIM getfsstat64_attrs       DECNUM { $$ = evstruct_i(GETFSSTAT64, getfsstat64, $3, $4); }
| ET_GETDTABLESIZE     DELIM getdtablesize_attrs     DECNUM { $$ = evstruct_i(GETDTABLESIZE, getdtablesize, $3, $4); }
| ET_CHDIR             DELIM chdir_attrs             DECNUM { $$ = evstruct_i(CHDIR, chdir, $3, $4); }
| ET_FCHDIR            DELIM fchdir_attrs            DECNUM { $$ = evstruct_i(FCHDIR, fchdir, $3, $4); }
| ET_KILL              DELIM kill_attrs              DECNUM { $$ = evstruct_i(KILL, kill, $3, $4); }
| ET_FCHFLAGS          DELIM fchflags_attrs          DECNUM { $$ = evstruct_i(FCHFLAGS, fchflags, $3, $4); }
| ET_FCHMOD            DELIM fchmod_attrs            DECNUM { $$ = evstruct_i(FCHMOD, fchmod, $3, $4); }
| ET_FCHOWN            DELIM fchown_attrs            DECNUM { $$ = evstruct_i(FCHOWN, fchown, $3, $4); }
| ET_REMOVEXATTR       DELIM removexattr_attrs       DECNUM { $$ = evstruct_i(REMOVEXATTR, removexattr, $3, $4); }
| ET_FREMOVEXATTR      DELIM fremovexattr_attrs      DECNUM { $$ = evstruct_i(FREMOVEXATTR, fremovexattr, $3, $4); }
| ET_SETXATTR          DELIM setxattr_attrs          DECNUM { $$ = evstruct_i(SETXATTR, setxattr, $3, $4); }
| ET_HFS_VNOP_PAGEIN   DELIM hfs_vnop_pagein_attrs   DECNUM { $$ = evstruct_i(HFS_VNOP_PAGEIN, hfs_vnop_pagein, $3, $4); }
| ET_POSIX_SPAWN       DELIM posix_spawn_attrs       DECNUM { $$ = evstruct_i(POSIX_SPAWN, posix_spawn, $3, $4); }

/* These don't have a return value */
| ET_CONTROLLER        DELIM controller_attrs        { $$ = evstruct_i(CONTROLLER, controller, $3, -1); }
| ET_RESOLVER          DELIM resolver_attrs          { $$ = evstruct_i(RESOLVER, resolver, $3, -1); }
| ET_ALL_CALLS         DELIM all_calls_attrs         { $$ = evstruct_i(ALL_CALLS, all_calls, $3, -1); }
| ET_SETTER            DELIM setter_attrs            { $$ = evstruct_i(SETTER, setter, $3, -1); }
| ET_BUILD_PATH        DELIM build_path_attrs        { $$ = evstruct_i(BUILD_PATH, build_path, $3, -1); }
| ET_NAMEI             DELIM namei_attrs             { $$ = evstruct_i(NAMEI, namei, $3, -1); }
| ET_VN_CREATE         DELIM vn_create_attrs         { $$ = evstruct_i(VN_CREATE, vn_create, $3, -1); }
| ET_SYSCALL_MAP       DELIM syscall_map_attrs       { $$ = evstruct_i(SYSCALL_MAP, syscall_map, $3, -1); }

/*
 * These *had* return values, but they were removed because it lead to
 * S/R conflicts.  They don't appear in the traces anyway, so meh.
 */
| ET_VNODE_PAGEIN      DELIM vnode_pagein_attrs      { $$ = evstruct_i(VNODE_PAGEIN, vnode_pagein, $3, -1); }
| ET_EXECVE            DELIM execve_attrs            { $$ = evstruct_i(EXECVE, execve, $3, -1); }
;

inum: DECNUM { $$ = $1; }
| RSRC_INUM { $$ = $1; }
;

open_attrs: STRING DELIM DECNUM DELIM o_flags DELIM DECNUM DELIM inum DELIM STRING DELIM STRING DELIM {
	$$.path = hack_path($1);
	$$.flags = $5;

	$$.dummy1 = $11;
	$$.dummy2 = $13;
};

shm_open_attrs: STRING DELIM DECNUM DELIM {
	$$.name = $1;
	$$.oflag = $3;
}

close_attrs: DECNUM DELIM {
	$$.fd = FD($1);
}

dup_attrs: DECNUM DELIM {
	$$.fd = FD($1);
}

kqueue_attrs: {
}

socket_attrs: DECNUM DELIM DECNUM DELIM DECNUM DELIM {
	$$.domain = $1;
	$$.type = $3;
	$$.protocol = $5;
}

socketpair_attrs: DECNUM DELIM DECNUM DELIM DECNUM DELIM DECNUM DELIM DECNUM DELIM {
	$$.domain = $1;
	$$.type = $3;
	$$.protocol = $5;

	$$.fd1 = FD($7);
	$$.fd2 = FD($9);
}

read_attrs: DECNUM DELIM DECNUM DELIM DECNUM DELIM STRING DELIM {
	$$.fd = FD($1);
	$$.count = $3;

	$$.dummy1 = $7;
}

write_attrs: DECNUM DELIM DECNUM DELIM DECNUM DELIM STRING DELIM {
	$$.fd = FD($1);
	$$.count = $3;

	$$.dummy1 = $7;
}

pread_attrs: DECNUM DELIM DECNUM DELIM DECNUM DELIM STRING DELIM {
	$$.fd = FD($1);
	$$.count = $3;
	$$.offset = $5;

	$$.dummy1 = $7;
}

pwrite_attrs: DECNUM DELIM DECNUM DELIM DECNUM DELIM STRING DELIM {
	$$.fd = FD($1);
	$$.count = $3;
	$$.offset = $5;

	$$.dummy1 = $7;
}

aio_read_attrs: HEXNUM DELIM DECNUM DELIM DECNUM DELIM DECNUM DELIM DECNUM DELIM STRING DELIM {
	$$.tag = $1;
	$$.fd = FD($3);
	$$.offset = $5;
	$$.nbytes = $7;

	$$.dummy1 = $11;
}

aio_error_attrs: HEXNUM DELIM {
	$$.tag = $1;
	$$.fd = FD(-1);
}

aio_return_attrs: HEXNUM DELIM {
	$$.tag = $1;
	$$.fd = FD(-1);
}

aio_suspend_attrs: DECNUM DELIM {
	$$.nent = $1;
}

lseek_attrs: DECNUM DELIM DECNUM DELIM DECNUM DELIM {
	$$.fd = FD($1);
	$$.offset = $3;
	$$.whence = $5;
}

fsync_attrs: DECNUM DELIM STRING DELIM {
	$$.fd = FD($1);

	$$.dummy1 = $3;
}

fcntl_attrs: DECNUM DELIM DECNUM DELIM DECNUM DELIM FCNTL_CMD DELIM STRING DELIM {
	/* FIXME: do we have enough info here on all these commands?  (e.g. RDADVISE) */
	$$.fd = FD($1);
	$$.cmdnum = $3;
	$$.arg.l = $5;
	$$.cmdname = $7;

	$$.dummy1 = $9;
}

ftruncate_attrs: DECNUM DELIM DECNUM DELIM STRING DELIM {
	$$.fd = FD($1);
	$$.length = $3;

	$$.dummy1 = $5;
}

link_attrs: STRING DELIM STRING DELIM {
	$$.oldpath = hack_path($1);
	$$.newpath = hack_path($3);
}

symlink_attrs: STRING DELIM STRING DELIM {
	$$.oldpath = hack_path($1);
	$$.newpath = hack_path($3);
}

rmdir_attrs: STRING DELIM {
	$$.pathname = hack_path($1);
}

unlink_attrs: STRING DELIM {
	$$.pathname = hack_path($1);
}

readlink_attrs: STRING DELIM DECNUM DELIM STRING DELIM {
	$$.path = hack_path($1);
	$$.bufsiz = $3;

	$$.dummy1 = $5;
}
| STRING DELIM DECNUM DELIM DELIM {
	$$.path = hack_path($1);
	$$.bufsiz = $3;

	$$.dummy1 = NULL;
}

rename_attrs: STRING DELIM STRING DELIM inum DELIM DECNUM DELIM inum DELIM inum DELIM STRING DELIM {
	$$.oldpath = hack_path($1);
	$$.newpath = hack_path($3);

	$$.dummy1 = $13;
}

exchangedata_attrs: STRING DELIM STRING DELIM inum DELIM inum DELIM {
	$$.path1 = hack_path($1);
	$$.path2 = hack_path($3);
}

mkdir_attrs: STRING DELIM DECNUM DELIM {
	$$.pathname = hack_path($1);
	$$.mode = $3;
}

flock_attrs: DECNUM DELIM DECNUM DELIM STRING DELIM {
	$$.fd = FD($1);
	$$.operation = $3;

	$$.dummy1 = $5;
}

mmap_attrs: HEXNUM DELIM DECNUM DELIM DECNUM DELIM DECNUM DELIM DECNUM DELIM DECNUM DELIM prot_flags DELIM map_flags DELIM {
	$$.addr = (void*)(intptr_t)$1;
	$$.length = $3;
	$$.prot = $13;
	$$.flags = $15;
	$$.fd = FD($9);
	$$.offset = $11; /* FIXME: the values for this in the trace seem wrong. */
}

futimes_attrs: DECNUM DELIM {
	$$.fd = FD($1);
}

utimes_attrs: DECNUM DELIM {
}

statfs_attrs: STRING DELIM {
	$$.path = hack_path($1);
}

statfs64_attrs: STRING DELIM {
	$$.path = hack_path($1);
}

fstatfs64_attrs: DECNUM DELIM {
	$$.fd = FD($1);
}

fstat_attrs: DECNUM DELIM {
	$$.fd = FD($1);
}

lstat_attrs: STRING DELIM {
	$$.path = hack_path($1);
}

stat_attrs: STRING DELIM {
	$$.path = hack_path($1);
}

fstat64_attrs: DECNUM DELIM {
	$$.fd = FD($1);
}

lstat64_attrs: STRING DELIM {
	$$.path = hack_path($1);
}

stat64_attrs: STRING DELIM {
	$$.path = hack_path($1);
}

setattrlist_attrs: STRING DELIM {
	$$.path = hack_path($1);
}

getattrlist_attrs: STRING DELIM {
	$$.path = hack_path($1);
}

getxattr_attrs: STRING DELIM {
	$$.path = hack_path($1);
}

fgetxattr_attrs: DECNUM DELIM {
	$$.fd = FD($1);
}

listxattr_attrs: STRING DELIM {
	$$.path = hack_path($1);
}

flistxattr_attrs: DECNUM DELIM {
	$$.fd = FD($1);
}

access_attrs: STRING DELIM DECNUM DELIM {
	$$.pathname = hack_path($1);
	$$.mode = $3;
}

chown_attrs: STRING DELIM DECNUM DELIM DECNUM DELIM {
	$$.path = hack_path($1);
	$$.owner = $3;
	$$.group = $5;
}

chmod_attrs: STRING DELIM DECNUM DELIM {
	$$.path = hack_path($1);
	$$.mode = $3;
}

fsetxattr_attrs: DECNUM DELIM STRING DELIM {
	$$.fd = FD($1);
	$$.name = $3;
}

fsgetpath_attrs: inum DELIM {
	$$.inum = $1;
}

getdirentries64_attrs: DECNUM DELIM {
	$$.fd = FD($1);
}

getdirentriesattr_attrs: DECNUM DELIM {
	$$.fd = FD($1);
}

getfsstat64_attrs: {
}

getdtablesize_attrs: {
}

chdir_attrs: STRING DELIM {
	$$.path = hack_path($1);
}

fchdir_attrs: DECNUM DELIM {
	$$.fd = FD($1);
}

kill_attrs: DECNUM DELIM DECNUM DELIM {
	$$.pid = $1;
	$$.sig = $3;
}

fchflags_attrs: DECNUM DELIM DECNUM DELIM {
	$$.fd = FD($1);
	$$.flags = $3;
}

fchmod_attrs: DECNUM DELIM DECNUM DELIM {
	$$.fd = FD($1);
	$$.mode = $3;
}

fchown_attrs: DECNUM DELIM DECNUM DELIM DECNUM DELIM {
	$$.fd = FD($1);
	$$.owner = $3;
	$$.group = $5;
}

removexattr_attrs: STRING DELIM STRING DELIM {
	$$.path = hack_path($1);
	$$.name = $3;
}

fremovexattr_attrs: DECNUM DELIM STRING DELIM {
	$$.fd = FD($1);
	$$.name = $3;
}

setxattr_attrs: STRING DELIM STRING DELIM {
	$$.path = hack_path($1);
	$$.name = $3;
}

controller_attrs: STRING {
	$$.msg = $1;
}

hfs_vnop_pagein_attrs: DECNUM DELIM DECNUM DELIM DECNUM DELIM STRING DELIM DECNUM DELIM {
	$$.offset = $1;
	$$.size = $3;
	$$.name = $7;
	$$.actual_size = $9;
}

syscall_map_attrs: STRING {
	$$.dummy1 = $1;
}

posix_spawn_attrs: STRING DELIM {
	$$.path = hack_path($1);
}

/* Mysterious odds and ends that aren't in any traces */
vnode_pagein_attrs: fieldlist { };
resolver_attrs: fieldlist { };
all_calls_attrs: fieldlist { };
setter_attrs: fieldlist { };
build_path_attrs: fieldlist { };
namei_attrs: fieldlist { };
vn_create_attrs: fieldlist { };
execve_attrs: fieldlist { };

fieldlist: field { assert(0 && "fieldlist"); }
| fieldlist DELIM field { assert(0 && "fieldlist"); }
;

field: /* empty */
| DECNUM
| HEXNUM
| STRING
;

o_flags: FLAG_O {
	$$ = newflag($1);
}
| o_flags FLAG_O {
	$$ = newflag($2);
	$$->next = $1;
}
;

prot_flags: FLAG_PROT {
	$$ = newflag($1);
}
| prot_flags FLAG_PROT {
	$$ = newflag($2);
	$$->next = $1;
}
;

map_flags: FLAG_MAP {
	$$ = newflag($1);
}
| map_flags FLAG_MAP {
	$$ = newflag($2);
	$$->next = $1;
}
;

%%

struct event* parse_ibench_trace(FILE* tracefile)
{
	int status;
	struct parser_state pstate = {
		.linenum = 1,
		.events = NULL,
		.last = NULL,
	};

	ibenchrestart(tracefile);

	status = ibenchparse(&pstate);

	ibenchlex_destroy();

	return status ? NULL : pstate.events;
}

void ibench_parser_teardown(void)
{
	pe_assoc_dealloc(&thread_prev_assoc, NULL, NULL);
}

static void ibencherror(struct parser_state* state, char const* s)
{
	fprintf(stderr, "line %d: %s\n", state->linenum, s);
}
