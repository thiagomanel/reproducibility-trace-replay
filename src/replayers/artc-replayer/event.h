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

#ifndef EVENT_H
#define EVENT_H

#include "trace.h"

#ifndef ARTC
#define runtime_const const
#else
#define runtime_const
#endif /* ARTC */

#ifdef ARTC
#define EVENT_MAXPATHS 3

extern const char* benchroot;

void free_event(struct event* ev);
void free_event_temporal_deps(struct event* ev);
void merge_event(struct event* entry, const struct event* exit);
size_t event_getiobufsize(const struct event* ev);
int event_should_succeed(const struct event* ev);
int event_elidable(const struct event* ev);
const char* event_type_enumname(evid_t t);
void event_genattrs(const struct event* ev, FILE* output);

char* event_get_paths(const struct event* ev, char* used[EVENT_MAXPATHS],
                      int follows[EVENT_MAXPATHS], char** removed, char** negdep);

int flags_include(const struct symflag* flags, const char* s);
unsigned int remove_flag(struct symflag** flags, const char* name);

const char* event_get_accessed_path(const struct event* ev);
void event_get_raw_paths(struct event* ev, char*** p1, char*** p2);

#else

int event_type_uses_fd(evid_t t);
#endif /* ARTC */

int event_returns_fd(const struct event* ev);
int event_unordered(const struct event* e);

const char* event_type_name(evid_t t);

typedef int (*fd_visitor_t)(runtime_const struct event*, runtime_const struct fduse*, void*);
void event_visit_fds(runtime_const struct event* ev, fd_visitor_t fn, void* arg);

#endif /* EVENT_H */
