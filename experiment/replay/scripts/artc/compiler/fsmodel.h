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
#ifndef FSMODEL_H
#define FSMODEL_H

#include <assert.h>
#include <sys/types.h>

#ifdef ARTC
#include "misc.h"
#include "htab.h"
#else
#include "trace.h"
#endif

typedef enum {
	FT_DIR,
	FT_FILE,
	FT_SYMLINK,
#ifdef ARTC
	/* Magic symlink type relative to host system's FS root */
	FT_HOSTLINK,
	FT_RSRCFORK,
#endif
} filetype_t;

#ifdef ARTC
struct initent {
	char* path;
	int initline;
	filetype_t type;
	union {
		off_t size;
		char* target;
	};
	struct initent* next;
};
#else
struct initpath {
	char* path;
	int idx;
};
#endif

struct pginfo {
#ifdef ARTC
	unsigned int refcnt;
	unsigned int evcount;
	int id;
#endif
	filetype_t type;
	union {
		off_t size;
		char* target;
#ifdef ARTC
		struct {
			struct htab* entries;
			struct pathent* parent;
		} dir;
#endif
	};
};

/*
 * Describes one generation of a path.
 */
struct pathgen {
#ifdef ARTC
	/* Unique id across all generations of all paths. */
	unsigned int id;
#else
	struct resstatus status;
#endif
	unsigned int usecnt;
#ifdef ARTC
	struct pginfo* info;
#else
	struct pginfo info;
#endif
};

#ifdef ARTC

/* for struct pathent 'flags' field */
#define PE_FROMINIT 1
#define PE_ALIVE 2
#define PE_USED 4

struct pathent {
	struct pathgen gen0;
	unsigned int gencnt;
	struct pathgen* xgens;

	unsigned int numsymlinks;
	struct pathent** symlinks;

	uint8_t flags;
};

static inline int pathent_frominit(const struct pathent* pe)
{
	return !!(pe->flags & PE_FROMINIT);
}

static inline int pathent_alive(const struct pathent* pe)
{
	return !!(pe->flags & PE_ALIVE);
}

static inline int pathent_used(const struct pathent* pe)
{
	return !!(pe->flags & PE_USED);
}

struct fsmodel {
	unsigned int idctr, pgictr;
	int record_uses;
	struct pathent root;
};

struct fsmodel* new_fsmodel(void);

static inline struct pathgen* path_getgen(struct pathent* p, unsigned int g)
{
	if (!g)
		return &p->gen0;

	assert(g < p->gencnt);
	return &p->xgens[g-1];
}

static inline struct pathgen* path_lastgen(struct pathent* p)
{
	assert(p->gencnt);
	return path_getgen(p, p->gencnt - 1);
}

static inline struct pathgen* path_newgen(struct pathent* p)
{
	if (!p->gencnt) {
		p->gencnt++;
		return &p->gen0;
	}

	/* gencnt++ instead of ++gencount because gen0 is inlined */
	p->xgens = xrealloc(p->xgens, p->gencnt++ * sizeof(*p->xgens));
	return path_lastgen(p);
}

#define FSM_FOLLOW 1
#define FSM_CREATEPARENTS 2
#define FSM_FROMINIT 4
#define FSM_DEADOK 8

struct pathent* fsm_lookup(struct fsmodel* fsm, const char* path, unsigned int flags);
struct pathent* fsm_mkdir(struct fsmodel* fsm, const char* path, unsigned int flags);
struct pathent* fsm_creat(struct fsmodel* fsm, const char* path, off_t size,
                          unsigned int flags);
struct pathent* fsm_symlink(struct fsmodel* fsm, char* oldpath, const char* newpath,
                            unsigned int flags);
struct pathent* fsm_rename(struct fsmodel* fsm, const char* oldpath,
                           const char* newpath);
struct pathent* fsm_remove(struct fsmodel* fsm, const char* path);

typedef void (*fsm_visitor_t)(const char* path, struct pathent* pe,
                              struct pathgen* pg, void* arg);

void fsm_visit(struct fsmodel* fsm, fsm_visitor_t fn, void* arg);
void fsm_visit_subtree(struct pathent* pe, fsm_visitor_t fn, void* arg);

void fsm_free(struct fsmodel* fsm);
#endif

#endif /* FSMODEL_H */
