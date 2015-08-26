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
%{

#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "trace.h"
#include "misc.h"
#include "strace-parse.tab.h"

extern YYSTYPE stracelval;

%}

%option nounput
%option noinput
%option nodefault
%option noyywrap
%option yylineno
%option prefix="strace"

hexconst	0[Xx][0-9A-Fa-f]+
decconst	[-]?[1-9][0-9]*|0
octconst	0[0-9]+

o_flag		O_(RDONLY|WRONLY|RDWR|APPEND|CREAT|EVTONLY|EXCL|EXLOCK|NOFOLLOW|SHLOCK|TRUNC|NONBLOCK|CLOEXEC|LARGEFILE|DIRECTORY|ASYNC|NOCTTY|DIRECT|DSYNC)

prot_flag	PROT_(READ|WRITE|EXEC|NONE)

map_flag	MAP_(SHARED|PRIVATE|FIXED|NOCACHE|ANONYMOUS|DENYWRITE|NORESERVE|STACK)

fd_flag		FD_CLOEXEC

access_flag	[FRWX]_OK

at_flag		AT_SYMLINK_NOFOLLOW

seek_whence	SEEK_(SET|CUR|END)

posix_fadvice	POSIX_FADV_(NORMAL|SEQUENTIAL|RANDOM|NOREUSE|WILLNEED|DONTNEED)

flock_op	LOCK_(SH|EX|UN)

sockdomain	PF_FILE
socktype_flag	SOCK_(STREAM|CLOEXEC|NONBLOCK)

o_flag_ret	flags\ {o_flag}([|]{o_flag})*
fd_flag_ret	flags\ {fd_flag}([|]{fd_flag})*

newline		[\n]
space		[ ]
lparen		[(]
rparen		[)]
comma		[,]
lessthan	[<]
greaterthan	[>]
equal		[=]
pipe		[|]
colon		[:]
lbrace		[{]
rbrace		[}]
lbracket	\[
rbracket	\]
time		{decconst}\.[0-9]+

string		["]([^\"\\]|\\.)*["](\.\.\.)?

stat_struct	\{st_dev=[^}]+\}
dent_struct	\{d_ino=[^}]+d_name={string}(,\ d_type=DT_[A-Z]+)?\}
statfs_struct	\{f_type=[^}]+,\ f_fsid=\{[^}]+\},\ [^}]+\}
flock_struct	\{type=F_(RD|WR|UN)LCK,\ whence=SEEK_(SET|CUR|END),\ start=[^}]+\}
timespec_struct \{{decconst},\ {decconst}\}
sockaddr_struct \{sa_family=[A-Z_]+,\ NULL\}

errno		E(PERM|NOENT|SRCH|INTR|IO|NXIO|2BIG|NOEXEC|BADF|CHILD|AGAIN|NOMEM|ACCES|FAULT|NOTBLK|BUSY|EXIST|XDEV|NODEV|NOTDIR|ISDIR|INVAL|NFILE|MFILE|NOTTY|TXTBSY|FBIG|NOSPC|SPIPE|ROFS|MLINK|PIPE|DOM|RANGE|NODATA|NOTEMPTY)

errstr		\([-/A-Za-z ]+\)

unfinished	<unfinished\ \.\.\.>
resume_start	<\.\.\.
resume_end	resumed>
detached	<detached\ \.\.\.>

thread_exit	\+\+\+\ exited\ with\ {decconst}\ \+\+\+

%%

{hexconst} {
	char* tmp;
	stracelval.i = strtoll(stracetext, &tmp, 16);
	return HEXNUM;
}

{decconst} {
	char* tmp;
	stracelval.i = strtoll(stracetext, &tmp, 10);
	return DECNUM;
}

{octconst} {
	char* tmp;
	stracelval.i = strtoll(stracetext, &tmp, 8);
	return OCTNUM;
}

{time} {
	char* tmp;
	nstime_t seconds, microseconds;
	seconds = strtoull(stracetext, &tmp, 10);
	assert(*tmp == '.');
	microseconds = strtoull(tmp+1, &tmp, 10);
	stracelval.i = (seconds * 1000000000) + (microseconds * 1000);
	return TIME;
}

{newline}     return NEWLINE;
{lparen}      return LPAREN;
{rparen}      return RPAREN;
{lbrace}      return LBRACE;
{rbrace}      return RBRACE;
{lbracket}    return LBRACKET;
{rbracket}    return RBRACKET;
{comma}       return COMMA;
{lessthan}    return LESSTHAN;
{greaterthan} return GREATERTHAN;
{equal}       return EQUAL;
{pipe}        return PIPE;
{colon}       return COLON;
{space}

{stat_struct}     return STAT_STRUCT;
{dent_struct}     return DENT_STRUCT;
{statfs_struct}   return STATFS_STRUCT;
{flock_struct}    return FLOCK_STRUCT;
{timespec_struct} return TIMESPEC_STRUCT;
{sockaddr_struct} return SOCKADDR_STRUCT;

{unfinished}    return UNFINISHED;
{resume_start}  return RESUME_START;
{resume_end}    return RESUME_END;
{detached}      return DETACHED;

{o_flag_ret}  return FLAG_RET;
{fd_flag_ret} return FLAG_RET;

"data"        return KW_DATA;
"filedes"     return KW_FILEDES;
"str"         return KW_STR;
"nbytes"      return KW_NBYTES;
"offset"      return KW_OFFSET;
"buf"         return KW_BUF;

"AT_FDCWD"    return KW_AT_FDCWD;

"open"              return ET_OPEN;
"openat"            return ET_OPENAT;
"close"             return ET_CLOSE;
"dup"               return ET_DUP;
"read"              return ET_READ;
"write"             return ET_WRITE;
"pread"             return ET_PREAD;
"pwrite"            return ET_PWRITE;
"aio_read"          return ET_AIO_READ;
"aio_error"         return ET_AIO_ERROR;
"aio_return"        return ET_AIO_RETURN;
"aio_suspend"       return ET_AIO_SUSPEND;
"lseek"             return ET_LSEEK;
"fsync"             return ET_FSYNC;
"fdatasync"         return ET_FDATASYNC;
"fcntl"             return ET_FCNTL;
"ftruncate"         return ET_FTRUNCATE;
"link"              return ET_LINK;
"symlink"           return ET_SYMLINK;
"rmdir"             return ET_RMDIR;
"unlink"            return ET_UNLINK;
"readlink"          return ET_READLINK;
"rename"            return ET_RENAME;
"exchangedata"      return ET_EXCHANGEDATA;
"mkdir"             return ET_MKDIR;
"flock"             return ET_FLOCK;
"mmap"              return ET_MMAP;
"utimes"            return ET_UTIMES;
"statfs"            return ET_STATFS;
"statfs64"          return ET_STATFS64;
"fstatfs64"         return ET_FSTATFS64;
"fstat"             return ET_FSTAT;
"lstat"             return ET_LSTAT;
"stat"              return ET_STAT;
"fstat64"           return ET_FSTAT64;
"lstat64"           return ET_LSTAT64;
"stat64"            return ET_STAT64;
"getxattr"          return ET_GETXATTR;
"fgetxattr"         return ET_FGETXATTR;
"listxattr"         return ET_LISTXATTR;
"flistxattr"        return ET_FLISTXATTR;
"access"            return ET_ACCESS;
"chown"             return ET_CHOWN;
"chmod"             return ET_CHMOD;
"fsetxattr"         return ET_FSETXATTR;
"chdir"             return ET_CHDIR;
"getcwd"            return ET_GETCWD;
"fchmod"            return ET_FCHMOD;
"fchown"            return ET_FCHOWN;
"removexattr"       return ET_REMOVEXATTR;
"fremovexattr"      return ET_FREMOVEXATTR;
"setxattr"          return ET_SETXATTR;
"socket"            return ET_SOCKET;

"epoll_create"      return ET_EPOLL_CREATE;
"fadvise64"         return ET_FADVISE64;
"fstatfs"           return ET_FSTATFS;
"getdents"          return ET_GETDENTS;
"fallocate"         return ET_FALLOCATE;
"utimensat"         return ET_UTIMENSAT;
"accept"            return ET_ACCEPT;
"pipe"              return ET_PIPE;
"dup2"              return ET_DUP2;
"faccessat"         return ET_FACCESSAT;
"newfstatat"        return ET_NEWFSTATAT;
"fchownat"          return ET_FCHOWNAT;
"unlinkat"          return ET_UNLINKAT;
"dup3"              return ET_DUP3;

"io_setup"          return ET_IO_SETUP;
"io_submit"         return ET_IO_SUBMIT;
"io_getevents"      return ET_IO_GETEVENTS;
"io_destroy"        return ET_IO_DESTROY;

"F_DUPFD"  return FCMD_DUPFD;
"F_SETFD"  return FCMD_SETFD;
"F_GETFD"  return FCMD_GETFD;
"F_GETFL"  return FCMD_GETFL;
"F_SETFL"  return FCMD_SETFL;
"F_SETLK"  return FCMD_SETLK;
"F_SETLKW" return FCMD_SETLKW;

"NULL"|"(nil)" {
	/* treat NULL as a literal zero */
	stracelval.i = 0;
	return HEXNUM;
}

{o_flag} {
	stracelval.str = stracetext;
	return FLAG_O;
}

{prot_flag} {
	stracelval.str = stracetext;
	return FLAG_PROT;
}

{map_flag} {
	stracelval.str = stracetext;
	return FLAG_MAP;
}

{fd_flag} {
	stracelval.str = stracetext;
	return FLAG_FD;
}

{access_flag} {
	stracelval.str = stracetext;
	return FLAG_ACCESS;
}

{at_flag} {
	stracelval.str = stracetext;
	return FLAG_AT;
}

{posix_fadvice} {
	stracelval.str = xstrdup(stracetext);
	return POSIX_FADVICE;
}

{flock_op} {
	stracelval.str = xstrdup(stracetext);
	return FLOCK_OP;
}

{sockdomain} {
	stracelval.i = -1;
	return SOCKDOMAIN;
}

{socktype_flag} {
	stracelval.i = -1;
	return FLAG_SOCK;
}

{seek_whence} {
	stracelval.str = stracetext;
	return SEEK_WHENCE;
}

{string} {
	char* end;

	stracelval.strlit.s = xstrdup(stracetext+1);
	end = strrchr(stracelval.strlit.s, '"');
	*end = '\0';

	end = strrchr(stracetext, '"');

	stracelval.strlit.truncated = ((end - stracetext + 4) == straceleng
	                               && !strcmp(end+1, "..."));

	return STRING;
}

{errno} {
	/* errno values aren't actually used by the compiler */
	/* stracelval.str = xstrdup(stracetext); */
	return ERRNO;
}

{errstr} return ERRSTR;

{thread_exit} {
	char* tmp;
	stracelval.i = strtoll(stracetext + strlen("+++ exited with "), &tmp, 10);
	return THREAD_EXIT;
}

<*>.|\n {
	fprintf(stderr, "unrecognized token at line %d: %s\n", stracelineno,
	        stracetext);
	exit(1);
}

%%
