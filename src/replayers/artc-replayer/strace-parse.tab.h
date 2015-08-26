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

#ifndef YY_STRACE_STRACE_PARSE_TAB_H_INCLUDED
# define YY_STRACE_STRACE_PARSE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int stracedebug;
#endif
/* "%code requires" blocks.  */
#line 19 "strace-parse.y" /* yacc.c:1909  */


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


#line 71 "strace-parse.tab.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    END = 0,
    DECNUM = 258,
    HEXNUM = 259,
    OCTNUM = 260,
    TIME = 261,
    THREAD_EXIT = 262,
    FLAG_O = 263,
    FLAG_PROT = 264,
    FLAG_MAP = 265,
    FLAG_FD = 266,
    FLAG_ACCESS = 267,
    FLAG_AT = 268,
    POSIX_FADVICE = 269,
    FLOCK_OP = 270,
    SEEK_WHENCE = 271,
    SOCKDOMAIN = 272,
    SOCKTYPE = 273,
    STRING = 274,
    ERRNO = 275,
    ERRSTR = 276,
    UNFINISHED = 277,
    DETACHED = 278,
    RESUME_START = 279,
    RESUME_END = 280,
    FLAG_RET = 281,
    FLAG_SOCK = 282,
    KW_DATA = 283,
    KW_FILEDES = 284,
    KW_NBYTES = 285,
    KW_OFFSET = 286,
    KW_STR = 287,
    KW_BUF = 288,
    KW_AT_FDCWD = 289,
    NEWLINE = 290,
    COMMA = 291,
    EQUAL = 292,
    PIPE = 293,
    COLON = 294,
    LPAREN = 295,
    RPAREN = 296,
    LESSTHAN = 297,
    GREATERTHAN = 298,
    LBRACE = 299,
    RBRACE = 300,
    LBRACKET = 301,
    RBRACKET = 302,
    STAT_STRUCT = 303,
    DENT_STRUCT = 304,
    STATFS_STRUCT = 305,
    FLOCK_STRUCT = 306,
    TIMESPEC_STRUCT = 307,
    SOCKADDR_STRUCT = 308,
    FCMD_DUPFD = 309,
    FCMD_GETFD = 310,
    FCMD_SETFD = 311,
    FCMD_GETFL = 312,
    FCMD_SETFL = 313,
    FCMD_SETLK = 314,
    FCMD_SETLKW = 315,
    ET_OPEN = 316,
    ET_OPENAT = 317,
    ET_CLOSE = 318,
    ET_DUP = 319,
    ET_READ = 320,
    ET_PREAD = 321,
    ET_WRITE = 322,
    ET_PWRITE = 323,
    ET_AIO_READ = 324,
    ET_AIO_ERROR = 325,
    ET_AIO_RETURN = 326,
    ET_AIO_SUSPEND = 327,
    ET_ACCEPT = 328,
    ET_LSEEK = 329,
    ET_FSYNC = 330,
    ET_FDATASYNC = 331,
    ET_FCNTL = 332,
    ET_FTRUNCATE = 333,
    ET_LINK = 334,
    ET_SYMLINK = 335,
    ET_READLINK = 336,
    ET_UNLINK = 337,
    ET_UNLINKAT = 338,
    ET_RMDIR = 339,
    ET_MKDIR = 340,
    ET_RENAME = 341,
    ET_EXCHANGEDATA = 342,
    ET_FLOCK = 343,
    ET_MMAP = 344,
    ET_UTIMES = 345,
    ET_UTIMENSAT = 346,
    ET_STAT = 347,
    ET_FSTAT = 348,
    ET_NEWFSTATAT = 349,
    ET_STAT64 = 350,
    ET_FSTAT64 = 351,
    ET_LSTAT = 352,
    ET_LSTAT64 = 353,
    ET_STATFS = 354,
    ET_FSTATFS = 355,
    ET_STATFS64 = 356,
    ET_FSTATFS64 = 357,
    ET_SETXATTR = 358,
    ET_FSETXATTR = 359,
    ET_LISTXATTR = 360,
    ET_FLISTXATTR = 361,
    ET_GETXATTR = 362,
    ET_FGETXATTR = 363,
    ET_REMOVEXATTR = 364,
    ET_FREMOVEXATTR = 365,
    ET_ACCESS = 366,
    ET_FACCESSAT = 367,
    ET_CHOWN = 368,
    ET_FCHOWN = 369,
    ET_FCHOWNAT = 370,
    ET_CHMOD = 371,
    ET_FCHMOD = 372,
    ET_CHDIR = 373,
    ET_GETCWD = 374,
    ET_SOCKET = 375,
    ET_PIPE = 376,
    ET_DUP2 = 377,
    ET_DUP3 = 378,
    ET_EPOLL_CREATE = 379,
    ET_FADVISE64 = 380,
    ET_GETDENTS = 381,
    ET_FALLOCATE = 382,
    ET_IO_SETUP = 383,
    ET_IO_SUBMIT = 384,
    ET_IO_GETEVENTS = 385,
    ET_IO_DESTROY = 386
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 46 "strace-parse.y" /* yacc.c:1909  */

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

#line 297 "strace-parse.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE stracelval;

int straceparse (struct parser_state* state);

#endif /* !YY_STRACE_STRACE_PARSE_TAB_H_INCLUDED  */
