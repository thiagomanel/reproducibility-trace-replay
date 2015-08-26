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

#ifndef YY_IOSTAT_IOSTAT_PARSE_TAB_H_INCLUDED
# define YY_IOSTAT_IOSTAT_PARSE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int iostatdebug;
#endif
/* "%code requires" blocks.  */
#line 19 "iostat-parse.y" /* yacc.c:1909  */


#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>

#include "trace.h"
#include "misc.h"
#include "assoc.h"
#include "event.h"

extern int iostatdebug;

float* parse_iostat(FILE* tracefile, int* num, unsigned long* cpucount);

struct iostatparser_state;


#line 64 "iostat-parse.tab.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    END = 0,
    CPUTAG_USER = 258,
    CPUTAG_NICE = 259,
    CPUTAG_SYSTEM = 260,
    CPUTAG_IOWAIT = 261,
    CPUTAG_STEAL = 262,
    CPUTAG_IDLE = 263,
    AVGCPU = 264,
    DEVICE = 265,
    NEWLINE = 266,
    TAG = 267,
    DVAL = 268,
    CPUCOUNT = 269
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 39 "iostat-parse.y" /* yacc.c:1909  */

	unsigned long ul;
	float f;

#line 97 "iostat-parse.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE iostatlval;

int iostatparse (struct iostatparser_state* state);

#endif /* !YY_IOSTAT_IOSTAT_PARSE_TAB_H_INCLUDED  */
