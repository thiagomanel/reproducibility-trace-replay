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
#define yyparse         ibenchparse
#define yylex           ibenchlex
#define yyerror         ibencherror
#define yydebug         ibenchdebug
#define yynerrs         ibenchnerrs

#define yylval          ibenchlval
#define yychar          ibenchchar

/* Copy the first part of user declarations.  */

#line 75 "ibench-parse.tab.c" /* yacc.c:339  */

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
   by #include "ibench-parse.tab.h".  */
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
#line 19 "ibench-parse.y" /* yacc.c:355  */


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


#line 124 "ibench-parse.tab.c" /* yacc.c:355  */

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
#line 38 "ibench-parse.y" /* yacc.c:355  */

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

#line 316 "ibench-parse.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE ibenchlval;

int ibenchparse (struct parser_state* state);

#endif /* !YY_IBENCH_IBENCH_PARSE_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */
#line 216 "ibench-parse.y" /* yacc.c:358  */



#line 334 "ibench-parse.tab.c" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 123 "ibench-parse.y" /* yacc.c:359  */

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


#line 429 "ibench-parse.tab.c" /* yacc.c:359  */

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
#define YYLAST   573

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  94
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  88
/* YYNRULES -- Number of rules.  */
#define YYNRULES  176
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  623

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   348

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
      85,    86,    87,    88,    89,    90,    91,    92,    93
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   418,   418,   419,   470,   479,   487,   497,   498,   499,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,   567,   570,   571,   572,   573,
     574,   575,   576,   577,   583,   584,   587,   588,   591,   599,
     604,   608,   612,   615,   621,   630,   637,   644,   652,   660,
     669,   674,   679,   683,   689,   695,   705,   712,   717,   722,
     726,   730,   736,   743,   750,   755,   760,   767,   776,   780,
     783,   787,   791,   795,   799,   803,   807,   811,   815,   819,
     823,   827,   831,   835,   839,   843,   848,   854,   859,   864,
     868,   872,   876,   879,   882,   886,   890,   895,   900,   905,
     911,   916,   921,   926,   930,   937,   941,   946,   947,   948,
     949,   950,   951,   952,   953,   955,   956,   959,   960,   961,
     962,   965,   968,   974,   977,   983,   986
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"EOF\"", "error", "$undefined", "DECNUM", "HEXNUM", "FLAG_O",
  "FLAG_PROT", "FLAG_MAP", "FCNTL_CMD", "STRING", "SOR", "DELIM", "EOR",
  "SUBTYPE_ENTRY", "SUBTYPE_RETURN", "SUBTYPE_COMMENT", "THREAD_NA",
  "RSRC_INUM", "ET_OPEN", "ET_SHM_OPEN", "ET_CLOSE", "ET_DUP", "ET_KQUEUE",
  "ET_SOCKET", "ET_SOCKETPAIR", "ET_READ", "ET_WRITE", "ET_PREAD",
  "ET_PWRITE", "ET_AIO_READ", "ET_AIO_ERROR", "ET_AIO_RETURN",
  "ET_AIO_SUSPEND", "ET_LSEEK", "ET_FSYNC", "ET_FCNTL", "ET_FTRUNCATE",
  "ET_LINK", "ET_SYMLINK", "ET_RMDIR", "ET_UNLINK", "ET_READLINK",
  "ET_RENAME", "ET_EXCHANGEDATA", "ET_MKDIR", "ET_FLOCK", "ET_MMAP",
  "ET_FUTIMES", "ET_UTIMES", "ET_STATFS", "ET_STATFS64", "ET_FSTATFS64",
  "ET_FSTAT", "ET_LSTAT", "ET_STAT", "ET_FSTAT64", "ET_LSTAT64",
  "ET_STAT64", "ET_SETATTRLIST", "ET_GETATTRLIST", "ET_GETXATTR",
  "ET_FGETXATTR", "ET_LISTXATTR", "ET_FLISTXATTR", "ET_ACCESS", "ET_CHOWN",
  "ET_CHMOD", "ET_FSETXATTR", "ET_FSGETPATH", "ET_GETDIRENTRIES64",
  "ET_GETDIRENTRIESATTR", "ET_GETFSSTAT64", "ET_GETDTABLESIZE", "ET_CHDIR",
  "ET_FCHDIR", "ET_KILL", "ET_FCHFLAGS", "ET_FCHMOD", "ET_FCHOWN",
  "ET_REMOVEXATTR", "ET_FREMOVEXATTR", "ET_SETXATTR", "ET_CONTROLLER",
  "ET_RESOLVER", "ET_ALL_CALLS", "ET_SETTER", "ET_BUILD_PATH", "ET_NAMEI",
  "ET_VN_CREATE", "ET_HFS_VNOP_PAGEIN", "ET_VNODE_PAGEIN",
  "ET_SYSCALL_MAP", "ET_POSIX_SPAWN", "ET_EXECVE", "$accept", "input",
  "event", "evprefix", "subtype", "evinfo", "inum", "open_attrs",
  "shm_open_attrs", "close_attrs", "dup_attrs", "kqueue_attrs",
  "socket_attrs", "socketpair_attrs", "read_attrs", "write_attrs",
  "pread_attrs", "pwrite_attrs", "aio_read_attrs", "aio_error_attrs",
  "aio_return_attrs", "aio_suspend_attrs", "lseek_attrs", "fsync_attrs",
  "fcntl_attrs", "ftruncate_attrs", "link_attrs", "symlink_attrs",
  "rmdir_attrs", "unlink_attrs", "readlink_attrs", "rename_attrs",
  "exchangedata_attrs", "mkdir_attrs", "flock_attrs", "mmap_attrs",
  "futimes_attrs", "utimes_attrs", "statfs_attrs", "statfs64_attrs",
  "fstatfs64_attrs", "fstat_attrs", "lstat_attrs", "stat_attrs",
  "fstat64_attrs", "lstat64_attrs", "stat64_attrs", "setattrlist_attrs",
  "getattrlist_attrs", "getxattr_attrs", "fgetxattr_attrs",
  "listxattr_attrs", "flistxattr_attrs", "access_attrs", "chown_attrs",
  "chmod_attrs", "fsetxattr_attrs", "fsgetpath_attrs",
  "getdirentries64_attrs", "getdirentriesattr_attrs", "getfsstat64_attrs",
  "getdtablesize_attrs", "chdir_attrs", "fchdir_attrs", "kill_attrs",
  "fchflags_attrs", "fchmod_attrs", "fchown_attrs", "removexattr_attrs",
  "fremovexattr_attrs", "setxattr_attrs", "controller_attrs",
  "hfs_vnop_pagein_attrs", "syscall_map_attrs", "posix_spawn_attrs",
  "vnode_pagein_attrs", "resolver_attrs", "all_calls_attrs",
  "setter_attrs", "build_path_attrs", "namei_attrs", "vn_create_attrs",
  "execve_attrs", "fieldlist", "field", "o_flags", "prot_flags",
  "map_flags", YY_NULLPTR
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
     345,   346,   347,   348
};
# endif

#define YYPACT_NINF -508

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-508)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -508,     3,  -508,     7,  -508,    11,    18,     1,    76,    23,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    60,    61,    62,    63,    64,    65,    66,
      68,    69,    70,    71,    72,    73,    74,    75,    77,    78,
      79,    80,    81,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,     0,    17,    17,
      19,   185,    84,   192,  -508,   193,   194,   195,   196,   197,
     198,    22,   199,   200,   202,   203,   204,   205,   206,   201,
     207,   208,   209,   210,   211,   212,   213,   220,   221,   223,
     224,   215,   219,   226,   227,   222,   225,   229,   228,   230,
     231,   232,   233,   235,   234,   241,   236,   237,   238,   245,
      -1,   246,   247,  -508,  -508,   242,   249,   250,   251,   252,
     253,   248,   255,   254,   256,    15,    15,    15,    15,    15,
      15,   257,    15,   258,   259,    15,  -508,  -508,  -508,  -508,
    -508,  -508,   191,   261,   260,   263,   262,   266,   264,   267,
     269,   265,   271,   268,   274,   270,   275,   272,   277,   273,
     279,   276,   282,   278,   283,   280,   285,   281,   287,   284,
     290,   286,   291,   288,   293,   289,   295,   292,   298,   294,
     299,   296,   301,   297,   303,   300,   306,   302,   307,   304,
     309,   305,   311,   308,   314,   310,   315,   312,   317,   313,
     319,   316,   322,   318,   323,   320,   325,   321,   327,   324,
     330,   326,   331,   328,   333,   329,   335,   332,   338,   334,
     339,   336,   341,   337,   343,   340,   346,   342,   347,   344,
     349,   345,   351,   348,   354,   350,   355,   352,   357,   353,
     359,  -508,  -508,   356,   362,   358,   363,   360,   365,   367,
     369,   364,   370,   366,   371,   368,   373,   372,   375,   374,
     377,   376,   378,   379,   381,   380,   383,   382,   385,  -508,
    -508,  -508,  -508,  -508,  -508,   384,  -508,  -508,   384,  -508,
     384,  -508,   384,  -508,   384,  -508,   384,   386,   389,  -508,
     384,  -508,  -508,   387,   391,  -508,   384,   393,  -508,   396,
    -508,  -508,  -508,  -508,  -508,  -508,   397,  -508,   398,  -508,
     399,  -508,   400,  -508,   401,  -508,   402,  -508,   403,  -508,
    -508,  -508,  -508,  -508,  -508,  -508,   404,  -508,   405,  -508,
     406,  -508,   407,  -508,   408,  -508,   409,  -508,  -508,  -508,
    -508,  -508,   410,  -508,   411,  -508,   412,  -508,   413,  -508,
     416,  -508,   419,  -508,  -508,  -508,  -508,  -508,  -508,  -508,
    -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,
    -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,
    -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,   420,  -508,
     421,  -508,   422,  -508,   417,  -508,  -508,  -508,  -508,  -508,
    -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,   424,  -508,
     425,  -508,   426,  -508,   427,  -508,   423,  -508,   428,  -508,
     429,  -508,    15,   430,  -508,  -508,  -508,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,  -508,
     463,   470,  -508,   473,   474,   475,   476,   477,   478,   479,
     480,  -508,   481,   482,  -508,  -508,    24,    -1,    -1,  -508,
     483,   484,  -508,   485,  -508,  -508,  -508,  -508,  -508,   486,
    -508,  -508,  -508,   487,  -508,    10,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   497,   498,  -508,   499,   500,
     501,   502,   503,   504,   505,  -508,   514,  -508,   515,   510,
     511,   512,   513,   520,  -508,   516,  -508,  -508,   522,    -1,
    -508,   523,  -508,  -508,   518,   517,   519,   521,   524,   525,
     526,   527,   528,   529,   530,   531,   532,    -1,   541,  -508,
    -508,  -508,  -508,   542,   537,    -1,  -508,   544,   545,   538,
     539,   540,   543,   546,   547,   548,   551,  -508,   552,  -508,
      -1,   549,  -508,   553,   554,   555,   556,   559,  -508,   560,
     550,   561,   562,  -508,    14,  -508,  -508,  -508,   563,  -508,
      16,  -508,  -508
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     3,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   142,   143,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   167,   167,   167,   167,   167,
     167,     0,   167,     0,     0,   167,     4,     7,     8,     9,
       5,     6,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   153,
      76,   168,   169,   170,    77,   158,   165,    78,   159,    79,
     160,    80,   161,    81,   162,    82,   163,     0,     0,    84,
     157,   155,    83,     0,     0,    85,   164,     0,    10,     0,
      11,    90,    12,    91,    13,    14,     0,    15,     0,    16,
       0,    17,     0,    18,     0,    19,     0,    20,     0,    21,
     100,    22,   101,    23,   102,    24,     0,    25,     0,    26,
       0,    27,     0,    28,     0,    29,     0,    30,   109,    31,
     110,    32,     0,    33,     0,    34,     0,    35,     0,    36,
       0,    37,     0,    38,   118,    39,   119,    40,   120,    41,
     121,    42,   122,    43,   123,    44,   124,    45,   125,    46,
     126,    47,   127,    48,   128,    49,   129,    50,   130,    51,
     131,    52,   132,    53,   133,    54,   134,    55,     0,    56,
       0,    57,     0,    58,     0,    59,   139,    60,   140,    61,
     141,    62,    63,    64,   144,    65,   145,    66,     0,    67,
       0,    68,     0,    69,     0,    70,     0,    71,     0,    72,
       0,    73,   167,     0,    74,   156,    75,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   166,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,   104,     0,     0,   107,   108,     0,     0,     0,   115,
       0,     0,   135,     0,   137,   138,   146,   147,   148,     0,
     150,   151,   152,     0,   171,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   112,     0,     0,
       0,     0,     0,     0,     0,   172,     0,    93,     0,     0,
       0,     0,     0,     0,   103,     0,   106,   111,     0,     0,
     116,     0,   136,   149,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,     0,     0,     0,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,   105,
       0,     0,   154,     0,     0,     0,     0,     0,    99,     0,
       0,     0,     0,   173,     0,    88,   113,   174,     0,   175,
       0,   176,   117
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -508,  -508,  -508,  -508,   122,  -508,  -507,  -508,  -508,  -508,
    -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,
    -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,
    -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,
    -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,
    -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,
    -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,
    -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,
    -508,  -508,  -508,  -151,  -240,  -508,  -508,  -508
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     4,     6,   170,    87,   273,   173,   175,   177,
     179,   180,   182,   184,   186,   188,   190,   192,   194,   196,
     198,   200,   202,   204,   206,   208,   210,   212,   214,   216,
     218,   220,   222,   224,   226,   228,   230,   232,   234,   236,
     238,   240,   242,   244,   246,   248,   250,   252,   254,   256,
     258,   260,   262,   264,   266,   268,   270,   274,   276,   278,
     279,   280,   282,   284,   286,   288,   290,   292,   294,   296,
     298,   300,   318,   322,   324,   319,   304,   307,   309,   311,
     313,   315,   325,   305,   306,   525,   614,   620
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
     538,   539,   271,     2,     9,   308,   310,   312,   314,   316,
       5,   320,   166,     3,   326,   545,   272,    10,   301,   302,
     617,   546,     7,   621,   303,   618,   193,   622,   172,     8,
     167,   168,   169,   536,    88,   537,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   574,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     589,   122,   123,   124,   125,   126,   127,   128,   593,   129,
     130,   131,   132,   133,   134,   135,   136,   176,   137,   138,
     139,   140,   141,   605,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   174,   178,   181,   183,   185,   187,
     189,   191,   327,   195,   197,   199,   201,   203,   205,   207,
     209,   171,   489,     0,     0,     0,   211,   213,   215,   217,
     219,   221,   223,   225,   233,   227,   229,   231,   235,   237,
     239,   241,   245,     0,   243,     0,     0,   247,   257,   249,
     251,   253,   255,   259,   261,   263,   265,   267,   269,   275,
     277,   281,   283,   285,   287,   289,   291,   293,   295,     0,
     317,     0,     0,   297,   328,   299,   330,   321,   323,   332,
     334,   329,   335,   331,   337,   333,   336,   339,   341,   338,
     343,   340,   345,   342,   344,   347,   349,   346,   351,   348,
     353,   350,   352,   355,   357,   354,   359,   356,   361,   358,
     360,   363,   365,   362,   367,   364,   369,   366,   368,   371,
     373,   370,   375,   372,   377,   374,   376,   379,   381,   378,
     383,   380,   385,   382,   384,   387,   389,   386,   391,   388,
     393,   390,   392,   395,   397,   394,   399,   396,   401,   398,
     400,   403,   405,   402,   407,   404,   409,   406,   408,   411,
     413,   410,   415,   412,   417,   414,   416,   419,   421,   418,
     423,   420,   425,   422,   424,   427,   429,   426,   431,   428,
     432,   430,   433,   435,   437,   434,   439,   436,   441,   438,
     443,   445,     0,   440,   447,   442,   449,   444,   451,     0,
     446,   448,   454,   450,   456,   452,   457,   453,   455,   458,
     459,   460,   461,   462,   463,   464,   465,   466,     0,   468,
     469,     0,     0,   472,   467,     0,   475,   470,   471,   476,
     473,   474,   477,   478,   479,   480,   481,   482,   483,   484,
     485,     0,   486,   490,     0,     0,     0,   487,   488,     0,
       0,     0,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,   517,   518,
     519,   520,   521,   522,   523,   524,   526,   527,   528,   529,
     530,   531,   532,   533,   534,     0,     0,   541,   542,   543,
     544,   535,   540,     0,     0,     0,     0,     0,     0,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,   573,   575,   576,   577,     0,
     578,     0,   579,     0,     0,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   590,   591,   592,   594,   595,   596,
     597,   598,   606,     0,   599,     0,   613,   600,   601,   602,
     603,   604,     0,     0,   607,   608,   609,   610,   611,   612,
     619,     0,   615,   616
};

static const yytype_int16 yycheck[] =
{
     507,   508,     3,     0,     3,   156,   157,   158,   159,   160,
       3,   162,    12,    10,   165,     5,    17,    16,     3,     4,
       6,    11,    11,     7,     9,    11,     4,    11,     9,    11,
      13,    14,    15,     9,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,   559,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
     577,    11,    11,    11,    11,    11,    11,    11,   585,    11,
      11,    11,    11,    11,    11,    11,    11,     3,    11,    11,
      11,    11,    11,   600,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,     9,     3,     3,     3,     3,     3,
       3,     3,    11,     4,     4,     3,     3,     3,     3,     3,
       9,    89,   452,    -1,    -1,    -1,     9,     9,     9,     9,
       9,     9,     9,     3,     9,     4,     3,     3,     9,     3,
       3,     9,     3,    -1,     9,    -1,    -1,     9,     3,     9,
       9,     9,     9,     9,     3,     9,     9,     9,     3,     3,
       3,     9,     3,     3,     3,     3,     3,     9,     3,    -1,
       3,    -1,    -1,     9,     3,     9,     3,     9,     9,     3,
       3,    11,     3,    11,     3,    11,    11,     3,     3,    11,
       3,    11,     3,    11,    11,     3,     3,    11,     3,    11,
       3,    11,    11,     3,     3,    11,     3,    11,     3,    11,
      11,     3,     3,    11,     3,    11,     3,    11,    11,     3,
       3,    11,     3,    11,     3,    11,    11,     3,     3,    11,
       3,    11,     3,    11,    11,     3,     3,    11,     3,    11,
       3,    11,    11,     3,     3,    11,     3,    11,     3,    11,
      11,     3,     3,    11,     3,    11,     3,    11,    11,     3,
       3,    11,     3,    11,     3,    11,    11,     3,     3,    11,
       3,    11,     3,    11,    11,     3,     3,    11,     3,    11,
       3,    11,     3,     3,     3,    11,     3,    11,     3,    11,
       3,     3,    -1,    11,     3,    11,     3,    11,     3,    -1,
      11,    11,     3,    11,     3,    11,     3,    11,    11,     3,
       3,     3,     3,     3,     3,     3,     3,     3,    -1,     3,
       3,    -1,    -1,     3,     9,    -1,     3,     9,     9,     3,
       9,     9,     3,     3,     3,     3,     9,     3,     3,     3,
       3,    -1,     9,     3,    -1,    -1,    -1,     9,     9,    -1,
      -1,    -1,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,     5,     3,     3,     3,     3,
       3,     3,     3,     3,     3,    -1,    -1,     3,     3,     3,
       3,     9,     9,    -1,    -1,    -1,    -1,    -1,    -1,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,     3,     3,     9,
       9,     9,     9,     3,     8,     3,     3,     9,    11,    -1,
      11,    -1,    11,    -1,    -1,    11,    11,    11,    11,    11,
      11,    11,    11,    11,     3,     3,     9,     3,     3,    11,
      11,    11,     3,    -1,    11,    -1,     6,    11,    11,    11,
       9,     9,    -1,    -1,    11,    11,    11,    11,     9,     9,
       7,    -1,    11,    11
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    95,     0,    10,    96,     3,    97,    11,    11,     3,
      16,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    99,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    12,    13,    14,    15,
      98,    98,     9,   101,     9,   102,     3,   103,     3,   104,
     105,     3,   106,     3,   107,     3,   108,     3,   109,     3,
     110,     3,   111,     4,   112,     4,   113,     4,   114,     3,
     115,     3,   116,     3,   117,     3,   118,     3,   119,     9,
     120,     9,   121,     9,   122,     9,   123,     9,   124,     9,
     125,     9,   126,     9,   127,     3,   128,     4,   129,     3,
     130,     3,   131,     9,   132,     9,   133,     3,   134,     3,
     135,     9,   136,     9,   137,     3,   138,     9,   139,     9,
     140,     9,   141,     9,   142,     9,   143,     3,   144,     9,
     145,     3,   146,     9,   147,     9,   148,     9,   149,     3,
     150,     3,    17,   100,   151,     3,   152,     3,   153,   154,
     155,     9,   156,     3,   157,     3,   158,     3,   159,     3,
     160,     3,   161,     9,   162,     3,   163,     9,   164,     9,
     165,     3,     4,     9,   170,   177,   178,   171,   177,   172,
     177,   173,   177,   174,   177,   175,   177,     3,   166,   169,
     177,     9,   167,     9,   168,   176,   177,    11,     3,    11,
       3,    11,     3,    11,     3,     3,    11,     3,    11,     3,
      11,     3,    11,     3,    11,     3,    11,     3,    11,     3,
      11,     3,    11,     3,    11,     3,    11,     3,    11,     3,
      11,     3,    11,     3,    11,     3,    11,     3,    11,     3,
      11,     3,    11,     3,    11,     3,    11,     3,    11,     3,
      11,     3,    11,     3,    11,     3,    11,     3,    11,     3,
      11,     3,    11,     3,    11,     3,    11,     3,    11,     3,
      11,     3,    11,     3,    11,     3,    11,     3,    11,     3,
      11,     3,    11,     3,    11,     3,    11,     3,    11,     3,
      11,     3,    11,     3,    11,     3,    11,     3,    11,     3,
      11,     3,     3,     3,    11,     3,    11,     3,    11,     3,
      11,     3,    11,     3,    11,     3,    11,     3,    11,     3,
      11,     3,    11,    11,     3,    11,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     9,     3,     3,
       9,     9,     3,     9,     9,     3,     3,     3,     3,     3,
       3,     9,     3,     3,     3,     3,     9,     9,     9,   178,
       3,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,     5,   179,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     9,     9,    11,   100,   100,
       9,     3,     3,     3,     3,     5,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,     3,     3,     9,     9,     9,
       9,     3,     8,     3,   100,     3,     9,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,   100,
       3,     3,     9,   100,     3,     3,    11,    11,    11,    11,
      11,    11,    11,     9,     9,   100,     3,    11,    11,    11,
      11,     9,     9,     6,   180,    11,    11,     6,    11,     7,
     181,     7,    11
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    94,    95,    95,    96,    97,    97,    98,    98,    98,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,   100,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   177,   178,   178,   178,
     178,   179,   179,   180,   180,   181,   181
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     5,     5,     5,     1,     1,     1,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,    14,     4,
       2,     2,     0,     6,    10,     8,     8,     8,     8,    12,
       2,     2,     2,     6,     4,    10,     6,     4,     4,     2,
       2,     6,     5,    14,     8,     4,     6,    16,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     4,     6,     4,     4,     2,
       2,     2,     0,     0,     2,     2,     4,     4,     4,     6,
       4,     4,     4,     1,    10,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     1,     1,
       1,     1,     2,     1,     2,     1,     2
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
#line 419 "ibench-parse.y" /* yacc.c:1646  */
    {
	struct event* entry;
	struct event* ev;

	if ((yyvsp[0].ev).subtype == ST_COMMENT) {
		xfree((yyvsp[0].ev).attrs.syscall_map.dummy1);
		goto ignore;
	}

	ev = xmalloc(sizeof(struct event));
	*ev = (yyvsp[0].ev);

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
}
#line 1971 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 470 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.ev) = (yyvsp[-3].ev);
	(yyval.ev).type = (yyvsp[-1].ev).type;
	(yyval.ev).attrs = (yyvsp[-1].ev).attrs;
	(yyval.ev).ret = (yyvsp[-1].ev).ret;
	(yyval.ev).next = NULL;
	(yyval.ev).traceline = state->linenum++;
}
#line 1984 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 479 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.ev).thread = (yyvsp[-2].i);
	(yyval.ev).subtype = (yyvsp[0].subtype);
	if ((yyval.ev).subtype == ST_RETURN)
		(yyval.ev).time.exit = (yyvsp[-4].i);
	else
		(yyval.ev).time.entry = (yyvsp[-4].i);
}
#line 1997 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 487 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.ev).thread = -1;
	(yyval.ev).subtype = (yyvsp[0].subtype);
	if ((yyval.ev).subtype == ST_RETURN)
		(yyval.ev).time.exit = (yyvsp[-4].i);
	else
		(yyval.ev).time.exit = (yyvsp[-4].i);
}
#line 2010 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 497 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.subtype) = ST_ENTRY; }
#line 2016 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 498 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.subtype) = ST_RETURN; }
#line 2022 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 499 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.subtype) = ST_COMMENT; }
#line 2028 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 502 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(OPEN, open, (yyvsp[-1].open), (yyvsp[0].i)); }
#line 2034 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 503 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(SHM_OPEN, shm_open, (yyvsp[-1].shm_open), (yyvsp[0].i)); }
#line 2040 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 504 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(CLOSE, close, (yyvsp[-1].close), (yyvsp[0].i)); }
#line 2046 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 505 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(DUP, dup, (yyvsp[-1].dup), (yyvsp[0].i)); }
#line 2052 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 506 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(KQUEUE, kqueue, (yyvsp[-1].kqueue), (yyvsp[0].i)); }
#line 2058 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 507 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(SOCKET, socket, (yyvsp[-1].socket), (yyvsp[0].i)); }
#line 2064 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 508 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(SOCKETPAIR, socketpair, (yyvsp[-1].socketpair), (yyvsp[0].i)); }
#line 2070 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 509 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(READ, read, (yyvsp[-1].read), (yyvsp[0].i)); }
#line 2076 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 510 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(WRITE, write, (yyvsp[-1].write), (yyvsp[0].i)); }
#line 2082 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 511 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(PREAD, pread, (yyvsp[-1].pread), (yyvsp[0].i)); }
#line 2088 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 512 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(PWRITE, pwrite, (yyvsp[-1].pwrite), (yyvsp[0].i)); }
#line 2094 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 513 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(AIO_READ, aio_read, (yyvsp[-1].aio_read), (yyvsp[0].i)); }
#line 2100 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 514 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(AIO_ERROR, aio_error, (yyvsp[-1].aio_error), (yyvsp[0].i)); }
#line 2106 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 515 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(AIO_RETURN, aio_return, (yyvsp[-1].aio_return), (yyvsp[0].i)); }
#line 2112 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 516 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(AIO_SUSPEND, aio_suspend, (yyvsp[-1].aio_suspend), (yyvsp[0].i)); }
#line 2118 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 517 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_o(LSEEK, lseek, (yyvsp[-1].lseek), (yyvsp[0].i)); }
#line 2124 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 518 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FSYNC, fsync, (yyvsp[-1].fsync), (yyvsp[0].i)); }
#line 2130 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 519 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FCNTL, fcntl, (yyvsp[-1].fcntl), (yyvsp[0].i)); }
#line 2136 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 520 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FTRUNCATE, ftruncate, (yyvsp[-1].ftruncate), (yyvsp[0].i)); }
#line 2142 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 521 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(LINK, link, (yyvsp[-1].link), (yyvsp[0].i)); }
#line 2148 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 522 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(SYMLINK, symlink, (yyvsp[-1].symlink), (yyvsp[0].i)); }
#line 2154 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 523 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(RMDIR, rmdir, (yyvsp[-1].rmdir), (yyvsp[0].i)); }
#line 2160 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 524 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(UNLINK, unlink, (yyvsp[-1].unlink), (yyvsp[0].i)); }
#line 2166 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 525 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(READLINK, readlink, (yyvsp[-1].readlink), (yyvsp[0].i)); }
#line 2172 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 526 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(RENAME, rename, (yyvsp[-1].rename), (yyvsp[0].i)); }
#line 2178 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 527 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(EXCHANGEDATA, exchangedata, (yyvsp[-1].exchangedata), (yyvsp[0].i)); }
#line 2184 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 528 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(MKDIR, mkdir, (yyvsp[-1].mkdir), (yyvsp[0].i)); }
#line 2190 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 529 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FLOCK, flock, (yyvsp[-1].flock), (yyvsp[0].i)); }
#line 2196 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 530 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_p(MMAP, mmap, (yyvsp[-1].mmap), (void*)(intptr_t)(yyvsp[0].i)); }
#line 2202 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 531 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FUTIMES, futimes, (yyvsp[-1].futimes), (yyvsp[0].i)); }
#line 2208 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 532 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(UTIMES, utimes, (yyvsp[-1].utimes), (yyvsp[0].i)); }
#line 2214 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 533 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(STATFS, statfs, (yyvsp[-1].statfs), (yyvsp[0].i)); }
#line 2220 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 534 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(STATFS64, statfs64, (yyvsp[-1].statfs64), (yyvsp[0].i)); }
#line 2226 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 535 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FSTATFS64, fstatfs64, (yyvsp[-1].fstatfs64), (yyvsp[0].i)); }
#line 2232 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 536 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FSTAT, fstat, (yyvsp[-1].fstat), (yyvsp[0].i)); }
#line 2238 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 537 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(LSTAT, lstat, (yyvsp[-1].lstat), (yyvsp[0].i)); }
#line 2244 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 538 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(STAT, stat, (yyvsp[-1].stat), (yyvsp[0].i)); }
#line 2250 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 539 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FSTAT64, fstat64, (yyvsp[-1].fstat64), (yyvsp[0].i)); }
#line 2256 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 540 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(LSTAT64, lstat64, (yyvsp[-1].lstat64), (yyvsp[0].i)); }
#line 2262 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 541 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(STAT64, stat64, (yyvsp[-1].stat64), (yyvsp[0].i)); }
#line 2268 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 542 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(SETATTRLIST, setattrlist, (yyvsp[-1].setattrlist), (yyvsp[0].i)); }
#line 2274 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 543 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(GETATTRLIST, getattrlist, (yyvsp[-1].getattrlist), (yyvsp[0].i)); }
#line 2280 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 544 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(GETXATTR, getxattr, (yyvsp[-1].getxattr), (yyvsp[0].i)); }
#line 2286 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 545 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FGETXATTR, fgetxattr, (yyvsp[-1].fgetxattr), (yyvsp[0].i)); }
#line 2292 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 546 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(LISTXATTR, listxattr, (yyvsp[-1].listxattr), (yyvsp[0].i)); }
#line 2298 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 547 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FLISTXATTR, flistxattr, (yyvsp[-1].flistxattr), (yyvsp[0].i)); }
#line 2304 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 548 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(ACCESS, access, (yyvsp[-1].access), (yyvsp[0].i)); }
#line 2310 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 549 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(CHOWN, chown, (yyvsp[-1].chown), (yyvsp[0].i)); }
#line 2316 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 550 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(CHMOD, chmod, (yyvsp[-1].chmod), (yyvsp[0].i)); }
#line 2322 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 551 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FSETXATTR, fsetxattr, (yyvsp[-1].fsetxattr), (yyvsp[0].i)); }
#line 2328 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 552 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FSGETPATH, fsgetpath, (yyvsp[-1].fsgetpath), (yyvsp[0].i)); }
#line 2334 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 553 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(GETDIRENTRIES64, getdirentries64, (yyvsp[-1].getdirentries64), (yyvsp[0].i)); }
#line 2340 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 554 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(GETDIRENTRIESATTR, getdirentriesattr, (yyvsp[-1].getdirentriesattr), (yyvsp[0].i)); }
#line 2346 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 555 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(GETFSSTAT64, getfsstat64, (yyvsp[-1].getfsstat64), (yyvsp[0].i)); }
#line 2352 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 556 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(GETDTABLESIZE, getdtablesize, (yyvsp[-1].getdtablesize), (yyvsp[0].i)); }
#line 2358 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 557 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(CHDIR, chdir, (yyvsp[-1].chdir), (yyvsp[0].i)); }
#line 2364 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 558 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FCHDIR, fchdir, (yyvsp[-1].fchdir), (yyvsp[0].i)); }
#line 2370 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 559 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(KILL, kill, (yyvsp[-1].kill), (yyvsp[0].i)); }
#line 2376 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 560 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FCHFLAGS, fchflags, (yyvsp[-1].fchflags), (yyvsp[0].i)); }
#line 2382 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 561 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FCHMOD, fchmod, (yyvsp[-1].fchmod), (yyvsp[0].i)); }
#line 2388 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 562 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FCHOWN, fchown, (yyvsp[-1].fchown), (yyvsp[0].i)); }
#line 2394 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 563 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(REMOVEXATTR, removexattr, (yyvsp[-1].removexattr), (yyvsp[0].i)); }
#line 2400 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 564 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(FREMOVEXATTR, fremovexattr, (yyvsp[-1].fremovexattr), (yyvsp[0].i)); }
#line 2406 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 565 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(SETXATTR, setxattr, (yyvsp[-1].setxattr), (yyvsp[0].i)); }
#line 2412 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 566 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(HFS_VNOP_PAGEIN, hfs_vnop_pagein, (yyvsp[-1].hfs_vnop_pagein), (yyvsp[0].i)); }
#line 2418 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 567 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(POSIX_SPAWN, posix_spawn, (yyvsp[-1].posix_spawn), (yyvsp[0].i)); }
#line 2424 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 570 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(CONTROLLER, controller, (yyvsp[0].controller), -1); }
#line 2430 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 571 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(RESOLVER, resolver, (yyvsp[0].resolver), -1); }
#line 2436 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 572 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(ALL_CALLS, all_calls, (yyvsp[0].all_calls), -1); }
#line 2442 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 573 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(SETTER, setter, (yyvsp[0].setter), -1); }
#line 2448 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 574 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(BUILD_PATH, build_path, (yyvsp[0].build_path), -1); }
#line 2454 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 575 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(NAMEI, namei, (yyvsp[0].namei), -1); }
#line 2460 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 576 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(VN_CREATE, vn_create, (yyvsp[0].vn_create), -1); }
#line 2466 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 577 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(SYSCALL_MAP, syscall_map, (yyvsp[0].syscall_map), -1); }
#line 2472 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 583 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(VNODE_PAGEIN, vnode_pagein, (yyvsp[0].vnode_pagein), -1); }
#line 2478 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 584 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.ev) = evstruct_i(EXECVE, execve, (yyvsp[0].execve), -1); }
#line 2484 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 587 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 2490 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 588 "ibench-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 2496 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 591 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.open).path = hack_path((yyvsp[-13].str));
	(yyval.open).flags = (yyvsp[-9].flags);

	(yyval.open).dummy1 = (yyvsp[-3].str);
	(yyval.open).dummy2 = (yyvsp[-1].str);
}
#line 2508 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 599 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.shm_open).name = (yyvsp[-3].str);
	(yyval.shm_open).oflag = (yyvsp[-1].i);
}
#line 2517 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 604 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.close).fd = FD((yyvsp[-1].i));
}
#line 2525 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 608 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.dup).fd = FD((yyvsp[-1].i));
}
#line 2533 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 612 "ibench-parse.y" /* yacc.c:1646  */
    {
}
#line 2540 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 615 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.socket).domain = (yyvsp[-5].i);
	(yyval.socket).type = (yyvsp[-3].i);
	(yyval.socket).protocol = (yyvsp[-1].i);
}
#line 2550 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 621 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.socketpair).domain = (yyvsp[-9].i);
	(yyval.socketpair).type = (yyvsp[-7].i);
	(yyval.socketpair).protocol = (yyvsp[-5].i);

	(yyval.socketpair).fd1 = FD((yyvsp[-3].i));
	(yyval.socketpair).fd2 = FD((yyvsp[-1].i));
}
#line 2563 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 630 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.read).fd = FD((yyvsp[-7].i));
	(yyval.read).count = (yyvsp[-5].i);

	(yyval.read).dummy1 = (yyvsp[-1].str);
}
#line 2574 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 637 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.write).fd = FD((yyvsp[-7].i));
	(yyval.write).count = (yyvsp[-5].i);

	(yyval.write).dummy1 = (yyvsp[-1].str);
}
#line 2585 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 644 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.pread).fd = FD((yyvsp[-7].i));
	(yyval.pread).count = (yyvsp[-5].i);
	(yyval.pread).offset = (yyvsp[-3].i);

	(yyval.pread).dummy1 = (yyvsp[-1].str);
}
#line 2597 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 652 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.pwrite).fd = FD((yyvsp[-7].i));
	(yyval.pwrite).count = (yyvsp[-5].i);
	(yyval.pwrite).offset = (yyvsp[-3].i);

	(yyval.pwrite).dummy1 = (yyvsp[-1].str);
}
#line 2609 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 660 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.aio_read).tag = (yyvsp[-11].i);
	(yyval.aio_read).fd = FD((yyvsp[-9].i));
	(yyval.aio_read).offset = (yyvsp[-7].i);
	(yyval.aio_read).nbytes = (yyvsp[-5].i);

	(yyval.aio_read).dummy1 = (yyvsp[-1].str);
}
#line 2622 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 669 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.aio_error).tag = (yyvsp[-1].i);
	(yyval.aio_error).fd = FD(-1);
}
#line 2631 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 674 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.aio_return).tag = (yyvsp[-1].i);
	(yyval.aio_return).fd = FD(-1);
}
#line 2640 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 679 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.aio_suspend).nent = (yyvsp[-1].i);
}
#line 2648 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 683 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.lseek).fd = FD((yyvsp[-5].i));
	(yyval.lseek).offset = (yyvsp[-3].i);
	(yyval.lseek).whence = (yyvsp[-1].i);
}
#line 2658 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 689 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.fsync).fd = FD((yyvsp[-3].i));

	(yyval.fsync).dummy1 = (yyvsp[-1].str);
}
#line 2668 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 695 "ibench-parse.y" /* yacc.c:1646  */
    {
	/* FIXME: do we have enough info here on all these commands?  (e.g. RDADVISE) */
	(yyval.fcntl).fd = FD((yyvsp[-9].i));
	(yyval.fcntl).cmdnum = (yyvsp[-7].i);
	(yyval.fcntl).arg.l = (yyvsp[-5].i);
	(yyval.fcntl).cmdname = (yyvsp[-3].str);

	(yyval.fcntl).dummy1 = (yyvsp[-1].str);
}
#line 2682 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 705 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.ftruncate).fd = FD((yyvsp[-5].i));
	(yyval.ftruncate).length = (yyvsp[-3].i);

	(yyval.ftruncate).dummy1 = (yyvsp[-1].str);
}
#line 2693 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 712 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.link).oldpath = hack_path((yyvsp[-3].str));
	(yyval.link).newpath = hack_path((yyvsp[-1].str));
}
#line 2702 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 717 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.symlink).oldpath = hack_path((yyvsp[-3].str));
	(yyval.symlink).newpath = hack_path((yyvsp[-1].str));
}
#line 2711 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 722 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.rmdir).pathname = hack_path((yyvsp[-1].str));
}
#line 2719 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 726 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.unlink).pathname = hack_path((yyvsp[-1].str));
}
#line 2727 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 730 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.readlink).path = hack_path((yyvsp[-5].str));
	(yyval.readlink).bufsiz = (yyvsp[-3].i);

	(yyval.readlink).dummy1 = (yyvsp[-1].str);
}
#line 2738 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 736 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.readlink).path = hack_path((yyvsp[-4].str));
	(yyval.readlink).bufsiz = (yyvsp[-2].i);

	(yyval.readlink).dummy1 = NULL;
}
#line 2749 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 743 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.rename).oldpath = hack_path((yyvsp[-13].str));
	(yyval.rename).newpath = hack_path((yyvsp[-11].str));

	(yyval.rename).dummy1 = (yyvsp[-1].str);
}
#line 2760 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 750 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.exchangedata).path1 = hack_path((yyvsp[-7].str));
	(yyval.exchangedata).path2 = hack_path((yyvsp[-5].str));
}
#line 2769 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 755 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.mkdir).pathname = hack_path((yyvsp[-3].str));
	(yyval.mkdir).mode = (yyvsp[-1].i);
}
#line 2778 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 760 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.flock).fd = FD((yyvsp[-5].i));
	(yyval.flock).operation = (yyvsp[-3].i);

	(yyval.flock).dummy1 = (yyvsp[-1].str);
}
#line 2789 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 767 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.mmap).addr = (void*)(intptr_t)(yyvsp[-15].i);
	(yyval.mmap).length = (yyvsp[-13].i);
	(yyval.mmap).prot = (yyvsp[-3].flags);
	(yyval.mmap).flags = (yyvsp[-1].flags);
	(yyval.mmap).fd = FD((yyvsp[-7].i));
	(yyval.mmap).offset = (yyvsp[-5].i); /* FIXME: the values for this in the trace seem wrong. */
}
#line 2802 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 776 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.futimes).fd = FD((yyvsp[-1].i));
}
#line 2810 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 780 "ibench-parse.y" /* yacc.c:1646  */
    {
}
#line 2817 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 783 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.statfs).path = hack_path((yyvsp[-1].str));
}
#line 2825 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 787 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.statfs64).path = hack_path((yyvsp[-1].str));
}
#line 2833 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 791 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.fstatfs64).fd = FD((yyvsp[-1].i));
}
#line 2841 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 795 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.fstat).fd = FD((yyvsp[-1].i));
}
#line 2849 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 799 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.lstat).path = hack_path((yyvsp[-1].str));
}
#line 2857 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 803 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.stat).path = hack_path((yyvsp[-1].str));
}
#line 2865 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 807 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.fstat64).fd = FD((yyvsp[-1].i));
}
#line 2873 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 811 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.lstat64).path = hack_path((yyvsp[-1].str));
}
#line 2881 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 815 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.stat64).path = hack_path((yyvsp[-1].str));
}
#line 2889 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 819 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.setattrlist).path = hack_path((yyvsp[-1].str));
}
#line 2897 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 823 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.getattrlist).path = hack_path((yyvsp[-1].str));
}
#line 2905 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 827 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.getxattr).path = hack_path((yyvsp[-1].str));
}
#line 2913 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 831 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.fgetxattr).fd = FD((yyvsp[-1].i));
}
#line 2921 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 835 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.listxattr).path = hack_path((yyvsp[-1].str));
}
#line 2929 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 839 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.flistxattr).fd = FD((yyvsp[-1].i));
}
#line 2937 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 843 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.access).pathname = hack_path((yyvsp[-3].str));
	(yyval.access).mode = (yyvsp[-1].i);
}
#line 2946 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 848 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.chown).path = hack_path((yyvsp[-5].str));
	(yyval.chown).owner = (yyvsp[-3].i);
	(yyval.chown).group = (yyvsp[-1].i);
}
#line 2956 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 854 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.chmod).path = hack_path((yyvsp[-3].str));
	(yyval.chmod).mode = (yyvsp[-1].i);
}
#line 2965 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 859 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.fsetxattr).fd = FD((yyvsp[-3].i));
	(yyval.fsetxattr).name = (yyvsp[-1].str);
}
#line 2974 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 864 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.fsgetpath).inum = (yyvsp[-1].i);
}
#line 2982 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 868 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.getdirentries64).fd = FD((yyvsp[-1].i));
}
#line 2990 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 872 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.getdirentriesattr).fd = FD((yyvsp[-1].i));
}
#line 2998 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 876 "ibench-parse.y" /* yacc.c:1646  */
    {
}
#line 3005 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 879 "ibench-parse.y" /* yacc.c:1646  */
    {
}
#line 3012 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 882 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.chdir).path = hack_path((yyvsp[-1].str));
}
#line 3020 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 886 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.fchdir).fd = FD((yyvsp[-1].i));
}
#line 3028 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 890 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.kill).pid = (yyvsp[-3].i);
	(yyval.kill).sig = (yyvsp[-1].i);
}
#line 3037 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 895 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.fchflags).fd = FD((yyvsp[-3].i));
	(yyval.fchflags).flags = (yyvsp[-1].i);
}
#line 3046 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 900 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.fchmod).fd = FD((yyvsp[-3].i));
	(yyval.fchmod).mode = (yyvsp[-1].i);
}
#line 3055 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 905 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.fchown).fd = FD((yyvsp[-5].i));
	(yyval.fchown).owner = (yyvsp[-3].i);
	(yyval.fchown).group = (yyvsp[-1].i);
}
#line 3065 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 911 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.removexattr).path = hack_path((yyvsp[-3].str));
	(yyval.removexattr).name = (yyvsp[-1].str);
}
#line 3074 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 916 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.fremovexattr).fd = FD((yyvsp[-3].i));
	(yyval.fremovexattr).name = (yyvsp[-1].str);
}
#line 3083 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 921 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.setxattr).path = hack_path((yyvsp[-3].str));
	(yyval.setxattr).name = (yyvsp[-1].str);
}
#line 3092 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 926 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.controller).msg = (yyvsp[0].str);
}
#line 3100 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 930 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.hfs_vnop_pagein).offset = (yyvsp[-9].i);
	(yyval.hfs_vnop_pagein).size = (yyvsp[-7].i);
	(yyval.hfs_vnop_pagein).name = (yyvsp[-3].str);
	(yyval.hfs_vnop_pagein).actual_size = (yyvsp[-1].i);
}
#line 3111 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 937 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.syscall_map).dummy1 = (yyvsp[0].str);
}
#line 3119 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 941 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.posix_spawn).path = hack_path((yyvsp[-1].str));
}
#line 3127 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 946 "ibench-parse.y" /* yacc.c:1646  */
    { }
#line 3133 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 947 "ibench-parse.y" /* yacc.c:1646  */
    { }
#line 3139 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 948 "ibench-parse.y" /* yacc.c:1646  */
    { }
#line 3145 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 949 "ibench-parse.y" /* yacc.c:1646  */
    { }
#line 3151 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 950 "ibench-parse.y" /* yacc.c:1646  */
    { }
#line 3157 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 951 "ibench-parse.y" /* yacc.c:1646  */
    { }
#line 3163 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 952 "ibench-parse.y" /* yacc.c:1646  */
    { }
#line 3169 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 953 "ibench-parse.y" /* yacc.c:1646  */
    { }
#line 3175 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 955 "ibench-parse.y" /* yacc.c:1646  */
    { assert(0 && "fieldlist"); }
#line 3181 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 956 "ibench-parse.y" /* yacc.c:1646  */
    { assert(0 && "fieldlist"); }
#line 3187 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 965 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
}
#line 3195 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 968 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
	(yyval.flags)->next = (yyvsp[-1].flags);
}
#line 3204 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 974 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
}
#line 3212 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 977 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
	(yyval.flags)->next = (yyvsp[-1].flags);
}
#line 3221 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 983 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
}
#line 3229 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 986 "ibench-parse.y" /* yacc.c:1646  */
    {
	(yyval.flags) = newflag((yyvsp[0].str));
	(yyval.flags)->next = (yyvsp[-1].flags);
}
#line 3238 "ibench-parse.tab.c" /* yacc.c:1646  */
    break;


#line 3242 "ibench-parse.tab.c" /* yacc.c:1646  */
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
#line 992 "ibench-parse.y" /* yacc.c:1906  */


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
