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
#ifndef FLAG_H
#define FLAG_H

#ifndef MAP_NOCACHE
#define MAP_NOCACHE 0
#endif

#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif

#ifndef MAP_DENYWRITE
#define MAP_DENYWRITE 0
#endif

/* Various O_* flags that aren't universal */
#ifndef O_EVTONLY
#define O_EVTONLY 0
#endif

#ifndef O_EXLOCK
#define O_EXLOCK 0
#endif

#ifndef O_NOFOLLOW
#define O_NOFOLLOW 0
#endif

#ifndef O_SHLOCK
#define O_SHLOCK 0
#endif

#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif

/*
 * #define missing F_* constants to negatives so they don't clash with
 * any existing ones.
 */
#ifndef F_RDADVISE
#define F_RDADVISE -1
#endif

#ifndef F_NOCACHE
#define F_NOCACHE -2
#endif

#ifndef F_FULLFSYNC
#define F_FULLFSYNC -3
#endif

#ifndef F_GETPATH
#define F_GETPATH -4
#endif

#ifndef F_ADDSIGS
#define F_ADDSIGS -5
#endif

#ifndef F_PREALLOCATE
#define F_PREALLOCATE -6
#endif

/* From Linux/glibc /usr/include/fcntl.h */
#ifndef AT_SYMLINK_NOFOLLOW
#define AT_SYMLINK_NOFOLLOW 0x100
#endif

/* From Linux/glibc /usr/include/linux/fcntl.h */
#ifndef AT_FDCWD
#define AT_FDCWD -100
#endif

/* posix_fadvise() advice types */
#ifndef POSIX_FADV_NORMAL
#define POSIX_FADV_NORMAL 0
#endif

#ifndef POSIX_FADV_RANDOM
#define POSIX_FADV_RANDOM 1
#endif

#ifndef POSIX_FADV_SEQUENTIAL
#define POSIX_FADV_SEQUENTIAL 2
#endif

#ifndef POSIX_FADV_WILLNEED
#define POSIX_FADV_WILLNEED 3
#endif

#ifndef POSIX_FADV_DONTNEED
#define POSIX_FADV_DONTNEED 4
#endif

#ifndef POSIX_FADV_NOREUSE
#define POSIX_FADV_NOREUSE 5
#endif

/* SunOS only has these in /usr/ucbinclude */
#ifndef LOCK_SH
#define LOCK_SH 1
#endif

#ifndef LOCK_EX
#define LOCK_EX 2
#endif

#ifndef LOCK_NB
#define LOCK_NB 4
#endif

#ifndef LOCK_UN
#define LOCK_UN 8
#endif

#endif /* FLAG_H */
