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
 * This is...not terribly efficient.  Oh well.
 */

#include <stdlib.h>
#include <stdint.h>

#include "misc.h"
#include "htab.h"
#include "fnv.h"

struct hbucket {
	union {
		char* s;
		uintptr_t p;
	} key;
	struct hbucket* next;
	void* value;
};

unsigned int htpop(const struct htab* ht)
{
	return ht->pop;
}

/* Threshold at which we expand */
#define RESIZE_PCT 80

#define GROWTH_FACTOR 1.5

struct htab* new_htab(unsigned int initsize)
{
	struct htab* ht;

	/* Don't be silly. */
	if (initsize < 2)
		initsize = 2;

	ht = xmalloc(sizeof(*ht));

	ht->size = initsize;
	ht->pop = 0;
	ht->table = xcalloc(initsize * sizeof(*ht->table));

	return ht;
}

void* htslookup(const struct htab* ht, const char* k)
{
	struct hbucket* b;

	if (!ht->pop)
		return NULL;

	for (b = ht->table[fnv1as(k) % ht->size]; b; b = b->next) {
		if (!strcmp(k, b->key.s))
			return b->value;
	}

	return NULL;
}

void* htplookup(const struct htab* ht, uintptr_t p)
{
	struct hbucket* b;

	if (!ht->pop)
		return NULL;

	for (b = ht->table[fnv1a(&p, sizeof(p)) % ht->size]; b; b = b->next) {
		if (b->key.p == p)
			return b->value;
	}

	return NULL;
}

static void place_bucket(struct htab* ht, int string, struct hbucket* b)
{
	unsigned int idx = string ? fnv1as(b->key.s) : fnv1a(&b->key.p, sizeof(b->key.p));

	idx %= ht->size;

	b->next = ht->table[idx];
	ht->table[idx] = b;
}

static void grow_table(struct htab* ht, int string)
{
	struct hbucket* b;
	struct hbucket* next;
	unsigned int i, oldsize = ht->size;
	struct hbucket** oldtable = ht->table;

	ht->size *= GROWTH_FACTOR;
	ht->size += 8;
	ht->table = xcalloc(ht->size * sizeof(*ht->table));

	for (i = 0; i < oldsize; i++) {
		for (b = oldtable[i]; b; b = next) {
			next = b->next;
			place_bucket(ht, string, b);
		}
	}

	xfree(oldtable);
}

void htsinsert(struct htab* ht, char* k, void* v)
{
	struct hbucket* b;

	if (ht->pop == ht->size || ht->pop * 100 / ht->size >= RESIZE_PCT)
		grow_table(ht, 1);

	b = xmalloc(sizeof(*b));
	b->key.s = k;
	b->value = v;

	place_bucket(ht, 1, b);
	ht->pop += 1;
}

void htpinsert(struct htab* ht, uintptr_t p, void* v)
{
	struct hbucket* b;

	if (ht->pop == ht->size || ht->pop * 100 / ht->size >= RESIZE_PCT)
		grow_table(ht, 0);

	b = xmalloc(sizeof(*b));
	b->key.p = p;
	b->value = v;

	place_bucket(ht, 0, b);
	ht->pop += 1;
}

void* htsremove(struct htab* ht, const char* k)
{
	struct hbucket* b;
	struct hbucket** prev;
	void* tmp;

	if (!ht->size)
		return NULL;

	prev = &ht->table[fnv1as(k) % ht->size];
	for (b = *prev; b; b = b->next) {
		if (!strcmp(k, b->key.s)) {
			*prev = b->next;
			tmp = b->value;
			xfree(b);
			ht->pop -= 1;
			return tmp;
		} else
			prev = &b->next;
	}

	return NULL;
}

void* htpremove(struct htab* ht, uintptr_t p)
{
	struct hbucket* b;
	struct hbucket** prev;
	void* tmp;

	if (!ht->size)
		return NULL;

	prev = &ht->table[fnv1a(&p, sizeof(p)) % ht->size];
	for (b = *prev; b; b = b->next) {
		if (p == b->key.p) {
			*prev = b->next;
			tmp = b->value;
			xfree(b);
			ht->pop -= 1;
			return tmp;
		} else
			prev = &b->next;
	}

	return NULL;
}

void htsvisit(const struct htab* ht,
              void (*fn)(const char* key, void* val, void* arg), void* arg)
{
	unsigned int i;
	const struct hbucket* b;

	for (i = 0; i < ht->size; i++) {
		for (b = ht->table[i]; b; b = b->next)
			fn(b->key.s, b->value, arg);
	}
}

void htpvisit(const struct htab* ht,
              void (*fn)(uintptr_t p, void* val, void* arg), void* arg)
{
	unsigned int i;
	const struct hbucket* b;

	for (i = 0; i < ht->size; i++) {
		for (b = ht->table[i]; b; b = b->next)
			fn(b->key.p, b->value, arg);
	}
}

void free_shtab(struct htab* ht, void (*fn)(char* key, void* val, void* arg),
                void* arg)
{
	int i;
	struct hbucket* b;
	struct hbucket* next;

	for (i = 0; i < ht->size; i++) {
		for (b = ht->table[i]; b; b = next) {
			next = b->next;
			if (fn)
				fn(b->key.s, b->value, arg);
			xfree(b);
		}
	}

	xfree(ht->table);
}

void free_phtab(struct htab* ht, void (*fn)(uintptr_t p, void* val, void* arg),
                void* arg)
{
	int i;
	struct hbucket* b;
	struct hbucket* next;

	for (i = 0; i < ht->size; i++) {
		for (b = ht->table[i]; b; b = next) {
			next = b->next;
			if (fn)
				fn(b->key.p, b->value, arg);
			xfree(b);
		}
	}

	xfree(ht->table);
}
