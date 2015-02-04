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
/*
 * An in-memory model of filesystem structure.
 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "fsmodel.h"
#include "htab.h"
#include "misc.h"

/*
 * SunOS omits this, though their headers give a pretty convincing reason
 * for doing so.
 */
#ifndef NAME_MAX
#define NAME_MAX _XOPEN_NAME_MAX
#endif

/*
 * Write the name of the first element in 'path' into 'name' (assumed
 * to be NAME_MAX in size or larger).  No slashes are written into
 * 'name'.  The length of 'name' plus any slashes before it is
 * returned (i.e. the offset from 'path' to the character immediately
 * following its first element, possibly NUL).
 */
static size_t get_name(const char* path, char* name)
{
	size_t namelen;
	const char* p = path + strspn(path, "/");
	namelen = strcspn(p, "/");
	memcpy(name, p, namelen);
	name[namelen] = '\0';
	return p + namelen - path;
}

/*
 * Write the prefix of 'path' comprising all but its last component into 'par'
 * (assumed to be PATH_MAX in size or larger).  Trailing slashes will be
 * removed from 'par'.  The returned value is the offset from 'path' to the
 * first character in its last component.
 */
static size_t get_parent(const char* path, char* par)
{
	char* p;
	char* q;
	strcpy(par, path);
	p = par + strlen(par) - 1;

	/* If path has no slashes in it, parent is '.', with offset zero. */
	if (!strchr(path, '/')) {
		strcpy(par, ".");
		return 0;
	}

	/* Trim any trailing slashes, if present */
	while (p > par && *p == '/')
		--p;

	/* Trim last path component */
	while (p > par && *p != '/')
		--p;

	q = p;

	/* Another round of slash-removal */
	while (p > par && *p == '/')
		--p;

	/* Truncate at last non-terminal slash (unless it's /) */
	p[1] = '\0';

	return q + 1 - par;
}

static inline void ref_pathent(const struct fsmodel* fsm, struct pathent* pe)
{
	if (fsm->record_uses)
		pe->flags |= PE_USED;
}

static struct pathent* lookup_dirent(struct pathent* dirpe, const char* name)
{
	assert(path_lastgen(dirpe)->info->type == FT_DIR);

	if (!strcmp(name, "."))
		return dirpe;
	else if (!strcmp(name, ".."))
		return path_lastgen(dirpe)->info->dir.parent;
	else
		return htslookup(path_lastgen(dirpe)->info->dir.entries, name);
}

static void insert_dirent(struct pathent* dirpe, const char* name,
                          struct pathent* newent)
{
	assert(path_lastgen(dirpe)->info->type == FT_DIR);

	if (!strcmp(name, ".") || !strcmp(name, "..")) {
		fprintf(stderr, "can't insert special entry '%s'\n", name);
		abort();
	} else {
		htsinsert(path_lastgen(dirpe)->info->dir.entries, xstrdup(name), newent);
		if (path_lastgen(newent)->info->type == FT_DIR)
			path_lastgen(newent)->info->dir.parent = dirpe;
	}
}

/* Forward declaration for mutual recursion with fsm_follow_symlink() */
static struct pathent* fsm_follow_symlink(struct fsmodel* fsm, struct pathent* dir,
                                          struct pathgen* pg, int deadok);

static struct pathent* fsm_lookup_rel(struct fsmodel* fsm, struct pathent* dir,
                                      const char* relpath, int follow, int deadok)
{
	char name[NAME_MAX];
	struct pathent* pe;
	struct pathent* tmp;
	struct pathgen* pg;
	const char* rp = relpath;

	if (path_lastgen(dir)->info->type != FT_DIR) {
		fprintf(stderr, "Lookup in non-directory\n");
		abort();
	}

	rp += get_name(rp, name);

	if (!strcmp(name, ""))
		return dir;

	if (!pathent_alive(dir) && !deadok)
		return NULL;

	pe = lookup_dirent(dir, name);
	if (!pe || (!pathent_alive(pe) && !deadok))
		return NULL;

	ref_pathent(fsm, pe);

	pg = path_lastgen(pe);

	switch (pg->info->type) {
	case FT_HOSTLINK:
	case FT_FILE:
		if (*rp) {
			fprintf(stderr, "'%s' is not a directory\n", name);
			abort();
		} else
			return pe;
		break;

	case FT_DIR:
		/* Don't lookup again if the remainder is just trailing slashes */
		if (*rp && strspn(rp, "/") != strlen(rp))
			return fsm_lookup_rel(fsm, pe, rp, follow, deadok);
		else
			return pe;
		break;

	case FT_SYMLINK:
		if (!*rp) {
			if (follow)
				return fsm_follow_symlink(fsm, dir, pg, deadok);
			else
				return pe;
		} else {
			tmp = fsm_follow_symlink(fsm, dir, pg, deadok);
			if (!tmp)
				return NULL;
			return fsm_lookup_rel(fsm, tmp, rp, follow, deadok);
		}
		break;

	default:
		abort();
	}
}

static struct pathent* fsm_follow_symlink(struct fsmodel* fsm, struct pathent* dir,
                                          struct pathgen* pg, int deadok)
{
	struct pathent* pe;
	struct pathgen* link = pg;

	if (pg->info->type != FT_SYMLINK) {
		fprintf(stderr, "can't follow non-symlink!\n");
		abort();
	}

	while (link->info->type == FT_SYMLINK) {
		if (link->info->target[0] == '/')
			pe = fsm_lookup_rel(fsm, &fsm->root, link->info->target, 1,
			                    deadok);
		else
			pe = fsm_lookup_rel(fsm, dir, link->info->target, 1, deadok);

		if (!pe)
			return NULL;
		else
			link = path_lastgen(pe);
	}

	return pe;
}

/*
 * Return the pathent for 'path' in 'fsm'.  If 'flags' does not include
 * FSM_FOLLOW and 'path' refers to a symlink, the pathent for the symlink
 * itself is returned; if 'path' is a symlink and 'flags' includes FSM_FOLLOW,
 * the symlink is dereferenced (repeatedly, if necessary) and the pathent for
 * its target is returned if it exists (NULL is returned if it does not).
 */
struct pathent* fsm_lookup(struct fsmodel* fsm, const char* path, unsigned int flags)
{
	if (flags & ~(FSM_FOLLOW|FSM_DEADOK)) {
		fprintf(stderr, "Invalid flags to fsm_lookup: %u\n", flags);
		abort();
	}
	ref_pathent(fsm, &fsm->root);
	return fsm_lookup_rel(fsm, &fsm->root, path, !!(flags & FSM_FOLLOW),
	                      !!(flags & FSM_DEADOK));
}

static struct pathent* fsm_insert(struct fsmodel* fsm, const char* path,
                                  const struct pathgen* pg, unsigned int flags)
{
	const char* name;
	char parpath[PATH_MAX];
	struct pathent* pardir;
	struct pathgen* pargen;
	struct pathent* pe;

	name = path + get_parent(path, parpath);
	if (!strcmp(parpath, "."))
		strcpy(parpath, "/");

	pardir = fsm_lookup(fsm, parpath, FSM_FOLLOW);
	if (!pardir) {
		if (flags & FSM_CREATEPARENTS)
			pardir = fsm_mkdir(fsm, parpath, flags);
		else {
			fprintf(stderr, "can't insert into nonexistent path '%s'!\n", parpath);
			abort();
		}
	}

	pargen = path_lastgen(pardir);

	if (pargen->info->type != FT_DIR) {
		fprintf(stderr, "can't insert into non-directory '%s'!\n", parpath);
		abort();
	}

	pe = lookup_dirent(pardir, name);

	if (!pe) {
		pe = xmalloc(sizeof(*pe));
		*pe = (struct pathent) {
			.gen0 = pg ? *pg : (struct pathgen) {},
			.gencnt = pg ? 1 : 0,
			.xgens = NULL,
			.numsymlinks = 0,
			.symlinks = NULL,
			.flags = (((flags & FSM_FROMINIT) ? PE_FROMINIT : 0)
			          | (pg ? PE_ALIVE : 0)),
		};
		insert_dirent(pardir, name, pe);
	} else {
		assert(pg);
		pe->flags |= PE_ALIVE;
		*path_newgen(pe) = *pg;
	}

	ref_pathent(fsm, pe);

	return pe;
}

struct pathent* fsm_mkdir(struct fsmodel* fsm, const char* path, unsigned int flags)
{
	struct pathgen pg;
	struct pginfo* pgi = xmalloc(sizeof(*pgi));

	*pgi = (struct pginfo) {
		.refcnt = 1,
		.evcount = 0,
		.id = fsm->pgictr++,
		.type = FT_DIR,
		{ .dir = { .entries = new_htab(32), .parent = NULL, }, },
	};

	pg = (struct pathgen) {
		.id = fsm->idctr++,
		.usecnt = 0,
		.info = pgi,
	};

	return fsm_insert(fsm, path, &pg, flags);
}

struct pathent* fsm_creat(struct fsmodel* fsm, const char* path, off_t size,
                          unsigned int flags)
{
	struct pathgen pg;
	struct pginfo* pgi = xmalloc(sizeof(*pgi));

	*pgi = (struct pginfo) {
		.refcnt = 1,
		.evcount = 0,
		.id = fsm->pgictr++,
		.type = FT_FILE,
		{ .size = size, },
	};

	pg = (struct pathgen) {
		.id = fsm->idctr++,
		.usecnt = 0,
		.info = pgi,
	};

	return fsm_insert(fsm, path, &pg, flags);
}

struct pathent* fsm_symlink(struct fsmodel* fsm, char* oldpath, const char* newpath,
                            unsigned int flags)
{
	struct pathgen pg;
	struct pathent* newpe;
	struct pginfo* pgi = xmalloc(sizeof(*pgi));

	*pgi = (struct pginfo) {
		.refcnt = 1,
		.evcount = 0,
		.id = fsm->pgictr++,
		.type = FT_SYMLINK,
		{ .target = oldpath, },
	};

	pg = (struct pathgen) {
		.id = fsm->idctr++,
		.usecnt = 0,
		.info = pgi,
	};

	newpe = fsm_insert(fsm, newpath, &pg, flags);

	return newpe;
}

struct fsm_visit_args {
	char* pathbuf;
	void* arg;
	fsm_visitor_t fn;
};

/* Forward declaration for mutual recursion with fsm_visit_inner */
static void fsm_dirent_visit(const char* key, void* val, void* arg);

static void fsm_visit_inner(struct pathent* pe, int gen, char* pathbuf, fsm_visitor_t fn,
                            void* arg)
{
	struct fsm_visit_args fva;
	struct pathgen* pg = path_getgen(pe, gen);

	fn(pathbuf, pe, pg, arg);

	if (pg->info->type == FT_DIR) {
		fva.pathbuf = pathbuf;
		fva.arg = arg;
		fva.fn = fn;
		htsvisit(pg->info->dir.entries, fsm_dirent_visit, &fva);
	}
}

static void fsm_dirent_visit(const char* key, void* val, void* arg)
{
	int i;
	struct pathent* pe = val;
	struct fsm_visit_args* fva = arg;
	char* p = fva->pathbuf + strlen(fva->pathbuf);

	p[0] = '/';
	strcpy(p + 1, key);

	for (i = 0; i < pe->gencnt; i++)
		fsm_visit_inner(pe, i, fva->pathbuf, fva->fn, fva->arg);

	p[0] = '\0';
}

void fsm_visit_subtree(struct pathent* pe, fsm_visitor_t fn, void* arg)
{
	int i;
	char pathbuf[PATH_MAX] = "";

	for (i = 0; i < pe->gencnt; i++)
		fsm_visit_inner(pe, i, pathbuf, fn, arg);
}

void fsm_visit(struct fsmodel* fsm, fsm_visitor_t fn, void* arg)
{
	fsm_visit_subtree(&fsm->root, fn, arg);
}

struct cdearg {
	struct fsmodel* fsm;
	struct htab* dst;
};

/* Forward declaration for mutual recursion with copy_dirent() */
static struct pathent* copy_tree(struct fsmodel* fsm, struct pathent* orig);

static void copy_dirent(const char* key, void* val, void* arg)
{
	struct cdearg* args = arg;
	struct pathent* newent = copy_tree(args->fsm, val);

	if (newent)
		htsinsert(args->dst, xstrdup(key), newent);
}

static void mark_dead(const char* path, struct pathent* pe, struct pathgen* pg,
                      void* arg)
{
	pe->flags &= ~PE_ALIVE;
}

/* FIXME: I think this probably could/should be structured as a _visit(). */
static struct pathent* copy_tree(struct fsmodel* fsm, struct pathent* orig)
{
	struct pathent* new;
	struct cdearg cdearg;
	struct pginfo* oldinfo = path_lastgen(orig)->info;
	struct pginfo* newinfo;

	if (!pathent_alive(orig))
		return NULL;

	newinfo = xmalloc(sizeof(*newinfo));
	newinfo->refcnt = 1;
	newinfo->evcount = 0;
	newinfo->id = fsm->pgictr++,
	newinfo->type = oldinfo->type;

	new = xmalloc(sizeof(*new));
	*new = (struct pathent) {
		.gen0 = (struct pathgen) {
			.id = fsm->idctr++,
			.usecnt = 0,
			.info = newinfo,
		},
		.gencnt = 1,
		.xgens = NULL,
		.numsymlinks = 0,
		.symlinks = NULL,
		.flags = PE_ALIVE,
	};

	/* Mark the existing path as referred to */
	ref_pathent(fsm, orig);

	switch (newinfo->type) {
	case FT_FILE:
		newinfo->size = oldinfo->size;
		break;

	case FT_SYMLINK:
		newinfo->target = xstrdup(oldinfo->target);
		break;

	case FT_DIR:
		newinfo->dir.entries = new_htab(16);
		newinfo->dir.parent = NULL;
		cdearg.fsm = fsm;
		cdearg.dst = newinfo->dir.entries;
		htsvisit(oldinfo->dir.entries, copy_dirent, &cdearg);
		break;

	default:
		abort();
	}

	new->flags = orig->flags & ~PE_FROMINIT;

	return new;
}

struct pathent* fsm_rename(struct fsmodel* fsm, const char* oldpath,
                           const char* newpath)
{
	struct pathent* oldpe;
	struct pathent* tmp;
	struct pathent* newpe;
	char pathbuf[PATH_MAX] = "";

	oldpe = fsm_lookup(fsm, oldpath, 0);

	if (!oldpe)
		return NULL;

	tmp = copy_tree(fsm, oldpe);

	if (!tmp)
		return NULL;

	newpe = fsm_insert(fsm, newpath, &tmp->gen0, 0);

	xfree(tmp);

	fsm_visit_inner(oldpe, oldpe->gencnt - 1, pathbuf, mark_dead, NULL);
	oldpe->flags &= ~PE_ALIVE;

	return newpe;
}

struct pathent* fsm_remove(struct fsmodel* fsm, const char* path)
{
	struct pathent* pe;

	pe = fsm_lookup(fsm, path, 0);

	if (!pe) {
		fprintf(stderr, "'%s' not found\n", path);
		abort();
	}

	if (!pathent_alive(pe)) {
		fprintf(stderr, "remove target '%s' already dead\n", path);
		abort();
	}

	pe->flags &= ~PE_ALIVE;

	return pe;
}

/* Forward declaration for mutual recursion with fsm_free_inner */
static void fsm_free_dirent(char* key, void* val, void* arg);

static void fsm_free_inner(struct pathent* pe, int gen)
{
	struct pathgen* pg = path_getgen(pe, gen);

	if (--pg->info->refcnt)
		return;

	switch (pg->info->type) {
	case FT_DIR:
		free_shtab(pg->info->dir.entries, fsm_free_dirent, NULL);
		xfree(pg->info->dir.entries);
		break;

	case FT_SYMLINK:
	case FT_HOSTLINK:
		xfree(pg->info->target);
		break;

	default:
		break;
	}

	xfree(pg->info);
}

static void fsm_free_dirent(char* key, void* val, void* arg)
{
	int i;
	struct pathent* pe = val;

	for (i = 0; i < pe->gencnt; i++)
		fsm_free_inner(pe, i);

	if (pe->xgens)
		xfree(pe->xgens);
	if (pe->symlinks)
		xfree(pe->symlinks);
	xfree(pe);
	xfree(key);
}

void fsm_free(struct fsmodel* fsm)
{
	int i;

	for (i = 0; i < fsm->root.gencnt; i++)
		fsm_free_inner(&fsm->root, i);

	xfree(fsm);
}

struct fsmodel* new_fsmodel(void)
{
	struct fsmodel* fsm = xmalloc(sizeof(*fsm));
	struct pginfo* pgi = xmalloc(sizeof(*pgi));

	*pgi = (struct pginfo) {
		.refcnt = 1,
		.evcount = 0,
		.id = 0,
		.type = FT_DIR,
		{ .dir = { .entries = new_htab(16), .parent = &fsm->root, }, },
	};

	fsm->idctr = 1;
	fsm->pgictr = 1;
	fsm->record_uses = 0;
	fsm->root = (struct pathent) {
		.gen0 = (struct pathgen) {
			.id = 0,
			.usecnt = 0,
			.info = pgi,
		},
		.gencnt = 1,
		.xgens = NULL,
		.flags = PE_FROMINIT|PE_ALIVE,
	};

	return fsm;
}
