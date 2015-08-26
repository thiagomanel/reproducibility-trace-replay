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
 * This is gross, and grosser still due to being a backward-compatibility hack
 * with an earlier version that was, believe it or not, even *more*
 * embarrassing than this version.
 */

#ifndef ASSOC_H
#define ASSOC_H

#include <assert.h>

#include "htab.h"

/* So we don't get a warning if a generated function is unused */
#define unused __attribute__((unused))

#define MAKE_ASSOC_TYPE(name, ktype, vtype) \
	typedef struct htab name##_t

/* external-to-internal and internal-to-external value conversions. */
#define MAKE_ASSOC_VCONVERSIONS(name, vtype, vinval) \
	static inline void* __##name##_etoi(vtype v) \
	{ \
		assert(v != vinval); \
		return v ? (void*)(uintptr_t)v \
			: (vinval ? (void*)(uintptr_t)vinval \
			   : (abort(), (void*)NULL)); \
	} \
	static inline vtype __##name##_itoe(void* v) \
	{ \
		return (v == (void*)(uintptr_t)vinval) ? (vtype)0 \
			: (v ? (vtype)(uintptr_t)v : vinval); \
	}

#define MAKE_ASSOC_INSERT(name, ktype, vtype, kinval, vinval) \
	static unused void name##_insert(name##_t* a, ktype k, vtype v) \
	{ \
		assert(sizeof(vtype) <= sizeof(void*)); \
		assert(sizeof(ktype) <= sizeof(uintptr_t)); \
		assert(k != kinval); \
		assert(v != vinval); \
		htpinsert(a, (uintptr_t)k, __##name##_etoi(v)); \
	}

#define MAKE_ASSOC_LOOKUP(name, ktype, vtype, kinval, vinval) \
	static unused vtype name##_lookup(name##_t* a, ktype k) \
	{ \
		assert(k != kinval); \
		return __##name##_itoe(htplookup(a, (uintptr_t)k)); \
	}

#define MAKE_ASSOC_DELETE(name, ktype, vtype, kinval, vinval) \
	static unused vtype name##_delete(name##_t* a, ktype k) \
	{ \
		assert(k != kinval); \
		return __##name##_itoe(htpremove(a, (uintptr_t)k)); \
	}

#define MAKE_ASSOC_HELPER_ADAPTER_ARG_TYPE(name, ktype, vtype) \
	struct name##_haa { \
		void (*origfn)(ktype, vtype, void*); \
		void* origarg; \
	}

#define MAKE_ASSOC_VISIT(name, ktype, vtype, kinval, vinval) \
	static void name##_visithelper(uintptr_t p, void* val, void* arg) \
	{ \
		struct name##_haa* a = arg; \
		a->origfn((ktype)p, __##name##_itoe(val), a->origarg); \
	} \
	static unused void name##_visit(name##_t* a, void (*fn)(ktype, vtype, void*), \
	                                void* arg) \
	{ \
		struct name##_haa tmp = { .origfn = fn, .origarg = arg, }; \
		htpvisit(a, name##_visithelper, &tmp); \
	}

#define MAKE_ASSOC_DEALLOC(name, ktype, vtype) \
	static void name##_deallochelper(uintptr_t p, void* val, void* arg) \
	{ \
		struct name##_haa* a = arg; \
		if (a->origfn) \
			a->origfn((ktype)p, __##name##_itoe(val), a->origarg); \
	} \
	static unused void name##_dealloc(name##_t* a, \
	                                  void (*fn)(ktype k, vtype v, void* arg), \
	                                  void* arg) \
	{ \
		struct name##_haa tmp = { .origfn = fn, .origarg = arg, }; \
		free_phtab(a, name##_deallochelper, &tmp); \
	}

#define MAKE_ASSOC(name, ktype, kinval, vtype, vinval) \
	MAKE_ASSOC_VCONVERSIONS(name, vtype, vinval); \
	MAKE_ASSOC_TYPE(name, ktype, vtype); \
	MAKE_ASSOC_HELPER_ADAPTER_ARG_TYPE(name, ktype, vtype); \
	MAKE_ASSOC_INSERT(name, ktype, vtype, kinval, vinval); \
	MAKE_ASSOC_LOOKUP(name, ktype, vtype, kinval, vinval); \
	MAKE_ASSOC_DELETE(name, ktype, vtype, kinval, vinval); \
	MAKE_ASSOC_VISIT(name, ktype, vtype, kinval, vinval); \
	MAKE_ASSOC_DEALLOC(name, ktype, vtype)

#define ASSOC_INIT HTAB_INIT

#endif /* ASSOC_H */
