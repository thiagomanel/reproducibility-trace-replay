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

#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>

#include "trace.h"
#include "fsmodel.h"

#define THREAD_FILE_MAX_EVENTS 100000

#define evsel(ev, other, t, f) \
	((((other) && (other)->thread != (ev)->thread) || include_intrathread_deps) ? (t) : (f))

/* thread0_chunk0_trace is #defined to NULL as a quick codegen-simplification hack. */
#define EVPTR_FMT "&thread%d_chunk%d_trace[%d]"
#define EVDEP_ARGS(ev, other) (evsel(ev, other, (other)->thread, 0)), \
		(evsel(ev, other, (other)->thread_evidx / THREAD_FILE_MAX_EVENTS, 0)), \
		(evsel(ev, other, (other)->thread_evidx % THREAD_FILE_MAX_EVENTS, 0))

void output_posix_aiocb(const struct event* e);
void output_linux_aiocb(const struct artc_iocb* cb);

void generate_code(const struct threadtrace* firstthread);

#endif /* CODEGEN_H */
