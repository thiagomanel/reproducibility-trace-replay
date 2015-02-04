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

}

%union {
	unsigned long ul;
	float f;
};

%code {

#include "iostat-lex.yy.h"

struct iostatparser_state {
	int linenum;
	unsigned long cpucount;
	float* array;
	int size, used;
};

static void iostaterror(struct iostatparser_state*, char const*);

static void append_dval(float d, struct iostatparser_state* ps)
{
	if (ps->used == ps->size) {
		ps->size *= 1.5;
		ps->size += 8;
		ps->array = xrealloc(ps->array, ps->size * sizeof(*ps->array));
	}

	ps->array[ps->used++] = d;
}

}

%{

%}

%token CPUTAG_USER CPUTAG_NICE CPUTAG_SYSTEM
%token CPUTAG_IOWAIT CPUTAG_STEAL CPUTAG_IDLE

%token AVGCPU DEVICE
%token NEWLINE
%token TAG

%token <f> DVAL
%token <ul> CPUCOUNT

%token END 0 "EOF"

%type <f> cpuvalues cpuchunk chunk

%debug

%parse-param {struct iostatparser_state* state}

%name-prefix "iostat"

%error-verbose

%%

input: header chunk {
	/* First chunk is just "stats-since-boot", ignore it */
}
| input chunk {
	append_dval($2, state);
};

/* OS type, hostname, date, etc. */
header: TAG TAG TAG TAG TAG CPUCOUNT NEWLINE NEWLINE {
	state->linenum += 1;
	state->cpucount = $6;
};

/* The periodic chunk of CPU/IO data */
chunk: cpuchunk devicechunk {
	$$ = $1;
};

cpuchunk: cpuheader cpuvalues NEWLINE {
	$$ = $2;
	state->linenum += 1;
};

cpuheader: AVGCPU CPUTAG_USER other_cputags NEWLINE {
	state->linenum += 1;
};

cpuvalues: DVAL more_dvals NEWLINE {
	/* iostat prints percentages, we convert to a plain fraction here. */
	$$ = $1 / 100;

	state->linenum += 1;
};

other_cputags: /* empty */
| other_cputags CPUTAG_NICE { }
| other_cputags CPUTAG_SYSTEM { }
| other_cputags CPUTAG_IOWAIT { }
| other_cputags CPUTAG_STEAL { }
| other_cputags CPUTAG_IDLE { };

more_dvals: /* empty */
| more_dvals DVAL { };

devicechunk: deviceheader devicelines NEWLINE { };

deviceheader: DEVICE ignored_tags NEWLINE {
	state->linenum += 1;
};

ignored_tags: /* empty */
| ignored_tags TAG { };

devicelines: /* empty */
| devicelines deviceline { };

deviceline: TAG more_dvals NEWLINE {
	state->linenum += 1;
};

%%

float* parse_iostat(FILE* tracefile, int* num, unsigned long* cpucount)
{
	int status;
	struct iostatparser_state pstate = {
		.linenum = 1,
		.cpucount = 0,
		.array = NULL,
		.size = 0,
		.used = 0,
	};

	iostatrestart(tracefile);

	status = iostatparse(&pstate);

	iostatlex_destroy();

	if (!status) {
		*num = pstate.used;
		*cpucount = pstate.cpucount;
		return pstate.array;
	} else {
		*num = 0;
		*cpucount = 0;
		xfree(pstate.array);
		return NULL;
	}
}

static void iostaterror(struct iostatparser_state* state, char const* s)
{
	fprintf(stderr, "line %d: %s\n", state->linenum, s);
}
