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
%code requires {

#define _GNU_SOURCE /* asprintf */
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>

#include "fsmodel.h"
#include "trace.h"
#include "misc.h"

extern int ibench_initdebug;

struct initent* parse_ibench_init(FILE* initfile);

struct initparser_state;

}

%union {
	uint64_t ui;
	char* str;
	filetype_t type;
	struct initent ent;
};

%code {
#include "ibench-initlex.yy.h"

struct initparser_state {
	int linenum;
	struct initent* ents;
	struct initent* last;
};

static void ibench_initerror(struct initparser_state*, char const*);

}

%{

%}

%token <ui> UINT
%token <ui> RSRCINUM

%token <ui> SIZE
%token <str> PATH

%token <type> FILETYPE
%token <type> DIRTYPE
%token <type> LINKTYPE

/* start-of-record, field delimiter, end-of-record */
%token SOR DELIM EOR

%token END 0 "EOF"

%type <ent> entry initent

%debug

%parse-param {struct initparser_state* state}

%name-prefix "ibench_init"

%error-verbose

%%

input: /* empty */
| input entry {
	struct initent* ent;

	if ($2.type == FT_RSRCFORK)
		goto skip_resource_fork;

	ent = xmalloc(sizeof(struct initent));
	*ent = $2;
	ent->initline = state->linenum;

	if (!state->ents) {
		state->last = state->ents = ent;
	} else {
		state->last->next = ent;
		state->last = ent;
	}

skip_resource_fork:

	state->linenum++;
}
;

entry: SOR initent EOR {
	$$ = $2;
};

initent: DIRTYPE DELIM PATH {
	$$.path = $3;
	$$.type = $1;
	$$.next = NULL;
}
| FILETYPE DELIM UINT DELIM PATH DELIM UINT {
	/* iBench-style init listings with inode numbers for regular files */
	$$.path = $5;
	$$.type = $1;
	$$.size = $7;
	$$.next = NULL;
}
| FILETYPE DELIM PATH DELIM UINT {
	$$.path = $3;
	$$.type = $1;
	$$.size = $5;
}
| FILETYPE DELIM RSRCINUM DELIM PATH DELIM UINT {
	$$.type = FT_RSRCFORK;
	xfree($5);
}
| LINKTYPE DELIM PATH DELIM PATH {
	$$.path = $3;
	$$.type = $1;
	$$.target = $5;
}
/*
 * Handle the case where the target of a symlink is a purely numeric
 * path.
 */
| LINKTYPE DELIM PATH DELIM UINT {
	$$.path = $3;
	$$.type = $1;
	if (asprintf(&$$.target, "%"PRIu64"u", $5) < 0) {
		fprintf(stderr, "asprintf(): %s\n", strerror(errno));
		abort();
	}
}
;

%%

struct initent* parse_ibench_init(FILE* initfile)
{
	int status;
	struct initparser_state pstate = {
		.linenum = 1,
		.ents = NULL,
		.last = NULL,
	};

	ibench_initrestart(initfile);

	status = ibench_initparse(&pstate);

	ibench_initlex_destroy();

	return status ? NULL : pstate.ents;
}

static void ibench_initerror(struct initparser_state* state, char const* s)
{
	fprintf(stderr, "line %d: %s\n", state->linenum, s);
}
