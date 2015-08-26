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

#ifndef SYSTEM_H
#define SYSTEM_H

#if defined(__APPLE__) || defined(__MACH__)
#define OSX
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__gnu_linux__)
#define LINUX
#elif defined(__FreeBSD__)
#define FREEBSD
#elif defined(sun) || defined(__sun) || defined(__sun__)
#define SUN
#else
#warning Unrecognized/untested system type
#endif

/* Perhaps I should consider autotools... */
#if defined(LINUX) || defined(FREEBSD) || defined(SUN)

#ifdef SUN
#define RUSAGE_THREAD RUSAGE_LWP
#endif

#define HAVE_RUSAGE_THREAD
#endif

#endif /* SYSTEM_H */
