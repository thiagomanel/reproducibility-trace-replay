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

#include <sys/fcntl.h>

#include "trace.h"
#include "misc.h"
#include "assoc.h"
#include "event.h"
#include "flags.h"

extern int stracedebug;

struct event* parse_strace(FILE* tracefile);
void strace_parser_teardown(void);

struct strlit {
	char* s;
	int truncated;
};

}

%union {
	int64_t i;
	struct event ev;
	struct strlit strlit;
	char* str;
	struct symflag* flags;
	struct artc_iocb* iocbs;
	struct artc_ioevent* ioevents;

	struct open_attrs open;
	struct close_attrs close;
	struct dup_attrs dup;
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
	struct fdatasync_attrs fdatasync;
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
	struct getxattr_attrs getxattr;
	struct fgetxattr_attrs fgetxattr;
	struct listxattr_attrs listxattr;
	struct flistxattr_attrs flistxattr;
	struct access_attrs access;
	struct chown_attrs chown;
	struct chmod_attrs chmod;
	struct fsetxattr_attrs fsetxattr;
	struct chdir_attrs chdir;
	struct getcwd_attrs getcwd;
	struct fchmod_attrs fchmod;
	struct fchown_attrs fchown;
	struct removexattr_attrs removexattr;
	struct fremovexattr_attrs fremovexattr;
	struct setxattr_attrs setxattr;
	struct socket_attrs socket;

	struct fadvise64_attrs fadvise64;
	struct fstatfs_attrs fstatfs;
	struct getdents_attrs getdents;
	struct epoll_create_attrs epoll_create;
	struct fallocate_attrs fallocate;
	struct utimensat_attrs utimensat;
	struct pipe_attrs pipe;
	struct dup2_attrs dup2;
	struct faccessat_attrs faccessat;
	struct newfstatat_attrs newfstatat;
	struct fchownat_attrs fchownat;
	struct unlinkat_attrs unlinkat;

	struct io_setup_attrs io_setup;
	struct io_submit_attrs io_submit;
	struct io_getevents_attrs io_getevents;
	struct io_destroy_attrs io_destroy;
};

%code {
#include <sys/file.h>

#include "strace-lex.yy.h"

MAKE_ASSOC(te_assoc, int32_t, INT32_MIN, struct event*, NULL);

static te_assoc_t outstanding_events = ASSOC_INIT;

static inline struct event* thread_evlookup(int32_t tid)
{
	return te_assoc_lookup(&outstanding_events, tid);
}

static inline void thread_evinsert(int32_t tid, struct event* ev)
{
	te_assoc_insert(&outstanding_events, tid, ev);
}

static inline struct event* thread_evdelete(int32_t tid)
{
	return te_assoc_delete(&outstanding_events, tid);
}

static void straceerror(struct parser_state*, char const*);

static struct symflag* newflag(const char* s)
{
	struct symflag* f = xmalloc(sizeof(struct symflag) + strlen(s) + 1);
	f->next = NULL;
	strcpy(f->name, s);
	return f;
}

/*
 * Append 'cb' to the the list 'to'.  Yeah, yeah, it's O(n).  The
 * lists are short; we'll survive.
 */
static inline void iocb_list_append(struct artc_iocb* to, struct artc_iocb* cb)
{
	struct artc_iocb* i;

	for (i = to; i->next; i = i->next);

	i->next = cb;
	cb->next = NULL;
}

/* And this is even more embarrassing for being duplicated. */
static inline void ioevent_list_append(struct artc_ioevent* to, struct artc_ioevent* ev)
{
	struct artc_ioevent* i;

	for (i = to; i->next; i = i->next);

	i->next = ev;
	ev->next = NULL;
}

#define evstruct(t, n, a) (struct event) { \
		.type = EV_##t, \
		.attrs.n = a, \
	}

#define FD(f) (struct fduse) { .traceval = (f), .rtidx = -1, .provider = NULL, .lastuse = NULL, }

#define ZERO(x) memset(x, 0, sizeof(*x))

static char* getstr(const struct strlit* s, const char* name, const struct parser_state* state)
{
	if (s->truncated)
		fprintf(stderr, "Warning: truncated string in %s() call! (line %d)\n",
		        name, state->linenum);
	return s->s;
}

static char* getpath(const struct strlit* s, const char* name, const struct parser_state* state)
{
	if (s->truncated)
		fprintf(stderr, "Warning: truncated path (!) in %s() call! (line %d)\n",
		        name, state->linenum);
	return normalize_path(s->s);
}

static int match_whence(const char* str, const struct parser_state* state)
{
	if (!strcmp(str, "SEEK_SET"))
		return SEEK_SET;
	else if (!strcmp(str, "SEEK_CUR"))
		return SEEK_CUR;
	else if (!strcmp(str, "SEEK_END"))
		return SEEK_END;
	else {
		fprintf(stderr, "Warning: oddball lseek whence: \"%s\", treating as -1"
		        " (line %d)\n", str, state->linenum);
		return -1;
	}
}

static int match_flockop(const char* str, const struct parser_state* state)
{
	if (!strcmp(str, "LOCK_SH"))
		return LOCK_SH;
	else if (!strcmp(str, "LOCK_EX"))
		return LOCK_EX;
	else if (!strcmp(str, "LOCK_UN"))
		return LOCK_UN;
	else {
		fprintf(stderr, "Warning: oddball flock operation: \"%s\", treating as -1"
		        " (line %d)\n", str, state->linenum);
		return -1;
	}
}

static void set_evret(struct event* ev, int64_t value)
{
	switch (ev->type) {
	case EV_MMAP:
		ev->ret.p = (void*)(intptr_t)value;
		break;
	case EV_LSEEK:
		ev->ret.o = (off_t)value;
		break;
	default:
		ev->ret.i = value;
		break;
	}
}

}

%{

%}

%token <i> DECNUM HEXNUM OCTNUM TIME
%token <i> THREAD_EXIT
%token <str> FLAG_O FLAG_PROT FLAG_MAP FLAG_FD FLAG_ACCESS FLAG_AT
%token <str> POSIX_FADVICE FLOCK_OP SEEK_WHENCE
%token <i> SOCKDOMAIN SOCKTYPE
%token <strlit> STRING

%token <str> ERRNO
%token ERRSTR

%token UNFINISHED DETACHED
%token RESUME_START RESUME_END
%token FLAG_RET

/* Typeless because its value is (at least currently) unused. */
%token FLAG_SOCK

%token KW_DATA KW_FILEDES KW_NBYTES KW_OFFSET KW_STR KW_BUF

/* The KW_ prefix is slightly out of place here. */
%token KW_AT_FDCWD

%token NEWLINE COMMA EQUAL PIPE COLON
%token LPAREN RPAREN
%token LESSTHAN GREATERTHAN
%token LBRACE RBRACE
%token LBRACKET RBRACKET

%token STAT_STRUCT DENT_STRUCT STATFS_STRUCT
%token FLOCK_STRUCT TIMESPEC_STRUCT SOCKADDR_STRUCT

%token FCMD_DUPFD FCMD_GETFD FCMD_SETFD FCMD_GETFL
%token FCMD_SETFL FCMD_SETLK FCMD_SETLKW

%token ET_OPEN ET_OPENAT
%token ET_CLOSE
%token ET_DUP
%token ET_READ ET_PREAD
%token ET_WRITE ET_PWRITE
%token ET_AIO_READ ET_AIO_ERROR ET_AIO_RETURN ET_AIO_SUSPEND
%token ET_ACCEPT
%token ET_LSEEK
%token ET_FSYNC ET_FDATASYNC
%token ET_FCNTL
%token ET_FTRUNCATE
%token ET_LINK ET_SYMLINK ET_READLINK
%token ET_UNLINK ET_UNLINKAT ET_RMDIR ET_MKDIR
%token ET_RENAME ET_EXCHANGEDATA
%token ET_FLOCK
%token ET_MMAP
%token ET_UTIMES ET_UTIMENSAT
%token ET_STAT ET_FSTAT ET_NEWFSTATAT ET_STAT64 ET_FSTAT64 ET_LSTAT ET_LSTAT64
%token ET_STATFS ET_FSTATFS ET_STATFS64 ET_FSTATFS64
%token ET_SETXATTR ET_FSETXATTR
%token ET_LISTXATTR ET_FLISTXATTR
%token ET_GETXATTR ET_FGETXATTR
%token ET_REMOVEXATTR ET_FREMOVEXATTR
%token ET_ACCESS ET_FACCESSAT
%token ET_CHOWN ET_FCHOWN ET_FCHOWNAT ET_CHMOD ET_FCHMOD
%token ET_CHDIR ET_GETCWD
%token ET_SOCKET
%token ET_PIPE
%token ET_DUP2 ET_DUP3

%token ET_EPOLL_CREATE
%token ET_FADVISE64
%token ET_GETDENTS
%token ET_FALLOCATE

%token ET_IO_SETUP ET_IO_SUBMIT ET_IO_GETEVENTS ET_IO_DESTROY

%token END 0 "EOF"

%type <i> elapsed_time
%type <i> integer
%type <str> maybe_string
%type <i> call_result

%type <ev> full_event full_call
%type <ev> event_begin call_begin
%type <ev> event_end call_end

%type <open>         open_allparams         open_beginparams         open_endparams
/* openat(AT_FDCWD,...) is translated to open(...) */
%type <open>         openat_allparams       openat_beginparams       openat_endparams
%type <close>        close_allparams        close_beginparams        close_endparams
%type <dup>          dup_allparams          dup_beginparams          dup_endparams
/* accept(2) is translated to dup(2) */
%type <dup>          accept_allparams       accept_beginparams       accept_endparams
%type <read>         read_allparams         read_beginparams         read_endparams
%type <write>        write_allparams        write_beginparams        write_endparams
%type <pread>        pread_allparams        pread_beginparams        pread_endparams
%type <pwrite>       pwrite_allparams       pwrite_beginparams       pwrite_endparams
%type <lseek>        lseek_allparams        lseek_beginparams        lseek_endparams
%type <fsync>        fsync_allparams        fsync_beginparams        fsync_endparams
%type <fdatasync>    fdatasync_allparams    fdatasync_beginparams    fdatasync_endparams
%type <fcntl>        fcntl_allparams        fcntl_beginparams        fcntl_endparams
%type <ftruncate>    ftruncate_allparams    ftruncate_beginparams    ftruncate_endparams
%type <link>         link_allparams         link_beginparams         link_endparams
%type <rmdir>        rmdir_allparams        rmdir_beginparams        rmdir_endparams
%type <unlink>       unlink_allparams       unlink_beginparams       unlink_endparams
%type <readlink>     readlink_allparams     readlink_beginparams     readlink_endparams
%type <rename>       rename_allparams       rename_beginparams       rename_endparams
%type <mkdir>        mkdir_allparams        mkdir_beginparams        mkdir_endparams
%type <flock>        flock_allparams        flock_beginparams        flock_endparams
%type <mmap>         mmap_allparams         mmap_beginparams         mmap_endparams
%type <utimensat>    utimensat_allparams    utimensat_beginparams    utimensat_endparams
%type <statfs>       statfs_allparams       statfs_beginparams       statfs_endparams
%type <fstat>        fstat_allparams        fstat_beginparams        fstat_endparams
%type <lstat>        lstat_allparams        lstat_beginparams        lstat_endparams
%type <stat>         stat_allparams         stat_beginparams         stat_endparams
%type <getxattr>     getxattr_allparams     getxattr_beginparams     getxattr_endparams
%type <fgetxattr>    fgetxattr_allparams    fgetxattr_beginparams    fgetxattr_endparams
%type <listxattr>    listxattr_allparams    listxattr_beginparams    listxattr_endparams
%type <flistxattr>   flistxattr_allparams   flistxattr_beginparams   flistxattr_endparams
%type <access>       access_allparams       access_beginparams       access_endparams
%type <chown>        chown_allparams        chown_beginparams        chown_endparams
%type <chmod>        chmod_allparams        chmod_beginparams        chmod_endparams
%type <fsetxattr>    fsetxattr_allparams    fsetxattr_beginparams    fsetxattr_endparams
%type <chdir>        chdir_allparams        chdir_beginparams        chdir_endparams
%type <getcwd>       getcwd_allparams       getcwd_beginparams       getcwd_endparams
%type <fchmod>       fchmod_allparams       fchmod_beginparams       fchmod_endparams
%type <fchown>       fchown_allparams       fchown_beginparams       fchown_endparams
%type <removexattr>  removexattr_allparams  removexattr_beginparams  removexattr_endparams
%type <fremovexattr> fremovexattr_allparams fremovexattr_beginparams fremovexattr_endparams
%type <setxattr>     setxattr_allparams     setxattr_beginparams     setxattr_endparams
%type <symlink>      symlink_allparams      symlink_beginparams      symlink_endparams
%type <socket>       socket_allparams       socket_beginparams       socket_endparams
%type <pipe>         pipe_allparams         pipe_beginparams         pipe_endparams
%type <dup2>         dup2_allparams         dup2_beginparams         dup2_endparams
%type <faccessat>    faccessat_allparams    faccessat_beginparams    faccessat_endparams
%type <newfstatat>   newfstatat_allparams   newfstatat_beginparams   newfstatat_endparams
%type <fchownat>     fchownat_allparams     fchownat_beginparams     fchownat_endparams
%type <unlinkat>     unlinkat_allparams     unlinkat_beginparams     unlinkat_endparams
/* dup3(2) is translated to dup2(2) */
%type <dup2>         dup3_allparams         dup3_beginparams         dup3_endparams

%type <fadvise64>    fadvise64_allparams    fadvise64_beginparams    fadvise64_endparams
%type <fstatfs>      fstatfs_allparams      fstatfs_beginparams      fstatfs_endparams
%type <getdents>     getdents_allparams     getdents_beginparams     getdents_endparams
%type <epoll_create> epoll_create_allparams epoll_create_beginparams epoll_create_endparams
%type <fallocate>    fallocate_allparams    fallocate_beginparams    fallocate_endparams

%type <io_setup>     io_setup_allparams     io_setup_beginparams     io_setup_endparams
%type <io_submit>    io_submit_allparams    io_submit_beginparams    io_submit_endparams
%type <io_getevents> io_getevents_allparams io_getevents_beginparams io_getevents_endparams
%type <io_destroy>   io_destroy_allparams   io_destroy_beginparams   io_destroy_endparams

%type <flags> o_flags optional_o_flags prot_flags map_flags access_flags at_flags
%type <str> fd_flags

%type <iocbs> iocb_array optional_iocbs iocb_list iocb
%type <i> iocb_fd iocb_nbytes iocb_offset maybe_iocb_data_comma
%type <str> iocb_str

%type <ioevents> ioevent_array optional_ioevents ioevent_list ioevent

%debug

%parse-param {struct parser_state* state}

%name-prefix "strace"

%error-verbose

%%

input: /* empty */
| input full_event NEWLINE {
	struct event* ev = xmalloc(sizeof(struct event));
	*ev = $2;

	ev->traceline = state->linenum++;
	ev->subtype = ST_CALL;

	append_event(ev, state);
}
| input event_begin NEWLINE {
	struct event* ev = xmalloc(sizeof(*ev));
	*ev = $2;

	ev->traceline = state->linenum++;
	ev->subtype = ST_CALL;

	if (thread_evlookup(ev->thread)) {
		fprintf(stderr, "error: multiple simultaneous outstanding calls"
		        " in thread %d at line %d\n", ev->thread, state->linenum);
		exit(1);
	}
	thread_evinsert(ev->thread, ev);

	if (ev->type == EV_IO_SUBMIT || ev->type == EV_CLOSE)
		append_event(ev, state);
}
| input event_end NEWLINE {
	nstime_t tdiff;
	struct event* entry = thread_evdelete($2.thread);
	if (!entry) {
		fprintf(stderr, "malformed trace file: no entry found for "
		        "call completion on line %d\n", state->linenum);
		exit(1);
	}

	assert($2.thread == entry->thread);

	entry->time.exit = $2.time.exit;
	tdiff = $2.time.entry - entry->time.entry;
	tdiff = tdiff < 0 ? -tdiff : tdiff;
	if (tdiff > 1000000)
		fprintf(stderr, "Warning: strace internal timing inaccuracy > 1ms"
		        " (%"PRInst" ns at line %d)\n", tdiff, state->linenum);

	merge_event(entry, &$2);
	entry->ret = $2.ret;

	if (entry->type != EV_IO_SUBMIT && entry->type != EV_CLOSE)
		append_event(entry, state);

	state->linenum++;
}
| input thread_exit NEWLINE {
	state->linenum++;
};

full_event: DECNUM TIME full_call call_result elapsed_time {
	$$.thread = $1;
	$$.time.entry = $2;
	$$.type = $3.type;
	$$.attrs = $3.attrs;
	set_evret(&$$, $4);
	$$.time.exit = $$.time.entry + $5;
};

event_end: DECNUM TIME RESUME_START call_end RPAREN call_result elapsed_time {
	$$.thread = $1;
	$$.time.exit = $2;
	$$.type = $4.type;
	$$.attrs = $4.attrs;
	set_evret(&$$, $6);
	$$.time.entry = $$.time.exit - $7;
};

call_end: UNFINISHED { }
| DETACHED { };

event_begin: DECNUM TIME call_begin call_end {
	$$.type = $3.type;
	$$.attrs = $3.attrs;
	$$.thread = $1;
	$$.time.entry = $2;
};

thread_exit: DECNUM TIME THREAD_EXIT {
};

call_result: EQUAL integer {
	$$ = $2;
}
| EQUAL DECNUM ERRNO ERRSTR {
	$$ = $2;
}
| EQUAL integer LPAREN FLAG_RET RPAREN {
	$$ = $2;
};

elapsed_time: LESSTHAN TIME GREATERTHAN {
	$$ = $2;
};

/* ---------------------------------------------------------------------- */

full_call: ET_OPEN LPAREN open_allparams         RPAREN { $$ = evstruct(OPEN, open, $3); }
| ET_READ          LPAREN read_allparams         RPAREN { $$ = evstruct(READ, read, $3); }
| ET_CLOSE         LPAREN close_allparams        RPAREN { $$ = evstruct(CLOSE, close, $3); }
| ET_STAT          LPAREN stat_allparams         RPAREN { $$ = evstruct(STAT, stat, $3); }
| ET_PREAD         LPAREN pread_allparams        RPAREN { $$ = evstruct(PREAD, pread, $3); }
| ET_MMAP          LPAREN mmap_allparams         RPAREN { $$ = evstruct(MMAP, mmap, $3); }
| ET_FADVISE64     LPAREN fadvise64_allparams    RPAREN { $$ = evstruct(FADVISE64, fadvise64, $3); }
| ET_STATFS        LPAREN statfs_allparams       RPAREN { $$ = evstruct(STATFS, statfs, $3); }
| ET_FCNTL         LPAREN fcntl_allparams        RPAREN { $$ = evstruct(FCNTL, fcntl, $3); }
| ET_FSTATFS       LPAREN fstatfs_allparams      RPAREN { $$ = evstruct(FSTATFS, fstatfs, $3); }
| ET_LSEEK         LPAREN lseek_allparams        RPAREN { $$ = evstruct(LSEEK, lseek, $3); }
| ET_GETDENTS      LPAREN getdents_allparams     RPAREN { $$ = evstruct(GETDENTS, getdents, $3); }
| ET_FSTAT         LPAREN fstat_allparams        RPAREN { $$ = evstruct(FSTAT, fstat, $3); }
| ET_EPOLL_CREATE  LPAREN epoll_create_allparams RPAREN { $$ = evstruct(EPOLL_CREATE, epoll_create, $3); }
| ET_LSTAT         LPAREN lstat_allparams        RPAREN { $$ = evstruct(LSTAT, lstat, $3); }
| ET_ACCESS        LPAREN access_allparams       RPAREN { $$ = evstruct(ACCESS, access, $3); }
| ET_FGETXATTR     LPAREN fgetxattr_allparams    RPAREN { $$ = evstruct(FGETXATTR, fgetxattr, $3); }
| ET_READLINK      LPAREN readlink_allparams     RPAREN { $$ = evstruct(READLINK, readlink, $3); }
| ET_LISTXATTR     LPAREN listxattr_allparams    RPAREN { $$ = evstruct(LISTXATTR, listxattr, $3); }
| ET_WRITE         LPAREN write_allparams        RPAREN { $$ = evstruct(WRITE, write, $3); }
| ET_DUP           LPAREN dup_allparams          RPAREN { $$ = evstruct(DUP, dup, $3); }
| ET_MKDIR         LPAREN mkdir_allparams        RPAREN { $$ = evstruct(MKDIR, mkdir, $3); }
| ET_FLOCK         LPAREN flock_allparams        RPAREN { $$ = evstruct(FLOCK, flock, $3); }
| ET_FTRUNCATE     LPAREN ftruncate_allparams    RPAREN { $$ = evstruct(FTRUNCATE, ftruncate, $3); }
| ET_FSYNC         LPAREN fsync_allparams        RPAREN { $$ = evstruct(FSYNC, fsync, $3); }
| ET_FDATASYNC     LPAREN fdatasync_allparams    RPAREN { $$ = evstruct(FDATASYNC, fdatasync, $3); }
| ET_CHMOD         LPAREN chmod_allparams        RPAREN { $$ = evstruct(CHMOD, chmod, $3); }
| ET_CHOWN         LPAREN chown_allparams        RPAREN { $$ = evstruct(CHOWN, chown, $3); }
| ET_RENAME        LPAREN rename_allparams       RPAREN { $$ = evstruct(RENAME, rename, $3); }
| ET_CHDIR         LPAREN chdir_allparams        RPAREN { $$ = evstruct(CHDIR, chdir, $3); }
| ET_GETCWD        LPAREN getcwd_allparams       RPAREN { $$ = evstruct(GETCWD, getcwd, $3); }
| ET_FSETXATTR     LPAREN fsetxattr_allparams    RPAREN { $$ = evstruct(FSETXATTR, fsetxattr, $3); }
| ET_PWRITE        LPAREN pwrite_allparams       RPAREN { $$ = evstruct(PWRITE, pwrite, $3); }
| ET_UNLINK        LPAREN unlink_allparams       RPAREN { $$ = evstruct(UNLINK, unlink, $3); }
| ET_SETXATTR      LPAREN setxattr_allparams     RPAREN { $$ = evstruct(SETXATTR, setxattr, $3); }
| ET_LINK          LPAREN link_allparams         RPAREN { $$ = evstruct(LINK, link, $3); }
| ET_RMDIR         LPAREN rmdir_allparams        RPAREN { $$ = evstruct(RMDIR, rmdir, $3); }
| ET_GETXATTR      LPAREN getxattr_allparams     RPAREN { $$ = evstruct(GETXATTR, getxattr, $3); }
| ET_FLISTXATTR    LPAREN flistxattr_allparams   RPAREN { $$ = evstruct(FLISTXATTR, flistxattr, $3); }
| ET_REMOVEXATTR   LPAREN removexattr_allparams  RPAREN { $$ = evstruct(REMOVEXATTR, removexattr, $3); }
| ET_FREMOVEXATTR  LPAREN fremovexattr_allparams RPAREN { $$ = evstruct(FREMOVEXATTR, fremovexattr, $3); }
| ET_FALLOCATE     LPAREN fallocate_allparams    RPAREN { $$ = evstruct(FALLOCATE, fallocate, $3); }
| ET_FCHMOD        LPAREN fchmod_allparams       RPAREN { $$ = evstruct(FCHMOD, fchmod, $3); }
| ET_UTIMENSAT     LPAREN utimensat_allparams    RPAREN { $$ = evstruct(UTIMENSAT, utimensat, $3); }
| ET_FCHOWN        LPAREN fchown_allparams       RPAREN { $$ = evstruct(FCHOWN, fchown, $3); }
| ET_SYMLINK       LPAREN symlink_allparams      RPAREN { $$ = evstruct(SYMLINK, symlink, $3); }
/* accept(2) is treated as dup(2) */
| ET_ACCEPT        LPAREN accept_allparams       RPAREN { $$ = evstruct(DUP, dup, $3); }
| ET_IO_SETUP      LPAREN io_setup_allparams     RPAREN { $$ = evstruct(IO_SETUP, io_setup, $3); }
| ET_IO_SUBMIT     LPAREN io_submit_allparams    RPAREN { $$ = evstruct(IO_SUBMIT, io_submit, $3); }
| ET_IO_GETEVENTS  LPAREN io_getevents_allparams RPAREN { $$ = evstruct(IO_GETEVENTS, io_getevents, $3); }
| ET_IO_DESTROY    LPAREN io_destroy_allparams   RPAREN { $$ = evstruct(IO_DESTROY, io_destroy, $3); }
/* openat(AT_FDCWD,...) is treated as open(...) */
| ET_OPENAT        LPAREN openat_allparams       RPAREN { $$ = evstruct(OPEN, open, $3); }
| ET_SOCKET        LPAREN socket_allparams       RPAREN { $$ = evstruct(SOCKET, socket, $3); }
| ET_PIPE          LPAREN pipe_allparams         RPAREN { $$ = evstruct(PIPE, pipe, $3); }
| ET_DUP2          LPAREN dup2_allparams         RPAREN { $$ = evstruct(DUP2, dup2, $3); }
| ET_FACCESSAT     LPAREN faccessat_allparams    RPAREN { $$ = evstruct(FACCESSAT, faccessat, $3); }
| ET_NEWFSTATAT    LPAREN newfstatat_allparams   RPAREN { $$ = evstruct(NEWFSTATAT, newfstatat, $3); }
| ET_FCHOWNAT      LPAREN fchownat_allparams     RPAREN { $$ = evstruct(FCHOWNAT, fchownat, $3); }
| ET_UNLINKAT      LPAREN unlinkat_allparams     RPAREN { $$ = evstruct(UNLINKAT, unlinkat, $3); }
/* dup3(2) is treated as dup2(2) */
| ET_DUP3          LPAREN dup3_allparams         RPAREN { $$ = evstruct(DUP2, dup2, $3); }
;

call_begin: ET_STAT LPAREN stat_beginparams         { $$ = evstruct(STAT, stat, $3); }
| ET_EPOLL_CREATE   LPAREN epoll_create_beginparams { $$ = evstruct(EPOLL_CREATE, epoll_create, $3); }
| ET_OPEN           LPAREN open_beginparams         { $$ = evstruct(OPEN, open, $3); }
| ET_FCNTL          LPAREN fcntl_beginparams        { $$ = evstruct(FCNTL, fcntl, $3); }
| ET_FSTATFS        LPAREN fstatfs_beginparams      { $$ = evstruct(FSTATFS, fstatfs, $3); }
| ET_DUP            LPAREN dup_beginparams          { $$ = evstruct(DUP, dup, $3); }
| ET_LSEEK          LPAREN lseek_beginparams        { $$ = evstruct(LSEEK, lseek, $3); }
| ET_CLOSE          LPAREN close_beginparams        { $$ = evstruct(CLOSE, close, $3); }
| ET_GETDENTS       LPAREN getdents_beginparams     { $$ = evstruct(GETDENTS, getdents, $3); }
| ET_READ           LPAREN read_beginparams         { $$ = evstruct(READ, read, $3); }
| ET_WRITE          LPAREN write_beginparams        { $$ = evstruct(WRITE, write, $3); }
| ET_MMAP           LPAREN mmap_beginparams         { $$ = evstruct(MMAP, mmap, $3); }
| ET_FSTAT          LPAREN fstat_beginparams        { $$ = evstruct(FSTAT, fstat, $3); }
| ET_LSTAT          LPAREN lstat_beginparams        { $$ = evstruct(LSTAT, lstat, $3); }
| ET_PREAD          LPAREN pread_beginparams        { $$ = evstruct(PREAD, pread, $3); }
| ET_ACCESS         LPAREN access_beginparams       { $$ = evstruct(ACCESS, access, $3); }
| ET_FGETXATTR      LPAREN fgetxattr_beginparams    { $$ = evstruct(FGETXATTR, fgetxattr, $3); }
| ET_STATFS         LPAREN statfs_beginparams       { $$ = evstruct(STATFS, statfs, $3); }
| ET_FLOCK          LPAREN flock_beginparams        { $$ = evstruct(FLOCK, flock, $3); }
| ET_READLINK       LPAREN readlink_beginparams     { $$ = evstruct(READLINK, readlink, $3); }
| ET_FADVISE64      LPAREN fadvise64_beginparams    { $$ = evstruct(FADVISE64, fadvise64, $3); }
| ET_LISTXATTR      LPAREN listxattr_beginparams    { $$ = evstruct(LISTXATTR, listxattr, $3); }
| ET_CHDIR          LPAREN chdir_beginparams        { $$ = evstruct(CHDIR, chdir, $3); }
| ET_GETCWD         LPAREN getcwd_beginparams       { $$ = evstruct(GETCWD, getcwd, $3); }
| ET_FSETXATTR      LPAREN fsetxattr_beginparams    { $$ = evstruct(FSETXATTR, fsetxattr, $3); }
| ET_PWRITE         LPAREN pwrite_beginparams       { $$ = evstruct(PWRITE, pwrite, $3); }
| ET_UNLINK         LPAREN unlink_beginparams       { $$ = evstruct(UNLINK, unlink, $3); }
| ET_FSYNC          LPAREN fsync_beginparams        { $$ = evstruct(FSYNC, fsync, $3); }
| ET_FDATASYNC      LPAREN fdatasync_beginparams    { $$ = evstruct(FDATASYNC, fdatasync, $3); }
| ET_SETXATTR       LPAREN setxattr_beginparams     { $$ = evstruct(SETXATTR, setxattr, $3); }
| ET_LINK           LPAREN link_beginparams         { $$ = evstruct(LINK, link, $3); }
| ET_CHMOD          LPAREN chmod_beginparams        { $$ = evstruct(CHMOD, chmod, $3); }
| ET_CHOWN          LPAREN chown_beginparams        { $$ = evstruct(CHOWN, chown, $3); }
| ET_RENAME         LPAREN rename_beginparams       { $$ = evstruct(RENAME, rename, $3); }
| ET_FTRUNCATE      LPAREN ftruncate_beginparams    { $$ = evstruct(FTRUNCATE, ftruncate, $3); }
| ET_RMDIR          LPAREN rmdir_beginparams        { $$ = evstruct(RMDIR, rmdir, $3); }
| ET_GETXATTR       LPAREN getxattr_beginparams     { $$ = evstruct(GETXATTR, getxattr, $3); }
| ET_MKDIR          LPAREN mkdir_beginparams        { $$ = evstruct(MKDIR, mkdir, $3); }
| ET_FLISTXATTR     LPAREN flistxattr_beginparams   { $$ = evstruct(FLISTXATTR, flistxattr, $3); }
| ET_REMOVEXATTR    LPAREN removexattr_beginparams  { $$ = evstruct(REMOVEXATTR, removexattr, $3); }
| ET_FREMOVEXATTR   LPAREN fremovexattr_beginparams { $$ = evstruct(FREMOVEXATTR, fremovexattr, $3); }
| ET_FALLOCATE      LPAREN fallocate_beginparams    { $$ = evstruct(FALLOCATE, fallocate, $3); }
| ET_FCHMOD         LPAREN fchmod_beginparams       { $$ = evstruct(FCHMOD, fchmod, $3); }
| ET_UTIMENSAT      LPAREN utimensat_beginparams    { $$ = evstruct(UTIMENSAT, utimensat, $3); }
| ET_FCHOWN         LPAREN fchown_beginparams       { $$ = evstruct(FCHOWN, fchown, $3); }
| ET_SYMLINK        LPAREN symlink_beginparams      { $$ = evstruct(SYMLINK, symlink, $3); }
/* accept(2) is treated as dup(2) */
| ET_ACCEPT         LPAREN accept_beginparams       { $$ = evstruct(DUP, dup, $3); }
| ET_IO_SETUP       LPAREN io_setup_beginparams     { $$ = evstruct(IO_SETUP, io_setup, $3); }
| ET_IO_SUBMIT      LPAREN io_submit_beginparams    { $$ = evstruct(IO_SUBMIT, io_submit, $3); }
| ET_IO_GETEVENTS   LPAREN io_getevents_beginparams { $$ = evstruct(IO_GETEVENTS, io_getevents, $3); }
| ET_IO_DESTROY     LPAREN io_destroy_beginparams   { $$ = evstruct(IO_DESTROY, io_destroy, $3); }
/* openat(AT_FDCWD,...) is treated as open(...) */
| ET_OPENAT         LPAREN openat_beginparams       { $$ = evstruct(OPEN, open, $3); }
| ET_SOCKET         LPAREN socket_beginparams       { $$ = evstruct(SOCKET, socket, $3); }
| ET_PIPE           LPAREN pipe_beginparams         { $$ = evstruct(PIPE, pipe, $3); }
| ET_DUP2           LPAREN dup2_beginparams         { $$ = evstruct(DUP2, dup2, $3); }
| ET_FACCESSAT      LPAREN faccessat_beginparams    { $$ = evstruct(FACCESSAT, faccessat, $3); }
| ET_NEWFSTATAT     LPAREN newfstatat_beginparams   { $$ = evstruct(NEWFSTATAT, newfstatat, $3); }
| ET_FCHOWNAT       LPAREN fchownat_beginparams     { $$ = evstruct(FCHOWNAT, fchownat, $3); }
| ET_UNLINKAT       LPAREN unlinkat_beginparams     { $$ = evstruct(UNLINKAT, unlinkat, $3); }
/* dup3(2) is treated as dup2(2) */
| ET_DUP3           LPAREN dup3_beginparams         { $$ = evstruct(DUP2, dup2, $3); }
;

call_end: ET_STAT RESUME_END stat_endparams         { $$ = evstruct(STAT, stat, $3); }
| ET_EPOLL_CREATE RESUME_END epoll_create_endparams { $$ = evstruct(EPOLL_CREATE, epoll_create, $3); }
| ET_OPEN         RESUME_END open_endparams         { $$ = evstruct(OPEN, open, $3); }
| ET_FCNTL        RESUME_END fcntl_endparams        { $$ = evstruct(FCNTL, fcntl, $3); }
| ET_FSTATFS      RESUME_END fstatfs_endparams      { $$ = evstruct(FSTATFS, fstatfs, $3); }
| ET_DUP          RESUME_END dup_endparams          { $$ = evstruct(DUP, dup, $3); }
| ET_LSEEK        RESUME_END lseek_endparams        { $$ = evstruct(LSEEK, lseek, $3); }
| ET_CLOSE        RESUME_END close_endparams        { $$ = evstruct(CLOSE, close, $3); }
| ET_GETDENTS     RESUME_END getdents_endparams     { $$ = evstruct(GETDENTS, getdents, $3); }
| ET_READ         RESUME_END read_endparams         { $$ = evstruct(READ, read, $3); }
| ET_WRITE        RESUME_END write_endparams        { $$ = evstruct(WRITE, write, $3); }
| ET_MMAP         RESUME_END mmap_endparams         { $$ = evstruct(MMAP, mmap, $3); }
| ET_FSTAT        RESUME_END fstat_endparams        { $$ = evstruct(FSTAT, fstat, $3); }
| ET_LSTAT        RESUME_END lstat_endparams        { $$ = evstruct(LSTAT, lstat, $3); }
| ET_PREAD        RESUME_END pread_endparams        { $$ = evstruct(PREAD, pread, $3); }
| ET_ACCESS       RESUME_END access_endparams       { $$ = evstruct(ACCESS, access, $3); }
| ET_FGETXATTR    RESUME_END fgetxattr_endparams    { $$ = evstruct(FGETXATTR, fgetxattr, $3); }
| ET_STATFS       RESUME_END statfs_endparams       { $$ = evstruct(STATFS, statfs, $3); }
| ET_FLOCK        RESUME_END flock_endparams        { $$ = evstruct(FLOCK, flock, $3); }
| ET_READLINK     RESUME_END readlink_endparams     { $$ = evstruct(READLINK, readlink, $3); }
| ET_FADVISE64    RESUME_END fadvise64_endparams    { $$ = evstruct(FADVISE64, fadvise64, $3); }
| ET_LISTXATTR    RESUME_END listxattr_endparams    { $$ = evstruct(LISTXATTR, listxattr, $3); }
| ET_CHDIR        RESUME_END chdir_endparams        { $$ = evstruct(CHDIR, chdir, $3); }
| ET_GETCWD       RESUME_END getcwd_endparams       { $$ = evstruct(GETCWD, getcwd, $3); }
| ET_FSETXATTR    RESUME_END fsetxattr_endparams    { $$ = evstruct(FSETXATTR, fsetxattr, $3); }
| ET_PWRITE       RESUME_END pwrite_endparams       { $$ = evstruct(PWRITE, pwrite, $3); }
| ET_UNLINK       RESUME_END unlink_endparams       { $$ = evstruct(UNLINK, unlink, $3); }
| ET_FSYNC        RESUME_END fsync_endparams        { $$ = evstruct(FSYNC, fsync, $3); }
| ET_FDATASYNC    RESUME_END fdatasync_endparams    { $$ = evstruct(FDATASYNC, fdatasync, $3); }
| ET_SETXATTR     RESUME_END setxattr_endparams     { $$ = evstruct(SETXATTR, setxattr, $3); }
| ET_LINK         RESUME_END link_endparams         { $$ = evstruct(LINK, link, $3); }
| ET_CHMOD        RESUME_END chmod_endparams        { $$ = evstruct(CHMOD, chmod, $3); }
| ET_CHOWN        RESUME_END chown_endparams        { $$ = evstruct(CHOWN, chown, $3); }
| ET_RENAME       RESUME_END rename_endparams       { $$ = evstruct(RENAME, rename, $3); }
| ET_FTRUNCATE    RESUME_END ftruncate_endparams    { $$ = evstruct(FTRUNCATE, ftruncate, $3); }
| ET_RMDIR        RESUME_END rmdir_endparams        { $$ = evstruct(RMDIR, rmdir, $3); }
| ET_GETXATTR     RESUME_END getxattr_endparams     { $$ = evstruct(GETXATTR, getxattr, $3); }
| ET_MKDIR        RESUME_END mkdir_endparams        { $$ = evstruct(MKDIR, mkdir, $3); }
| ET_FLISTXATTR   RESUME_END flistxattr_endparams   { $$ = evstruct(FLISTXATTR, flistxattr, $3); }
| ET_REMOVEXATTR  RESUME_END removexattr_endparams  { $$ = evstruct(REMOVEXATTR, removexattr, $3); }
| ET_FREMOVEXATTR RESUME_END fremovexattr_endparams { $$ = evstruct(FREMOVEXATTR, fremovexattr, $3); }
| ET_FALLOCATE    RESUME_END fallocate_endparams    { $$ = evstruct(FALLOCATE, fallocate, $3); }
| ET_FCHMOD       RESUME_END fchmod_endparams       { $$ = evstruct(FCHMOD, fchmod, $3); }
| ET_UTIMENSAT    RESUME_END utimensat_endparams    { $$ = evstruct(UTIMENSAT, utimensat, $3); }
| ET_FCHOWN       RESUME_END fchown_endparams       { $$ = evstruct(FCHOWN, fchown, $3); }
| ET_SYMLINK      RESUME_END symlink_endparams      { $$ = evstruct(SYMLINK, symlink, $3); }
/* accept(2) is treated as dup(2) */
| ET_ACCEPT       RESUME_END accept_endparams       { $$ = evstruct(DUP, dup, $3); }
| ET_IO_SETUP     RESUME_END io_setup_endparams     { $$ = evstruct(IO_SETUP, io_setup, $3); }
| ET_IO_SUBMIT    RESUME_END io_submit_endparams    { $$ = evstruct(IO_SUBMIT, io_submit, $3); }
| ET_IO_GETEVENTS RESUME_END io_getevents_endparams { $$ = evstruct(IO_GETEVENTS, io_getevents, $3); }
| ET_IO_DESTROY   RESUME_END io_destroy_endparams   { $$ = evstruct(IO_DESTROY, io_destroy, $3); }
/* openat(AT_FDCWD,...) is treated as open(...) */
| ET_OPENAT       RESUME_END openat_endparams       { $$ = evstruct(OPEN, open, $3); }
| ET_SOCKET       RESUME_END socket_endparams       { $$ = evstruct(SOCKET, socket, $3); }
| ET_PIPE         RESUME_END pipe_endparams         { $$ = evstruct(PIPE, pipe, $3); }
| ET_DUP2         RESUME_END dup2_endparams         { $$ = evstruct(DUP2, dup2, $3); }
| ET_FACCESSAT    RESUME_END faccessat_endparams    { $$ = evstruct(FACCESSAT, faccessat, $3); }
| ET_NEWFSTATAT   RESUME_END newfstatat_endparams   { $$ = evstruct(NEWFSTATAT, newfstatat, $3); }
| ET_FCHOWNAT     RESUME_END fchownat_endparams     { $$ = evstruct(FCHOWNAT, fchownat, $3); }
| ET_UNLINKAT     RESUME_END unlinkat_endparams     { $$ = evstruct(UNLINKAT, unlinkat, $3); }
/* dup3(2) is treated as dup2(2) */
| ET_DUP3         RESUME_END dup3_endparams         { $$ = evstruct(DUP2, dup2, $3); }
;

/* ---------------------------------------- */

open_allparams: STRING COMMA o_flags {
	ZERO(&$$);
	$$.path = getpath(&$1, "open", state);
	$$.flags = $3;
}
| STRING COMMA o_flags COMMA integer {
	ZERO(&$$);
	$$.path = getpath(&$1, "open", state);
	$$.flags = $3;
	$$.mode = $5;
};

openat_allparams: KW_AT_FDCWD COMMA open_allparams {
	$$ = $3;
};

open_beginparams: open_allparams {
	ZERO(&$$);
	$$ = $1;
};

openat_beginparams: KW_AT_FDCWD COMMA open_beginparams {
	$$ = $3;
};

open_endparams: /* empty */ { };

openat_endparams: /* empty */ { };

/* ---------------------------------------- */

read_allparams: DECNUM COMMA maybe_string COMMA DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.count = $5;

	$$.dummy1 = $3;
};

read_beginparams: DECNUM COMMA {
	ZERO(&$$);
	$$.fd = FD($1);
};

read_endparams: maybe_string COMMA DECNUM {
	ZERO(&$$);
	$$.count = $3;

	$$.dummy1 = $1;
};

/* ---------------------------------------- */

close_allparams: DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
};

close_beginparams: close_allparams {
	ZERO(&$$);
	$$ = $1;
};

close_endparams: /* empty */ { };

/* ---------------------------------------- */

stat_allparams: STRING COMMA stat_struct {
	ZERO(&$$);
	$$.path = getpath(&$1, "stat", state);
};

stat_beginparams: STRING COMMA {
	ZERO(&$$);
	$$.path = getpath(&$1, "stat", state);
};

stat_endparams: stat_struct {
	ZERO(&$$);
};

/* ---------------------------------------- */

pread_allparams: DECNUM COMMA maybe_string COMMA DECNUM COMMA DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.count = $5;
	$$.offset = $7;

	$$.dummy1 = $3;
};

pread_beginparams: DECNUM COMMA {
	ZERO(&$$);
	$$.fd = FD($1);
};

pread_endparams: maybe_string COMMA DECNUM COMMA DECNUM {
	ZERO(&$$);
	$$.count = $3;
	$$.offset = $5;

	$$.dummy1 = $1;
};

/* ---------------------------------------- */

mmap_allparams: HEXNUM COMMA DECNUM COMMA prot_flags COMMA map_flags COMMA DECNUM COMMA integer {
	ZERO(&$$);
	$$.addr = (void*)(intptr_t)$1;
	$$.length = $3;
	$$.prot = $5;
	$$.flags = $7;
	$$.fd = FD($9);
	$$.offset = $11;
};

mmap_beginparams: mmap_allparams {
	ZERO(&$$);
	$$ = $1;
};

mmap_endparams: /* empty */ { };

/* ---------------------------------------- */

fadvise64_allparams: DECNUM COMMA DECNUM COMMA DECNUM COMMA POSIX_FADVICE {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.offset = $3;
	$$.len = $5;
	$$.advice = $7;
};

fadvise64_beginparams: fadvise64_allparams {
	ZERO(&$$);
	$$ = $1;
};

fadvise64_endparams: /* empty */ { };

/* ---------------------------------------- */

statfs_allparams: STRING COMMA statfs_struct {
	ZERO(&$$);
	$$.path = getpath(&$1, "statfs", state);
};

statfs_beginparams: STRING COMMA {
	ZERO(&$$);
	$$.path = getpath(&$1, "statfs", state);
};

statfs_endparams: statfs_struct {
	ZERO(&$$);
};

/* ---------------------------------------- */

fcntl_allparams: DECNUM COMMA FCMD_DUPFD COMMA DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.cmdname = xstrdup("F_DUPFD");
	$$.arg.l = $5;
}
| DECNUM COMMA FCMD_SETFD COMMA fd_flags {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.cmdname = xstrdup("F_SETFD");
	/* FIXME: this is brittle. */
	$$.arg.l = (!$5 || strcmp($5, "FD_CLOEXEC")) ? 0 : FD_CLOEXEC;
}
| DECNUM COMMA FCMD_GETFD {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.cmdname = xstrdup("F_GETFD");
	$$.arg.l = -1;
}
| DECNUM COMMA FCMD_GETFL {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.cmdname = xstrdup("F_GETFL");
	$$.arg.l = -1;
}
| DECNUM COMMA FCMD_SETFL COMMA o_flags {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.cmdname = xstrdup("F_SETFL");
	$$.arg.flags = $5;
}
| DECNUM COMMA FCMD_SETLK COMMA FLOCK_STRUCT {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.cmdname = xstrdup("F_SETLK");
	$$.arg.l = 0; /* FIXME */
}
| DECNUM COMMA FCMD_SETLKW COMMA FLOCK_STRUCT {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.cmdname = xstrdup("F_SETLKW");
	$$.arg.l = 0; /* FIXME */
};

fcntl_beginparams: fcntl_allparams {
	ZERO(&$$);
	$$ = $1;
};

fcntl_endparams: /* empty */ { };

/* ---------------------------------------- */

fstatfs_allparams: DECNUM COMMA statfs_struct {
	ZERO(&$$);
	$$.fd = FD($1);
};

fstatfs_beginparams: DECNUM COMMA {
	ZERO(&$$);
	$$.fd = FD($1);
};

fstatfs_endparams: statfs_struct {
	ZERO(&$$);
};

/* ---------------------------------------- */

lseek_allparams: DECNUM COMMA DECNUM COMMA SEEK_WHENCE {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.offset = $3;
	$$.whence = match_whence($5, state);
};

lseek_beginparams: lseek_allparams {
	ZERO(&$$);
	$$ = $1;
};

lseek_endparams: /* empty */ { };

/* ---------------------------------------- */

getdents_allparams: DECNUM COMMA dent_array COMMA DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.count = $5;
};

getdents_beginparams: DECNUM COMMA {
	ZERO(&$$);
	$$.fd = FD($1);
};

getdents_endparams: dent_array COMMA DECNUM {
	ZERO(&$$);
	$$.count = $3;
};

/* ---------------------------------------- */

fstat_allparams: DECNUM COMMA stat_struct {
	ZERO(&$$);
	$$.fd = FD($1);
};

fstat_beginparams: DECNUM COMMA {
	ZERO(&$$);
	$$.fd = FD($1);
};

fstat_endparams: stat_struct {
	ZERO(&$$);
};

/* ---------------------------------------- */

epoll_create_allparams: DECNUM {
	ZERO(&$$);
	$$.size = $1;
};

epoll_create_beginparams: epoll_create_allparams {
	ZERO(&$$);
	$$ = $1;
};

epoll_create_endparams: /* empty */ { };

/* ---------------------------------------- */

socket_allparams: SOCKDOMAIN COMMA socktype_flags COMMA DECNUM {
	ZERO(&$$);
	$$.domain = -1;
	$$.type = -1;
	$$.protocol = -1;
};

socket_beginparams: socket_allparams {
	ZERO(&$$);
	$$ = $1;
};

socket_endparams: /* empty */ { };

/* ---------------------------------------- */

lstat_allparams: STRING COMMA stat_struct {
	ZERO(&$$);
	$$.path = getpath(&$1, "lstat", state);
};

lstat_beginparams: STRING COMMA {
	ZERO(&$$);
	$$.path = getpath(&$1, "lstat", state);
};

lstat_endparams: stat_struct {
	ZERO(&$$);
};

/* ---------------------------------------- */

access_allparams: STRING COMMA access_flags {
	ZERO(&$$);
	$$.pathname = getpath(&$1, "access", state);
	$$.flags = $3;
};

access_beginparams: access_allparams {
	ZERO(&$$);
	$$ = $1;
};

access_endparams: /* empty */ { };

/* ---------------------------------------- */

fgetxattr_allparams: DECNUM COMMA STRING COMMA maybe_string COMMA DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.name = getstr(&$3, "fgetxattr", state);
	$$.size = $7;

	$$.dummy1 = $5;
};

fgetxattr_beginparams: DECNUM COMMA STRING {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.name = getstr(&$3, "fgetxattr", state);
}

fgetxattr_endparams: COMMA maybe_string COMMA DECNUM {
	ZERO(&$$);
	$$.size = $4;

	$$.dummy1 = $2;
};

/* ---------------------------------------- */

readlink_allparams: STRING COMMA maybe_string COMMA DECNUM {
	ZERO(&$$);
	$$.path = getpath(&$1, "readlink", state);
	$$.bufsiz = $5;

	$$.dummy1 = $3;
};

readlink_beginparams: STRING COMMA {
	ZERO(&$$);
	$$.path = getpath(&$1, "readlink", state);
};

readlink_endparams: maybe_string COMMA DECNUM {
	ZERO(&$$);
	$$.bufsiz = $3;

	$$.dummy1 = $1;
};

/* ---------------------------------------- */

listxattr_allparams: STRING COMMA STRING COMMA DECNUM {
	ZERO(&$$);
	$$.path = getpath(&$1, "listxattr", state);
	$$.size = $5;

	$$.dummy1 = $3.s;
};

listxattr_beginparams: STRING COMMA {
	ZERO(&$$);
	$$.path = getpath(&$1, "listxattr", state);
};

listxattr_endparams: STRING COMMA DECNUM {
	ZERO(&$$);
	$$.size = $3;

	$$.dummy1 = $1.s;
};

/* ---------------------------------------- */

write_allparams: DECNUM COMMA STRING COMMA DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.count = $5;

	$$.dummy1 = $3.s;
};

write_beginparams: write_allparams {
	ZERO(&$$);
	$$ = $1;
};

write_endparams: /* empty */ { };

/* ---------------------------------------- */

dup_allparams: DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
};

dup_beginparams: dup_allparams {
	ZERO(&$$);
	$$ = $1;
};

dup_endparams: /* empty */ { };

/* ---------------------------------------- */

mkdir_allparams: STRING COMMA integer {
	ZERO(&$$);
	$$.pathname = getpath(&$1, "mkdir", state);
	$$.mode = $3;
};

mkdir_beginparams: mkdir_allparams {
	ZERO(&$$);
	$$ = $1;
};

mkdir_endparams: /* empty */ { };

/* ---------------------------------------- */

flock_allparams: DECNUM COMMA FLOCK_OP {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.operation = match_flockop($3, state);
};

flock_beginparams: flock_allparams {
	ZERO(&$$);
	$$ = $1;
};

flock_endparams: /* empty */ { };

/* ---------------------------------------- */

ftruncate_allparams: DECNUM COMMA DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.length = $3;
};

ftruncate_beginparams: ftruncate_allparams {
	ZERO(&$$);
	$$ = $1;
};

ftruncate_endparams: /* empty */ { };

/* ---------------------------------------- */

fsync_allparams: DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
};

fsync_beginparams: fsync_allparams {
	ZERO(&$$);
	$$ = $1;
};

fsync_endparams: /* empty */ { };

/* ---------------------------------------- */

fdatasync_allparams: DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
};

fdatasync_beginparams: fdatasync_allparams {
	ZERO(&$$);
	$$ = $1;
};

fdatasync_endparams: /* empty */ { };

/* ---------------------------------------- */

chmod_allparams: STRING COMMA integer {
	ZERO(&$$);
	$$.path = getpath(&$1, "chmod", state);
	$$.mode = $3;
};

chmod_beginparams: chmod_allparams {
	ZERO(&$$);
	$$ = $1;
};

chmod_endparams: /* empty */ { };

/* ---------------------------------------- */

chown_allparams: STRING COMMA DECNUM COMMA DECNUM {
	ZERO(&$$);
	$$.path = getpath(&$1, "chown", state);
};

chown_beginparams: chown_allparams {
	ZERO(&$$);
	$$ = $1;
};

chown_endparams: /* empty */ { };

/* ---------------------------------------- */

rename_allparams: STRING COMMA STRING {
	ZERO(&$$);
	$$.oldpath = getpath(&$1, "rename", state);
	$$.newpath = getpath(&$3, "rename", state);
};

rename_beginparams: rename_allparams {
	ZERO(&$$);
	$$ = $1;
};

rename_endparams: /* empty */ { };

/* ---------------------------------------- */

chdir_allparams: STRING {
	ZERO(&$$);
	$$.path = getpath(&$1, "chdir", state);
};

chdir_beginparams: chdir_allparams {
	ZERO(&$$);
	$$ = $1;
};

chdir_endparams: /* empty */ { };

/* ---------------------------------------- */

getcwd_allparams: STRING COMMA DECNUM {
	ZERO(&$$);
	$$.size = $3;

	$$.dummy1 = $1.s;
};

/*
 * The begin/end split is a guess here, I don't have an actual example on hand
 * at the moment as I write this.
 */
getcwd_beginparams: /* empty */ { };

getcwd_endparams: getcwd_allparams {
	ZERO(&$$);
	$$ = $1;
};

/* ---------------------------------------- */

fsetxattr_allparams: DECNUM COMMA STRING COMMA STRING COMMA DECNUM COMMA DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.name = getstr(&$3, "fsetxattr", state);
	$$.size = $7;

	$$.dummy1 = $5.s;
};

fsetxattr_beginparams: fsetxattr_allparams {
	ZERO(&$$);
	$$ = $1;
};

fsetxattr_endparams: /* empty */ { };

/* ---------------------------------------- */

pwrite_allparams: DECNUM COMMA STRING COMMA DECNUM COMMA DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.count = $5;
	$$.offset = $7;

	$$.dummy1 = $3.s;
};

pwrite_beginparams: pwrite_allparams {
	ZERO(&$$);
	$$ = $1;
};

pwrite_endparams: /* empty */ { };

/* ---------------------------------------- */

unlink_allparams: STRING {
	ZERO(&$$);
	$$.pathname = getpath(&$1, "unlink", state);
};

unlink_beginparams: unlink_allparams {
	ZERO(&$$);
	$$ = $1;
};

unlink_endparams: /* empty */ { };

/* ---------------------------------------- */

setxattr_allparams: STRING COMMA STRING COMMA STRING COMMA DECNUM COMMA DECNUM {
	ZERO(&$$);
	$$.path = getpath(&$1, "setxattr", state);
	$$.name = getstr(&$3, "setxattr", state);
	$$.size = $7;

	$$.dummy1 = $5.s;
};

setxattr_beginparams: setxattr_allparams {
	ZERO(&$$);
	$$ = $1;
};

setxattr_endparams: /* empty */ { };

/* ---------------------------------------- */

link_allparams: STRING COMMA STRING {
	ZERO(&$$);
	$$.oldpath = getpath(&$1, "link", state);
	$$.newpath = getpath(&$3, "link", state);
};

link_beginparams: link_allparams {
	ZERO(&$$);
	$$ = $1;
};

link_endparams: /* empty */ { };

/* ---------------------------------------- */

rmdir_allparams: STRING {
	ZERO(&$$);
	$$.pathname = getpath(&$1, "rmdir", state);
};

rmdir_beginparams: rmdir_allparams {
	ZERO(&$$);
	$$ = $1;
};

rmdir_endparams: /* empty */ { };

/* ---------------------------------------- */

getxattr_allparams: STRING COMMA STRING COMMA maybe_string COMMA DECNUM {
	ZERO(&$$);
	$$.path = getpath(&$1, "getxattr", state);
	$$.name = getstr(&$3, "getxattr", state);
	$$.size = $7;

	$$.dummy1 = $5;
};

getxattr_beginparams: STRING COMMA STRING {
	ZERO(&$$);
	$$.path = getpath(&$1, "getxattr", state);
	$$.name = getstr(&$3, "getxattr", state);
};

getxattr_endparams: COMMA maybe_string COMMA DECNUM {
	ZERO(&$$);
	$$.size = $4;

	$$.dummy1 = $2;
};

/* ------------------------------------------------- */

flistxattr_allparams: DECNUM COMMA STRING COMMA DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.size = $5;

	$$.dummy1 = $3.s;
};

flistxattr_beginparams: DECNUM COMMA {
	ZERO(&$$);
	$$.fd = FD($1);
};

flistxattr_endparams: STRING COMMA DECNUM {
	ZERO(&$$);
	$$.size = $3;

	$$.dummy1 = $1.s;
};

/* ---------------------------------------- */

removexattr_allparams: STRING COMMA STRING {
	ZERO(&$$);
	$$.path = getpath(&$1, "removexattr", state);
	$$.name = getstr(&$3, "removexattr", state);
};

removexattr_beginparams: removexattr_allparams {
	ZERO(&$$);
	$$ = $1;
 };

removexattr_endparams: /* empty */ { };

/* ---------------------------------------- */

fremovexattr_allparams: DECNUM COMMA STRING {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.name = getstr(&$3, "fremovexattr", state);
};

fremovexattr_beginparams: fremovexattr_allparams {
	ZERO(&$$);
	$$ = $1;
};

fremovexattr_endparams: /* empty */ { };

/* ---------------------------------------- */

fallocate_allparams: DECNUM COMMA DECNUM COMMA DECNUM COMMA DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.mode = $3;
	$$.offset = $5;
	$$.len = $7;
};

fallocate_beginparams: fallocate_allparams {
	ZERO(&$$);
	$$ = $1;
};

fallocate_endparams: /* empty */ { };

/* ---------------------------------------- */

fchmod_allparams: DECNUM COMMA integer {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.mode = $3;
};

fchmod_beginparams: fchmod_allparams {
	ZERO(&$$);
	$$ = $1;
};

fchmod_endparams: /* empty */ { };

/* ---------------------------------------- */

utimensat_allparams: DECNUM COMMA STRING COMMA timespec_pair COMMA DECNUM {
	ZERO(&$$);
	$$.dirfd = FD($1);
	$$.pathname = getpath(&$3, "utimensat", state);
	$$.flags = $7;
}
| DECNUM COMMA HEXNUM COMMA timespec_pair COMMA DECNUM {
	ZERO(&$$);
	$$.dirfd = FD($1);
	if ($3 != 0) {
		fprintf(stderr, "Error: numeric but non-NULL pathname string "
		        "in utimensat (line %d)\n", state->linenum);
		exit(1);
	}
	$$.pathname = NULL;
	$$.flags = $7;
};

utimensat_beginparams: utimensat_allparams {
	ZERO(&$$);
	$$ = $1;
};

utimensat_endparams: /* empty */ { };

/* ---------------------------------------- */

pipe_allparams: LBRACKET DECNUM COMMA DECNUM RBRACKET {
	ZERO(&$$);
	$$.rd = FD($2);
	$$.wr = FD($4);
};

pipe_beginparams: /* empty */ { };

pipe_endparams: pipe_allparams {
	ZERO(&$$);
	$$ = $1;
};

/* ---------------------------------------- */

dup2_allparams: DECNUM COMMA DECNUM {
	ZERO(&$$);
	$$.old = FD($1);
	$$.new = FD($3);
};

dup2_beginparams: dup2_allparams {
	ZERO(&$$);
	$$ = $1;
};

dup2_endparams: /* empty */ { };

/* ---------------------------------------- */

faccessat_allparams: KW_AT_FDCWD COMMA STRING COMMA access_flags {
	ZERO(&$$);
	$$.dirfd = FD(AT_FDCWD);
	$$.pathname = getpath(&$3, "faccessat", state);
	$$.mode = $5;
	/* as of v4.7, strace doesn't print 'flags' argument */
};

faccessat_beginparams: faccessat_allparams {
	ZERO(&$$);
	$$ = $1;
};

faccessat_endparams: /* empty */ { };

/* ---------------------------------------- */

newfstatat_allparams: KW_AT_FDCWD COMMA STRING COMMA stat_struct COMMA at_flags {
	ZERO(&$$);
	$$.dirfd = FD(AT_FDCWD);
	$$.pathname = getpath(&$3, "newfstatat", state);
	$$.flags = $7;
};

newfstatat_beginparams: newfstatat_allparams {
	ZERO(&$$);
	$$ = $1;
};

newfstatat_endparams: /* empty */ { };

/* ---------------------------------------- */

fchownat_allparams: KW_AT_FDCWD COMMA STRING COMMA DECNUM COMMA DECNUM COMMA at_flags {
	ZERO(&$$);
	$$.dirfd = FD(AT_FDCWD);
	$$.pathname = getpath(&$3, "fchownat", state);
	$$.owner = $5;
	$$.group = $7;
	$$.flags = $9;
};

fchownat_beginparams: fchownat_allparams {
	ZERO(&$$);
	$$ = $1;
};

fchownat_endparams: /* empty */ { };

/* ---------------------------------------- */

unlinkat_allparams: KW_AT_FDCWD COMMA STRING COMMA at_flags {
	ZERO(&$$);
	$$.dirfd = FD(AT_FDCWD);
	$$.pathname = getpath(&$3, "unlinkat", state);
	$$.flags = $5;
};

unlinkat_beginparams: unlinkat_allparams {
	ZERO(&$$);
	$$ = $1;
};

unlinkat_endparams: /* empty */ { };

/* ---------------------------------------- */

dup3_allparams: DECNUM COMMA DECNUM COMMA optional_o_flags {
	ZERO(&$$);
	$$.old = FD($1);
	$$.new = FD($3);
	$$.flags = $5;
};

dup3_beginparams: dup3_allparams {
	ZERO(&$$);
	$$ = $1;
};

dup3_endparams: /* empty */ { };

/* ---------------------------------------- */

fchown_allparams: DECNUM COMMA DECNUM COMMA DECNUM {
	ZERO(&$$);
	$$.fd = FD($1);
	$$.owner = $3;
	$$.group = $5;
};

fchown_beginparams: fchown_allparams {
	ZERO(&$$);
	$$ = $1;
};

fchown_endparams: /* empty */ { };

/* ---------------------------------------- */

symlink_allparams: STRING COMMA STRING {
	ZERO(&$$);
	$$.oldpath = getpath(&$1, "symlink", state);
	$$.newpath = getpath(&$3, "symlink", state);
};

symlink_beginparams: symlink_allparams {
	ZERO(&$$);
	$$ = $1;
};

symlink_endparams: /* empty */ { };

/* ---------------------------------------- */

accept_allparams: DECNUM COMMA SOCKADDR_STRUCT COMMA LBRACKET DECNUM RBRACKET {
	ZERO(&$$);
	$$.fd = FD($1);
};

accept_beginparams: DECNUM COMMA {
	ZERO(&$$);
	$$.fd = FD($1);
};

accept_endparams: SOCKADDR_STRUCT COMMA LBRACKET DECNUM RBRACKET {
	/* Nothing we care about. */
};

/* ---------------------------------------- */

io_setup_allparams: DECNUM COMMA LBRACE DECNUM RBRACE {
	ZERO(&$$);
	$$.nr_events = $1;
	$$.ctx = $4;
};

io_setup_beginparams: DECNUM COMMA {
	ZERO(&$$);
	$$.nr_events = $1;
};

io_setup_endparams: LBRACE DECNUM RBRACE {
	ZERO(&$$);
	$$.ctx = $2;
};

/* ---------------------------------------- */

io_submit_allparams: DECNUM COMMA DECNUM COMMA iocb_array {
	ZERO(&$$);
	$$.ctx = $1;
	$$.nr = $3;
	$$.iocbs = $5;
};

io_submit_beginparams: io_submit_allparams {
	ZERO(&$$);
	$$ = $1;
};

io_submit_endparams: /* empty */ { };

/* ---------------------------------------- */

io_getevents_allparams: DECNUM COMMA DECNUM COMMA DECNUM COMMA ioevent_array maybe_comma maybe_timespec {
	ZERO(&$$);
	$$.ctx = $1;
	$$.min_nr = $3;
	$$.nr = $5;
	$$.ioevents = $7;
	/* FIXME: do we want/need the timespec struct? */
};

io_getevents_beginparams: DECNUM COMMA DECNUM COMMA DECNUM COMMA {
	ZERO(&$$);
	$$.ctx = $1;
	$$.min_nr = $3;
	$$.nr = $5;
};

/*
 * This grammar is based on a development version of strace, which doesn't
 * have a comma (or even a space) between the io_event struct array and the
 * timespec struct when the ioevent array is empty.  Odd, and I suspect would
 * be considered "a bug".
 */
io_getevents_endparams: ioevent_array maybe_comma TIMESPEC_STRUCT {
	ZERO(&$$);
	$$.ioevents = $1;
};

/* ---------------------------------------- */

io_destroy_allparams: DECNUM {
	ZERO(&$$);
	$$.ctx = $1;
};

io_destroy_beginparams: io_destroy_allparams {
	ZERO(&$$);
	$$ = $1;
};

io_destroy_endparams: /* empty */ { };

/* ---------------------------------------------------------------------- */

maybe_comma: /* empty */ | COMMA;

stat_struct: STAT_STRUCT | HEXNUM;
statfs_struct: STATFS_STRUCT | HEXNUM;

maybe_string: STRING {
	$$ = $1.s;
}
| HEXNUM {
	$$ = NULL;
};

maybe_timespec: HEXNUM | TIMESPEC_STRUCT;

dent_array: LBRACE optional_dents RBRACE;

optional_dents: /* empty */ | optional_dents DENT_STRUCT;

iocb_array: LBRACE optional_iocbs RBRACE {
	$$ = $2;
};

optional_iocbs: /* empty */ {
	$$ = NULL;
}
| iocb_list {
	$$ = $1;
};

iocb_list: iocb {
	$$ = $1;
}
| iocb_list COMMA iocb {
	iocb_list_append($1, $3);
	$$ = $1;
};

iocb: LBRACE maybe_iocb_data_comma ET_PWRITE COMMA iocb_fd COMMA iocb_str COMMA iocb_nbytes COMMA iocb_offset RBRACE {
	$$ = xmalloc(sizeof(*$$));
	$$->type = AT_PWRITE;
	$$->data = $2;
	$$->fd = FD($5);
	$$->dummy = $7;
	$$->nbytes = $9;
	$$->offset = $11;
	$$->next = NULL;
}
| LBRACE maybe_iocb_data_comma ET_PREAD COMMA iocb_fd COMMA iocb_buf COMMA iocb_nbytes COMMA iocb_offset RBRACE {
	$$ = xmalloc(sizeof(*$$));
	$$->type = AT_PREAD;
	$$->data = $2;
	$$->fd = FD($5);
	$$->dummy = NULL;
	$$->nbytes = $9;
	$$->offset = $11;
	$$->next = NULL;
};

maybe_iocb_data_comma: /* empty -- strace only prints this member if it's non-zero */ {
	$$ = 0;
}
| KW_DATA COLON HEXNUM COMMA {
	$$ = $3;
};

iocb_buf: KW_BUF COLON HEXNUM { };

iocb_str: KW_STR COLON STRING {
	$$ = $3.s;
};

iocb_fd: KW_FILEDES COLON DECNUM {
	$$ = $3;
};

iocb_nbytes: KW_NBYTES COLON DECNUM {
	$$ = $3;
};

iocb_offset: KW_OFFSET COLON DECNUM {
	$$ = $3;
};

ioevent_array: LBRACE optional_ioevents RBRACE {
	$$ = $2;
};

optional_ioevents: /* empty */ {
	$$ = NULL;
}
| ioevent_list {
	$$ = $1;
};

ioevent_list: ioevent {
	$$ = $1;
}
| ioevent_list COMMA ioevent {
	ioevent_list_append($1, $3);
	$$ = $1;
};

ioevent: LBRACE HEXNUM COMMA HEXNUM COMMA DECNUM COMMA DECNUM RBRACE {
	$$ = xmalloc(sizeof(*$$));
	$$->data = $2;
	$$->obj = $4;
	$$->res = $6;
	$$->res2 = $8;
	$$->next = NULL;

	/* This information is figured out later by the compiler. */
	$$->fd = FD(-1);
};

timespec_pair: LBRACE TIMESPEC_STRUCT COMMA TIMESPEC_STRUCT RBRACE
| HEXNUM /* handle NULL */
;

fd_flags: FLAG_FD {
	$$ = $1;
}
| DECNUM {
	if ($1)
		fprintf(stderr, "Warning: unrecognized FD flag, ignoring (line %d)\n",
		        state->linenum);
	$$ = 0;
};

integer: DECNUM { $$ = $1; }
| HEXNUM { $$ = $1; }
| OCTNUM { $$ = $1; }
;

o_flags: FLAG_O {
	$$ = newflag($1);
}
| o_flags PIPE FLAG_O {
	$$ = newflag($3);
	$$->next = $1;
};

optional_o_flags: o_flags {
	$$ = $1;
}
| DECNUM {
	if ($1)
		fprintf(stderr, "warning: unrecognized O_* flag, ignoring (line %d)\n",
		        state->linenum);
	$$ = NULL;
};

prot_flags: FLAG_PROT {
	$$ = newflag($1);
}
| prot_flags PIPE FLAG_PROT {
	$$ = newflag($3);
	$$->next = $1;
};

map_flags: FLAG_MAP {
	$$ = newflag($1);
}
| map_flags PIPE FLAG_MAP {
	$$ = newflag($3);
	$$->next = $1;
};

access_flags: FLAG_ACCESS {
	$$ = newflag($1);
}
| access_flags PIPE FLAG_ACCESS {
	$$ = newflag($3);
	$$->next = $1;
};

/*
 * Doesn't return anything because we don't actually do anything with these
 * flags.
 */
socktype_flags: FLAG_SOCK
| socktype_flags PIPE FLAG_SOCK;

at_flags: FLAG_AT {
	$$ = newflag($1);
}
| at_flags PIPE FLAG_AT {
	$$ = newflag($3);
	$$->next = $1;
}
| DECNUM {
	if ($1)
		fprintf(stderr, "Warning: unrecognized AT_* flag, ignoring (line %d)\n",
		        state->linenum);
	$$ = NULL;
};

%%

static void check_all_completed(int thread, struct event* ev, void* arg)
{
	int* all_completed = arg;
	*all_completed = 0;
	fprintf(stderr, "Warning: %s() call from line %d still outstanding in thread %d"
	        " at end of trace\n", event_type_name(ev->type), ev->traceline, thread);

	assert(!ev->next);
	if (ev->prev)
		ev->prev->next = NULL;
	free_event(ev);
}

struct event* parse_strace(FILE* tracefile)
{
	int status, all_completed;
	struct parser_state pstate = {
		.linenum = 1,
		.events = NULL,
		.last = NULL,
	};

	stracerestart(tracefile);

	status = straceparse(&pstate);

	all_completed = 1;
	te_assoc_visit(&outstanding_events, check_all_completed, &all_completed);
	/* FIXME: dealloc outstanding_events */

	stracelex_destroy();

	return status ? NULL : pstate.events;
}

void strace_parser_teardown(void)
{
	te_assoc_dealloc(&outstanding_events, NULL, NULL);
}

static void straceerror(struct parser_state* state, char const* s)
{
	fprintf(stderr, "line %d: %s\n", state->linenum, s);
}
