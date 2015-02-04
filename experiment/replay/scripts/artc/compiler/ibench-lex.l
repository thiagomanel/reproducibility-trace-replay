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
#include "ibench-parse.tab.h"

extern YYSTYPE ibenchlval;

/*
 * Ugh...this whole use of start conditions is a gross hack, but the ad-hoc
 * grammar of the iBench traces makes parsing them a bit of a pain.
 */

static int stringcount = 0;

#define matchstrings(n) do { stringcount = n; BEGIN(matchstring); } while (0)

%}

%option nounput
%option noinput
%option nodefault
%option noyywrap
%option prefix="ibench"

%s matchstring

hexconst	0[Xx][0-9A-Fa-f]+
decconst	[-]?[0-9]+

thread_na	N\/A

rsrc_inum	{decconst}rsrc

o_flag		(O_RDONLY|O_WRONLY|O_RDWR|O_APPEND|O_CREAT|O_EVTONLY|O_EXCL|O_EXLOCK|O_NOFOLLOW|O_SHLOCK|O_TRUNC|O_NONBLOCK)[ ]

prot_flag	(PROT_READ|PROT_WRITE|PROT_EXEC)[ ]

map_flag	(MAP_SHARED|MAP_PRIVATE|MAP_FIXED|MAP_NOCACHE)[ ]

fcntl_cmd	F_ADDSIGS|F_ComponentDispatch|F_DComponentDispatch|F_DGetCodecInfo|F_DUPFD|F_FulfillDeferredFault|F_FULLFSYNC|F_GETFD|F_GETFL|F_GETPATH|F_GraphicsImportComponentDispatch|F_GraphicsImportGetMIMETypeList|F_NOCACHE|F_PREALLOCATE|F_RDADVISE|F_SETFD|F_SETLK|F_SETLKW

startofrecord	\[
delim		\];\ \[
endofrecord	\];\ \n

string		[^\[\]\n]+

file_table	[X_]+

%%

{hexconst} {
	char* tmp;
	ibenchlval.i = strtoll(ibenchtext, &tmp, 16);
	return HEXNUM;
}

{decconst} {
	char* tmp;
	ibenchlval.i = strtoll(ibenchtext, &tmp, 10);
	return DECNUM;
}

{thread_na} return THREAD_NA;
{rsrc_inum} return RSRC_INUM;

{startofrecord} return SOR;
{delim}	        return DELIM;
{endofrecord}	return EOR;

entry   return SUBTYPE_ENTRY;
return  return SUBTYPE_RETURN;
comment return SUBTYPE_COMMENT;

open              { matchstrings(1); return ET_OPEN; }
shm_open          { matchstrings(1); return ET_SHM_OPEN; }
close             return ET_CLOSE;
dup               return ET_DUP;
kqueue            return ET_KQUEUE;
socket            return ET_SOCKET;
socketpair        return ET_SOCKETPAIR;
read              return ET_READ;
write             return ET_WRITE;
pread             return ET_PREAD;
pwrite            return ET_PWRITE;
aio_read          return ET_AIO_READ;
aio_error         return ET_AIO_ERROR;
aio_return        return ET_AIO_RETURN;
aio_suspend       return ET_AIO_SUSPEND;
lseek             return ET_LSEEK;
fsync             return ET_FSYNC;
fcntl             return ET_FCNTL;
ftruncate         return ET_FTRUNCATE;
link              { matchstrings(2); return ET_LINK; }
symlink           { matchstrings(2); return ET_SYMLINK; }
rmdir             { matchstrings(1); return ET_RMDIR; }
unlink            { matchstrings(1); return ET_UNLINK; }
readlink          { matchstrings(2); return ET_READLINK; }
rename            { matchstrings(2); return ET_RENAME; }
exchangedata      { matchstrings(2); return ET_EXCHANGEDATA; }
mkdir             { matchstrings(1); return ET_MKDIR; }
flock             return ET_FLOCK;
mmap              return ET_MMAP;
futimes           return ET_FUTIMES;
utimes            return ET_UTIMES;
statfs            { matchstrings(1); return ET_STATFS; }
statfs64          { matchstrings(1); return ET_STATFS64; }
fstatfs64         return ET_FSTATFS64;
fstat             return ET_FSTAT;
lstat             { matchstrings(1); return ET_LSTAT; }
stat              { matchstrings(1); return ET_STAT; }
fstat64           return ET_FSTAT64;
lstat64           { matchstrings(1); return ET_LSTAT64; }
stat64            { matchstrings(1); return ET_STAT64; }
setattrlist       { matchstrings(1); return ET_SETATTRLIST; }
getattrlist       { matchstrings(1); return ET_GETATTRLIST; }
getxattr          { matchstrings(1); return ET_GETXATTR; }
fgetxattr         return ET_FGETXATTR;
listxattr         { matchstrings(1); return ET_LISTXATTR; }
flistxattr        return ET_FLISTXATTR;
access            { matchstrings(1); return ET_ACCESS; }
chown             { matchstrings(1); return ET_CHOWN; }
chmod             { matchstrings(1); return ET_CHMOD; }
fsetxattr         { matchstrings(1); return ET_FSETXATTR; }
fsgetpath         return ET_FSGETPATH;
getdirentries64   return ET_GETDIRENTRIES64;
getdirentriesattr return ET_GETDIRENTRIESATTR;
getfsstat64       return ET_GETFSSTAT64;
getdtablesize     return ET_GETDTABLESIZE;
chdir             { matchstrings(1); return ET_CHDIR; }
fchdir            return ET_FCHDIR;
kill              return ET_KILL;
fchflags          return ET_FCHFLAGS;
fchmod            return ET_FCHMOD;
fchown            return ET_FCHOWN;
removexattr       { matchstrings(2); return ET_REMOVEXATTR; }
fremovexattr      { matchstrings(1); return ET_FREMOVEXATTR; }
setxattr          { matchstrings(2); return ET_SETXATTR; }
controller        { matchstrings(1); return ET_CONTROLLER; }
resolver          return ET_RESOLVER;
all_calls         return ET_ALL_CALLS;
setter            return ET_SETTER;
build_path        return ET_BUILD_PATH;
namei             return ET_NAMEI;
vn_create         return ET_VN_CREATE;
hfs_vnop_pagein   { matchstrings(1); return ET_HFS_VNOP_PAGEIN; }
vnode_pagein      return ET_VNODE_PAGEIN;
syscall_map       { matchstrings(1); return ET_SYSCALL_MAP; }
posix_spawn       { matchstrings(1); return ET_POSIX_SPAWN; }
execve            return ET_EXECVE;

{o_flag} {
	ibenchlval.str = ibenchtext;
	assert(ibenchlval.str[ibenchleng-1] == ' ');
	ibenchlval.str[ibenchleng-1] = '\0';
	return FLAG_O;
}

{prot_flag} {
	ibenchlval.str = ibenchtext;
	assert(ibenchlval.str[ibenchleng-1] == ' ');
	ibenchlval.str[ibenchleng-1] = '\0';
	return FLAG_PROT;
}

{map_flag} {
	ibenchlval.str = ibenchtext;
	assert(ibenchlval.str[ibenchleng-1] == ' ');
	ibenchlval.str[ibenchleng-1] = '\0';
	return FLAG_MAP;
}

{fcntl_cmd} {
	ibenchlval.str = xstrdup(ibenchtext);
	return FCNTL_CMD;
}

<matchstring>{string} {
	ibenchlval.str = xstrdup(ibenchtext);
	if (!--stringcount)
		BEGIN(INITIAL);
	return STRING;
}

{file_table} {
	ibenchlval.str = xstrdup(ibenchtext);
	return STRING;
}

(dyld|libSystem.B.dylib)`[^;]+/\] {
	ibenchlval.str = xstrdup(ibenchtext);
	return STRING;
}

<*>.|\n REJECT;

%%
