/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         straceparse
#define yylex           stracelex
#define yyerror         straceerror
#define yydebug         stracedebug
#define yynerrs         stracenerrs

#define yylval          stracelval
#define yychar          stracechar

/* Copy the first part of user declarations.  */

#line 75 "strace-parse.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "strace-parse.tab.h".  */
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
#line 19 "strace-parse.y" /* yacc.c:355  */


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


#line 132 "strace-parse.tab.c" /* yacc.c:355  */

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
#line 46 "strace-parse.y" /* yacc.c:355  */

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

#line 358 "strace-parse.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE stracelval;

int straceparse (struct parser_state* state);

#endif /* !YY_STRACE_STRACE_PARSE_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */
#line 258 "strace-parse.y" /* yacc.c:358  */



#line 376 "strace-parse.tab.c" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 127 "strace-parse.y" /* yacc.c:359  */

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


#line 509 "strace-parse.tab.c" /* yacc.c:359  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   781

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  132
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  222
/* YYNRULES -- Number of rules.  */
#define YYNRULES  439
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  958

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   386

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   429,   429,   430,   439,   456,   482,   486,   495,   504,
     505,   507,   514,   517,   520,   523,   527,   533,   534,   535,
     536,   537,   538,   539,   540,   541,   542,   543,   544,   545,
     546,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,   573,   574,   575,
     576,   577,   578,   580,   581,   582,   583,   584,   586,   587,
     588,   589,   590,   591,   592,   593,   595,   598,   599,   600,
     601,   602,   603,   604,   605,   606,   607,   608,   609,   610,
     611,   612,   613,   614,   615,   616,   617,   618,   619,   620,
     621,   622,   623,   624,   625,   626,   627,   628,   629,   630,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
     641,   642,   643,   645,   646,   647,   648,   649,   651,   652,
     653,   654,   655,   656,   657,   658,   660,   663,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,   679,   680,   681,   682,   683,   684,   685,
     686,   687,   688,   689,   690,   691,   692,   693,   694,   695,
     696,   697,   698,   699,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   710,   711,   712,   713,   714,   716,   717,
     718,   719,   720,   721,   722,   723,   725,   730,   735,   742,
     746,   751,   755,   757,   761,   769,   774,   783,   788,   793,
     797,   802,   807,   813,   822,   827,   837,   847,   852,   856,
     864,   869,   873,   878,   883,   889,   895,   902,   908,   914,
     920,   926,   933,   938,   942,   947,   952,   958,   965,   970,
     974,   980,   985,   992,   997,  1002,  1008,  1013,  1018,  1022,
    1029,  1034,  1038,  1043,  1048,  1054,  1060,  1065,  1069,  1078,
    1084,  1093,  1101,  1106,  1115,  1123,  1128,  1137,  1145,  1150,
    1154,  1159,  1164,  1168,  1174,  1179,  1183,  1189,  1194,  1198,
    1204,  1209,  1213,  1218,  1223,  1227,  1232,  1237,  1241,  1247,
    1252,  1256,  1261,  1266,  1270,  1276,  1281,  1285,  1290,  1295,
    1299,  1310,  1312,  1319,  1328,  1333,  1337,  1346,  1351,  1355,
    1360,  1365,  1369,  1378,  1383,  1387,  1393,  1398,  1402,  1407,
    1412,  1416,  1425,  1431,  1440,  1448,  1453,  1462,  1468,  1473,
    1477,  1483,  1488,  1492,  1500,  1505,  1509,  1515,  1520,  1524,
    1530,  1542,  1547,  1551,  1557,  1559,  1566,  1572,  1577,  1581,
    1589,  1594,  1598,  1605,  1610,  1614,  1623,  1628,  1632,  1639,
    1644,  1648,  1655,  1660,  1664,  1671,  1676,  1680,  1686,  1691,
    1695,  1700,  1705,  1711,  1717,  1722,  1729,  1736,  1741,  1745,
    1754,  1767,  1774,  1779,  1784,  1788,  1788,  1790,  1790,  1791,
    1791,  1793,  1796,  1800,  1800,  1802,  1804,  1804,  1806,  1810,
    1813,  1817,  1820,  1825,  1835,  1846,  1849,  1853,  1855,  1859,
    1863,  1867,  1871,  1875,  1878,  1882,  1885,  1890,  1902,  1903,
    1906,  1909,  1916,  1917,  1918,  1921,  1924,  1929,  1932,  1939,
    1942,  1947,  1950,  1955,  1958,  1967,  1968,  1970,  1973,  1977
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"EOF\"", "error", "$undefined", "DECNUM", "HEXNUM", "OCTNUM", "TIME",
  "THREAD_EXIT", "FLAG_O", "FLAG_PROT", "FLAG_MAP", "FLAG_FD",
  "FLAG_ACCESS", "FLAG_AT", "POSIX_FADVICE", "FLOCK_OP", "SEEK_WHENCE",
  "SOCKDOMAIN", "SOCKTYPE", "STRING", "ERRNO", "ERRSTR", "UNFINISHED",
  "DETACHED", "RESUME_START", "RESUME_END", "FLAG_RET", "FLAG_SOCK",
  "KW_DATA", "KW_FILEDES", "KW_NBYTES", "KW_OFFSET", "KW_STR", "KW_BUF",
  "KW_AT_FDCWD", "NEWLINE", "COMMA", "EQUAL", "PIPE", "COLON", "LPAREN",
  "RPAREN", "LESSTHAN", "GREATERTHAN", "LBRACE", "RBRACE", "LBRACKET",
  "RBRACKET", "STAT_STRUCT", "DENT_STRUCT", "STATFS_STRUCT",
  "FLOCK_STRUCT", "TIMESPEC_STRUCT", "SOCKADDR_STRUCT", "FCMD_DUPFD",
  "FCMD_GETFD", "FCMD_SETFD", "FCMD_GETFL", "FCMD_SETFL", "FCMD_SETLK",
  "FCMD_SETLKW", "ET_OPEN", "ET_OPENAT", "ET_CLOSE", "ET_DUP", "ET_READ",
  "ET_PREAD", "ET_WRITE", "ET_PWRITE", "ET_AIO_READ", "ET_AIO_ERROR",
  "ET_AIO_RETURN", "ET_AIO_SUSPEND", "ET_ACCEPT", "ET_LSEEK", "ET_FSYNC",
  "ET_FDATASYNC", "ET_FCNTL", "ET_FTRUNCATE", "ET_LINK", "ET_SYMLINK",
  "ET_READLINK", "ET_UNLINK", "ET_UNLINKAT", "ET_RMDIR", "ET_MKDIR",
  "ET_RENAME", "ET_EXCHANGEDATA", "ET_FLOCK", "ET_MMAP", "ET_UTIMES",
  "ET_UTIMENSAT", "ET_STAT", "ET_FSTAT", "ET_NEWFSTATAT", "ET_STAT64",
  "ET_FSTAT64", "ET_LSTAT", "ET_LSTAT64", "ET_STATFS", "ET_FSTATFS",
  "ET_STATFS64", "ET_FSTATFS64", "ET_SETXATTR", "ET_FSETXATTR",
  "ET_LISTXATTR", "ET_FLISTXATTR", "ET_GETXATTR", "ET_FGETXATTR",
  "ET_REMOVEXATTR", "ET_FREMOVEXATTR", "ET_ACCESS", "ET_FACCESSAT",
  "ET_CHOWN", "ET_FCHOWN", "ET_FCHOWNAT", "ET_CHMOD", "ET_FCHMOD",
  "ET_CHDIR", "ET_GETCWD", "ET_SOCKET", "ET_PIPE", "ET_DUP2", "ET_DUP3",
  "ET_EPOLL_CREATE", "ET_FADVISE64", "ET_GETDENTS", "ET_FALLOCATE",
  "ET_IO_SETUP", "ET_IO_SUBMIT", "ET_IO_GETEVENTS", "ET_IO_DESTROY",
  "$accept", "input", "full_event", "event_end", "call_end", "event_begin",
  "thread_exit", "call_result", "elapsed_time", "full_call", "call_begin",
  "open_allparams", "openat_allparams", "open_beginparams",
  "openat_beginparams", "open_endparams", "openat_endparams",
  "read_allparams", "read_beginparams", "read_endparams",
  "close_allparams", "close_beginparams", "close_endparams",
  "stat_allparams", "stat_beginparams", "stat_endparams",
  "pread_allparams", "pread_beginparams", "pread_endparams",
  "mmap_allparams", "mmap_beginparams", "mmap_endparams",
  "fadvise64_allparams", "fadvise64_beginparams", "fadvise64_endparams",
  "statfs_allparams", "statfs_beginparams", "statfs_endparams",
  "fcntl_allparams", "fcntl_beginparams", "fcntl_endparams",
  "fstatfs_allparams", "fstatfs_beginparams", "fstatfs_endparams",
  "lseek_allparams", "lseek_beginparams", "lseek_endparams",
  "getdents_allparams", "getdents_beginparams", "getdents_endparams",
  "fstat_allparams", "fstat_beginparams", "fstat_endparams",
  "epoll_create_allparams", "epoll_create_beginparams",
  "epoll_create_endparams", "socket_allparams", "socket_beginparams",
  "socket_endparams", "lstat_allparams", "lstat_beginparams",
  "lstat_endparams", "access_allparams", "access_beginparams",
  "access_endparams", "fgetxattr_allparams", "fgetxattr_beginparams",
  "fgetxattr_endparams", "readlink_allparams", "readlink_beginparams",
  "readlink_endparams", "listxattr_allparams", "listxattr_beginparams",
  "listxattr_endparams", "write_allparams", "write_beginparams",
  "write_endparams", "dup_allparams", "dup_beginparams", "dup_endparams",
  "mkdir_allparams", "mkdir_beginparams", "mkdir_endparams",
  "flock_allparams", "flock_beginparams", "flock_endparams",
  "ftruncate_allparams", "ftruncate_beginparams", "ftruncate_endparams",
  "fsync_allparams", "fsync_beginparams", "fsync_endparams",
  "fdatasync_allparams", "fdatasync_beginparams", "fdatasync_endparams",
  "chmod_allparams", "chmod_beginparams", "chmod_endparams",
  "chown_allparams", "chown_beginparams", "chown_endparams",
  "rename_allparams", "rename_beginparams", "rename_endparams",
  "chdir_allparams", "chdir_beginparams", "chdir_endparams",
  "getcwd_allparams", "getcwd_beginparams", "getcwd_endparams",
  "fsetxattr_allparams", "fsetxattr_beginparams", "fsetxattr_endparams",
  "pwrite_allparams", "pwrite_beginparams", "pwrite_endparams",
  "unlink_allparams", "unlink_beginparams", "unlink_endparams",
  "setxattr_allparams", "setxattr_beginparams", "setxattr_endparams",
  "link_allparams", "link_beginparams", "link_endparams",
  "rmdir_allparams", "rmdir_beginparams", "rmdir_endparams",
  "getxattr_allparams", "getxattr_beginparams", "getxattr_endparams",
  "flistxattr_allparams", "flistxattr_beginparams", "flistxattr_endparams",
  "removexattr_allparams", "removexattr_beginparams",
  "removexattr_endparams", "fremovexattr_allparams",
  "fremovexattr_beginparams", "fremovexattr_endparams",
  "fallocate_allparams", "fallocate_beginparams", "fallocate_endparams",
  "fchmod_allparams", "fchmod_beginparams", "fchmod_endparams",
  "utimensat_allparams", "utimensat_beginparams", "utimensat_endparams",
  "pipe_allparams", "pipe_beginparams", "pipe_endparams", "dup2_allparams",
  "dup2_beginparams", "dup2_endparams", "faccessat_allparams",
  "faccessat_beginparams", "faccessat_endparams", "newfstatat_allparams",
  "newfstatat_beginparams", "newfstatat_endparams", "fchownat_allparams",
  "fchownat_beginparams", "fchownat_endparams", "unlinkat_allparams",
  "unlinkat_beginparams", "unlinkat_endparams", "dup3_allparams",
  "dup3_beginparams", "dup3_endparams", "fchown_allparams",
  "fchown_beginparams", "fchown_endparams", "symlink_allparams",
  "symlink_beginparams", "symlink_endparams", "accept_allparams",
  "accept_beginparams", "accept_endparams", "io_setup_allparams",
  "io_setup_beginparams", "io_setup_endparams", "io_submit_allparams",
  "io_submit_beginparams", "io_submit_endparams", "io_getevents_allparams",
  "io_getevents_beginparams", "io_getevents_endparams",
  "io_destroy_allparams", "io_destroy_beginparams", "io_destroy_endparams",
  "maybe_comma", "stat_struct", "statfs_struct", "maybe_string",
  "maybe_timespec", "dent_array", "optional_dents", "iocb_array",
  "optional_iocbs", "iocb_list", "iocb", "maybe_iocb_data_comma",
  "iocb_buf", "iocb_str", "iocb_fd", "iocb_nbytes", "iocb_offset",
  "ioevent_array", "optional_ioevents", "ioevent_list", "ioevent",
  "timespec_pair", "fd_flags", "integer", "o_flags", "optional_o_flags",
  "prot_flags", "map_flags", "access_flags", "socktype_flags", "at_flags", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386
};
# endif

#define YYPACT_NINF -853

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-853)))

#define YYTABLE_NINF -200

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -853,    27,  -853,    14,   -16,    -9,    -1,    18,    -2,  -853,
    -853,  -853,  -853,  -853,   112,     7,    17,    45,    48,    53,
      59,    60,    91,    93,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     141,   142,   143,   159,   162,   167,   168,   170,   173,   174,
     204,   205,   206,   207,   208,    21,   112,  -853,  -853,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   270,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   271,
     133,   286,   308,   310,   311,   312,   313,   314,   315,   316,
     318,   319,   320,   321,   306,   307,   309,   317,   293,   322,
     323,   324,   326,   327,   329,   325,   330,   296,   328,   331,
     335,   332,   336,   333,   337,   342,   343,   344,   345,   346,
     300,   347,   351,   334,   348,   359,   350,   352,   353,   299,
     361,   369,   370,   371,   372,   373,   374,   375,   376,   377,
      28,   339,  -853,  -853,  -853,  -853,  -853,     9,     9,  -853,
    -853,   338,  -853,  -853,  -853,  -853,  -853,  -853,  -853,     9,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,     2,     2,
    -853,     2,     6,     6,  -853,  -853,   363,   364,   349,   354,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
     352,  -853,   299,  -853,  -853,  -853,  -853,   305,  -853,   340,
    -853,   355,  -853,    21,   356,   357,  -853,   358,   360,  -853,
    -853,   362,  -853,  -853,   365,  -853,   366,   367,  -853,   368,
     378,  -853,   379,   380,  -853,   381,   382,  -853,   384,   383,
    -853,   386,   385,  -853,  -853,   387,  -853,  -853,   388,  -853,
     389,   390,  -853,   391,   392,  -853,   394,   393,  -853,   396,
     395,  -853,   399,   397,  -853,  -853,   398,  -853,   401,   400,
    -853,  -853,   402,  -853,   404,   403,  -853,   406,   405,  -853,
     409,   407,  -853,   411,   408,  -853,   414,   410,  -853,   416,
     412,  -853,   418,   415,  -853,   419,   417,  -853,   421,   420,
    -853,   423,   422,  -853,   424,   425,  -853,   426,   427,  -853,
     428,   429,  -853,   431,   430,  -853,   433,   432,  -853,   436,
     434,  -853,   438,   435,  -853,   441,   437,  -853,   443,   439,
    -853,   445,   442,  -853,   446,   444,  -853,   448,   447,  -853,
     450,   449,  -853,   451,   452,  -853,   453,   454,  -853,   455,
     456,  -853,  -853,   457,  -853,   458,   459,  -853,   460,   461,
    -853,   413,   462,  -853,   463,   464,  -853,   465,   466,  -853,
    -853,   467,  -853,   468,   469,  -853,   470,   471,  -853,   473,
     472,  -853,   475,   474,  -853,   478,   476,  -853,   480,   477,
    -853,  -853,   479,  -853,   499,  -853,  -853,   481,   486,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,   487,  -853,   488,
    -853,  -853,   489,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,   490,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,   491,  -853,
     492,  -853,     9,  -853,     9,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,   493,  -853,   519,
    -853,  -853,   494,  -853,   495,  -853,   339,   522,  -853,   133,
    -853,  -853,  -853,     9,  -853,     9,  -853,   513,  -853,   514,
    -853,   482,  -853,   531,  -853,  -853,  -853,   -18,  -853,   533,
    -853,   518,  -853,   520,  -853,     9,  -853,  -853,   521,  -853,
    -853,    40,  -853,   523,  -853,   528,  -853,   541,  -853,    10,
    -853,     2,  -853,     2,  -853,   526,  -853,     2,  -853,     6,
    -853,     6,  -853,   527,  -853,   529,  -853,   530,  -853,   532,
    -853,   534,  -853,   535,  -853,   536,  -853,   537,  -853,   538,
    -853,   539,  -853,   544,  -853,   549,  -853,   540,  -853,    40,
    -853,    40,  -853,  -853,   554,  -853,   542,  -853,   524,  -853,
     558,  -853,   559,  -853,  -853,   560,  -853,   305,  -853,   561,
    -853,   543,  -853,   562,  -853,   565,  -853,  -853,   550,   546,
     545,   567,   570,   341,   571,   572,   573,   547,   548,   -28,
     574,   551,   575,   552,   553,  -853,  -853,   555,  -853,  -853,
      31,   557,  -853,   556,   563,   564,   566,   568,   569,   576,
    -853,   577,  -853,   578,   579,   580,  -853,  -853,  -853,   581,
     582,  -853,  -853,  -853,  -853,   583,   584,   585,  -853,  -853,
     586,  -853,  -853,  -853,   587,   588,   589,   590,   591,   592,
    -853,  -853,  -853,   593,   594,   596,   597,   598,  -853,  -853,
    -853,  -853,    32,   600,  -853,   599,   601,   602,   603,   605,
     604,   606,  -853,   595,  -853,  -853,   607,   608,  -853,  -853,
    -853,   626,   638,  -853,  -853,  -853,  -853,   609,  -853,   494,
    -853,    40,   636,   643,   644,   645,   646,   610,   634,   648,
      13,   522,   611,   612,   649,    12,   650,     5,     5,     2,
     635,   639,   652,   654,     9,     9,   641,   538,   657,   658,
     661,   662,   640,   619,    15,   665,   666,   667,   627,   629,
     668,  -853,   671,   628,  -853,  -853,   672,  -853,  -853,  -853,
    -853,   642,  -853,   647,   674,  -853,  -853,  -853,  -853,  -853,
     651,  -853,  -853,  -853,  -853,  -853,   653,  -853,    58,  -853,
     630,   656,   659,   660,   663,   664,  -853,  -853,   669,   670,
    -853,   593,  -853,  -853,   673,  -853,  -853,  -853,  -853,   651,
    -853,   675,  -853,   676,  -853,   637,  -853,   677,  -853,  -853,
     678,   681,   682,   632,   680,   684,   679,   683,   687,   694,
      12,   695,   698,   699,   700,   701,   693,   705,   688,   685,
     686,  -853,   355,   707,  -853,  -853,  -853,  -853,  -853,    94,
    -853,   689,  -853,  -853,   653,   690,   691,  -853,  -853,   692,
    -853,  -853,   696,   -20,  -853,   637,   495,   697,   712,   708,
     702,   714,   717,    12,   719,   703,   704,  -853,     3,   718,
     706,  -853,  -853,  -853,  -853,   653,   709,   715,   715,  -853,
    -853,  -853,   710,    40,  -853,   711,   713,   716,  -853,  -853,
     721,   720,   655,  -853,   722,   723,   724,   726,   725,   727,
     729,   727,  -853,   728,   730,  -853,   732,   731,   733,   733,
    -853,   734,   735,   736,   740,  -853,  -853,  -853
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     3,
       5,     4,     6,    12,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     9,    10,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   301,     0,   344,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    11,   202,   203,   209,   272,     0,     0,   269,
     308,     0,   239,   284,   287,   233,   281,   317,   369,     0,
     311,   360,   320,   275,   296,   278,   218,   342,     0,     0,
     354,     0,     0,     0,   314,   305,     0,     0,     0,     0,
     329,   332,   257,   351,   293,   366,   357,   290,   338,   299,
       0,   251,     0,   348,   363,   248,   221,     0,   335,     0,
     378,     0,   384,     0,     0,   200,    79,     0,     0,   128,
     207,   208,    84,   270,   271,    82,     0,     0,    86,     0,
       0,    91,     0,   268,    87,     0,   307,   102,     0,     0,
     123,     0,   238,    83,   282,   283,   104,   285,   286,   105,
       0,   232,    80,     0,   280,   111,     0,   316,   107,     0,
     368,   122,     0,     0,    96,   309,   310,   103,     0,   359,
     135,   318,   319,   112,     0,   274,   114,     0,   295,   110,
       0,   277,    95,     0,   217,    88,     0,   341,   120,     0,
       0,    77,     0,     0,    89,     0,   353,   133,     0,     0,
      90,     0,     0,    94,     0,     0,    81,     0,   313,   106,
       0,   304,   101,     0,     0,    98,     0,     0,   115,     0,
       0,   113,     0,     0,    93,     0,   328,   116,     0,   331,
     117,     0,   256,    92,     0,   350,   132,     0,   292,   109,
       0,   365,   121,     0,   356,   134,     0,   289,   108,     0,
     337,   119,   297,   298,    99,     0,     0,   100,     0,   250,
     129,     0,     0,   130,     0,   347,   131,     0,   362,   136,
     246,   247,    78,     0,   220,    97,     0,     0,    85,     0,
     334,   118,     0,     0,   124,     0,   377,   125,     0,     0,
     126,   382,   383,   127,   422,   423,   424,    13,     0,     7,
     139,   188,   144,   142,   392,   391,   146,     0,   151,     0,
     147,   162,     0,   183,   143,   164,   165,   140,   171,   167,
     182,   156,     0,   163,   195,   172,   174,   170,   155,   148,
     180,   388,   387,   137,   212,   149,   245,   193,   150,   254,
     390,   389,   154,   224,   141,   236,   166,   161,     0,   158,
       0,   175,     0,   173,     0,   153,   176,   177,   152,   192,
     169,   181,   194,   168,   179,   159,   302,   160,   189,   345,
     190,   191,   196,   138,   157,   396,   145,     0,   178,     0,
     184,   185,   413,   186,   385,   187,     0,     0,    17,     0,
      68,    19,    37,   205,    18,   214,    21,     0,    36,     0,
      49,   371,    63,     0,    27,    41,    42,     0,    25,     0,
      40,     0,    52,     0,    62,   262,    34,    50,     0,    75,
      53,     0,    38,     0,    45,     0,    39,     0,    22,     0,
      60,   211,    20,   244,    29,     0,    73,   253,    31,   223,
      24,   235,    26,     0,    51,     0,    48,   265,    35,   325,
      55,     0,    54,     0,    33,     0,    56,     0,    57,     0,
      32,     0,    72,     0,    44,     0,    61,     0,    74,     0,
      43,     0,    59,    46,     0,    47,     0,    69,     0,    70,
       0,    71,     0,    76,    30,     0,    23,   241,    28,     0,
      58,   374,    64,     0,    65,     0,    66,    67,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   414,   415,   386,     0,     8,   425,
     197,   200,   201,     0,     0,     0,     0,     0,     0,     0,
     227,     0,   228,     0,     0,     0,   279,   315,   367,     0,
       0,   422,   273,   294,   276,     0,     0,     0,   210,   243,
       0,   252,   222,   234,     0,     0,     0,     0,   322,   259,
     327,   330,   433,   255,     0,     0,     0,     0,   288,   336,
     300,   435,     0,     0,   346,     0,     0,     0,     0,     0,
       0,     0,    14,     0,    16,   206,     0,     0,   263,   266,
     326,     0,     0,   395,   397,   242,   375,     0,   412,     0,
     381,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    15,     0,     0,   323,   260,     0,   416,   198,   426,
     204,     0,   267,     0,     0,   237,   225,   421,   420,   226,
     229,   230,   231,   261,   439,   437,   358,   429,     0,   419,
       0,     0,     0,     0,     0,     0,   264,   324,     0,     0,
     434,   349,   291,   364,     0,   249,   436,   343,   428,   427,
     361,     0,   240,     0,   373,   399,   376,     0,   215,   372,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   405,     0,
     400,   401,   380,     0,   213,   306,   370,   438,   431,     0,
     430,     0,   340,   339,   352,     0,     0,   321,   258,     0,
     219,   333,     0,     0,   398,     0,   385,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,     0,     0,
       0,   432,   418,   312,   303,   355,     0,     0,     0,   393,
     394,   379,     0,     0,   406,     0,     0,     0,   417,   216,
       0,     0,     0,   409,     0,     0,     0,     0,     0,     0,
       0,     0,   407,     0,     0,   408,     0,     0,     0,     0,
     410,     0,     0,     0,     0,   404,   403,   411
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -853,  -853,  -853,  -853,   502,  -853,  -853,   134,  -150,  -853,
    -853,  -151,  -853,  -146,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,   139,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,   144,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,  -853,
    -853,  -501,  -228,  -231,  -208,  -853,  -237,  -853,  -853,  -853,
    -853,  -490,  -853,  -853,  -853,  -511,  -532,  -539,  -461,  -853,
    -853,  -337,  -355,  -853,  -567,  -749,  -853,  -853,  -853,  -363,
    -853,  -852
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     4,     5,   139,     6,     7,   201,   449,    75,
      76,   265,   268,   266,   269,   450,   451,   277,   278,   456,
     271,   272,   452,   340,   341,   483,   280,   281,   458,   334,
     335,   479,   424,   425,   524,   352,   353,   492,   301,   302,
     467,   355,   356,   494,   292,   293,   464,   427,   428,   526,
     343,   344,   485,   421,   422,   523,   409,   410,   518,   349,
     350,   488,   382,   383,   508,   373,   374,   505,   313,   314,
     471,   364,   365,   499,   283,   284,   460,   274,   275,   453,
     325,   326,   476,   331,   332,   478,   304,   305,   468,   295,
     296,   465,   298,   299,   466,   397,   398,   513,   388,   389,
     510,   328,   329,   477,   403,   404,   515,   406,   407,   517,
     361,   362,   497,   286,   287,   461,   316,   317,   473,   358,
     359,   496,   307,   308,   469,   322,   323,   475,   370,   371,
     503,   367,   368,   501,   376,   377,   506,   379,   380,   507,
     430,   431,   528,   400,   401,   514,   337,   338,   480,   412,
     413,   520,   415,   416,   521,   385,   386,   509,   346,   347,
     487,   394,   395,   512,   319,   320,   474,   418,   419,   522,
     391,   392,   511,   310,   311,   470,   289,   290,   463,   433,
     434,   530,   436,   437,   531,   439,   440,   533,   442,   443,
     535,   667,   484,   493,   457,   921,   527,   659,   846,   869,
     870,   871,   893,   935,   937,   926,   944,   952,   534,   663,
     664,   665,   821,   809,   447,   670,   840,   818,   879,   713,
     722,   816
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     459,   486,   495,   489,   692,    13,   481,   919,   884,   819,
     490,   472,   810,   454,   696,   814,   807,   743,   838,     9,
       8,   744,    14,   669,   808,   815,    10,     2,   455,   697,
       3,   444,   445,   446,    11,   839,   679,   680,   681,   682,
     683,   684,   685,   691,   445,   446,   905,   140,   906,   820,
     482,   915,   718,    12,   719,   920,   491,   141,   200,    15,
      16,    17,    18,    19,    20,    21,    22,   751,   781,   752,
     782,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,   142,    37,    38,   143,    39,
      40,    41,    42,   144,   855,    43,   856,    44,    45,   145,
     146,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
     898,   147,   899,   148,    77,    78,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   264,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,    79,    80,    81,    82,    83,    84,    85,
      86,   185,   186,   187,   798,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   188,
     101,   102,   189,   103,   104,   105,   106,   190,   191,   107,
     192,   108,   109,   193,   194,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   195,   196,   197,   198,   199,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   657,   248,   658,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   270,   263,   273,   276,   279,   282,   285,   288,   291,
     267,   294,   297,   300,   303,   306,   309,   318,   312,   330,
     345,   333,   336,   342,   384,   673,   315,   674,   354,   360,
     366,   321,   324,   327,   339,   411,   372,   348,   378,   525,
     351,   357,   363,   698,   390,   699,   929,   689,   702,   701,
     703,   369,   399,   375,   414,   381,   387,   396,   393,   402,
     408,   405,   417,   420,   423,   426,   429,   432,   435,   438,
     441,   448,   498,   500,   529,   502,   668,   737,   671,   516,
     504,   462,   537,   672,   539,   908,   519,   536,   538,   532,
     727,   540,   543,   541,   545,   907,   542,   927,   544,   946,
     953,   896,   797,   822,   831,   547,   628,   549,     0,   546,
     551,   548,   553,   550,   552,   557,   554,   559,   555,   556,
     561,   558,   563,   560,   562,   565,   564,   568,   566,   567,
     571,   569,   573,   570,   572,   575,   574,   577,   576,   578,
     579,   580,   581,   582,   583,   585,   584,   587,   586,   589,
     591,   588,   593,   590,   595,     0,   592,   597,   594,   599,
     596,   598,   601,   600,   603,   602,   604,   605,   606,   607,
     608,   609,   611,   610,   613,   612,   615,   617,   614,   619,
     616,   621,   650,   618,   624,   620,   626,   622,   623,   630,
     625,   632,   627,   629,   635,   631,   637,   633,   634,   639,
     636,   641,   638,   640,   643,   642,   645,   644,   646,   648,
     647,   649,   661,   651,   652,   653,   654,   655,   656,   660,
     669,   666,   675,   676,   678,   677,   686,   687,   662,   688,
     690,   823,   693,   694,   695,   700,   704,   715,   705,   706,
     712,   707,   716,   708,   709,   710,   711,   720,   714,   717,
     723,   724,   725,   726,   728,   730,   828,   829,   731,   721,
     735,   732,   733,   736,   738,   739,   740,   745,   202,   747,
       0,     0,     0,   741,   742,     0,     0,   729,   734,   749,
       0,     0,   753,     0,     0,     0,   746,   748,  -199,   754,
     755,     0,   756,   783,   757,   758,     0,   750,   788,     0,
       0,   793,   759,   760,   761,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,   772,   773,   774,   775,   794,
     777,   776,   778,   779,   780,   784,   791,   785,   786,   787,
     789,   795,   790,   792,   799,   796,   800,   801,   802,   803,
     805,   806,   813,   830,   824,   826,   804,   827,   825,   817,
     832,   833,   811,   812,   834,   835,   837,   836,   841,   842,
     843,   847,   844,   845,   848,   849,   850,   853,   851,   876,
       0,   868,   857,   852,   874,   875,     0,   936,   880,   752,
     882,   854,   858,   877,   878,   859,   860,   883,   885,   861,
     862,   886,   887,   888,   889,   863,   864,   890,   891,   865,
     897,   866,   867,   872,   873,   910,   892,   913,   911,   881,
     914,   922,   895,   916,   933,     0,   901,   902,   903,   942,
     894,     0,     0,   909,   950,   904,     0,     0,     0,   917,
     918,   900,   923,   957,   925,   924,     0,   912,   945,   931,
     930,     0,   932,   934,     0,   928,     0,   943,     0,   939,
       0,   938,   941,   940,   951,     0,   948,   947,   949,     0,
       0,     0,     0,   954,     0,     0,     0,     0,     0,     0,
     955,   956
};

static const yytype_int16 yycheck[] =
{
     208,   229,   233,   231,   571,     7,     4,     4,   860,     4,
       4,   219,   761,     4,     4,     3,     3,    45,     3,    35,
       6,    49,    24,     8,    11,    13,    35,     0,    19,    19,
       3,     3,     4,     5,    35,   784,    54,    55,    56,    57,
      58,    59,    60,     3,     4,     5,    66,    40,    68,    44,
      48,   903,   619,    35,   621,    52,    50,    40,    37,    61,
      62,    63,    64,    65,    66,    67,    68,    36,    36,    38,
      38,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    40,    88,    89,    40,    91,
      92,    93,    94,    40,    36,    97,    38,    99,   100,    40,
      40,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
      36,    40,    38,    40,    22,    23,    40,    40,    40,    40,
      40,    40,    40,    40,    40,    40,    40,    40,    40,    40,
      40,    40,    19,    40,    40,    40,    40,    40,    40,    40,
      40,    40,    40,    40,    40,    40,    40,    40,    40,    40,
      40,    40,    40,    61,    62,    63,    64,    65,    66,    67,
      68,    40,    40,    40,   751,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    40,
      88,    89,    40,    91,    92,    93,    94,    40,    40,    97,
      40,    99,   100,    40,    40,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,    40,    40,    40,    40,    40,    25,
      25,    25,    25,    25,    25,    25,    25,    25,    25,    25,
      25,    25,    25,    25,    25,    25,    25,    25,    25,    25,
      25,    25,    25,    25,    25,    25,    25,    25,    25,    25,
      25,    25,    25,    25,    25,    25,    25,    25,    25,    25,
      25,    25,    25,    25,   502,    25,   504,    25,    25,    25,
      25,    25,    25,    25,    25,    25,    25,    25,    25,    25,
      25,     3,    41,     3,     3,     3,     3,     3,     3,     3,
      34,     3,     3,     3,     3,    19,    19,    34,    19,     3,
      34,     4,     3,     3,    34,   543,    19,   545,     3,     3,
       3,    19,    19,    19,    19,    46,     3,    19,     3,    44,
      19,    19,    19,   581,     3,   583,   923,   565,   589,   587,
     591,    19,     3,    19,     3,    19,    19,    19,    34,    19,
      17,    19,     3,     3,     3,     3,     3,     3,     3,     3,
       3,    42,    19,    19,    44,    36,   536,    46,   539,   250,
      36,    53,    36,   539,    36,   896,   252,   263,    41,    44,
     637,    41,    36,    41,    36,   895,    41,   918,    41,   941,
     949,   872,   749,   768,   777,    36,     3,    36,    -1,    41,
      36,    41,    36,    41,    41,    36,    41,    36,    41,    41,
      36,    41,    36,    41,    41,    36,    41,    36,    41,    41,
      36,    41,    36,    41,    41,    36,    41,    36,    41,    41,
      36,    41,    36,    41,    36,    36,    41,    36,    41,    36,
      36,    41,    36,    41,    36,    -1,    41,    36,    41,    36,
      41,    41,    36,    41,    36,    41,    41,    36,    41,    36,
      41,    36,    36,    41,    36,    41,    36,    36,    41,    36,
      41,    36,     6,    41,    36,    41,    36,    41,    41,    36,
      41,    36,    41,    41,    36,    41,    36,    41,    41,    36,
      41,    36,    41,    41,    36,    41,    36,    41,    41,    20,
      41,    40,     3,    36,    36,    36,    36,    36,    36,    36,
       8,    36,    19,    19,     3,    53,     3,    19,    44,    19,
      19,   769,    19,    15,     3,    19,    19,     3,    19,    19,
      12,    19,     3,    19,    19,    19,    19,     3,    19,    19,
      36,     3,     3,     3,     3,     3,   774,   775,     3,    27,
       3,    21,    26,     3,     3,     3,     3,     3,    76,     4,
      -1,    -1,    -1,    36,    36,    -1,    -1,    44,    43,    36,
      -1,    -1,    36,    -1,    -1,    -1,    45,    45,    41,    36,
      36,    -1,    36,     3,    36,    36,    -1,    52,     3,    -1,
      -1,     3,    36,    36,    36,    36,    36,    36,    36,    36,
      36,    36,    36,    36,    36,    36,    36,    36,    36,     3,
      36,    38,    36,    36,    36,    36,    41,    36,    36,    36,
      36,     3,    36,    36,     8,    36,     3,     3,     3,     3,
      16,     3,     3,    12,    19,     3,    46,     3,    19,     9,
       3,     3,    51,    51,     3,     3,    47,    27,     3,     3,
       3,     3,    45,    44,     3,    47,     4,     3,    36,    47,
      -1,    44,    52,    36,     3,     3,    -1,    32,     9,    38,
       3,    38,    36,    13,    10,    36,    36,     3,     3,    36,
      36,     3,     3,     3,     3,    36,    36,    14,     3,    36,
       3,    36,    36,    36,    36,     3,    28,     3,    10,    36,
       3,     3,    36,     4,     3,    -1,    36,    36,    36,     4,
      45,    -1,    -1,    36,     3,    39,    -1,    -1,    -1,    36,
      36,    52,    36,     3,    29,    36,    -1,    45,    19,    36,
      39,    -1,    36,    33,    -1,    45,    -1,    30,    -1,    36,
      -1,    39,    36,    39,    31,    -1,    36,    39,    36,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    45
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   133,     0,     3,   134,   135,   137,   138,     6,    35,
      35,    35,    35,     7,    24,    61,    62,    63,    64,    65,
      66,    67,    68,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    88,    89,    91,
      92,    93,    94,    97,    99,   100,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   141,   142,    22,    23,    61,
      62,    63,    64,    65,    66,    67,    68,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    88,    89,    91,    92,    93,    94,    97,    99,   100,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   136,
      40,    40,    40,    40,    40,    40,    40,    40,    40,    40,
      40,    40,    40,    40,    40,    40,    40,    40,    40,    40,
      40,    40,    40,    40,    40,    40,    40,    40,    40,    40,
      40,    40,    40,    40,    40,    40,    40,    40,    40,    40,
      40,    40,    40,    40,    40,    40,    40,    40,    40,    40,
      40,    40,    40,    40,    40,    40,    40,    40,    40,    40,
      37,   139,   136,    25,    25,    25,    25,    25,    25,    25,
      25,    25,    25,    25,    25,    25,    25,    25,    25,    25,
      25,    25,    25,    25,    25,    25,    25,    25,    25,    25,
      25,    25,    25,    25,    25,    25,    25,    25,    25,    25,
      25,    25,    25,    25,    25,    25,    25,    25,    25,    25,
      25,    25,    25,    25,    25,    25,    25,    25,    25,    25,
      25,    25,    25,    41,    19,   143,   145,    34,   144,   146,
       3,   152,   153,     3,   209,   210,     3,   149,   150,     3,
     158,   159,     3,   206,   207,     3,   245,   246,     3,   308,
     309,     3,   176,   177,     3,   221,   222,     3,   224,   225,
       3,   170,   171,     3,   218,   219,    19,   254,   255,    19,
     305,   306,    19,   200,   201,    19,   248,   249,    34,   296,
     297,    19,   257,   258,    19,   212,   213,    19,   233,   234,
       3,   215,   216,     4,   161,   162,     3,   278,   279,    19,
     155,   156,     3,   182,   183,    34,   290,   291,    19,   191,
     192,    19,   167,   168,     3,   173,   174,    19,   251,   252,
       3,   242,   243,    19,   203,   204,     3,   263,   264,    19,
     260,   261,     3,   197,   198,    19,   266,   267,     3,   269,
     270,    19,   194,   195,    34,   287,   288,    19,   230,   231,
       3,   302,   303,    34,   293,   294,    19,   227,   228,     3,
     275,   276,    19,   236,   237,    19,   239,   240,    17,   188,
     189,    46,   281,   282,     3,   284,   285,     3,   299,   300,
       3,   185,   186,     3,   164,   165,     3,   179,   180,     3,
     272,   273,     3,   311,   312,     3,   314,   315,     3,   317,
     318,     3,   320,   321,     3,     4,     5,   346,    42,   140,
     147,   148,   154,   211,     4,    19,   151,   326,   160,   326,
     208,   247,    53,   310,   178,   223,   226,   172,   220,   256,
     307,   202,   326,   250,   298,   259,   214,   235,   217,   163,
     280,     4,    48,   157,   324,   184,   324,   292,   193,   324,
       4,    50,   169,   325,   175,   325,   253,   244,    19,   205,
      19,   265,    36,   262,    36,   199,   268,   271,   196,   289,
     232,   304,   295,   229,   277,   238,   239,   241,   190,   281,
     283,   286,   301,   187,   166,    44,   181,   328,   274,    44,
     313,   316,    44,   319,   340,   322,   139,    36,    41,    36,
      41,    41,    41,    36,    41,    36,    41,    36,    41,    36,
      41,    36,    41,    36,    41,    41,    41,    36,    41,    36,
      41,    36,    41,    36,    41,    36,    41,    41,    36,    41,
      41,    36,    41,    36,    41,    36,    41,    36,    41,    36,
      41,    36,    41,    36,    41,    36,    41,    36,    41,    36,
      41,    36,    41,    36,    41,    36,    41,    36,    41,    36,
      41,    36,    41,    36,    41,    36,    41,    36,    41,    36,
      41,    36,    41,    36,    41,    36,    41,    36,    41,    36,
      41,    36,    41,    41,    36,    41,    36,    41,     3,    41,
      36,    41,    36,    41,    41,    36,    41,    36,    41,    36,
      41,    36,    41,    36,    41,    36,    41,    41,    20,    40,
       6,    36,    36,    36,    36,    36,    36,   326,   326,   329,
      36,     3,    44,   341,   342,   343,    36,   323,   140,     8,
     347,   143,   145,   326,   326,    19,    19,    53,     3,    54,
      55,    56,    57,    58,    59,    60,     3,    19,    19,   326,
      19,     3,   346,    19,    15,     3,     4,    19,   324,   324,
      19,   324,   325,   325,    19,    19,    19,    19,    19,    19,
      19,    19,    12,   351,    19,     3,     3,    19,   346,   346,
       3,    27,   352,    36,     3,     3,     3,   328,     3,    44,
       3,     3,    21,    26,    43,     3,     3,    46,     3,     3,
       3,    36,    36,    45,    49,     3,    45,     4,    45,    36,
      52,    36,    38,    36,    36,    36,    36,    36,    36,    36,
      36,    36,    36,    36,    36,    36,    36,    36,    36,    36,
      36,    36,    36,    36,    36,    36,    38,    36,    36,    36,
      36,    36,    38,     3,    36,    36,    36,    36,     3,    36,
      36,    41,    36,     3,     3,     3,    36,   343,   346,     8,
       3,     3,     3,     3,    46,    16,     3,     3,    11,   345,
     347,    51,    51,     3,     3,    13,   353,     9,   349,     4,
      44,   344,   344,   324,    19,    19,     3,     3,   326,   326,
      12,   351,     3,     3,     3,     3,    27,    47,     3,   347,
     348,     3,     3,     3,    45,    44,   330,     3,     3,    47,
       4,    36,    36,     3,    38,    36,    38,    52,    36,    36,
      36,    36,    36,    36,    36,    36,    36,    36,    44,   331,
     332,   333,    36,    36,     3,     3,    47,    13,    10,   350,
       9,    36,     3,     3,   353,     3,     3,     3,     3,     3,
      14,     3,    28,   334,    45,    36,   340,     3,    36,    38,
      52,    36,    36,    36,    39,    66,    68,   333,   323,    36,
       3,    10,    45,     3,     3,   353,     4,    36,    36,     4,
      52,   327,     3,    36,    36,    29,   337,   337,    45,   346,
      39,    36,    36,     3,    33,   335,    32,   336,    39,    36,
      39,    36,     4,    30,   338,    19,   338,    39,    36,    36,
       3,    31,   339,   339,    39,    45,    45,     3
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   132,   133,   133,   133,   133,   133,   134,   135,   136,
     136,   137,   138,   139,   139,   139,   140,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   143,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   170,   170,   170,   170,
     170,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   323,   324,   324,   325,
     325,   326,   326,   327,   327,   328,   329,   329,   330,   331,
     331,   332,   332,   333,   333,   334,   334,   335,   336,   337,
     338,   339,   340,   341,   341,   342,   342,   343,   344,   344,
     345,   345,   346,   346,   346,   347,   347,   348,   348,   349,
     349,   350,   350,   351,   351,   352,   352,   353,   353,   353
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     3,     3,     3,     5,     7,     1,
       1,     4,     3,     2,     4,     5,     3,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     5,     3,
       1,     3,     0,     0,     5,     2,     3,     1,     1,     0,
       3,     2,     1,     7,     2,     5,    11,     1,     0,     7,
       1,     0,     3,     2,     1,     5,     5,     3,     3,     5,
       5,     5,     1,     0,     3,     2,     1,     5,     1,     0,
       5,     2,     3,     3,     2,     1,     1,     1,     0,     5,
       1,     0,     3,     2,     1,     3,     1,     0,     7,     3,
       4,     5,     2,     3,     5,     2,     3,     5,     1,     0,
       1,     1,     0,     3,     1,     0,     3,     1,     0,     3,
       1,     0,     1,     1,     0,     1,     1,     0,     3,     1,
       0,     5,     1,     0,     3,     1,     0,     1,     1,     0,
       3,     0,     1,     9,     1,     0,     7,     1,     0,     1,
       1,     0,     9,     1,     0,     3,     1,     0,     1,     1,
       0,     7,     3,     4,     5,     2,     3,     3,     1,     0,
       3,     1,     0,     7,     1,     0,     3,     1,     0,     7,
       7,     1,     0,     5,     0,     1,     3,     1,     0,     5,
       1,     0,     7,     1,     0,     9,     1,     0,     5,     1,
       0,     5,     1,     0,     5,     1,     0,     3,     1,     0,
       7,     2,     5,     5,     2,     3,     5,     1,     0,     9,
       6,     3,     1,     1,     0,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     2,     3,     0,
       1,     1,     3,    12,    12,     0,     4,     3,     3,     3,
       3,     3,     3,     0,     1,     1,     3,     9,     5,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (state, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, state); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, struct parser_state* state)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (state);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, struct parser_state* state)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, state);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, struct parser_state* state)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              , state);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, state); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, struct parser_state* state)
{
  YYUSE (yyvaluep);
  YYUSE (state);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (struct parser_state* state)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 430 "strace-parse.y" /* yacc.c:1646  */
    {
	struct event* ev = xmalloc(sizeof(struct event));
	*ev = (yyvsp[-1].ev);

	ev->traceline = state->linenum++;
	ev->subtype = ST_CALL;

	append_event(ev, state);
}
#line 2318 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 439 "strace-parse.y" /* yacc.c:1646  */
    {
	struct event* ev = xmalloc(sizeof(*ev));
	*ev = (yyvsp[-1].ev);

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
#line 2340 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 456 "strace-parse.y" /* yacc.c:1646  */
    {
	nstime_t tdiff;
	struct event* entry = thread_evdelete((yyvsp[-1].ev).thread);
	if (!entry) {
		fprintf(stderr, "malformed trace file: no entry found for "
		        "call completion on line %d\n", state->linenum);
		exit(1);
	}

	assert((yyvsp[-1].ev).thread == entry->thread);

	entry->time.exit = (yyvsp[-1].ev).time.exit;
	tdiff = (yyvsp[-1].ev).time.entry - entry->time.entry;
	tdiff = tdiff < 0 ? -tdiff : tdiff;
	if (tdiff > 1000000)
		fprintf(stderr, "Warning: strace internal timing inaccuracy > 1ms"
		        " (%"PRInst" ns at line %d)\n", tdiff, state->linenum);

	merge_event(entry, &(yyvsp[-1].ev));
	entry->ret = (yyvsp[-1].ev).ret;

	if (entry->type != EV_IO_SUBMIT && entry->type != EV_CLOSE)
		append_event(entry, state);

	state->linenum++;
}
#line 2371 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 482 "strace-parse.y" /* yacc.c:1646  */
    {
	state->linenum++;
}
#line 2379 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 486 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.ev).thread = (yyvsp[-4].i);
	(yyval.ev).time.entry = (yyvsp[-3].i);
	(yyval.ev).type = (yyvsp[-2].ev).type;
	(yyval.ev).attrs = (yyvsp[-2].ev).attrs;
	set_evret(&(yyval.ev), (yyvsp[-1].i));
	(yyval.ev).time.exit = (yyval.ev).time.entry + (yyvsp[0].i);
}
#line 2392 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 495 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.ev).thread = (yyvsp[-6].i);
	(yyval.ev).time.exit = (yyvsp[-5].i);
	(yyval.ev).type = (yyvsp[-3].ev).type;
	(yyval.ev).attrs = (yyvsp[-3].ev).attrs;
	set_evret(&(yyval.ev), (yyvsp[-1].i));
	(yyval.ev).time.entry = (yyval.ev).time.exit - (yyvsp[0].i);
}
#line 2405 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 504 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 2411 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 505 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 2417 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 507 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.ev).type = (yyvsp[-1].ev).type;
	(yyval.ev).attrs = (yyvsp[-1].ev).attrs;
	(yyval.ev).thread = (yyvsp[-3].i);
	(yyval.ev).time.entry = (yyvsp[-2].i);
}
#line 2428 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 514 "strace-parse.y" /* yacc.c:1646  */
    {
}
#line 2435 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 517 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.i) = (yyvsp[0].i);
}
#line 2443 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 520 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.i) = (yyvsp[-2].i);
}
#line 2451 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 523 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.i) = (yyvsp[-3].i);
}
#line 2459 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 527 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.i) = (yyvsp[-1].i);
}
#line 2467 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 533 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(OPEN, open, (yyvsp[-1].open)); }
#line 2473 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 534 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(READ, read, (yyvsp[-1].read)); }
#line 2479 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 535 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(CLOSE, close, (yyvsp[-1].close)); }
#line 2485 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 536 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(STAT, stat, (yyvsp[-1].stat)); }
#line 2491 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 537 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(PREAD, pread, (yyvsp[-1].pread)); }
#line 2497 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 538 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(MMAP, mmap, (yyvsp[-1].mmap)); }
#line 2503 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 539 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FADVISE64, fadvise64, (yyvsp[-1].fadvise64)); }
#line 2509 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 540 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(STATFS, statfs, (yyvsp[-1].statfs)); }
#line 2515 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 541 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FCNTL, fcntl, (yyvsp[-1].fcntl)); }
#line 2521 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 542 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FSTATFS, fstatfs, (yyvsp[-1].fstatfs)); }
#line 2527 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 543 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(LSEEK, lseek, (yyvsp[-1].lseek)); }
#line 2533 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 544 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(GETDENTS, getdents, (yyvsp[-1].getdents)); }
#line 2539 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 545 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FSTAT, fstat, (yyvsp[-1].fstat)); }
#line 2545 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 546 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(EPOLL_CREATE, epoll_create, (yyvsp[-1].epoll_create)); }
#line 2551 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 547 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(LSTAT, lstat, (yyvsp[-1].lstat)); }
#line 2557 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 548 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(ACCESS, access, (yyvsp[-1].access)); }
#line 2563 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 549 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FGETXATTR, fgetxattr, (yyvsp[-1].fgetxattr)); }
#line 2569 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 550 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(READLINK, readlink, (yyvsp[-1].readlink)); }
#line 2575 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 551 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(LISTXATTR, listxattr, (yyvsp[-1].listxattr)); }
#line 2581 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 552 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(WRITE, write, (yyvsp[-1].write)); }
#line 2587 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 553 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(DUP, dup, (yyvsp[-1].dup)); }
#line 2593 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 554 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(MKDIR, mkdir, (yyvsp[-1].mkdir)); }
#line 2599 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 555 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FLOCK, flock, (yyvsp[-1].flock)); }
#line 2605 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 556 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FTRUNCATE, ftruncate, (yyvsp[-1].ftruncate)); }
#line 2611 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 557 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FSYNC, fsync, (yyvsp[-1].fsync)); }
#line 2617 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 558 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FDATASYNC, fdatasync, (yyvsp[-1].fdatasync)); }
#line 2623 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 559 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(CHMOD, chmod, (yyvsp[-1].chmod)); }
#line 2629 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 560 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(CHOWN, chown, (yyvsp[-1].chown)); }
#line 2635 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 561 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(RENAME, rename, (yyvsp[-1].rename)); }
#line 2641 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 562 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(CHDIR, chdir, (yyvsp[-1].chdir)); }
#line 2647 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 563 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(GETCWD, getcwd, (yyvsp[-1].getcwd)); }
#line 2653 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 564 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FSETXATTR, fsetxattr, (yyvsp[-1].fsetxattr)); }
#line 2659 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 565 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(PWRITE, pwrite, (yyvsp[-1].pwrite)); }
#line 2665 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 566 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(UNLINK, unlink, (yyvsp[-1].unlink)); }
#line 2671 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 567 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(SETXATTR, setxattr, (yyvsp[-1].setxattr)); }
#line 2677 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 568 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(LINK, link, (yyvsp[-1].link)); }
#line 2683 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 569 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(RMDIR, rmdir, (yyvsp[-1].rmdir)); }
#line 2689 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 570 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(GETXATTR, getxattr, (yyvsp[-1].getxattr)); }
#line 2695 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 571 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FLISTXATTR, flistxattr, (yyvsp[-1].flistxattr)); }
#line 2701 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 572 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(REMOVEXATTR, removexattr, (yyvsp[-1].removexattr)); }
#line 2707 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 573 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FREMOVEXATTR, fremovexattr, (yyvsp[-1].fremovexattr)); }
#line 2713 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 574 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FALLOCATE, fallocate, (yyvsp[-1].fallocate)); }
#line 2719 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 575 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FCHMOD, fchmod, (yyvsp[-1].fchmod)); }
#line 2725 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 576 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(UTIMENSAT, utimensat, (yyvsp[-1].utimensat)); }
#line 2731 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 577 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FCHOWN, fchown, (yyvsp[-1].fchown)); }
#line 2737 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 578 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(SYMLINK, symlink, (yyvsp[-1].symlink)); }
#line 2743 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 580 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(DUP, dup, (yyvsp[-1].dup)); }
#line 2749 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 581 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(IO_SETUP, io_setup, (yyvsp[-1].io_setup)); }
#line 2755 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 582 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(IO_SUBMIT, io_submit, (yyvsp[-1].io_submit)); }
#line 2761 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 583 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(IO_GETEVENTS, io_getevents, (yyvsp[-1].io_getevents)); }
#line 2767 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 584 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(IO_DESTROY, io_destroy, (yyvsp[-1].io_destroy)); }
#line 2773 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 586 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(OPEN, open, (yyvsp[-1].open)); }
#line 2779 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 587 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(SOCKET, socket, (yyvsp[-1].socket)); }
#line 2785 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 588 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(PIPE, pipe, (yyvsp[-1].pipe)); }
#line 2791 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 589 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(DUP2, dup2, (yyvsp[-1].dup2)); }
#line 2797 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 590 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FACCESSAT, faccessat, (yyvsp[-1].faccessat)); }
#line 2803 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 591 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(NEWFSTATAT, newfstatat, (yyvsp[-1].newfstatat)); }
#line 2809 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 592 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FCHOWNAT, fchownat, (yyvsp[-1].fchownat)); }
#line 2815 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 593 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(UNLINKAT, unlinkat, (yyvsp[-1].unlinkat)); }
#line 2821 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 595 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(DUP2, dup2, (yyvsp[-1].dup2)); }
#line 2827 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 598 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(STAT, stat, (yyvsp[0].stat)); }
#line 2833 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 599 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(EPOLL_CREATE, epoll_create, (yyvsp[0].epoll_create)); }
#line 2839 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 600 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(OPEN, open, (yyvsp[0].open)); }
#line 2845 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 601 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FCNTL, fcntl, (yyvsp[0].fcntl)); }
#line 2851 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 602 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FSTATFS, fstatfs, (yyvsp[0].fstatfs)); }
#line 2857 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 603 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(DUP, dup, (yyvsp[0].dup)); }
#line 2863 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 604 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(LSEEK, lseek, (yyvsp[0].lseek)); }
#line 2869 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 605 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(CLOSE, close, (yyvsp[0].close)); }
#line 2875 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 606 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(GETDENTS, getdents, (yyvsp[0].getdents)); }
#line 2881 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 607 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(READ, read, (yyvsp[0].read)); }
#line 2887 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 608 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(WRITE, write, (yyvsp[0].write)); }
#line 2893 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 609 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(MMAP, mmap, (yyvsp[0].mmap)); }
#line 2899 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 610 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FSTAT, fstat, (yyvsp[0].fstat)); }
#line 2905 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 611 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(LSTAT, lstat, (yyvsp[0].lstat)); }
#line 2911 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 612 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(PREAD, pread, (yyvsp[0].pread)); }
#line 2917 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 613 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(ACCESS, access, (yyvsp[0].access)); }
#line 2923 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 614 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FGETXATTR, fgetxattr, (yyvsp[0].fgetxattr)); }
#line 2929 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 615 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(STATFS, statfs, (yyvsp[0].statfs)); }
#line 2935 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 616 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FLOCK, flock, (yyvsp[0].flock)); }
#line 2941 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 617 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(READLINK, readlink, (yyvsp[0].readlink)); }
#line 2947 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 618 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FADVISE64, fadvise64, (yyvsp[0].fadvise64)); }
#line 2953 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 619 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(LISTXATTR, listxattr, (yyvsp[0].listxattr)); }
#line 2959 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 620 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(CHDIR, chdir, (yyvsp[0].chdir)); }
#line 2965 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 621 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(GETCWD, getcwd, (yyvsp[0].getcwd)); }
#line 2971 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 622 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FSETXATTR, fsetxattr, (yyvsp[0].fsetxattr)); }
#line 2977 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 623 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(PWRITE, pwrite, (yyvsp[0].pwrite)); }
#line 2983 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 624 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(UNLINK, unlink, (yyvsp[0].unlink)); }
#line 2989 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 625 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FSYNC, fsync, (yyvsp[0].fsync)); }
#line 2995 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 626 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FDATASYNC, fdatasync, (yyvsp[0].fdatasync)); }
#line 3001 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 627 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(SETXATTR, setxattr, (yyvsp[0].setxattr)); }
#line 3007 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 628 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(LINK, link, (yyvsp[0].link)); }
#line 3013 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 629 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(CHMOD, chmod, (yyvsp[0].chmod)); }
#line 3019 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 630 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(CHOWN, chown, (yyvsp[0].chown)); }
#line 3025 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 631 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(RENAME, rename, (yyvsp[0].rename)); }
#line 3031 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 632 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FTRUNCATE, ftruncate, (yyvsp[0].ftruncate)); }
#line 3037 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 633 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(RMDIR, rmdir, (yyvsp[0].rmdir)); }
#line 3043 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 634 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(GETXATTR, getxattr, (yyvsp[0].getxattr)); }
#line 3049 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 635 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(MKDIR, mkdir, (yyvsp[0].mkdir)); }
#line 3055 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 636 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FLISTXATTR, flistxattr, (yyvsp[0].flistxattr)); }
#line 3061 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 637 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(REMOVEXATTR, removexattr, (yyvsp[0].removexattr)); }
#line 3067 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 638 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FREMOVEXATTR, fremovexattr, (yyvsp[0].fremovexattr)); }
#line 3073 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 639 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FALLOCATE, fallocate, (yyvsp[0].fallocate)); }
#line 3079 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 640 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FCHMOD, fchmod, (yyvsp[0].fchmod)); }
#line 3085 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 641 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(UTIMENSAT, utimensat, (yyvsp[0].utimensat)); }
#line 3091 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 642 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FCHOWN, fchown, (yyvsp[0].fchown)); }
#line 3097 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 643 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(SYMLINK, symlink, (yyvsp[0].symlink)); }
#line 3103 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 645 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(DUP, dup, (yyvsp[0].dup)); }
#line 3109 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 646 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(IO_SETUP, io_setup, (yyvsp[0].io_setup)); }
#line 3115 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 647 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(IO_SUBMIT, io_submit, (yyvsp[0].io_submit)); }
#line 3121 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 648 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(IO_GETEVENTS, io_getevents, (yyvsp[0].io_getevents)); }
#line 3127 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 649 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(IO_DESTROY, io_destroy, (yyvsp[0].io_destroy)); }
#line 3133 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 651 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(OPEN, open, (yyvsp[0].open)); }
#line 3139 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 652 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(SOCKET, socket, (yyvsp[0].socket)); }
#line 3145 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 653 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(PIPE, pipe, (yyvsp[0].pipe)); }
#line 3151 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 654 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(DUP2, dup2, (yyvsp[0].dup2)); }
#line 3157 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 655 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FACCESSAT, faccessat, (yyvsp[0].faccessat)); }
#line 3163 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 656 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(NEWFSTATAT, newfstatat, (yyvsp[0].newfstatat)); }
#line 3169 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 657 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FCHOWNAT, fchownat, (yyvsp[0].fchownat)); }
#line 3175 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 658 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(UNLINKAT, unlinkat, (yyvsp[0].unlinkat)); }
#line 3181 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 660 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(DUP2, dup2, (yyvsp[0].dup2)); }
#line 3187 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 663 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(STAT, stat, (yyvsp[0].stat)); }
#line 3193 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 664 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(EPOLL_CREATE, epoll_create, (yyvsp[0].epoll_create)); }
#line 3199 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 665 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(OPEN, open, (yyvsp[0].open)); }
#line 3205 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 666 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FCNTL, fcntl, (yyvsp[0].fcntl)); }
#line 3211 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 667 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FSTATFS, fstatfs, (yyvsp[0].fstatfs)); }
#line 3217 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 668 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(DUP, dup, (yyvsp[0].dup)); }
#line 3223 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 669 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(LSEEK, lseek, (yyvsp[0].lseek)); }
#line 3229 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 670 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(CLOSE, close, (yyvsp[0].close)); }
#line 3235 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 671 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(GETDENTS, getdents, (yyvsp[0].getdents)); }
#line 3241 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 672 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(READ, read, (yyvsp[0].read)); }
#line 3247 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 673 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(WRITE, write, (yyvsp[0].write)); }
#line 3253 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 674 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(MMAP, mmap, (yyvsp[0].mmap)); }
#line 3259 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 675 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FSTAT, fstat, (yyvsp[0].fstat)); }
#line 3265 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 676 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(LSTAT, lstat, (yyvsp[0].lstat)); }
#line 3271 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 677 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(PREAD, pread, (yyvsp[0].pread)); }
#line 3277 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 678 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(ACCESS, access, (yyvsp[0].access)); }
#line 3283 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 679 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FGETXATTR, fgetxattr, (yyvsp[0].fgetxattr)); }
#line 3289 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 680 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(STATFS, statfs, (yyvsp[0].statfs)); }
#line 3295 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 681 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FLOCK, flock, (yyvsp[0].flock)); }
#line 3301 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 682 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(READLINK, readlink, (yyvsp[0].readlink)); }
#line 3307 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 683 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FADVISE64, fadvise64, (yyvsp[0].fadvise64)); }
#line 3313 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 684 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(LISTXATTR, listxattr, (yyvsp[0].listxattr)); }
#line 3319 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 685 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(CHDIR, chdir, (yyvsp[0].chdir)); }
#line 3325 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 686 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(GETCWD, getcwd, (yyvsp[0].getcwd)); }
#line 3331 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 687 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FSETXATTR, fsetxattr, (yyvsp[0].fsetxattr)); }
#line 3337 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 688 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(PWRITE, pwrite, (yyvsp[0].pwrite)); }
#line 3343 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 689 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(UNLINK, unlink, (yyvsp[0].unlink)); }
#line 3349 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 690 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FSYNC, fsync, (yyvsp[0].fsync)); }
#line 3355 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 691 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FDATASYNC, fdatasync, (yyvsp[0].fdatasync)); }
#line 3361 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 692 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(SETXATTR, setxattr, (yyvsp[0].setxattr)); }
#line 3367 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 693 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(LINK, link, (yyvsp[0].link)); }
#line 3373 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 168:
#line 694 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(CHMOD, chmod, (yyvsp[0].chmod)); }
#line 3379 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 695 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(CHOWN, chown, (yyvsp[0].chown)); }
#line 3385 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 696 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(RENAME, rename, (yyvsp[0].rename)); }
#line 3391 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 697 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FTRUNCATE, ftruncate, (yyvsp[0].ftruncate)); }
#line 3397 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 698 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(RMDIR, rmdir, (yyvsp[0].rmdir)); }
#line 3403 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 699 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(GETXATTR, getxattr, (yyvsp[0].getxattr)); }
#line 3409 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 700 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(MKDIR, mkdir, (yyvsp[0].mkdir)); }
#line 3415 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 701 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FLISTXATTR, flistxattr, (yyvsp[0].flistxattr)); }
#line 3421 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 702 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(REMOVEXATTR, removexattr, (yyvsp[0].removexattr)); }
#line 3427 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 703 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FREMOVEXATTR, fremovexattr, (yyvsp[0].fremovexattr)); }
#line 3433 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 704 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FALLOCATE, fallocate, (yyvsp[0].fallocate)); }
#line 3439 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 705 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FCHMOD, fchmod, (yyvsp[0].fchmod)); }
#line 3445 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 706 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(UTIMENSAT, utimensat, (yyvsp[0].utimensat)); }
#line 3451 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 707 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FCHOWN, fchown, (yyvsp[0].fchown)); }
#line 3457 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 708 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(SYMLINK, symlink, (yyvsp[0].symlink)); }
#line 3463 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 183:
#line 710 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(DUP, dup, (yyvsp[0].dup)); }
#line 3469 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 711 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(IO_SETUP, io_setup, (yyvsp[0].io_setup)); }
#line 3475 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 712 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(IO_SUBMIT, io_submit, (yyvsp[0].io_submit)); }
#line 3481 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 186:
#line 713 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(IO_GETEVENTS, io_getevents, (yyvsp[0].io_getevents)); }
#line 3487 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 187:
#line 714 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(IO_DESTROY, io_destroy, (yyvsp[0].io_destroy)); }
#line 3493 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 716 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(OPEN, open, (yyvsp[0].open)); }
#line 3499 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 717 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(SOCKET, socket, (yyvsp[0].socket)); }
#line 3505 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 718 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(PIPE, pipe, (yyvsp[0].pipe)); }
#line 3511 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 191:
#line 719 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(DUP2, dup2, (yyvsp[0].dup2)); }
#line 3517 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 192:
#line 720 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FACCESSAT, faccessat, (yyvsp[0].faccessat)); }
#line 3523 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 193:
#line 721 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(NEWFSTATAT, newfstatat, (yyvsp[0].newfstatat)); }
#line 3529 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 194:
#line 722 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(FCHOWNAT, fchownat, (yyvsp[0].fchownat)); }
#line 3535 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 195:
#line 723 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(UNLINKAT, unlinkat, (yyvsp[0].unlinkat)); }
#line 3541 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 196:
#line 725 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct(DUP2, dup2, (yyvsp[0].dup2)); }
#line 3547 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 197:
#line 730 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.open));
	(yyval.open).path = getpath(&(yyvsp[-2].strlit), "open", state);
	(yyval.open).flags = (yyvsp[0].flags);
}
#line 3557 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 198:
#line 735 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.open));
	(yyval.open).path = getpath(&(yyvsp[-4].strlit), "open", state);
	(yyval.open).flags = (yyvsp[-2].flags);
	(yyval.open).mode = (yyvsp[0].i);
}
#line 3568 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 199:
#line 742 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.open) = (yyvsp[0].open);
}
#line 3576 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 200:
#line 746 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.open));
	(yyval.open) = (yyvsp[0].open);
}
#line 3585 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 201:
#line 751 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.open) = (yyvsp[0].open);
}
#line 3593 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 202:
#line 755 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 3599 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 203:
#line 757 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 3605 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 204:
#line 761 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.read));
	(yyval.read).fd = FD((yyvsp[-4].i));
	(yyval.read).count = (yyvsp[0].i);

	(yyval.read).dummy1 = (yyvsp[-2].str);
}
#line 3617 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 205:
#line 769 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.read));
	(yyval.read).fd = FD((yyvsp[-1].i));
}
#line 3626 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 206:
#line 774 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.read));
	(yyval.read).count = (yyvsp[0].i);

	(yyval.read).dummy1 = (yyvsp[-2].str);
}
#line 3637 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 207:
#line 783 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.close));
	(yyval.close).fd = FD((yyvsp[0].i));
}
#line 3646 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 208:
#line 788 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.close));
	(yyval.close) = (yyvsp[0].close);
}
#line 3655 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 209:
#line 793 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 3661 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 210:
#line 797 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.stat));
	(yyval.stat).path = getpath(&(yyvsp[-2].strlit), "stat", state);
}
#line 3670 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 211:
#line 802 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.stat));
	(yyval.stat).path = getpath(&(yyvsp[-1].strlit), "stat", state);
}
#line 3679 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 212:
#line 807 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.stat));
}
#line 3687 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 213:
#line 813 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.pread));
	(yyval.pread).fd = FD((yyvsp[-6].i));
	(yyval.pread).count = (yyvsp[-2].i);
	(yyval.pread).offset = (yyvsp[0].i);

	(yyval.pread).dummy1 = (yyvsp[-4].str);
}
#line 3700 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 214:
#line 822 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.pread));
	(yyval.pread).fd = FD((yyvsp[-1].i));
}
#line 3709 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 215:
#line 827 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.pread));
	(yyval.pread).count = (yyvsp[-2].i);
	(yyval.pread).offset = (yyvsp[0].i);

	(yyval.pread).dummy1 = (yyvsp[-4].str);
}
#line 3721 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 216:
#line 837 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.mmap));
	(yyval.mmap).addr = (void*)(intptr_t)(yyvsp[-10].i);
	(yyval.mmap).length = (yyvsp[-8].i);
	(yyval.mmap).prot = (yyvsp[-6].flags);
	(yyval.mmap).flags = (yyvsp[-4].flags);
	(yyval.mmap).fd = FD((yyvsp[-2].i));
	(yyval.mmap).offset = (yyvsp[0].i);
}
#line 3735 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 217:
#line 847 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.mmap));
	(yyval.mmap) = (yyvsp[0].mmap);
}
#line 3744 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 218:
#line 852 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 3750 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 219:
#line 856 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fadvise64));
	(yyval.fadvise64).fd = FD((yyvsp[-6].i));
	(yyval.fadvise64).offset = (yyvsp[-4].i);
	(yyval.fadvise64).len = (yyvsp[-2].i);
	(yyval.fadvise64).advice = (yyvsp[0].str);
}
#line 3762 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 220:
#line 864 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fadvise64));
	(yyval.fadvise64) = (yyvsp[0].fadvise64);
}
#line 3771 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 221:
#line 869 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 3777 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 222:
#line 873 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.statfs));
	(yyval.statfs).path = getpath(&(yyvsp[-2].strlit), "statfs", state);
}
#line 3786 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 223:
#line 878 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.statfs));
	(yyval.statfs).path = getpath(&(yyvsp[-1].strlit), "statfs", state);
}
#line 3795 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 224:
#line 883 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.statfs));
}
#line 3803 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 225:
#line 889 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fcntl));
	(yyval.fcntl).fd = FD((yyvsp[-4].i));
	(yyval.fcntl).cmdname = xstrdup("F_DUPFD");
	(yyval.fcntl).arg.l = (yyvsp[0].i);
}
#line 3814 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 226:
#line 895 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fcntl));
	(yyval.fcntl).fd = FD((yyvsp[-4].i));
	(yyval.fcntl).cmdname = xstrdup("F_SETFD");
	/* FIXME: this is brittle. */
	(yyval.fcntl).arg.l = (!(yyvsp[0].str) || strcmp((yyvsp[0].str), "FD_CLOEXEC")) ? 0 : FD_CLOEXEC;
}
#line 3826 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 227:
#line 902 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fcntl));
	(yyval.fcntl).fd = FD((yyvsp[-2].i));
	(yyval.fcntl).cmdname = xstrdup("F_GETFD");
	(yyval.fcntl).arg.l = -1;
}
#line 3837 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 228:
#line 908 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fcntl));
	(yyval.fcntl).fd = FD((yyvsp[-2].i));
	(yyval.fcntl).cmdname = xstrdup("F_GETFL");
	(yyval.fcntl).arg.l = -1;
}
#line 3848 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 229:
#line 914 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fcntl));
	(yyval.fcntl).fd = FD((yyvsp[-4].i));
	(yyval.fcntl).cmdname = xstrdup("F_SETFL");
	(yyval.fcntl).arg.flags = (yyvsp[0].flags);
}
#line 3859 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 230:
#line 920 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fcntl));
	(yyval.fcntl).fd = FD((yyvsp[-4].i));
	(yyval.fcntl).cmdname = xstrdup("F_SETLK");
	(yyval.fcntl).arg.l = 0; /* FIXME */
}
#line 3870 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 231:
#line 926 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fcntl));
	(yyval.fcntl).fd = FD((yyvsp[-4].i));
	(yyval.fcntl).cmdname = xstrdup("F_SETLKW");
	(yyval.fcntl).arg.l = 0; /* FIXME */
}
#line 3881 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 232:
#line 933 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fcntl));
	(yyval.fcntl) = (yyvsp[0].fcntl);
}
#line 3890 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 233:
#line 938 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 3896 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 234:
#line 942 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fstatfs));
	(yyval.fstatfs).fd = FD((yyvsp[-2].i));
}
#line 3905 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 235:
#line 947 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fstatfs));
	(yyval.fstatfs).fd = FD((yyvsp[-1].i));
}
#line 3914 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 236:
#line 952 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fstatfs));
}
#line 3922 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 237:
#line 958 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.lseek));
	(yyval.lseek).fd = FD((yyvsp[-4].i));
	(yyval.lseek).offset = (yyvsp[-2].i);
	(yyval.lseek).whence = match_whence((yyvsp[0].str), state);
}
#line 3933 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 238:
#line 965 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.lseek));
	(yyval.lseek) = (yyvsp[0].lseek);
}
#line 3942 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 239:
#line 970 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 3948 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 240:
#line 974 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.getdents));
	(yyval.getdents).fd = FD((yyvsp[-4].i));
	(yyval.getdents).count = (yyvsp[0].i);
}
#line 3958 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 241:
#line 980 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.getdents));
	(yyval.getdents).fd = FD((yyvsp[-1].i));
}
#line 3967 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 242:
#line 985 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.getdents));
	(yyval.getdents).count = (yyvsp[0].i);
}
#line 3976 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 243:
#line 992 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fstat));
	(yyval.fstat).fd = FD((yyvsp[-2].i));
}
#line 3985 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 244:
#line 997 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fstat));
	(yyval.fstat).fd = FD((yyvsp[-1].i));
}
#line 3994 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 245:
#line 1002 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fstat));
}
#line 4002 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 246:
#line 1008 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.epoll_create));
	(yyval.epoll_create).size = (yyvsp[0].i);
}
#line 4011 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 247:
#line 1013 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.epoll_create));
	(yyval.epoll_create) = (yyvsp[0].epoll_create);
}
#line 4020 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 248:
#line 1018 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4026 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 249:
#line 1022 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.socket));
	(yyval.socket).domain = -1;
	(yyval.socket).type = -1;
	(yyval.socket).protocol = -1;
}
#line 4037 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 250:
#line 1029 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.socket));
	(yyval.socket) = (yyvsp[0].socket);
}
#line 4046 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 251:
#line 1034 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4052 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 252:
#line 1038 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.lstat));
	(yyval.lstat).path = getpath(&(yyvsp[-2].strlit), "lstat", state);
}
#line 4061 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 253:
#line 1043 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.lstat));
	(yyval.lstat).path = getpath(&(yyvsp[-1].strlit), "lstat", state);
}
#line 4070 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 254:
#line 1048 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.lstat));
}
#line 4078 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 255:
#line 1054 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.access));
	(yyval.access).pathname = getpath(&(yyvsp[-2].strlit), "access", state);
	(yyval.access).flags = (yyvsp[0].flags);
}
#line 4088 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 256:
#line 1060 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.access));
	(yyval.access) = (yyvsp[0].access);
}
#line 4097 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 257:
#line 1065 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4103 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 258:
#line 1069 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fgetxattr));
	(yyval.fgetxattr).fd = FD((yyvsp[-6].i));
	(yyval.fgetxattr).name = getstr(&(yyvsp[-4].strlit), "fgetxattr", state);
	(yyval.fgetxattr).size = (yyvsp[0].i);

	(yyval.fgetxattr).dummy1 = (yyvsp[-2].str);
}
#line 4116 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 259:
#line 1078 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fgetxattr));
	(yyval.fgetxattr).fd = FD((yyvsp[-2].i));
	(yyval.fgetxattr).name = getstr(&(yyvsp[0].strlit), "fgetxattr", state);
}
#line 4126 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 260:
#line 1084 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fgetxattr));
	(yyval.fgetxattr).size = (yyvsp[0].i);

	(yyval.fgetxattr).dummy1 = (yyvsp[-2].str);
}
#line 4137 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 261:
#line 1093 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.readlink));
	(yyval.readlink).path = getpath(&(yyvsp[-4].strlit), "readlink", state);
	(yyval.readlink).bufsiz = (yyvsp[0].i);

	(yyval.readlink).dummy1 = (yyvsp[-2].str);
}
#line 4149 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 262:
#line 1101 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.readlink));
	(yyval.readlink).path = getpath(&(yyvsp[-1].strlit), "readlink", state);
}
#line 4158 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 263:
#line 1106 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.readlink));
	(yyval.readlink).bufsiz = (yyvsp[0].i);

	(yyval.readlink).dummy1 = (yyvsp[-2].str);
}
#line 4169 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 264:
#line 1115 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.listxattr));
	(yyval.listxattr).path = getpath(&(yyvsp[-4].strlit), "listxattr", state);
	(yyval.listxattr).size = (yyvsp[0].i);

	(yyval.listxattr).dummy1 = (yyvsp[-2].strlit).s;
}
#line 4181 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 265:
#line 1123 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.listxattr));
	(yyval.listxattr).path = getpath(&(yyvsp[-1].strlit), "listxattr", state);
}
#line 4190 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 266:
#line 1128 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.listxattr));
	(yyval.listxattr).size = (yyvsp[0].i);

	(yyval.listxattr).dummy1 = (yyvsp[-2].strlit).s;
}
#line 4201 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 267:
#line 1137 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.write));
	(yyval.write).fd = FD((yyvsp[-4].i));
	(yyval.write).count = (yyvsp[0].i);

	(yyval.write).dummy1 = (yyvsp[-2].strlit).s;
}
#line 4213 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 268:
#line 1145 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.write));
	(yyval.write) = (yyvsp[0].write);
}
#line 4222 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 269:
#line 1150 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4228 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 270:
#line 1154 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.dup));
	(yyval.dup).fd = FD((yyvsp[0].i));
}
#line 4237 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 271:
#line 1159 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.dup));
	(yyval.dup) = (yyvsp[0].dup);
}
#line 4246 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 272:
#line 1164 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4252 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 273:
#line 1168 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.mkdir));
	(yyval.mkdir).pathname = getpath(&(yyvsp[-2].strlit), "mkdir", state);
	(yyval.mkdir).mode = (yyvsp[0].i);
}
#line 4262 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 274:
#line 1174 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.mkdir));
	(yyval.mkdir) = (yyvsp[0].mkdir);
}
#line 4271 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 275:
#line 1179 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4277 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 276:
#line 1183 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.flock));
	(yyval.flock).fd = FD((yyvsp[-2].i));
	(yyval.flock).operation = match_flockop((yyvsp[0].str), state);
}
#line 4287 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 277:
#line 1189 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.flock));
	(yyval.flock) = (yyvsp[0].flock);
}
#line 4296 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 278:
#line 1194 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4302 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 279:
#line 1198 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.ftruncate));
	(yyval.ftruncate).fd = FD((yyvsp[-2].i));
	(yyval.ftruncate).length = (yyvsp[0].i);
}
#line 4312 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 280:
#line 1204 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.ftruncate));
	(yyval.ftruncate) = (yyvsp[0].ftruncate);
}
#line 4321 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 281:
#line 1209 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4327 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 282:
#line 1213 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fsync));
	(yyval.fsync).fd = FD((yyvsp[0].i));
}
#line 4336 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 283:
#line 1218 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fsync));
	(yyval.fsync) = (yyvsp[0].fsync);
}
#line 4345 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 284:
#line 1223 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4351 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 285:
#line 1227 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fdatasync));
	(yyval.fdatasync).fd = FD((yyvsp[0].i));
}
#line 4360 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 286:
#line 1232 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fdatasync));
	(yyval.fdatasync) = (yyvsp[0].fdatasync);
}
#line 4369 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 287:
#line 1237 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4375 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 288:
#line 1241 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.chmod));
	(yyval.chmod).path = getpath(&(yyvsp[-2].strlit), "chmod", state);
	(yyval.chmod).mode = (yyvsp[0].i);
}
#line 4385 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 289:
#line 1247 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.chmod));
	(yyval.chmod) = (yyvsp[0].chmod);
}
#line 4394 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 290:
#line 1252 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4400 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 291:
#line 1256 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.chown));
	(yyval.chown).path = getpath(&(yyvsp[-4].strlit), "chown", state);
}
#line 4409 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 292:
#line 1261 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.chown));
	(yyval.chown) = (yyvsp[0].chown);
}
#line 4418 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 293:
#line 1266 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4424 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 294:
#line 1270 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.rename));
	(yyval.rename).oldpath = getpath(&(yyvsp[-2].strlit), "rename", state);
	(yyval.rename).newpath = getpath(&(yyvsp[0].strlit), "rename", state);
}
#line 4434 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 295:
#line 1276 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.rename));
	(yyval.rename) = (yyvsp[0].rename);
}
#line 4443 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 296:
#line 1281 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4449 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 297:
#line 1285 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.chdir));
	(yyval.chdir).path = getpath(&(yyvsp[0].strlit), "chdir", state);
}
#line 4458 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 298:
#line 1290 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.chdir));
	(yyval.chdir) = (yyvsp[0].chdir);
}
#line 4467 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 299:
#line 1295 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4473 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 300:
#line 1299 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.getcwd));
	(yyval.getcwd).size = (yyvsp[0].i);

	(yyval.getcwd).dummy1 = (yyvsp[-2].strlit).s;
}
#line 4484 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 301:
#line 1310 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4490 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 302:
#line 1312 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.getcwd));
	(yyval.getcwd) = (yyvsp[0].getcwd);
}
#line 4499 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 303:
#line 1319 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fsetxattr));
	(yyval.fsetxattr).fd = FD((yyvsp[-8].i));
	(yyval.fsetxattr).name = getstr(&(yyvsp[-6].strlit), "fsetxattr", state);
	(yyval.fsetxattr).size = (yyvsp[-2].i);

	(yyval.fsetxattr).dummy1 = (yyvsp[-4].strlit).s;
}
#line 4512 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 304:
#line 1328 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fsetxattr));
	(yyval.fsetxattr) = (yyvsp[0].fsetxattr);
}
#line 4521 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 305:
#line 1333 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4527 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 306:
#line 1337 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.pwrite));
	(yyval.pwrite).fd = FD((yyvsp[-6].i));
	(yyval.pwrite).count = (yyvsp[-2].i);
	(yyval.pwrite).offset = (yyvsp[0].i);

	(yyval.pwrite).dummy1 = (yyvsp[-4].strlit).s;
}
#line 4540 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 307:
#line 1346 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.pwrite));
	(yyval.pwrite) = (yyvsp[0].pwrite);
}
#line 4549 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 308:
#line 1351 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4555 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 309:
#line 1355 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.unlink));
	(yyval.unlink).pathname = getpath(&(yyvsp[0].strlit), "unlink", state);
}
#line 4564 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 310:
#line 1360 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.unlink));
	(yyval.unlink) = (yyvsp[0].unlink);
}
#line 4573 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 311:
#line 1365 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4579 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 312:
#line 1369 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.setxattr));
	(yyval.setxattr).path = getpath(&(yyvsp[-8].strlit), "setxattr", state);
	(yyval.setxattr).name = getstr(&(yyvsp[-6].strlit), "setxattr", state);
	(yyval.setxattr).size = (yyvsp[-2].i);

	(yyval.setxattr).dummy1 = (yyvsp[-4].strlit).s;
}
#line 4592 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 313:
#line 1378 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.setxattr));
	(yyval.setxattr) = (yyvsp[0].setxattr);
}
#line 4601 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 314:
#line 1383 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4607 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 315:
#line 1387 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.link));
	(yyval.link).oldpath = getpath(&(yyvsp[-2].strlit), "link", state);
	(yyval.link).newpath = getpath(&(yyvsp[0].strlit), "link", state);
}
#line 4617 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 316:
#line 1393 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.link));
	(yyval.link) = (yyvsp[0].link);
}
#line 4626 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 317:
#line 1398 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4632 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 318:
#line 1402 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.rmdir));
	(yyval.rmdir).pathname = getpath(&(yyvsp[0].strlit), "rmdir", state);
}
#line 4641 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 319:
#line 1407 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.rmdir));
	(yyval.rmdir) = (yyvsp[0].rmdir);
}
#line 4650 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 320:
#line 1412 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4656 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 321:
#line 1416 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.getxattr));
	(yyval.getxattr).path = getpath(&(yyvsp[-6].strlit), "getxattr", state);
	(yyval.getxattr).name = getstr(&(yyvsp[-4].strlit), "getxattr", state);
	(yyval.getxattr).size = (yyvsp[0].i);

	(yyval.getxattr).dummy1 = (yyvsp[-2].str);
}
#line 4669 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 322:
#line 1425 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.getxattr));
	(yyval.getxattr).path = getpath(&(yyvsp[-2].strlit), "getxattr", state);
	(yyval.getxattr).name = getstr(&(yyvsp[0].strlit), "getxattr", state);
}
#line 4679 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 323:
#line 1431 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.getxattr));
	(yyval.getxattr).size = (yyvsp[0].i);

	(yyval.getxattr).dummy1 = (yyvsp[-2].str);
}
#line 4690 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 324:
#line 1440 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.flistxattr));
	(yyval.flistxattr).fd = FD((yyvsp[-4].i));
	(yyval.flistxattr).size = (yyvsp[0].i);

	(yyval.flistxattr).dummy1 = (yyvsp[-2].strlit).s;
}
#line 4702 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 325:
#line 1448 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.flistxattr));
	(yyval.flistxattr).fd = FD((yyvsp[-1].i));
}
#line 4711 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 326:
#line 1453 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.flistxattr));
	(yyval.flistxattr).size = (yyvsp[0].i);

	(yyval.flistxattr).dummy1 = (yyvsp[-2].strlit).s;
}
#line 4722 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 327:
#line 1462 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.removexattr));
	(yyval.removexattr).path = getpath(&(yyvsp[-2].strlit), "removexattr", state);
	(yyval.removexattr).name = getstr(&(yyvsp[0].strlit), "removexattr", state);
}
#line 4732 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 328:
#line 1468 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.removexattr));
	(yyval.removexattr) = (yyvsp[0].removexattr);
 }
#line 4741 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 329:
#line 1473 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4747 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 330:
#line 1477 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fremovexattr));
	(yyval.fremovexattr).fd = FD((yyvsp[-2].i));
	(yyval.fremovexattr).name = getstr(&(yyvsp[0].strlit), "fremovexattr", state);
}
#line 4757 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 331:
#line 1483 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fremovexattr));
	(yyval.fremovexattr) = (yyvsp[0].fremovexattr);
}
#line 4766 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 332:
#line 1488 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4772 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 333:
#line 1492 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fallocate));
	(yyval.fallocate).fd = FD((yyvsp[-6].i));
	(yyval.fallocate).mode = (yyvsp[-4].i);
	(yyval.fallocate).offset = (yyvsp[-2].i);
	(yyval.fallocate).len = (yyvsp[0].i);
}
#line 4784 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 334:
#line 1500 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fallocate));
	(yyval.fallocate) = (yyvsp[0].fallocate);
}
#line 4793 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 335:
#line 1505 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4799 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 336:
#line 1509 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fchmod));
	(yyval.fchmod).fd = FD((yyvsp[-2].i));
	(yyval.fchmod).mode = (yyvsp[0].i);
}
#line 4809 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 337:
#line 1515 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fchmod));
	(yyval.fchmod) = (yyvsp[0].fchmod);
}
#line 4818 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 338:
#line 1520 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4824 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 339:
#line 1524 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.utimensat));
	(yyval.utimensat).dirfd = FD((yyvsp[-6].i));
	(yyval.utimensat).pathname = getpath(&(yyvsp[-4].strlit), "utimensat", state);
	(yyval.utimensat).flags = (yyvsp[0].i);
}
#line 4835 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 340:
#line 1530 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.utimensat));
	(yyval.utimensat).dirfd = FD((yyvsp[-6].i));
	if ((yyvsp[-4].i) != 0) {
		fprintf(stderr, "Error: numeric but non-NULL pathname string "
		        "in utimensat (line %d)\n", state->linenum);
		exit(1);
	}
	(yyval.utimensat).pathname = NULL;
	(yyval.utimensat).flags = (yyvsp[0].i);
}
#line 4851 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 341:
#line 1542 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.utimensat));
	(yyval.utimensat) = (yyvsp[0].utimensat);
}
#line 4860 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 342:
#line 1547 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4866 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 343:
#line 1551 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.pipe));
	(yyval.pipe).rd = FD((yyvsp[-3].i));
	(yyval.pipe).wr = FD((yyvsp[-1].i));
}
#line 4876 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 344:
#line 1557 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4882 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 345:
#line 1559 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.pipe));
	(yyval.pipe) = (yyvsp[0].pipe);
}
#line 4891 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 346:
#line 1566 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.dup2));
	(yyval.dup2).old = FD((yyvsp[-2].i));
	(yyval.dup2).new = FD((yyvsp[0].i));
}
#line 4901 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 347:
#line 1572 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.dup2));
	(yyval.dup2) = (yyvsp[0].dup2);
}
#line 4910 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 348:
#line 1577 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4916 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 349:
#line 1581 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.faccessat));
	(yyval.faccessat).dirfd = FD(AT_FDCWD);
	(yyval.faccessat).pathname = getpath(&(yyvsp[-2].strlit), "faccessat", state);
	(yyval.faccessat).mode = (yyvsp[0].flags);
	/* as of v4.7, strace doesn't print 'flags' argument */
}
#line 4928 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 350:
#line 1589 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.faccessat));
	(yyval.faccessat) = (yyvsp[0].faccessat);
}
#line 4937 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 351:
#line 1594 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4943 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 352:
#line 1598 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.newfstatat));
	(yyval.newfstatat).dirfd = FD(AT_FDCWD);
	(yyval.newfstatat).pathname = getpath(&(yyvsp[-4].strlit), "newfstatat", state);
	(yyval.newfstatat).flags = (yyvsp[0].flags);
}
#line 4954 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 353:
#line 1605 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.newfstatat));
	(yyval.newfstatat) = (yyvsp[0].newfstatat);
}
#line 4963 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 354:
#line 1610 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4969 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 355:
#line 1614 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fchownat));
	(yyval.fchownat).dirfd = FD(AT_FDCWD);
	(yyval.fchownat).pathname = getpath(&(yyvsp[-6].strlit), "fchownat", state);
	(yyval.fchownat).owner = (yyvsp[-4].i);
	(yyval.fchownat).group = (yyvsp[-2].i);
	(yyval.fchownat).flags = (yyvsp[0].flags);
}
#line 4982 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 356:
#line 1623 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fchownat));
	(yyval.fchownat) = (yyvsp[0].fchownat);
}
#line 4991 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 357:
#line 1628 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 4997 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 358:
#line 1632 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.unlinkat));
	(yyval.unlinkat).dirfd = FD(AT_FDCWD);
	(yyval.unlinkat).pathname = getpath(&(yyvsp[-2].strlit), "unlinkat", state);
	(yyval.unlinkat).flags = (yyvsp[0].flags);
}
#line 5008 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 359:
#line 1639 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.unlinkat));
	(yyval.unlinkat) = (yyvsp[0].unlinkat);
}
#line 5017 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 360:
#line 1644 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 5023 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 361:
#line 1648 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.dup2));
	(yyval.dup2).old = FD((yyvsp[-4].i));
	(yyval.dup2).new = FD((yyvsp[-2].i));
	(yyval.dup2).flags = (yyvsp[0].flags);
}
#line 5034 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 362:
#line 1655 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.dup2));
	(yyval.dup2) = (yyvsp[0].dup2);
}
#line 5043 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 363:
#line 1660 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 5049 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 364:
#line 1664 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fchown));
	(yyval.fchown).fd = FD((yyvsp[-4].i));
	(yyval.fchown).owner = (yyvsp[-2].i);
	(yyval.fchown).group = (yyvsp[0].i);
}
#line 5060 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 365:
#line 1671 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.fchown));
	(yyval.fchown) = (yyvsp[0].fchown);
}
#line 5069 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 366:
#line 1676 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 5075 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 367:
#line 1680 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.symlink));
	(yyval.symlink).oldpath = getpath(&(yyvsp[-2].strlit), "symlink", state);
	(yyval.symlink).newpath = getpath(&(yyvsp[0].strlit), "symlink", state);
}
#line 5085 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 368:
#line 1686 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.symlink));
	(yyval.symlink) = (yyvsp[0].symlink);
}
#line 5094 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 369:
#line 1691 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 5100 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 370:
#line 1695 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.dup));
	(yyval.dup).fd = FD((yyvsp[-6].i));
}
#line 5109 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 371:
#line 1700 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.dup));
	(yyval.dup).fd = FD((yyvsp[-1].i));
}
#line 5118 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 372:
#line 1705 "strace-parse.y" /* yacc.c:1646  */
    {
	/* Nothing we care about. */
}
#line 5126 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 373:
#line 1711 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.io_setup));
	(yyval.io_setup).nr_events = (yyvsp[-4].i);
	(yyval.io_setup).ctx = (yyvsp[-1].i);
}
#line 5136 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 374:
#line 1717 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.io_setup));
	(yyval.io_setup).nr_events = (yyvsp[-1].i);
}
#line 5145 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 375:
#line 1722 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.io_setup));
	(yyval.io_setup).ctx = (yyvsp[-1].i);
}
#line 5154 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 376:
#line 1729 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.io_submit));
	(yyval.io_submit).ctx = (yyvsp[-4].i);
	(yyval.io_submit).nr = (yyvsp[-2].i);
	(yyval.io_submit).iocbs = (yyvsp[0].iocbs);
}
#line 5165 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 377:
#line 1736 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.io_submit));
	(yyval.io_submit) = (yyvsp[0].io_submit);
}
#line 5174 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 378:
#line 1741 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 5180 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 379:
#line 1745 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.io_getevents));
	(yyval.io_getevents).ctx = (yyvsp[-8].i);
	(yyval.io_getevents).min_nr = (yyvsp[-6].i);
	(yyval.io_getevents).nr = (yyvsp[-4].i);
	(yyval.io_getevents).ioevents = (yyvsp[-2].ioevents);
	/* FIXME: do we want/need the timespec struct? */
}
#line 5193 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 380:
#line 1754 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.io_getevents));
	(yyval.io_getevents).ctx = (yyvsp[-5].i);
	(yyval.io_getevents).min_nr = (yyvsp[-3].i);
	(yyval.io_getevents).nr = (yyvsp[-1].i);
}
#line 5204 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 381:
#line 1767 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.io_getevents));
	(yyval.io_getevents).ioevents = (yyvsp[-2].ioevents);
}
#line 5213 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 382:
#line 1774 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.io_destroy));
	(yyval.io_destroy).ctx = (yyvsp[0].i);
}
#line 5222 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 383:
#line 1779 "strace-parse.y" /* yacc.c:1646  */
    {
	ZERO(&(yyval.io_destroy));
	(yyval.io_destroy) = (yyvsp[0].io_destroy);
}
#line 5231 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 384:
#line 1784 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 5237 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 391:
#line 1793 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.str) = (yyvsp[0].strlit).s;
}
#line 5245 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 392:
#line 1796 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.str) = NULL;
}
#line 5253 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 398:
#line 1806 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.iocbs) = (yyvsp[-1].iocbs);
}
#line 5261 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 399:
#line 1810 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.iocbs) = NULL;
}
#line 5269 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 400:
#line 1813 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.iocbs) = (yyvsp[0].iocbs);
}
#line 5277 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 401:
#line 1817 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.iocbs) = (yyvsp[0].iocbs);
}
#line 5285 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 402:
#line 1820 "strace-parse.y" /* yacc.c:1646  */
    {
	iocb_list_append((yyvsp[-2].iocbs), (yyvsp[0].iocbs));
	(yyval.iocbs) = (yyvsp[-2].iocbs);
}
#line 5294 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 403:
#line 1825 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.iocbs) = xmalloc(sizeof(*(yyval.iocbs)));
	(yyval.iocbs)->type = AT_PWRITE;
	(yyval.iocbs)->data = (yyvsp[-10].i);
	(yyval.iocbs)->fd = FD((yyvsp[-7].i));
	(yyval.iocbs)->dummy = (yyvsp[-5].str);
	(yyval.iocbs)->nbytes = (yyvsp[-3].i);
	(yyval.iocbs)->offset = (yyvsp[-1].i);
	(yyval.iocbs)->next = NULL;
}
#line 5309 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 404:
#line 1835 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.iocbs) = xmalloc(sizeof(*(yyval.iocbs)));
	(yyval.iocbs)->type = AT_PREAD;
	(yyval.iocbs)->data = (yyvsp[-10].i);
	(yyval.iocbs)->fd = FD((yyvsp[-7].i));
	(yyval.iocbs)->dummy = NULL;
	(yyval.iocbs)->nbytes = (yyvsp[-3].i);
	(yyval.iocbs)->offset = (yyvsp[-1].i);
	(yyval.iocbs)->next = NULL;
}
#line 5324 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 405:
#line 1846 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.i) = 0;
}
#line 5332 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 406:
#line 1849 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.i) = (yyvsp[-1].i);
}
#line 5340 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 407:
#line 1853 "strace-parse.y" /* yacc.c:1646  */
    { }
#line 5346 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 408:
#line 1855 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.str) = (yyvsp[0].strlit).s;
}
#line 5354 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 409:
#line 1859 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.i) = (yyvsp[0].i);
}
#line 5362 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 410:
#line 1863 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.i) = (yyvsp[0].i);
}
#line 5370 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 411:
#line 1867 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.i) = (yyvsp[0].i);
}
#line 5378 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 412:
#line 1871 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.ioevents) = (yyvsp[-1].ioevents);
}
#line 5386 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 413:
#line 1875 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.ioevents) = NULL;
}
#line 5394 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 414:
#line 1878 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.ioevents) = (yyvsp[0].ioevents);
}
#line 5402 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 415:
#line 1882 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.ioevents) = (yyvsp[0].ioevents);
}
#line 5410 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 416:
#line 1885 "strace-parse.y" /* yacc.c:1646  */
    {
	ioevent_list_append((yyvsp[-2].ioevents), (yyvsp[0].ioevents));
	(yyval.ioevents) = (yyvsp[-2].ioevents);
}
#line 5419 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 417:
#line 1890 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.ioevents) = xmalloc(sizeof(*(yyval.ioevents)));
	(yyval.ioevents)->data = (yyvsp[-7].i);
	(yyval.ioevents)->obj = (yyvsp[-5].i);
	(yyval.ioevents)->res = (yyvsp[-3].i);
	(yyval.ioevents)->res2 = (yyvsp[-1].i);
	(yyval.ioevents)->next = NULL;

	/* This information is figured out later by the compiler. */
	(yyval.ioevents)->fd = FD(-1);
}
#line 5435 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 420:
#line 1906 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.str) = (yyvsp[0].str);
}
#line 5443 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 421:
#line 1909 "strace-parse.y" /* yacc.c:1646  */
    {
	if ((yyvsp[0].i))
		fprintf(stderr, "Warning: unrecognized FD flag, ignoring (line %d)\n",
		        state->linenum);
	(yyval.str) = 0;
}
#line 5454 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 422:
#line 1916 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 5460 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 423:
#line 1917 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 5466 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 424:
#line 1918 "strace-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 5472 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 425:
#line 1921 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
}
#line 5480 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 426:
#line 1924 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
	(yyval.flags)->next = (yyvsp[-2].flags);
}
#line 5489 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 427:
#line 1929 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = (yyvsp[0].flags);
}
#line 5497 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 428:
#line 1932 "strace-parse.y" /* yacc.c:1646  */
    {
	if ((yyvsp[0].i))
		fprintf(stderr, "warning: unrecognized O_* flag, ignoring (line %d)\n",
		        state->linenum);
	(yyval.flags) = NULL;
}
#line 5508 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 429:
#line 1939 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
}
#line 5516 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 430:
#line 1942 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
	(yyval.flags)->next = (yyvsp[-2].flags);
}
#line 5525 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 431:
#line 1947 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
}
#line 5533 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 432:
#line 1950 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
	(yyval.flags)->next = (yyvsp[-2].flags);
}
#line 5542 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 433:
#line 1955 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
}
#line 5550 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 434:
#line 1958 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
	(yyval.flags)->next = (yyvsp[-2].flags);
}
#line 5559 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 437:
#line 1970 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
}
#line 5567 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 438:
#line 1973 "strace-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
	(yyval.flags)->next = (yyvsp[-2].flags);
}
#line 5576 "strace-parse.tab.c" /* yacc.c:1646  */
    break;

  case 439:
#line 1977 "strace-parse.y" /* yacc.c:1646  */
    {
	if ((yyvsp[0].i))
		fprintf(stderr, "Warning: unrecognized AT_* flag, ignoring (line %d)\n",
		        state->linenum);
	(yyval.flags) = NULL;
}
#line 5587 "strace-parse.tab.c" /* yacc.c:1646  */
    break;


#line 5591 "strace-parse.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (state, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (state, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, state);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, state);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (state, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, state);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, state);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1984 "strace-parse.y" /* yacc.c:1906  */


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
