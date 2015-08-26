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
#include "initparse.tab.h"

extern YYSTYPE initlval;

%}

%option nounput
%option noinput
%option nodefault
%option noyywrap
%option prefix="init"

uint		[0-9]+
rsrcinum	[0-9]+rsrc

newline		[\n]

string		[^#\n]+

delim		#

%%

{uint} {
	char* tmp;
	initlval.ui = strtoll(inittext, &tmp, 10);
	return UINT;
}

{rsrcinum} {
	char* tmp;
	initlval.ui = strtoll(inittext, &tmp, 10);
	assert(!strcmp(tmp, "rsrc"));
	return RSRCINUM;
}

{newline}	return NEWLINE;
{delim}		return DELIM;

dir	{ initlval.type = FT_DIR; return DIRTYPE; }
file	{ initlval.type = FT_FILE; return FILETYPE; }
symlink	{ initlval.type = FT_SYMLINK; return LINKTYPE; }

{string} {
	initlval.str = xstrdup(inittext);
	return PATH;
}

<*>.|\n REJECT;

%%
