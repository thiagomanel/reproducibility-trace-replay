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
 * The FNV hash function; see http://www.isthe.com/chongo/tech/comp/fnv/
 */

#ifndef FNV_H
#define FNV_H

#include <stdint.h>

typedef uint32_t hash_t;

#define FNV1a_BASIS 2166136261U
#define FNV1a_PRIME 16777619

static inline hash_t fnv1a(const void* mem, size_t len)
{
	int i;
	const uint8_t* bytes = mem;
	hash_t h = FNV1a_BASIS;

	for (i = 0; i < len; i++) {
		h ^= bytes[i];
		h *= FNV1a_PRIME;
	}

	return h;
}

static inline hash_t fnv1as(const char* s)
{
	return fnv1a(s, strlen(s));
}

#endif /* FNV_H */
