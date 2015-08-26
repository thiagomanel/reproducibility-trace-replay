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

#ifndef HTAB_H
#define HTAB_H

#include <stdint.h>

struct hbucket;

struct htab {
	struct hbucket** table;
	unsigned int size, pop;
};

#define HTAB_INIT { .table = NULL, .size = 0, .pop = 0, }

struct htab* new_htab(unsigned int initsize);
unsigned int htpop(const struct htab* ht);

void* htslookup(const struct htab* ht, const char* k);
void* htplookup(const struct htab* ht, uintptr_t p);

void htsinsert(struct htab* ht, char* k, void* v);
void htpinsert(struct htab* ht, uintptr_t p, void* v);

void* htsremove(struct htab* ht, const char* k);
void* htpremove(struct htab* ht, uintptr_t p);

void htsvisit(const struct htab* ht, void (*fn)(const char* key, void* val, void* arg),
              void* arg);
void htpvisit(const struct htab* ht, void (*fn)(uintptr_t p, void* val, void* arg),
              void* arg);

void free_shtab(struct htab* ht, void (*fn)(char* key, void* val, void* arg),
                void* arg);
void free_phtab(struct htab* ht, void (*fn)(uintptr_t p, void* val, void* arg),
                void* arg);

#endif /* HTAB_H */
