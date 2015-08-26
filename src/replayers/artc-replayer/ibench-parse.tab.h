/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_IBENCH_IBENCH_PARSE_TAB_H_INCLUDED
# define YY_IBENCH_IBENCH_PARSE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int ibenchdebug;
#endif
/* "%code requires" blocks.  */
#line 19 "ibench-parse.y" /* yacc.c:1909  */


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


#line 63 "ibench-parse.tab.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    END = 0,
    DECNUM = 258,
    HEXNUM = 259,
    FLAG_O = 260,
    FLAG_PROT = 261,
    FLAG_MAP = 262,
    FCNTL_CMD = 263,
    STRING = 264,
    SOR = 265,
    DELIM = 266,
    EOR = 267,
    SUBTYPE_ENTRY = 268,
    SUBTYPE_RETURN = 269,
    SUBTYPE_COMMENT = 270,
    THREAD_NA = 271,
    RSRC_INUM = 272,
    ET_OPEN = 273,
    ET_SHM_OPEN = 274,
    ET_CLOSE = 275,
    ET_DUP = 276,
    ET_KQUEUE = 277,
    ET_SOCKET = 278,
    ET_SOCKETPAIR = 279,
    ET_READ = 280,
    ET_WRITE = 281,
    ET_PREAD = 282,
    ET_PWRITE = 283,
    ET_AIO_READ = 284,
    ET_AIO_ERROR = 285,
    ET_AIO_RETURN = 286,
    ET_AIO_SUSPEND = 287,
    ET_LSEEK = 288,
    ET_FSYNC = 289,
    ET_FCNTL = 290,
    ET_FTRUNCATE = 291,
    ET_LINK = 292,
    ET_SYMLINK = 293,
    ET_RMDIR = 294,
    ET_UNLINK = 295,
    ET_READLINK = 296,
    ET_RENAME = 297,
    ET_EXCHANGEDATA = 298,
    ET_MKDIR = 299,
    ET_FLOCK = 300,
    ET_MMAP = 301,
    ET_FUTIMES = 302,
    ET_UTIMES = 303,
    ET_STATFS = 304,
    ET_STATFS64 = 305,
    ET_FSTATFS64 = 306,
    ET_FSTAT = 307,
    ET_LSTAT = 308,
    ET_STAT = 309,
    ET_FSTAT64 = 310,
    ET_LSTAT64 = 311,
    ET_STAT64 = 312,
    ET_SETATTRLIST = 313,
    ET_GETATTRLIST = 314,
    ET_GETXATTR = 315,
    ET_FGETXATTR = 316,
    ET_LISTXATTR = 317,
    ET_FLISTXATTR = 318,
    ET_ACCESS = 319,
    ET_CHOWN = 320,
    ET_CHMOD = 321,
    ET_FSETXATTR = 322,
    ET_FSGETPATH = 323,
    ET_GETDIRENTRIES64 = 324,
    ET_GETDIRENTRIESATTR = 325,
    ET_GETFSSTAT64 = 326,
    ET_GETDTABLESIZE = 327,
    ET_CHDIR = 328,
    ET_FCHDIR = 329,
    ET_KILL = 330,
    ET_FCHFLAGS = 331,
    ET_FCHMOD = 332,
    ET_FCHOWN = 333,
    ET_REMOVEXATTR = 334,
    ET_FREMOVEXATTR = 335,
    ET_SETXATTR = 336,
    ET_CONTROLLER = 337,
    ET_RESOLVER = 338,
    ET_ALL_CALLS = 339,
    ET_SETTER = 340,
    ET_BUILD_PATH = 341,
    ET_NAMEI = 342,
    ET_VN_CREATE = 343,
    ET_HFS_VNOP_PAGEIN = 344,
    ET_VNODE_PAGEIN = 345,
    ET_SYSCALL_MAP = 346,
    ET_POSIX_SPAWN = 347,
    ET_EXECVE = 348
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 38 "ibench-parse.y" /* yacc.c:1909  */

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

#line 255 "ibench-parse.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE ibenchlval;

int ibenchparse (struct parser_state* state);

#endif /* !YY_IBENCH_IBENCH_PARSE_TAB_H_INCLUDED  */
