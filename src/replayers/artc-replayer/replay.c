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

#include "system.h"

#ifdef OSX
/* Without this getdirentries[64] doesn't exist */
#define _DARWIN_NO_64_BIT_INODE
#endif

#ifdef LINUX
#define _XOPEN_SOURCE 600 /* pread, posix_fadvise, etc. */
#define _BSD_SOURCE /* getdirentries */
#define _GNU_SOURCE /* so unistd.h declares environ...on some systems anyway. */
#endif

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <spawn.h>
#include <fcntl.h>
#include <signal.h>

#include <sys/file.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sys/mount.h>

#if defined(FREEBSD)
#include <sys/extattr.h>
#elif defined(LINUX) || defined(OSX)
#include <sys/xattr.h>
#endif

#ifdef OSX
#include <sys/dirent.h>
#endif

#ifdef LINUX
#include <sys/vfs.h>
#endif

#if defined(LINUX) || defined(OSX)
#include <sys/syscall.h>
#endif

#include "trace.h"
#include "misc.h"
#include "flags.h"
#include "artrun.h"

#define WARN_ONCE(s) do { \
		static int warned = 0; \
		if (!warned) { \
			warned = 1; \
			warn(s); \
		} \
	} while (0) \

/* ***NOTE***: this returns from the function that invokes it! */
#define UNSUPPORTED(s) do { \
		WARN_ONCE(s" unsupported on this system"); \
		errno = ENOSYS; \
		return -1; \
	} while (0)

#if defined(FREEBSD)

typedef struct statfs statfs_t;
typedef struct statfs statfs64_t;
typedef struct stat stat64_t;

static int getfsstat64(statfs64_t* buf, long bufsize, int flags)
{
	return getfsstat(buf, bufsize, flags);
}

static int statfs64(const char* path, statfs64_t* buf)
{
	return statfs(path, buf);
}

static int fstatfs64(int fd, statfs64_t* buf)
{
	return fstatfs(fd, buf);
}

static int stat64(const char* path, stat64_t* buf)
{
	return stat(path, buf);
}

static int fstat64(int fd, stat64_t* buf)
{
	return fstat(fd, buf);
}

static int lstat64(const char* path, stat64_t* buf)
{
	return lstat(path, buf);
}

#elif defined(SUN)

#include <sys/statvfs.h>

/*
 * This could probably be accomplished with '#define statfs statvfs' and so
 * forth, but that seems a bit risky/gross.
 */

typedef struct statvfs statfs_t;
typedef struct statvfs64 statfs64_t;
typedef struct stat64 stat64_t;

static int statfs(const char* path, statfs_t* buf)
{
	return statvfs(path, buf);
}

static int statfs64(const char* path, statfs64_t* buf)
{
	return statvfs64(path, buf);
}

static int fstatfs(int fd, statfs_t* buf)
{
	return fstatvfs(fd, buf);
}

static int fstatfs64(int fd, statfs64_t* buf)
{
	return fstatvfs64(fd, buf);
}

static int getdirentries(int fd, char* buf, size_t nbytes, off_t* basep)
{
	return getdents(fd, (struct dirent*)buf, nbytes);
}

/* flock(2) API translated into fcntl(2) locks internally. */
static int flock(int fd, int op)
{
	static const short locktypes[] = {
		[LOCK_SH] = F_RDLCK,
		[LOCK_EX] = F_WRLCK,
		[LOCK_UN] = F_UNLCK,
	};

	int fcmd;
	struct flock lock;

	/* Extract and clear the non-blocking (LOCK_NB) flag */
	fcmd = (op & LOCK_NB) ? F_SETLK : F_SETLKW;
	op &= ~LOCK_NB;

	lock.l_type = locktypes[op];
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;

	return fcntl(fd, fcmd, &lock);
}

static int sunos_setxattr(const char* path, const char* name, const void* value, size_t size)
{
	int ret, attrfd;

	if ((attrfd = attropen(path, name, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0)
		return -1;

	ret = (write(attrfd, value, size) == size) ? 0 : -1;

	if (close(attrfd))
		ret = -1;

	return ret;
}

static ssize_t sunos_getxattr(const char* path, const char* name, void* value, size_t size)
{
	ssize_t ret;
	int attrfd;

	if ((attrfd = attropen(path, name, O_RDONLY)) < 0)
		return -1;

	ret = read(attrfd, value, size);

	if (close(attrfd))
		ret = -1;

	return ret;
}

static int sunos_fremovexattr(int fd, const char* name)
{
	return unlinkat(fd, name, 0);
}

static int futimes(int fd, const struct timeval tv[2])
{
	return futimesat(fd, NULL, tv);
}

static ssize_t sunos_listxattr(const char* path, char* list, size_t size)
{
	DIR* dir;
	char debuf[sizeof(struct dirent) + PATH_MAX + 1];
	struct dirent* de;
	struct dirent* dep;
	size_t namelen;
	ssize_t ret;
	int attrdirfd = attropen(path, ".", O_RDONLY);

	if (attrdirfd < 0)
		return -1;

	if (!(dir = fdopendir(attrdirfd))) {
		close(attrdirfd);
		ret = -1;
	} else {
		ret = 0;
		de = (struct dirent*)debuf;
		while (!readdir_r(dir, de, &dep) && dep) {
			namelen = strlen(de->d_name);
			if (ret + namelen + 1 > size)
				break;
			else {
				strcpy(list + ret, de->d_name);
				ret += namelen + 1;
			}
		}

		if (closedir(dir))
			ret = -1;
	}

	return ret;
}

#else
typedef struct statfs statfs_t;
typedef struct statfs64 statfs64_t;
typedef struct stat64 stat64_t;
#endif

#ifndef LINUX
extern char** environ;
#endif

#if defined(OSX) || defined(FREEBSD)
typedef long gde64_arg4_t;
#else
typedef off_t gde64_arg4_t;
#endif
static inline int advisory_read(int fd, off_t offset, off_t len)
{
#if defined(OSX)
	struct radvisory ra = {
		.ra_offset = offset,
		.ra_count = len,
	};
	return fcntl(fd, F_RDADVISE, &ra);
#elif defined(FREEBSD)
#warning advisory_read() unimplemented/unemulated on this system
	UNSUPPORTED("advisory_read()");
#else
	/*
	 * I think this is the closest analogous posix_fadvise advice...
	 * (Note that Linux also has its own special readahead(2) syscall, but
	 * we'll use this instead.)
	 */
	return errno = posix_fadvise(fd, offset, len, POSIX_FADV_WILLNEED);
#endif
}

static inline int cache_file(int fd, int value)
{
#if defined(OSX)
	return fcntl(fd, F_NOCACHE, value);
#elif defined(FREEBSD)
	/* no F_NOCACHE or posix_fadvise, so we'll turn O_DIRECT on/off instead. */
	int flags = fcntl(fd, F_GETFL);
	flags = value ? (flags | O_DIRECT) : (flags & ~O_DIRECT);
	return fcntl(fd, F_SETFL, flags);
#else
	/* Seems like the most appropriate advice, I think. */
	return errno = posix_fadvise(fd, 0, 0,
	                             value ? POSIX_FADV_NOREUSE : POSIX_FADV_NORMAL);
#endif
}

static inline int full_fsync(int fd)
{
#ifdef OSX
	return fcntl(fd, F_FULLFSYNC);
#else
	/*
	 * On other systems (at least Linux, anyway), fsync() itself
	 * actually does what you'd hope it does.  Imagine that!
	 */
	return fsync(fd);
#endif
}

static inline int sys_fdatasync(int fd)
{
#if defined(OSX)
	/* TODO: check fsync_pt to maybe use fcntl(F_FULLFSYNC) instead? */
	return syscall(SYS_fdatasync, fd);
#elif defined(FREEBSD)
	return fsync(fd);
#else
	return fdatasync(fd);
#endif
}

#if defined(LINUX)
#define PROCFDPREFIX "/proc/self/fd/"
#elif defined(SUN)
#define PROCFDPREFIX "/proc/self/path/"
#endif

static inline int file_getpath(int fd, char* buf)
{
#if defined(OSX)
	return fcntl(fd, F_GETPATH, buf);
#elif defined(LINUX) || defined(SUN)
	ssize_t ret;
	char path[sizeof(PROCFDPREFIX)+16];
	snprintf(path, sizeof(path), "%s%d", PROCFDPREFIX, fd);
	ret = readlink(path, buf, PATH_MAX);
	return ret < 0 ? ret : 0;
#undef PROCFDPREFIX
#else
#warning file_getpath unimplemented/unemulated on this system
	UNSUPPORTED("file_getpath()");
#endif
}

/*
 * F_ADDSIGS is completely undocumented, but some googling indicates it checks
 * code-signing signatures or something, so I figure its a small metadata
 * update operation, which we'll fake on non-OSX systems by setting an xattr.
 */
static inline int addsigs(int fd, long arg)
{
#if defined(OSX)
	return fcntl(fd, F_ADDSIGS, arg);
#elif defined(LINUX)
	return fsetxattr(fd, xattr("sig"), "foobar", 6, 0);
#elif defined(FREEBSD)
	return extattr_set_fd(fd, EXTATTR_NAMESPACE_USER, "sig", "foobar", 6) != 6;
#elif defined(SUN)
	char path[PATH_MAX];
	if (file_getpath(fd, path))
		return -1;
	return sunos_setxattr(path, "sig", "foobar", 6);
#else
#warning addsigs() unimplemented/unemulated on this system
	UNSUPPORTED("addsigs()");
#endif
}

static inline int sys_setxattr(const char* path, const char* name, const void* value,
                               size_t size, unsigned int position, int flags)
{
#if defined(OSX)
	return setxattr(path, name, value, size, position, flags);
#elif defined(LINUX)
	/* no 'position' parameter */
	return setxattr(path, name, value, size, flags);
#elif defined(FREEBSD)
	int ret;
	ret = extattr_set_file(path, EXTATTR_NAMESPACE_USER, name, value, size);
	return ret != size;
#elif defined(SUN)
	return sunos_setxattr(path, name, value, size);
#else
#warning setxattr() unimplemented/unemulated on this system
	UNSUPPORTED("setxattr()");
#endif
}

static inline int sys_fsetxattr(int fd, const char* name, const void* value,
                                size_t size, unsigned int position, int flags)
{
#if defined(OSX)
	return fsetxattr(fd, name, value, size, position, flags);
#elif defined(LINUX)
	/* no 'position' parameter */
	return fsetxattr(fd, name, value, size, flags);
#elif defined(FREEBSD)
	return extattr_set_fd(fd, EXTATTR_NAMESPACE_USER, name, value, size) != size;
#elif defined(SUN)
	char path[PATH_MAX];
        if (file_getpath(fd, path))
	        return -1;
	else
		return sunos_setxattr(path, name, value, size);
#else
#warning fsetxattr() unimplemented/unemulated on this system
	UNSUPPORTED("fsetxattr()");
#endif
}

static inline ssize_t sys_listxattr(const char* path, char* list, size_t size, int options)
{
#if defined(OSX)
	return listxattr(path, list, size, options);
#elif defined(LINUX)
	/* no 'options' parameter */
	return listxattr(path, list, size);
#elif defined(FREEBSD)
	return extattr_list_file(path, EXTATTR_NAMESPACE_USER, list, size) == -1;
#elif defined(SUN)
	return sunos_listxattr(path, list, size);
#else
#warning listxattr() unimplemented/unemulated on this system
	UNSUPPORTED("listxattr()");
#endif
}

static inline ssize_t sys_flistxattr(int fd, char* namebuf, size_t size, int options)
{
#if defined(OSX)
	return flistxattr(fd, namebuf, size, options);
#elif defined(LINUX)
	/* no 'options' parameter */
	return flistxattr(fd, namebuf, size);
#elif defined(FREEBSD)
	return extattr_list_fd(fd, EXTATTR_NAMESPACE_USER, namebuf, size) == -1;
#elif defined(SUN)
	char path[PATH_MAX];
	if (file_getpath(fd, path))
		return -1;
	else
		return sunos_listxattr(path, namebuf, size);
#else
#warning flistxattr() unimplemented/unemulated on this system
	UNSUPPORTED("flistxattr()");
#endif
}

static inline int sys_removexattr(const char* path, const char* name, int options)
{
#if defined(OSX)
	return removexattr(path, name, options);
#elif defined(LINUX)
	/* no 'options' parameter */
	return removexattr(path, name);
#elif defined(FREEBSD)
	return extattr_delete_file(path, EXTATTR_NAMESPACE_USER, name) == -1;
#elif defined(SUN)
	int ret, fd = open(path, O_RDONLY);
	if (fd < 0)
		return -1;
	ret = sunos_fremovexattr(fd, name);
	if (close(fd))
		ret = -1;
	return ret;
#else
#warning removexattr() unimplemented/unemulated on this system
	UNSUPPORTED("removexattr()");
#endif
}

static inline int sys_fremovexattr(int fd, const char* name, int options)
{
#if defined(OSX)
	return fremovexattr(fd, name, options);
#elif defined(LINUX)
	/* no 'options' parameter */
	return fremovexattr(fd, name);
#elif defined(FREEBSD)
	return extattr_delete_fd(fd, EXTATTR_NAMESPACE_USER, name) == -1;
#elif defined(SUN)
	return sunos_fremovexattr(fd, name);
#else
#warning fremovexattr() unimplemented/unemulated on this system
	UNSUPPORTED("fremovexattr()");
#endif
}

static inline ssize_t sys_getxattr(const char* path, const char* name, void* value,
                                   size_t size, uint32_t position, int options)
{
#if defined(OSX)
	return getxattr(path, name, value, size, position, options);
#elif defined(LINUX)
	/* no 'options' or 'position' */
	return getxattr(path, name, value, size);
#elif defined(FREEBSD)
	ssize_t ret;
	ret = extattr_get_file(path, EXTATTR_NAMESPACE_USER, name, value, size);
	return ret < 0;
#elif defined(SUN)
	return sunos_getxattr(path, name, value, size);
#else
#warning getxattr() unimplemented/unemulated on this system
	UNSUPPORTED("getxattr()");
#endif
}

static inline ssize_t sys_fgetxattr(int fd, const char* name, void* value,
                                    size_t size, uint32_t position, int options)
{
#if defined(OSX)
	return fgetxattr(fd, name, value, size, position, options);
#elif defined(LINUX)
	/* no 'options' or 'position' */
	return fgetxattr(fd, name, value, size);
#elif defined(FREEBSD)
	return extattr_get_fd(fd, EXTATTR_NAMESPACE_USER, name, value, size) < 0;
#elif defined(SUN)
	char path[PATH_MAX];
	if (file_getpath(fd, path))
		return -1;
	else
		return sunos_getxattr(path, name, value, size);
#else
#warning fgetxattr() unimplemented/unemulated on this system
	UNSUPPORTED("fgetxattr()");
#endif
}

static inline int sys_preallocate(int fd, off_t len)
{
#if defined(OSX)
	return fcntl(fd, F_PREALLOCATE, len);
#elif defined(LINUX) || defined(SUN)
	int status;
	status = posix_fallocate(fd, 0, len);
	if (status)
		errno = status;
	return !!status;
#else
#warning sys_preallocate() unimplemented/unemulated on this system
	UNSUPPORTED("sys_preallocate()");
#endif
}

static inline int sys_kqueue(void)
{
	/*
	 * We're supposed to return a file descriptor of some sort,
	 * might as well be /dev/zero.
	 */
	return open("/dev/zero", O_RDWR);
}

static inline int sys_epoll_create(int size)
{
	/* Same as sys_kqueue() above. */
	return open("/dev/zero", O_RDWR);
}

static inline int sys_getattrlist(const char* path, void* attrlist, void* attrbuf,
                                  size_t attrbufsize, unsigned long options, int ret)
{
#ifdef OSX
	return getattrlist(path, attrlist, attrbuf, attrbufsize, options);
#else
	int status;
	struct stat stbuf;
	/*
	 * Quoth the OSX man page:
	 *
	 *   "You can think of getattrlist() as a seriously enhanced version
	 *    of stat(2)."
	 *
	 * Thus, lacking any "seriously enhanced" portable version, we'll just
	 * do a plain ol' stat(2).
	 */
	status = stat(path, &stbuf);

	/*
	 * These calls fail mysteriously sometimes (e.g. on '/' in
	 * itunes_album1), perhaps an ENOTSUP or EINVAL condition?  (No errno
	 * values in the trace to check against.)  In any case, our replay
	 * succeeds in a number of places where the traces show an error, but
	 * I don't think it's worth counting them as failures.
	 */
	return status ? status : ret;
#endif
}

/*
 * Approximating Darwin-specific setattrlist with setxattr (or similar) on
 * other systems.
 */
static inline int sys_setattrlist(const char* path, void* attrlist, void* attrbuf,
                                  size_t attrbufsize, unsigned long options)
{
#if defined(OSX)
	return setattrlist(path, attrlist, attrbuf, attrbufsize, options);
#elif defined(LINUX)
	return setxattr(path, xattr("some_xattr"), "foobar", 6, 0);
#elif defined(FREEBSD)
	int ret;
	ret = extattr_set_file(path, EXTATTR_NAMESPACE_USER, "some_xattr",
	                       "foobar", 6);
	return ret != attrbufsize;
#elif defined(SUN)
	return sunos_setxattr(path, "some_xattr", "foobar", 6);
#else
#warning setattrlist() unimplemented/unemulated on this system
	UNSUPPORTED("setattrlist()");
#endif
}

static inline int sys_fchflags(int fd, unsigned int flags)
{
#if defined(OSX) || defined(FREEBSD)
	return fchflags(fd, flags);
#else
	/*
	 * Some sort of small metadata operation...might as well be access
	 * times I guess?
	 */
	return futimes(fd, (struct timeval[2]){});
#endif
}

static int sys_getfsstat64(statfs64_t* buf, int bufsize, int flags, int numstats)
{
#if defined(OSX) || defined(FREEBSD)
	return getfsstat64(buf, bufsize, flags);
#else
	/*
	 * getmntent() doesn't seem right...but what instead?
	 * statfs() numstats times? (doing this for now)
	 * statfs once?
	 */
	int i;
	int status = 0, tmp;
	statfs_t stbuf;
	for (i = 0; i < numstats; i++) {
		tmp = statfs("/", &stbuf);
		if (tmp)
			status = tmp;
	}
	return status ? status : numstats;
#endif
}

static int sys_pagein(int ret)
{
	WARN_ONCE("pageins not emulated");

	/* Not counting this one as failing even though it's not emulated. */
	return ret;
}

/*
 * fsgetpath is apparently an actual syscall on OSX, but is completely
 * undocumented as far as I can tell (source code comments describe it
 * as "Obtain the full pathname of a file system object by id.").  Its
 * parameters are of somewhat mysterious types (an fsid and an "object
 * id"?), so I'm left without any reasonable way to construct a call
 * to it.
 */
static int sys_fsgetpath(int ret)
{
	WARN_ONCE("fsgetpath not emulated");

	/* Don't bother counting it as failed. */
	return ret;
}

static inline int sys_exchangedata(const char* path1, const char* path2,
                                   unsigned long options)
{
#ifdef OSX
	return exchangedata(path1, path2, options);
#else
	int fd;
	char tmp[PATH_MAX];

	/*
	 * Doesn't provide the same atomicity guarantees (there will be a
	 * period during which path2 does not exist), but hopefully we can get
	 * away with it.  Could use rwlock, acquiring in write mode here and
	 * read mode everywhere else (e.g. on each event playback).
	 */
	snprintf(tmp, sizeof(tmp), "%s.XXXXXX", path1);

	/*
	 * We're using mkstemp() here to make gcc happy (it warns on mktemp(),
	 * though our use of it would, I believe, actually be secure, since we
	 * don't blindly write to the file or anything, just create a link at
	 * the path).  But warnings are bad, so "mkstemp(), close(), unlink()"
	 * it shall be.
	 */
	if ((fd = mkstemp(tmp)) < 0 || close(fd) || unlink(tmp))
		return -1;
	return link(path1, tmp) || rename(path2, path1) || rename(tmp, path2);
#endif
}

static inline int sys_fadvise64(int fd, off_t offset, off_t len, int advice)
{
#if defined(LINUX) || defined(SUN)
	return posix_fadvise64(fd, offset, len, advice);
#elif defined(OSX) || defined(FREEBSD)
	switch (advice) {
	case POSIX_FADV_WILLNEED:
		/* Will need data => issue advisory read */
		return !!advisory_read(fd, offset, len);

	case POSIX_FADV_SEQUENTIAL:
		/* Sequential access => turn readahead on */
		return -(fcntl(fd, F_RDAHEAD, 1) == -1);

	case POSIX_FADV_RANDOM:
		/* Random access => turn readahead off */
		return -(fcntl(fd, F_RDAHEAD, 0) == -1);

	case POSIX_FADV_NOREUSE:
	case POSIX_FADV_DONTNEED:
		/* Data not needed, data not reused => disable caching */
		return !!cache_file(fd, 0);

	case POSIX_FADV_NORMAL:
		/* Normal access => enable caching */
		return !!cache_file(fd, 1);

	default:
		WARN_ONCE("unsupported fadvice type");
		return -1;
	}
#else
#warning sys_fadvise64() unimplemented/unemulated on this this system
	UNSUPPORTED("sys_fadvise64()");
#endif
}

static inline int sys_getdents(int fd, unsigned int count)
{
#if defined(LINUX)
	struct linux_dirent* tmp = xmalloc(count);
	/* "oooh" or "eeewww"? */
	int ret = syscall(SYS_getdents, fd, tmp, count);
	xfree(tmp);
	return ret;
#elif defined(SUN)
	struct dirent* tmp = xmalloc(count);
	int ret = getdents(fd, tmp, count);
	xfree(tmp);
	return ret;
#elif defined(OSX) || defined(FREEBSD)
	char* buf = xmalloc(count);
	long base = 0;
	int ret = getdirentries(fd, buf, count, &base);
	xfree(buf);
	return ret;
#else
#warning sys_getdents() unimplemented/unemulated on this system
	UNSUPPORTED("sys_getdents()");
#endif
}

static inline int sys_fchownat(int dirfd, const char* pathname, uid_t u, gid_t g,
                               int flags)
{
#if defined(LINUX) || defined(FREEBSD) || defined(SUN)
	return fchownat(dirfd, pathname, u, g, flags);
#elif defined(OSX)
	if (dirfd != AT_FDCWD)
		return -ENOSYS;
	else
		return chown(pathname, u, g);
#else
#error sys_fchownat() status unknown on this system
#endif
}

static inline int sys_faccessat(int dirfd, const char* pathname, mode_t mode, int flags)
{
#if defined(LINUX) || defined(FREEBSD) || defined(SUN)
	return faccessat(dirfd, pathname, mode, flags);
#elif defined(OSX)
	if (dirfd != AT_FDCWD) {
		WARN_ONCE("faccessat(dirfd, ...) with dirfd != AT_FDCWD unsupported");
		return -ENOSYS;
	} else
		return access(pathname, mode);
#else
#error sys_faccessat() status unknown on this system
#endif
}

static inline int sys_unlinkat(int dirfd, const char* pathname, int flags)
{
#if defined(LINUX) || defined(FREEBSD) || defined(SUN)
	return unlinkat(dirfd, pathname, flags);
#elif defined(OSX)
	if (dirfd != AT_FDCWD) {
		WARN_ONCE("unlinkat(dirfd, ...) with dirfd != AT_FDCWD unsupported");
		return -ENOSYS;
	} else
		return unlink(pathname);
#else
#error sys_unlinkat() status unknown on this system
#endif
}

#define IS_A(var, type) \
	__builtin_types_compatible_p(typeof(var), type)

#define COND(c, a, b) \
	__builtin_choose_expr(c, a, b)

#define FMTSPEC_FOR(v) \
	COND(IS_A(v, int), "%d", \
	COND(IS_A(v, long), "%ld", \
	COND(IS_A(v, char*), "%s", \
	COND(IS_A(v, unsigned), "%u", \
	COND(IS_A(v, long unsigned), "%lu", \
	COND(IS_A(v, long long), "%lld", \
	COND(IS_A(v, unsigned long long), "%llu", \
	COND(IS_A(v, mode_t), "%d", \
	COND(IS_A(v, void*), "%p", "%p"/*(void)0*/)))))))))

#define PRINT_ARG(outf, a) do { \
	fprintf(outf, FMTSPEC_FOR(a), a); \
} while (0)

#define PRINT_NEXTARG(outf, a) do { \
	fprintf(outf, ", "); \
	PRINT_ARG(outf, a); \
} while (0)

#define PRINTARGS_0(outf) do { } while (0)

#define PRINTARGS_1(outf, arg) do {	  \
	PRINT_ARG(outf, arg); \
} while (0)

#define PRINTARGS_2(outf, arg1, arg2) do { \
	PRINTARGS_1(outf, arg1); \
	PRINT_NEXTARG(outf, arg2); \
} while (0)

#define PRINTARGS_3(outf, arg1, arg2, arg3) do { \
	PRINTARGS_2(outf, arg1, arg2); \
	PRINT_NEXTARG(outf, arg3); \
} while (0)

#define PRINTARGS_4(outf, arg1, arg2, arg3, arg4) do { \
	PRINTARGS_3(outf, arg1, arg2, arg3); \
	PRINT_NEXTARG(outf, arg4); \
} while (0)

#define PRINTARGS_5(outf, arg1, arg2, arg3, arg4, arg5) do { \
	PRINTARGS_4(outf, arg1, arg2, arg3, arg4); \
	PRINT_NEXTARG(outf, arg5); \
} while (0)

#define PRINTARGS_6(outf, arg1, arg2, arg3, arg4, arg5, arg6) do { \
	PRINTARGS_5(outf, arg1, arg2, arg3, arg4, arg5); \
	PRINT_NEXTARG(outf, arg6); \
} while (0)

#define PRINTARGS_8(outf, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) do { \
	PRINTARGS_6(outf, arg1, arg2, arg3, arg4, arg5, arg6); \
	PRINT_NEXTARG(outf, arg7); \
	PRINT_NEXTARG(outf, arg8); \
} while (0)

static void warn_sysret(int retval, const char* cmp, long expected,
                        int serial, int line, int err)
{
	fprintf(stderr, ") returned %d (error: %s), expected %s %ld (%d, line %d)\n",
	        retval, strerror(err), cmp, expected, serial, line);
}

#define makesyscall_expect2_t(rettype, func, nargs, cmp1, expected1, check2, cmp2, \
                              expected2, ev, ...) \
	({ \
		int status; \
		do { \
			int err; \
			rettype sysret; \
			errno = 0; \
			sysret = func(__VA_ARGS__); \
			status = !(sysret cmp1 expected1 || \
			           (check2 && (sysret cmp2 expected2))); \
			if (status) { \
				err = errno; \
				xlock(&warn_lock); \
				fprintf(stderr, "Warning: "#func"("); \
				PRINTARGS_##nargs(stderr, ##__VA_ARGS__); \
				/* warning is half-assed here */ \
				warn_sysret(sysret, #cmp1, expected1, ev->serial, \
				            ev->traceline, err); \
				xunlock(&warn_lock); \
			} \
		} while (0); \
		status; \
	})

#define makesyscall_expect2(func, nargs, cmp1, expected1, check2, cmp2, expected2, \
                            ev, ...) \
	makesyscall_expect2_t(int, func, nargs, cmp1, expected1, check2, cmp2, \
	                      expected2, ev, ##__VA_ARGS__)

#define makesyscall_expect(func, nargs, cmp, expected, ev, ...) \
	makesyscall_expect2(func, nargs, cmp, expected, 0, ==, 0, \
	                    ev, ##__VA_ARGS__)

#define makesyscall_expect_o(func, nargs, cmp, expected, ev, ...) \
	makesyscall_expect2_t(off_t, func, nargs, cmp, expected, 0, ==, 0, \
	                      ev, ##__VA_ARGS__)

#define makesyscall(func, nargs, expected, ev, ...) \
	makesyscall_expect(func, nargs, ==, expected, ev, ##__VA_ARGS__)

static void warn_sysret_mmap(const void* retval, const void* expected,
                             int serial, int line, int err)
{
	fprintf(stderr, ") returned %p (error: %s),  expected %p (%d, line %d)\n",
	        retval, strerror(err), expected, serial, line);
}

#define makesyscall_mmap(func, nargs, expected, ev, ...) \
	({ \
		int status; \
		do { \
			int err; \
			void* sysret; \
			errno = 0; \
			sysret = func(__VA_ARGS__); \
			status = sysret == MAP_FAILED && expected != MAP_FAILED; \
			if (status) { \
				err = errno; \
				xlock(&warn_lock); \
				fprintf(stderr, "Warning: "#func"("); \
				PRINTARGS_##nargs(stderr, ##__VA_ARGS__); \
				warn_sysret_mmap(sysret, (void*)expected, ev->serial, \
				                 ev->traceline, err); \
				xunlock(&warn_lock); \
			} \
		} while (0); \
		status; \
	})

static void warn_sysret_fdret(int fdret, int serial, int line, int err)
{
	fprintf(stderr, ") returned %d (error: %s), expected file descriptor "
	        "(%d, line %d)\n", fdret, strerror(err), serial, line);
}

#define makesyscall_fdret(func, nargs, expected, ev, ...) \
	({ \
		int status; \
		do { \
			int fdret, err; \
			errno = 0; \
			fdret = func(__VA_ARGS__); \
			status = fdret < 0 && expected >= 0; \
			if (status) { \
				err = errno; \
				xlock(&warn_lock); \
				fprintf(stderr, "Warning: "#func"("); \
				PRINTARGS_##nargs(stderr, ##__VA_ARGS__); \
				warn_sysret_fdret(fdret, ev->serial, ev->traceline, \
				                  err); \
				if (options.debug.fds) \
					fprintf(stderr, "<fd>: failed to produce " \
					        "fds[%d]\n", expected); \
				xunlock(&warn_lock); \
				assign_fd(expected, -1, RES_ERR); \
			} else if (expected >= 0) { \
				assign_fd(expected, fdret, RES_AVAIL); \
				if (options.debug.fds) { \
					xlock(&warn_lock); \
					fprintf(stderr, "<fd>: "#func"("); \
					PRINTARGS_##nargs(stderr, ##__VA_ARGS__); \
					fprintf(stderr, ") yields fds[%d] = %d\n", \
					        expected, fdret); \
					xunlock(&warn_lock); \
				} \
			} \
		} while (0); \
		status; \
	})

static int play_socket(const struct event* e)
{
	return makesyscall_fdret(open, 2, e->ret.i, e, "/dev/zero", O_RDWR);
}

static int play_socketpair(const struct event* e)
{
	int s1, s2;
	const struct socketpair_attrs* a = &e->attrs.socketpair;

	if (a->fd1.rtidx < 0 || a->fd2.rtidx < 0)
		return e->ret.i ? 0 : -1;

	s1 = makesyscall_fdret(open, 2, a->fd1.rtidx, e, "/dev/zero", O_RDWR);
	s2 = makesyscall_fdret(dup, 1, a->fd2.rtidx, e, bench->fds[a->fd1.rtidx].fd);

	return s1 || s2;
}

static int play_pipe(const struct event* e)
{
	int s1, s2;
	const struct pipe_attrs* a = &e->attrs.pipe;

	if (a->rd.rtidx < 0 || a->wr.rtidx < 0)
		return e->ret.i ? 0 : -1;

	s1 = makesyscall_fdret(open, 2, a->rd.rtidx, e, "/dev/zero", O_RDONLY);
	s2 = makesyscall_fdret(open, 2, a->wr.rtidx, e, "/dev/zero", O_WRONLY);

	return s1 || s2;
}

static int play_close(const struct event* e)
{
	const struct close_attrs* a = &e->attrs.close;
	int status, sysret, err, fd = get_fd(a->fd.rtidx);

	if (fd < 0)
		return e->ret.i ? 0 : -1;

	if (options.constraints.fdstage)
		fd_wait_lastevent(a->fd.rtidx);

	errno = 0;
	sysret = close(fd);
	status = sysret != e->ret.i;

	if (status) {
		err = errno;
		xlock(&warn_lock);
		fprintf(stderr, "Warning: close(%d", fd);
		warn_sysret(sysret, "==", e->ret.i, e->serial, e->traceline, err);
		xunlock(&warn_lock);
	} else
		fd_debug("close(%d) closed fds[%d]", fd, a->fd.rtidx);

	assign_fd(a->fd.rtidx, -1, RES_DEAD);

	return status;
}

static int play_mmap(const struct event* e)
{
	const struct mmap_attrs* a = &e->attrs.mmap;
	int fd = get_fd(a->fd.rtidx);

	return makesyscall_mmap(mmap, 6, e->ret.p, e, a->addr, a->length, a->prot,
	                        a->flags, fd, 0 /* would be a->offset */);
}

/* We don't have the actual length from the traces, so fake it. */
#ifdef OSX
#define RDADVISE_LEN (1024 * 1024)
#else
/* With posix_fadvise, zero length means "whole file". */
#define RDADVISE_LEN 0
#endif

static int play_fcntl(const struct event* e)
{
	char pathbuf[PATH_MAX];
	struct flock lock;
	const struct fcntl_attrs* a = &e->attrs.fcntl;
	int status = 1;
	int fd = get_fd(a->fd.rtidx);

	switch (a->cmd) {
	case F_RDADVISE:
		status = makesyscall(advisory_read, 3, e->ret.i, e, fd, 0,
		                     RDADVISE_LEN);
		break;

	case F_NOCACHE:
		status = makesyscall(cache_file, 2, e->ret.i, e, fd, a->arg);
		break;

	case F_FULLFSYNC:
		status = makesyscall(full_fsync, 1, e->ret.i, e, fd);
		break;

	case F_GETPATH:
		status = makesyscall(file_getpath, 2, e->ret.i, e, fd, pathbuf);
		break;

	case F_ADDSIGS:
		status = makesyscall(addsigs, 2, e->ret.i, e, fd, a->arg);
		break;

	case F_PREALLOCATE:
		/*
		 * Arbitrarily-selected preallocation size; trace only has a
		 * literal pointer value, so there's not much we can do but
		 * completely fabricate one.
		 */
		status = makesyscall(sys_preallocate, 2, e->ret.i, e, fd,
		                     1024 * 1024);
		break;

	case F_SETLKW:
	case F_SETLK:
		/* Fake a read lock covering zero bytes of the file. */
		lock = (struct flock) {
			.l_type = F_RDLCK,
			.l_whence = SEEK_SET,
			.l_pid = getpid(),
			.l_start = 0,
			.l_len = 0,
		};
		/*
		 * There are a few places where these fail mysteriously in the
		 * traces (perhaps some EAGAIN or EINTR situation?), but since
		 * they *look* like they should succeed, we'll allow them to
		 * even if the trace has them as failed.
		 */
		status = makesyscall_expect2(fcntl, 3, ==, e->ret.i, 1, ==, 0, e,
		                             fd, a->cmd, &lock);
		break;

	case F_GETFD:
		status = makesyscall_expect(fcntl, 3, !=, -1, e, fd, F_GETFD, a->arg);
		break;

	case F_SETFD:
		/*
		 * FIXME: grossly abusing the fact that FD_CLOEXEC is the only
		 * available flag on both OSX and Linux and that it is
		 * numerically equal on both systems (FD_CLOEXEC == 1).
		 */
		status = makesyscall(fcntl, 3, e->ret.i, e, fd, F_SETFD, a->arg);
		break;

	case F_DUPFD:
		status = makesyscall_fdret(fcntl, 3, e->ret.i, e, fd, F_DUPFD,
		                           a->arg);
		break;

	case F_GETFL:
		/*
		 * Since the value of an F_GETFL result is highly
		 * system-dependent, we'll count any result != -1 as success.
		 */
		status = makesyscall_expect2(fcntl, 3, ==, e->ret.i, 1, !=, -1, e,
		                             fd, F_GETFL, a->arg);
		break;

	case F_SETFL:
		status = makesyscall(fcntl, 3, e->ret.i, e, fd, F_SETFL, a->arg);
		break;

	default:
		warn("fcntl(%d) NYI", a->cmd);
	}

	return status;
}

/*
 * dup2(2) is a bit of a weird case, on account of its second argument
 * ('newfd').  If newfd is already an open, in-use file descriptor, it gets
 * closed and a copy of oldfd replaces it.  But if it's not already in use, it
 * just specifies a "target" FD number to return.  In the former case we can
 * replay it in a pretty straightforward manner, but the latter case is
 * trickier.  We'd have to choose an FD number that's known to *not* be in use
 * so as not to incorrectly close something else, a task made non-trivial by
 * the fact that other threads are potentially busy opening and closing FDs
 * concurrently with us.
 *
 * We can, however, hack around this by just calling plain dup() instead.
 */
static int play_dup2(const struct event* e)
{
	const struct dup2_attrs* a = &e->attrs.dup2;

	if (a->new.rtidx >= 0)
		return makesyscall_fdret(dup2, 2, e->ret.i, e, get_fd(a->old.rtidx),
		                         get_fd(a->new.rtidx));
	else
		return makesyscall_fdret(dup, 1, e->ret.i, e, get_fd(a->old.rtidx));
}

#define ATTRSIZE 1024
#define NUM_STATFS_BUFS 8

#define MAKE_PLAYFUNC(name, macroname, ...) \
	static int play_##name(const struct event* e) \
	{ \
		const struct name##_attrs* a = &e->attrs.name; \
		(void)a; /* Avoid possible unused-variable warning */ \
		return macroname(__VA_ARGS__); \
	}

#define MAKE_PLAYFUNC_F(name, macroname, nargs, ...) \
	MAKE_PLAYFUNC(name, macroname, name, nargs, ##__VA_ARGS__)

/* "Basic" */
#define MAKE_PLAYFUNC_FB(name, nargs, ...) \
	MAKE_PLAYFUNC_F(name, makesyscall, nargs, e->ret.i, e, ##__VA_ARGS__)

/* Basic, with alternate success condition */
#define MAKE_PLAYFUNC_FB2(name, nargs, cmp2, expected2, ...) \
	MAKE_PLAYFUNC_F(name, makesyscall_expect2, nargs, ==, e->ret.i, 1, cmp2, \
	                expected2, e, ##__VA_ARGS__)

/* With system-specific implementation */
#define MAKE_PLAYFUNC_S(name, macroname, nargs, ...) \
	MAKE_PLAYFUNC(name, macroname, sys_##name, nargs, ##__VA_ARGS__)

/* "Basic" system-specific */
#define MAKE_PLAYFUNC_SB(name, nargs, ...) \
	MAKE_PLAYFUNC_S(name, makesyscall, nargs, e->ret.i, e, ##__VA_ARGS__)

MAKE_PLAYFUNC_F(open, makesyscall_fdret, 3, e->ret.i, e, a->path, a->flags, a->mode);
MAKE_PLAYFUNC_F(shm_open, makesyscall_fdret, 3, e->ret.i, e, a->name,
                a->oflag|O_CREAT, 0700);
MAKE_PLAYFUNC_F(dup, makesyscall_fdret, 1, e->ret.i, e, get_fd(a->fd.rtidx));
MAKE_PLAYFUNC_S(kqueue, makesyscall_fdret, 0, e->ret.i, e);
MAKE_PLAYFUNC_F(read, makesyscall_expect2, 3, ==, e->ret.i, 1, >, e->ret.i, e,
                get_fd(a->fd.rtidx), a->iobuf, a->count);
MAKE_PLAYFUNC_F(write, makesyscall_expect2, 3, ==, e->ret.i, 1, >, e->ret.i, e,
                get_fd(a->fd.rtidx), a->iobuf, a->count);
MAKE_PLAYFUNC_F(pread, makesyscall_expect2, 4, ==, e->ret.i, 1, >, e->ret.i, e,
                get_fd(a->fd.rtidx), a->iobuf, a->count, a->offset);
MAKE_PLAYFUNC_F(pwrite, makesyscall_expect2, 4, ==, e->ret.i, 1, >, e->ret.i, e,
                get_fd(a->fd.rtidx), a->iobuf, a->count, a->offset);
MAKE_PLAYFUNC_F(lseek, makesyscall_expect_o, 3, ==, e->ret.o, e, get_fd(a->fd.rtidx),
                a->offset, a->whence);

MAKE_PLAYFUNC_SB(fdatasync, 1, get_fd(a->fd.rtidx));
MAKE_PLAYFUNC_FB(futimes, 2, get_fd(a->fd.rtidx), (struct timeval[2]){});
MAKE_PLAYFUNC_FB(ftruncate, 2, get_fd(a->fd.rtidx), a->length);
MAKE_PLAYFUNC_FB(link, 2, a->oldpath, a->newpath);
MAKE_PLAYFUNC_FB(symlink, 2, a->oldpath, a->newpath);
MAKE_PLAYFUNC_FB(rmdir, 1, a->pathname);
MAKE_PLAYFUNC_FB(unlink, 1, a->pathname);
MAKE_PLAYFUNC_FB(rename, 2, a->oldpath, a->newpath);
MAKE_PLAYFUNC_SB(exchangedata, 3, a->path1, a->path2, 0);
MAKE_PLAYFUNC_FB(mkdir, 2, a->pathname, a->mode);
MAKE_PLAYFUNC_FB(flock, 2, get_fd(a->fd.rtidx), a->operation);
MAKE_PLAYFUNC_FB(statfs, 2, a->path, &(statfs_t){});
MAKE_PLAYFUNC_FB(statfs64, 2, a->path, &(statfs64_t){});
MAKE_PLAYFUNC_FB(fstatfs64, 2, get_fd(a->fd.rtidx), &(statfs64_t){});
MAKE_PLAYFUNC_FB(fstat, 2, get_fd(a->fd.rtidx), &(struct stat){});
MAKE_PLAYFUNC_FB(lstat, 2, a->path, &(struct stat){});
MAKE_PLAYFUNC_FB(stat, 2, a->path, &(struct stat){});
MAKE_PLAYFUNC_FB(fstat64, 2, get_fd(a->fd.rtidx), &(stat64_t){});
MAKE_PLAYFUNC_FB2(lstat64, 2, ==, 0, a->path, &(stat64_t){});
MAKE_PLAYFUNC_FB(stat64, 2, a->path, &(stat64_t){});
MAKE_PLAYFUNC_SB(setattrlist, 5, a->path, NULL, NULL, 0, 0);
MAKE_PLAYFUNC_SB(getattrlist, 6, a->path, NULL, NULL, 0, 0, e->ret.i);
MAKE_PLAYFUNC_SB(getxattr, 6, a->path, xattr("some_xattr"), (char[ATTRSIZE]){},
                 ATTRSIZE, 0, 0);
MAKE_PLAYFUNC_SB(fgetxattr, 6, get_fd(a->fd.rtidx), xattr("some_xattr"),
                 (char[ATTRSIZE]){}, ATTRSIZE, 0, 0);
MAKE_PLAYFUNC_FB2(access, 2, ==, 0, a->pathname, a->mode);
MAKE_PLAYFUNC_FB(chown, 3, a->path, uid, gid);
MAKE_PLAYFUNC_FB(chmod, 2, a->path, a->mode);
MAKE_PLAYFUNC_SB(fsetxattr, 6, get_fd(a->fd.rtidx), a->name, NULL, 0, 0, 0);
MAKE_PLAYFUNC_SB(fsgetpath, 1, e->ret.i);
MAKE_PLAYFUNC_SB(getfsstat64, 4, (statfs64_t[NUM_STATFS_BUFS]){}, NUM_STATFS_BUFS,
                 0, e->ret.i);
MAKE_PLAYFUNC_FB2(getdtablesize, 0, >=, 0);
MAKE_PLAYFUNC_FB(kill, 2, a->pid, a->sig);
MAKE_PLAYFUNC_SB(fchflags, 2, get_fd(a->fd.rtidx), a->flags);
MAKE_PLAYFUNC_FB(fchmod, 2, get_fd(a->fd.rtidx), a->mode);
MAKE_PLAYFUNC_FB(fchown, 3, get_fd(a->fd.rtidx), uid, gid);
MAKE_PLAYFUNC_SB(removexattr, 3, a->path, a->name, 0);
MAKE_PLAYFUNC_SB(fremovexattr, 3, get_fd(a->fd.rtidx), a->name, 0);
MAKE_PLAYFUNC_SB(setxattr, 6, a->path, a->name, NULL, 0, 0, 0);
MAKE_PLAYFUNC_FB(posix_spawn, 6, NULL, a->path, NULL, NULL,
                 ((char*[]) { a->path, NULL, }), environ);

MAKE_PLAYFUNC(hfs_vnop_pagein, makesyscall, sys_pagein, 1, e->ret.i, e, e->ret.i);

MAKE_PLAYFUNC_SB(fadvise64, 4, get_fd(a->fd.rtidx), a->offset, a->len, a->advice);
MAKE_PLAYFUNC_FB(fstatfs, 2, get_fd(a->fd.rtidx), &(statfs_t){});
MAKE_PLAYFUNC_S(epoll_create, makesyscall_fdret, 1, e->ret.i, e, a->size);

/* FIXME: carry flags through */
MAKE_PLAYFUNC_SB(fchownat, 5, get_fd(a->dirfd.rtidx), a->pathname, uid, gid, 0);
MAKE_PLAYFUNC_SB(faccessat, 4, get_fd(a->dirfd.rtidx), a->pathname, a->mode, 0);
MAKE_PLAYFUNC_SB(unlinkat, 3, get_fd(a->dirfd.rtidx), a->pathname, 0);

static int play_getdents(const struct event* e)
{
	const struct getdents_attrs* a = &e->attrs.getdents;

	return makesyscall_expect2(sys_getdents, 2, ==, e->ret.i, 1, >=, 0, e,
	                           get_fd(a->fd.rtidx), a->count);
}

/*
 * Original iBench traces don't have anything but the fd number for
 * this call, so we don't have any way of knowing how many dirents are
 * read on each one.  As a result, the length parameter is kind of
 * arbitrary.
 */
static int play_getdirentries64(const struct event* e)
{
	const struct getdirentries64_attrs* a = &e->attrs.getdirentries64;

	return makesyscall_expect(getdirentries, 4, >=, 0, e, get_fd(a->fd.rtidx),
	                          a->iobuf, a->iobufsize, (gde64_arg4_t[1]){});
}

static int play_readlink(const struct event* e)
{
	const struct readlink_attrs* a = &e->attrs.readlink;

	return makesyscall_expect2(readlink, 3, >, 0, 1, ==, e->ret.i, e, a->path,
	                           a->iobuf, a->bufsiz);
}

static int play_listxattr(const struct event* e)
{
	const struct listxattr_attrs* a = &e->attrs.listxattr;

	return makesyscall_expect2(sys_listxattr, 4, >=, 0, 1, ==, e->ret.i, e,
	                           a->path, (char[ATTRSIZE]){}, ATTRSIZE, 0);
}

static int play_flistxattr(const struct event* e)
{
	const struct flistxattr_attrs* a = &e->attrs.flistxattr;

	return makesyscall_expect2(sys_flistxattr, 4, >=, 0, 1, ==, e->ret.i, e,
	                           get_fd(a->fd.rtidx), (char[ATTRSIZE]){}, ATTRSIZE, 0);
}

/*
 * With relative paths encoded in the trace tables, we don't want to be
 * actually changing our $PWD.
 */
static int play_chdir(const struct event* e)
{
	/* hah. */
	if (chdir(".")) {
		warn("chdir(.) failed?? (%d, line %d)", e->serial, e->traceline);
		return 1;
	} else
		return 0;
}

static int play_fchdir(const struct event* e)
{
	/*
	 * fchdir fakes it the same way as chdir; however, it also has
	 * a file descriptor to wait for
	 */
	get_fd(evattr(fchdir, e, fd.rtidx));
	return play_chdir(e);
}

/*
 * getcwd() is a slightly odd case...
 *
 * On Linux: the library function returns a pointer (its first argument), but
 * the syscall just returns a byte count (length of string written to
 * argument), which is what a syscall trace will show.  So we have to invoke
 * it manually.
 *
 * On OS X: as far as I can tell it's not an actual system call (there's no
 * SYS_getcwd entry in sys/syscalls.h).
 */
static int play_getcwd(const struct event* e)
{
#ifdef LINUX
	/*
	 * And because our replay directory probably isn't at an absolute path
	 * of exactly the same length as that of the trace, we'll ignore a
	 * return value mismatch (and failures on ERANGE, for that matter).
	 */
	int ret = syscall(SYS_getcwd, evattr(getcwd, e, buf), evattr(getcwd, e, size));
	if (ret >= 0 || errno == ERANGE) {
		errno = 0;
		return 0;
	} else
		return 1;
#else
	char* ret = getcwd(evattr(getcwd, e, buf), evattr(getcwd, e, size));
	return ret ? 0 : 1;
#endif
}

#ifdef OSX
/*
 * The signature of getdirentriesattr(2) differs between 32-bit and
 * 64-bit versions of OSX.  Having to do this seems pretty stupid.
 */
#if __LP64__
typedef unsigned int gdea_arg_t;
#else
typedef unsigned long gdea_arg_t;
#endif

/* Not in the traces, totally arbitrary. */
#define GDEA_NUMENTS 8
#endif

static int play_getdirentriesattr(const struct event* e)
{
	const struct getdirentriesattr_attrs* a = &e->attrs.getdirentriesattr;
#ifdef OSX
	struct attrlist attrlist;
	char attrbuf[ATTRSIZE];
	gdea_arg_t basep, newstate, count = GDEA_NUMENTS, options = 0;

	/*
	 * Based on 'man 2 getdirentriesattr' example, extended in a
	 * completely arbitrary fashion.
	 */
	memset(&attrlist, 0, sizeof(attrlist));
	attrlist.bitmapcount = ATTR_BIT_MAP_COUNT;
	attrlist.commonattr = ATTR_CMN_NAME | ATTR_CMN_OBJTYPE | ATTR_CMN_FNDRINFO;
	/* attrlist.dirattr = ATTR_DIR_LINKCOUNT; */
	/* attrlist.fileattr = ATTR_FILE_TOTALSIZE | ATTR_FILE_CLUMPSIZE; */

	return makesyscall_expect2(getdirentriesattr, 8, >=, 0, 1, ==, e->ret.i, e,
	                           get_fd(a->fd.rtidx), &attrlist, attrbuf, sizeof(attrbuf),
	                           &count, &basep, &newstate, options);
#else
	/*
	 * getdirentriesattr is OSX-specific; its man page describes it
	 * thusly: "You can think of it as a combination of getdirentries(2)
	 * and getattrlist(2)."  So it's played back as (sort of) one of each
	 * -- though in the real thing, the getattrlist() is applied to the
	 * directory entries, not the directory itself.  Since we don't have a
	 * quick easy way of getting a path to pass to getattrlist
	 * (getdirentries() gives us entry names, but we don't have the path
	 * to the directory we're looking at), we instead fake it with
	 * fgetxattr, since it takes a file descriptor and hence doesn't
	 * require a path we don't have available to give it.
	 *
	 * And further, since we don't have much in the way of information
	 * from the traces about what attributes we're looking at and thus
	 * aren't creating them in the prep phase, the fgetxattr() is
	 * basically guaranteed to fail, which we're just going to ignore
	 * here.  Sigh...we tried.
	 */
	char buf[4096];
	char attrbuf[ATTRSIZE];
	return makesyscall_expect(getdirentries, 4, >=, 0, e, get_fd(a->fd.rtidx),
	                          buf, sizeof(buf), (gde64_arg4_t[1]){})
		|| makesyscall_expect2(sys_fgetxattr, 6, ==, 0, 1, ==, -1, e,
		                       get_fd(a->fd.rtidx), xattr("some_xattr"),
		                       attrbuf, sizeof(attrbuf), 0, 0);
#endif
}

static int fsync_nop(int fd)
{
	return 0;
}

static int fsync_normal(int fd)
{
	return fsync(fd);
}

#ifdef OSX
static int fsync_full(int fd)
{
	return fcntl(fd, F_FULLFSYNC);
}
#endif

static int (*const fsync_fns[])(int) = {
	fsync_nop,
	fsync_normal,
#ifdef OSX
	fsync_full,
#endif
};

static int play_fsync(const struct event* e)
{
	int (*syncfn)(int);
	int funcidx;
	double boostprob;
	const struct fsync_attrs* a = &e->attrs.fsync;
	int fd = get_fd(a->fd.rtidx);

	funcidx = (int)options.fsync_pt;
	boostprob = options.fsync_pt - (double)funcidx;
	if (boostprob > drand48())
		funcidx += 1;

	assert(funcidx >= 0);
	assert(funcidx < ARR_SIZE(fsync_fns));

	syncfn = fsync_fns[funcidx];

	return makesyscall(syncfn, 1, e->ret.i, e, fd);
}

static int play_aio_read(const struct event* e)
{
	int fdidx, ret;
	const struct aio_read_attrs* a = &e->attrs.aio_read;
	struct aio_info* info = get_aio_info(a->infoidx);
	struct genaio tmp = info->u.gencb;
	struct aiocb* cb = &info->u.poscb;

	assert(tmp.op == GA_PREAD);
	memset(cb, 0, sizeof(*cb));
	cb->aio_fildes = get_fd(a->fd.rtidx);
	cb->aio_buf = tmp.buf;
	cb->aio_offset = tmp.offset;
	cb->aio_nbytes = tmp.len;
	cb->aio_sigevent.sigev_notify = SIGEV_NONE;

	aio_debug("aio_read(%p [cb#%d, fd[%d]->%d]) [%d@%d]", cb, a->infoidx,
	          a->fd.rtidx, cb->aio_fildes, e->serial, e->traceline);

	info->times.sub = getnanos();
	ret = makesyscall(aio_read, 1, e->ret.i, e, cb);
	info->times.subdone = getnanos();

	if (options.constraints.aio)
		resstatus_change(&info->status, ret ? RES_ERR : RES_AVAIL);

	return ret;
}

/*
 * Shared by play_aio_return() and io_getevents() emulation on top of POSIX
 * AIO (play_io_getevents() on non-Linux systems).
 */
static ssize_t do_aio_return(const struct event* e, struct aio_info* info, int infoidx,
                             struct aiocb* cb)
{
	const struct aiocb* cblist = cb;
	int aiostatus, sysret, err;
	ssize_t status = 0;

	/*
	 * We don't want to aio_suspend() until we know the aio has
	 * actually been started.  (suspend() before starting it hangs
	 * up the suspend()er permanently, at least on OSX.)  So first
	 * wait for the starting thread to signal that it has done so.
	 *
	 * FIXME: should !options.constraints.aio even be allowed now?  With
	 * generic AIO info getting "translated" between different union
	 * members in struct aio_info, if we use poscb while the real info is
	 * still in gencb, we're jumping straight into completely-undefined
	 * garbage-land.
	 */
	aiostatus = options.constraints.aio ? resstatus_waitlive(&info->status) : RES_AVAIL;

	info->times.check = getnanos();
	/*
	 * If it didn't return -1 (error), we know the call had to
	 * have happened after the aio completed, so we suspend until
	 * we know it has done so.  In the event that it *did* fail,
	 * just go ahead and call it.
	 */
	if (aiostatus == RES_AVAIL && e->ret.i != -1) {
		do {
			aio_debug("pre-aio_return aio_suspend(%p [cb#%d, fd#%d]) [%d@%d]",
			          cb, infoidx, cb->aio_fildes, e->serial, e->traceline);
			status = aio_suspend(&cblist, 1, NULL);
		} while (status && (errno == EAGAIN || errno == EINTR));
		if (status) {
			err = errno;
			warn("aio_suspend guarding aio_return failed: %s", strerror(err));
		}
	}

	aio_debug("%saio_return(%p [cb#%d, fd#%d])%s [%d@%d]",
	          aiostatus == RES_ERR ? "skipping " : "", cb, infoidx, cb->aio_fildes,
	          aiostatus == RES_ERR ? " on failed start" : "", e->serial,
	          e->traceline);

	/* Don't run the aio_return() if the call didn't get started */
	if (aiostatus == RES_AVAIL) {
		errno = 0;
		status = aio_return(cb);
	} else {
		errno = EIO;
		status = -1;
	}

	info->times.ret = getnanos();

	return status;
}

static int play_aio_return(const struct event* e)
{
	ssize_t status;
	const struct aio_return_attrs* a = &e->attrs.aio_return;
	struct aio_info* info = get_aio_info(a->infoidx);
	struct aiocb* cb = &info->u.poscb;

	status = do_aio_return(e, info, a->infoidx, cb);

	if (status != e->ret.i) {
		xlock(&warn_lock);
		fprintf(stderr, "Warning: aio_return(%p", cb);
		warn_sysret(status, "==", e->ret.i, e->serial, e->traceline, errno);
		xunlock(&warn_lock);
	}

	return status != e->ret.i;
}

static int play_aio_error(const struct event* e)
{
	int status, sysret, err;
	const struct aio_error_attrs* a = &e->attrs.aio_error;
	struct aio_info* info = get_aio_info(a->infoidx);
	const struct aiocb* cb = &info->u.poscb;

	/*
	 * Like aio_suspend(), we should wait until it's gotten started to
	 * call this.  Unlike the suspend() case, this won't hang up the
	 * thread (it just errors with EINVAL), but calling it early seems
	 * impolite (and probably means this thread has "gotten ahead" of
	 * where the real application *would* be anyway).
	 *
	 * FIXME: !options.constraints.aio: even meaningfully implementable
	 * anymore?  See comment above in play_aio_return().
	 */
	if (options.constraints.aio)
		resstatus_waitlive(&info->status);

	aio_debug("aio_error(%p [cb#%d, fd#%d]) [%d@%d]", cb, a->infoidx, cb->aio_fildes,
	          e->serial, e->traceline);

	errno = 0;
	sysret = aio_error(cb);
	status = !(sysret == e->ret.i || !sysret || sysret == EINPROGRESS);
	if (status) {
		err = errno;
		xlock(&warn_lock);
		fprintf(stderr, "Warning: aio_error(%p", cb);
		/* FIXME: this error message is a little half-assed */
		warn_sysret(sysret, "==", e->ret.i, e->serial, e->traceline, err);
		xunlock(&warn_lock);
	}

	return status;
}

/*
 * aio_suspend() is left as a (success-returning) stub, since its playback is
 * implicit in aio_return(), and a thread calling it presumably implies "I've
 * got nothing left to do until this finishes", so its next syscall is
 * probably going to be an aio_return() anyway (an examination of the traces
 * bears this out).  Also, the original traces lack anything with which to
 * correlate it to an aiocb, so I wouldn't really have a good way of playing
 * it back directly, anyway.
 */
static int play_aio_suspend(const struct event* e)
{
	aio_debug("bypassing aio_suspend [%d@%d]", e->serial, e->traceline);
	return 0;
}

#ifdef LINUX
/*
 * libaio's routines unfortunately don't follow the normal syscall
 * conventions for error signaling; these wrappers adapt them to look
 * like they do.
 */
static int sys_io_setup(unsigned nr_events, io_context_t* ctx)
{
	int status;

	/* io_setup() requires this initialization (not sure quite why) */
	*ctx = 0;

	status = io_setup(nr_events, ctx);

	if (status < 0) {
		errno = -status;
		return -1;
	} else {
		errno = 0;
		return status;
	}
}

/*
 * Presents a slightly different signature than the real thing so it can zero
 * out the context afterward.  This lets us tell which ones are still set up
 * and clean them up after the benchmark's done executing.
 */
static int sys_io_destroy(io_context_t *ctx)
{
	int status = io_destroy(*ctx);
	*ctx = 0;

	if (status < 0) {
		errno = -status;
		return -1;
	} else {
		errno = 0;
		return status;
	}
}

static int sys_io_submit(io_context_t ctx, long nr, struct iocb** iocbs)
{
	int status = io_submit(ctx, nr, iocbs);

	if (status < 0) {
		errno = -status;
		return -1;
	} else {
		errno = 0;
		return status;
	}
}

static int sys_io_getevents(io_context_t ctx, long min_nr, long nr,
                            struct io_event* events, struct timespec* timeout)
{
	int status = io_getevents(ctx, min_nr, nr, events, timeout);

	if (status < 0) {
		errno = -status;
		return -1;
	} else {
		errno = 0;
		return status;
	}
}

MAKE_PLAYFUNC_SB(io_setup, 2, a->nr_events, &bench->aioctxs[a->ctx_id]);

static int play_io_submit(const struct event* e)
{
	int ret;
	const struct io_submit_attrs* a = &e->attrs.io_submit;
	struct aio_info* info = get_aio_info(a->infoidx);
	io_context_t ctx = bench->aioctxs[a->ctx_id];
	struct genaio tmp = info->u.gencb;
	struct iocb* cb = &info->u.lincb;

	memset(cb, 0, sizeof(*cb));
	cb->aio_fildes = get_fd(a->fd.rtidx);
	cb->u.c.offset = tmp.offset;
	cb->u.c.nbytes = tmp.len;
	cb->u.c.buf = tmp.buf;

	aio_debug("io_submit(%p [cb#%d, fd[%d]->%d, ctx:%ju]) [%d@%d]", cb,
	          a->infoidx, a->fd.rtidx, cb->aio_fildes, (uintmax_t)ctx,
	          e->serial, e->traceline);

	info->times.sub = getnanos();

	ret = makesyscall(sys_io_submit, 3, e->ret.i, e, ctx, 1, &cb);

	info->times.subdone = getnanos();

	return ret;
}

static int play_io_getevents(const struct event* e)
{
	int aiostatus, ret;
	struct io_event ioe;
	const struct io_getevents_attrs* a = &e->attrs.io_getevents;
	struct aio_info* info = get_aio_info(a->infoidx);
	struct iocb* cb = &info->u.lincb;
	io_context_t ctx = bench->aioctxs[a->ctx_id];

	aio_debug("io_getevents(%p [cb#%d, fd[%d]->%d, ctx:%ju]) [%d@%d]", cb,
	          a->infoidx, a->fd.rtidx, cb->aio_fildes, (uintmax_t)ctx,
	          e->serial, e->traceline);

	info->times.check = getnanos();
	ret = makesyscall(sys_io_getevents, 5, e->ret.i, e, ctx, 1, 1, &ioe, NULL);
	info->times.ret = getnanos();

	return ret;
}

MAKE_PLAYFUNC_SB(io_destroy, 1, &bench->aioctxs[a->ctx_id]);

#else

/*
 * Emulation of Linux's io_*() routines on other systems via POSIX aio_*().
 * They're not a great fit for each other, API-semantics-wise, but we do the
 * best we can.
 */

/* Not applicable to the POSIX AIO model; contexts don't exist. */
static int play_io_setup(const struct event* e)
{
	return 0;
}

static int play_io_submit(const struct event* e)
{
	int (*aiofn)(struct aiocb*);
	int expected, ret;
	const struct io_submit_attrs* a = &e->attrs.io_submit;
	struct aio_info* info = get_aio_info(a->infoidx);
	struct genaio tmp = info->u.gencb;
	struct aiocb* cb = &info->u.poscb;

	cb->aio_fildes = get_fd(a->fd.rtidx);
	cb->aio_offset = tmp.offset;
	cb->aio_nbytes = tmp.len;
	cb->aio_buf = tmp.buf;
	aiofn = tmp.op == GA_PREAD ? aio_read
		: tmp.op == GA_PWRITE ? aio_write
		: (abort(), (int (*)(struct aiocb*))NULL);

	/*
	 * io_submit() returns the number of requests successfully submitted;
	 * aio_{read,write}() return 0 on success; artc's codegen ensures we
	 * only have one request submitted per io_submit().
	 */
	expected = e->ret.i == 1 ? 0 : -1;

	aio_debug("[emulated] io_submit(%p[%c] [cb#%d, fd[%d]->%d]) [%d@%d]", cb,
	          tmp.op == GA_PREAD ? 'R' : 'W', a->infoidx, a->fd.rtidx,
	          cb->aio_fildes, e->serial, e->traceline);

	info->times.sub = getnanos();
	ret = makesyscall(aiofn, 1, expected, e, cb);
	info->times.subdone = getnanos();

	if (options.constraints.aio)
		resstatus_change(&info->status, ret ? RES_ERR : RES_AVAIL);

	return ret;
}

static int play_io_getevents(const struct event* e)
{
	int status;
	const struct io_getevents_attrs* a = &e->attrs.io_getevents;
	struct aio_info* info = get_aio_info(a->infoidx);
	struct aiocb* cb = &info->u.poscb;

	status = do_aio_return(e, info, a->infoidx, cb);

	if (e->ret.i >= 0 && status < 0) {
		xlock(&warn_lock);
		fprintf(stderr, "Warning: io_getevents[aio_return](%p", cb);
		warn_sysret(status, "==", 0, e->serial, e->traceline, errno);
		xunlock(&warn_lock);
	}

	return status;
}

/* Like io_setup(), a no-op. */
static int play_io_destroy(const struct event* e)
{
	return 0;
}
#endif

static int play_threadspawn(const struct event* e)
{
	int status;
	pthread_t dummy;
	const struct threadspawn_attrs* a = &e->attrs.threadspawn;

	if (options.eager_threadspawn)
		return 0;

	status = pthread_create(&dummy, NULL, threadfunc, a->thread);

	if (status)
		warn("failed to spawn thread %d!", a->thread->tid);

	return !!status;
}

int (*replay_fns[])(const struct event*) = {
	[EV_OPEN]              = play_open,
	[EV_SHM_OPEN]          = play_shm_open,
	[EV_CLOSE]             = play_close,
	[EV_DUP]               = play_dup,
	[EV_KQUEUE]            = play_kqueue,
	[EV_SOCKET]            = play_socket,
	[EV_SOCKETPAIR]        = play_socketpair,
	[EV_READ]              = play_read,
	[EV_WRITE]             = play_write,
	[EV_PREAD]             = play_pread,
	[EV_PWRITE]            = play_pwrite,
	[EV_AIO_READ]          = play_aio_read,
	[EV_AIO_RETURN]        = play_aio_return,
	[EV_AIO_ERROR]         = play_aio_error,
	[EV_AIO_SUSPEND]       = play_aio_suspend,
	[EV_LSEEK]             = play_lseek,
	[EV_FSYNC]             = play_fsync,
	[EV_FDATASYNC]         = play_fdatasync,
	[EV_FCNTL]             = play_fcntl,
	[EV_FTRUNCATE]         = play_ftruncate,
	[EV_LINK]              = play_link,
	[EV_SYMLINK]           = play_symlink,
	[EV_RMDIR]             = play_rmdir,
	[EV_UNLINK]            = play_unlink,
	[EV_READLINK]          = play_readlink,
	[EV_RENAME]            = play_rename,
	[EV_EXCHANGEDATA]      = play_exchangedata,
	[EV_MKDIR]             = play_mkdir,
	[EV_FLOCK]             = play_flock,
	[EV_MMAP]              = play_mmap,
	[EV_FUTIMES]           = play_futimes,
	[EV_STATFS]            = play_statfs,
	[EV_STATFS64]          = play_statfs64,
	[EV_FSTATFS64]         = play_fstatfs64,
	[EV_FSTAT]             = play_fstat,
	[EV_LSTAT]             = play_lstat,
	[EV_STAT]              = play_stat,
	[EV_FSTAT64]           = play_fstat64,
	[EV_LSTAT64]           = play_lstat64,
	[EV_STAT64]            = play_stat64,
	[EV_SETATTRLIST]       = play_setattrlist,
	[EV_GETATTRLIST]       = play_getattrlist,
	[EV_GETXATTR]          = play_getxattr,
	[EV_FGETXATTR]         = play_fgetxattr,
	[EV_LISTXATTR]         = play_listxattr,
	[EV_FLISTXATTR]        = play_flistxattr,
	[EV_ACCESS]            = play_access,
	[EV_CHOWN]             = play_chown,
	[EV_CHMOD]             = play_chmod,
	[EV_FSETXATTR]         = play_fsetxattr,
	[EV_FSGETPATH]         = play_fsgetpath,
	[EV_GETDIRENTRIES64]   = play_getdirentries64,
	[EV_GETDIRENTRIESATTR] = play_getdirentriesattr,
	[EV_GETFSSTAT64]       = play_getfsstat64,
	[EV_GETDTABLESIZE]     = play_getdtablesize,
	[EV_CHDIR]             = play_chdir,
	[EV_FCHDIR]            = play_fchdir,
	[EV_GETCWD]            = play_getcwd,
	[EV_KILL]              = play_kill,
	[EV_FCHFLAGS]          = play_fchflags,
	[EV_FCHMOD]            = play_fchmod,
	[EV_FCHOWN]            = play_fchown,
	[EV_REMOVEXATTR]       = play_removexattr,
	[EV_FREMOVEXATTR]      = play_fremovexattr,
	[EV_SETXATTR]          = play_setxattr,
	[EV_HFS_VNOP_PAGEIN]   = play_hfs_vnop_pagein,
	[EV_POSIX_SPAWN]       = play_posix_spawn,

	[EV_FADVISE64]         = play_fadvise64,
	[EV_FSTATFS]           = play_fstatfs,
	[EV_GETDENTS]          = play_getdents,
	[EV_EPOLL_CREATE]      = play_epoll_create,
	[EV_DUP2]              = play_dup2,
	[EV_FCHOWNAT]          = play_fchownat,
	[EV_FACCESSAT]         = play_faccessat,
	[EV_UNLINKAT]          = play_unlinkat,
	[EV_PIPE]              = play_pipe,

	[EV_IO_SETUP]          = play_io_setup,
	[EV_IO_SUBMIT]         = play_io_submit,
	[EV_IO_GETEVENTS]      = play_io_getevents,
	[EV_IO_DESTROY]        = play_io_destroy,

	[EV__THREADSPAWN]      = play_threadspawn,

	[EV__MAX_+1] = NULL,
};
