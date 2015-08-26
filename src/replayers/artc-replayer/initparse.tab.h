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

#ifndef YY_INIT_INITPARSE_TAB_H_INCLUDED
# define YY_INIT_INITPARSE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int initdebug;
#endif
/* "%code requires" blocks.  */
#line 19 "initparse.y" /* yacc.c:1909  */


#define _GNU_SOURCE /* asprintf */
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>

#include "fsmodel.h"
#include "trace.h"
#include "misc.h"

extern int initdebug;

struct initent* parse_init(FILE* initfile);

struct initparser_state;


#line 64 "initparse.tab.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    END = 0,
    UINT = 258,
    RSRCINUM = 259,
    SIZE = 260,
    PATH = 261,
    FILETYPE = 262,
    DIRTYPE = 263,
    LINKTYPE = 264,
    NEWLINE = 265,
    DELIM = 266
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 39 "initparse.y" /* yacc.c:1909  */

	uint64_t ui;
	char* str;
	filetype_t type;
	struct initent ent;

#line 96 "initparse.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE initlval;

int initparse (struct initparser_state* state);

#endif /* !YY_INIT_INITPARSE_TAB_H_INCLUDED  */
