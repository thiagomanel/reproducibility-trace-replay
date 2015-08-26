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

#include "trace.h"
#include "misc.h"
#include "iostat-parse.tab.h"

extern YYSTYPE iostatlval;

%}

%option nounput
%option noinput
%option nodefault
%option noyywrap
%option yylineno
%option prefix="iostat"

cpucount	\([0-9]+\ CPU\)
dval		[0-9]+\.[0-9]+
space		[ \t]+
tag		[[:^space:]]+
newline		[\n]

%%

"avg-cpu:"	return AVGCPU;
"Device:"	return DEVICE;

"%user"		return CPUTAG_USER;
"%nice"		return CPUTAG_NICE;
"%system"	return CPUTAG_SYSTEM;
"%iowait"	return CPUTAG_IOWAIT;
"%steal"	return CPUTAG_STEAL;
"%idle"		return CPUTAG_IDLE;

{cpucount} {
	char* tmp;
	assert(iostattext[0] == '(');
	iostatlval.ul = strtoul(iostattext+1, &tmp, 10);

	if (*tmp != ' ')
		abort();

	return CPUCOUNT;
}

{dval}	{
	char* tmp;
	iostatlval.f = strtod(iostattext, &tmp);

	/* Pattern matching should prevent this from occurring. */
	if (*tmp || tmp == iostattext)
		abort();

	return DVAL;
}

{space}
{newline}	return NEWLINE;
{tag}		return TAG;

<*>.|\n {
	fprintf(stderr, "unrecognized token at line %d: %s\n", iostatlineno,
	        iostattext);
	exit(1);
}

%%
