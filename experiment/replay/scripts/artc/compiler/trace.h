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

#ifndef TRACE_H
#define TRACE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>

#include "misc.h"

#ifdef ARTC

#include <limits.h>
#include "assoc.h"

struct pginfo;

#else

#include <aio.h>
#include <pthread.h>

#include "system.h"

#ifdef LINUX
#include <libaio.h>
#endif

struct initpath;

#endif

/*
 * The proper typing of this is a bit confusing.  Linux man pages reference
 * aio_context_t as if it were an opaque integer, and linux/aio_abi.h typedefs
 * it from __kernel_ulong_t, but the function prototypes in libaio.h refer
 * instead to 'io_context_t', which is typedef'd from 'struct io_context*'.
 *
 * (Shrug.)
 */
typedef uintptr_t aio_context_t;

typedef enum {
	EV__INVAL_ = 0,

	EV_OPEN,
	EV_SHM_OPEN,
	EV_CLOSE,
	EV_DUP,
	EV_KQUEUE,
	EV_SOCKET,
	EV_SOCKETPAIR,
	EV_READ,
	EV_WRITE,
	EV_PREAD,
	EV_PWRITE,
	EV_AIO_READ,
	EV_AIO_ERROR,
	EV_AIO_RETURN,
	EV_AIO_SUSPEND,
	EV_LSEEK,
	EV_FSYNC,
	EV_FDATASYNC,
	EV_FCNTL,
	EV_FTRUNCATE,
	EV_LINK,
	EV_SYMLINK,
	EV_RMDIR,
	EV_UNLINK,
	EV_READLINK,
	EV_RENAME,
	EV_EXCHANGEDATA,
	EV_MKDIR,
	EV_FLOCK,
	EV_MMAP,
	EV_FUTIMES,
	EV_UTIMES,
	EV_STATFS,
	EV_STATFS64,
	EV_FSTATFS64,
	EV_FSTAT,
	EV_LSTAT,
	EV_STAT,
	EV_FSTAT64,
	EV_LSTAT64,
	EV_STAT64,
	EV_SETATTRLIST,
	EV_GETATTRLIST,
	EV_GETXATTR,
	EV_FGETXATTR,
	EV_LISTXATTR,
	EV_FLISTXATTR,
	EV_ACCESS,
	EV_CHOWN,
	EV_CHMOD,
	EV_FSETXATTR,
	EV_FSGETPATH,
	EV_GETDIRENTRIES64,
	EV_GETDIRENTRIESATTR,
	EV_GETFSSTAT64,
	EV_GETDTABLESIZE,
	EV_CHDIR,
	EV_FCHDIR,
	EV_GETCWD,
	EV_KILL,
	EV_FCHFLAGS,
	EV_FCHMOD,
	EV_FCHOWN,
	EV_REMOVEXATTR,
	EV_FREMOVEXATTR,
	EV_SETXATTR,
#ifdef ARTC
	EV_CONTROLLER,
	EV_RESOLVER,
	EV_ALL_CALLS,
	EV_SETTER,
	EV_BUILD_PATH,
	EV_NAMEI,
	EV_VN_CREATE,
	EV_VNODE_PAGEIN,
	EV_SYSCALL_MAP,
#endif
	EV_HFS_VNOP_PAGEIN,
	EV_POSIX_SPAWN,
	EV_EXECVE,

	EV_FADVISE64,
	EV_FSTATFS,
	EV_GETDENTS,
	EV_EPOLL_CREATE,
	EV_FALLOCATE,
	EV_UTIMENSAT,
	EV_PIPE,
	EV_DUP2,
	EV_FACCESSAT,
	EV_NEWFSTATAT,
	EV_FCHOWNAT,
	EV_UNLINKAT,

	EV_IO_SETUP,
	EV_IO_SUBMIT,
	EV_IO_GETEVENTS,
	EV_IO_DESTROY,

	/* EV__* are pseudo-events inserted by artc itself */
	EV__THREADSPAWN,

	/*
	 * EV__THREADCONT is a hack to split a single thread's trace across
	 * multiple output files (when they would otherwise get prohibitively
	 * large).
	 */
	EV__THREADCONT,
	EV__THREADEXIT,

	EV__MIN_ = EV_OPEN,
	EV__MAX_ = EV__THREADEXIT,
} evid_t;

/*
 * Generalized representation of resource availability (for e.g. a file
 * descriptor or an aiocb).  Starts out with status == RES_FUTURE (meaning the
 * resource will [hopefully] become available in the future); users check
 * status and wait on 'birth' if it's still RES_FUTURE.  The provider of the
 * resource (e.g. the caller of open(), aio_read(), etc.) updates status to
 * RES_AVAIL or RES_ERR and broadcasts on 'birth' to wake up the waiting
 * consumers/users of the resource.  RES_DEAD indicates the resource has been
 * reaped and its lifespan has ended; a broadcast on 'death' is sent when it
 * transitions to this state.  Resources can also use 'evcount' to see how
 * many remaining events need this resource.  Each event that does use the
 * resource decrements this count on completion and sends a broadcast on the
 * 'event' condition variable.
 */
typedef enum {
	RES_FUTURE,
	RES_AVAIL,
	RES_DEAD,
	RES_ERR,
} resstate_t;

struct resstatus {
	resstate_t state;
	int evcount;
	pthread_cond_t cond;
	pthread_mutex_t lock;
};

#define RESSTATUS_INIT(s, c) { \
	.state = s, \
	.evcount = c, \
	.cond = PTHREAD_COND_INITIALIZER, \
	.lock = PTHREAD_MUTEX_INITIALIZER, \
}

struct fd_info {
	struct resstatus status;
	int fd;
	int file_idx;
};

struct fduse {
#ifdef ARTC
	/* Original trace FD value */
	int traceval;
#endif
	/* Runtime index into FDs array */
	int rtidx;

	/* Event that makes this FD available for use */
	struct event* provider;

	/* Event that last used this FD */
	struct event* lastuse;

#ifndef ARTC
	/* 1 if the FD refers to something other than a regular file, 0 otherwise */
	int nonfile;
#endif
};

typedef enum {
	AT_PWRITE,
	AT_PREAD,
} aio_type_t;

struct artc_iocb {
	aio_type_t type;
	uintptr_t data;
	struct fduse fd;
	size_t nbytes;
	off_t offset;
	struct artc_iocb* next;
	char* dummy;

	/* compilation/replay metadata */
	int aiocbnum;
};

struct artc_ioevent {
	uintptr_t data;
	uintptr_t obj;
	unsigned long res;
	unsigned long res2;
	struct artc_ioevent* next;

	/* compilation/replay metadata */
	struct fduse fd;
	int aiocbnum;
};

struct open_attrs {
	char* path;
#ifdef ARTC
	struct symflag* flags;
	mode_t mode; /* not in iBench traces */

	char* dummy1;
	char* dummy2;
#else
	int flags;
	mode_t mode;
#endif
};

struct shm_open_attrs {
	char* name;
	int oflag;
};

struct close_attrs {
	struct fduse fd;
};

struct dup_attrs {
	struct fduse fd;
};

struct kqueue_attrs {
};

struct socket_attrs {
	int domain, type, protocol;
};

struct socketpair_attrs {
	int domain, type, protocol;
	/* Not strictly a use; convenient though. */
	struct fduse fd1, fd2;
};

struct read_attrs {
	struct fduse fd;
	size_t count;
#ifdef ARTC
	char* dummy1;
#else
	void* iobuf;
#endif
};

struct write_attrs {
	struct fduse fd;
	size_t count;
#ifdef ARTC
	char* dummy1;
#else
	void* iobuf;
#endif
};

struct pread_attrs {
	struct fduse fd;
	size_t count;
	off_t offset;
#ifdef ARTC
	char* dummy1;
#else
	void* iobuf;
#endif
};

struct pwrite_attrs {
	struct fduse fd;
	size_t count;
	off_t offset;
#ifdef ARTC
	char* dummy1;
#else
	void* iobuf;
#endif
};

struct aio_read_attrs {
	struct fduse fd;
#ifdef ARTC
	uintptr_t tag;
	off_t offset;
	size_t nbytes;
	char* dummy1;
	int aiocbnum;
#else
	int infoidx;
#endif
};

struct aio_error_attrs {
	/* Not in trace, deduced via tag-matching by compiler */
	struct fduse fd;
#ifdef ARTC
	uintptr_t tag;
	int aiocbnum;
#else
	int infoidx;
#endif
};

struct aio_return_attrs {
	/* Not in trace, deduced via tag-matching by compiler */
	struct fduse fd;
#ifdef ARTC
	uintptr_t tag;
	int aiocbnum;
#else
	int infoidx;
#endif
};

struct aio_suspend_attrs {
	int nent;
};

struct lseek_attrs {
	struct fduse fd;
	off_t offset;
	int whence;
};

struct fsync_attrs {
	struct fduse fd;

	char* dummy1;
};

struct fdatasync_attrs {
	struct fduse fd;

	char* dummy1;
};

struct fcntl_attrs {
	struct fduse fd;
	int cmdnum;
#ifdef ARTC
	char* cmdname;
	union {
		long l;
		struct symflag* flags;
	} arg;
#else
	int cmd;
	long arg;
#endif

	char* dummy1;
};

struct ftruncate_attrs {
	struct fduse fd;
	off_t length;

	char* dummy1;
};

struct link_attrs {
	char* oldpath;
	char* newpath;
};

struct symlink_attrs {
	char* oldpath;
	char* newpath;
};

struct rmdir_attrs {
	char* pathname;
};

struct unlink_attrs {
	char* pathname;
};

struct readlink_attrs {
	char* path;
	size_t bufsiz;
#ifdef ARTC
	char* dummy1;
#else
	void* iobuf;
#endif
};

struct rename_attrs {
	char* oldpath;
	char* newpath;

	char* dummy1;
};

struct exchangedata_attrs {
	char* path1;
	char* path2;
};

struct mkdir_attrs {
	char* pathname;
	int mode;
};

struct flock_attrs {
	struct fduse fd;
	int operation;

	char* dummy1;
};

struct mmap_attrs {
	void* addr;
	size_t length;
#ifdef ARTC
	struct symflag* prot;
	struct symflag* flags;
#else
	int prot, flags;
#endif
	struct fduse fd;
	off_t offset;
};

struct futimes_attrs {
	struct fduse fd;
};

struct utimes_attrs {
	/* iBench traces have nothing useful for us. */
};

struct statfs_attrs {
	char* path;
};

struct statfs64_attrs {
	char* path;
};

struct fstatfs64_attrs {
	struct fduse fd;
};

struct fstat_attrs {
	struct fduse fd;
};

struct lstat_attrs {
	char* path;
};

struct stat_attrs {
	char* path;
};

struct fstat64_attrs {
	struct fduse fd;
};

struct lstat64_attrs {
	char* path;
};

struct stat64_attrs {
	char* path;
};

struct setattrlist_attrs {
	char* path;
};

struct getattrlist_attrs {
	char* path;
};

struct getxattr_attrs {
	char* path;

	/* Not in iBench traces */
	char* name;
	size_t size;
	char* dummy1;
};

struct fgetxattr_attrs {
	struct fduse fd;

	/* iBench traces don't have these */
	char* name;
	size_t size;
	char* dummy1;
};

struct listxattr_attrs {
	char* path;

	/* iBench traces don't have this */
	size_t size;
	char* dummy1;
};

struct flistxattr_attrs {
	struct fduse fd;

	/* Not in iBench traces */
	size_t size;
	char* dummy1;
};

struct access_attrs {
	char* pathname;
#ifdef ARTC
	union {
		int mode; /* iBench */
		struct symflag* flags; /* strace */
	};
#else
	int mode;
#endif
};

struct chown_attrs {
	char* path;
	uid_t owner;
	gid_t group;
};

struct chmod_attrs {
	char* path;
	mode_t mode;
};

struct fsetxattr_attrs {
	struct fduse fd;
	char* name;

	/* iBench traces don't have this */
	size_t size;
	char* dummy1;
};

struct fsgetpath_attrs {
	ino_t inum;
};

struct getdirentries64_attrs {
	struct fduse fd;
#ifndef ARTC
	void* iobuf;
	size_t iobufsize;
#endif
};

struct getdirentriesattr_attrs {
	struct fduse fd;
};

struct getfsstat64_attrs {
	/* Nothing useful in the traces. */
};

struct getdtablesize_attrs {
};

struct chdir_attrs {
	char* path;
};

struct fchdir_attrs {
	struct fduse fd;
};

struct getcwd_attrs {
	size_t size;

#ifdef ARTC
	char* dummy1;
#else
	void* buf;
#endif
};

struct kill_attrs {
	pid_t pid;
	int sig;
};

struct fchflags_attrs {
	struct fduse fd;
	int flags;
};

struct fchmod_attrs {
	struct fduse fd;
	mode_t mode;
};

struct fchown_attrs {
	struct fduse fd;
	uid_t owner;
	gid_t group;
};

struct removexattr_attrs {
	char* path;
	char* name;
};

struct fremovexattr_attrs {
	struct fduse fd;
	char* name;
};

struct setxattr_attrs {
	char* path;
	char* name;

	/* Not in iBench traces */
	size_t size;
	char* dummy1;
};

#ifdef ARTC
struct controller_attrs {
	char* msg;
};

struct resolver_attrs {
};

struct all_calls_attrs {
};

struct setter_attrs {
};

struct build_path_attrs {
};

struct namei_attrs {
};

struct vn_create_attrs {
};

struct vnode_pagein_attrs {
};

struct syscall_map_attrs {
	char* dummy1;
};
#endif

struct hfs_vnop_pagein_attrs {
	off_t offset;
	size_t size;
	char* name;

	/*
	 * How much file content actually gets brought in (could be
	 * less than page size).
	 */
	size_t actual_size;
};

struct posix_spawn_attrs {
	char* path;
};

struct execve_attrs {
};

struct fadvise64_attrs {
	struct fduse fd;
	off_t offset;
	off_t len;
#ifdef ARTC
	char* advice;
#else
	int advice;
#endif
};

struct fstatfs_attrs {
	struct fduse fd;
};

struct getdents_attrs {
	struct fduse fd;
	unsigned int count;
};

struct epoll_create_attrs {
	int size;
};

struct fallocate_attrs {
	struct fduse fd;
	int mode;
	off_t offset;
	off_t len;
};

struct utimensat_attrs {
	struct fduse dirfd;
	char* pathname;
	int flags;
};

struct pipe_attrs {
	struct fduse rd;
	struct fduse wr;
};

struct dup2_attrs {
	struct fduse old;
	struct fduse new;

	/*
	 * HACK: dup3 is folded into dup2, so we store (and ignore) its flags
	 * argument here, just so it's not leaked in the parser.
	 */
	struct symflag* flags;
};

struct faccessat_attrs {
	struct fduse dirfd;
	char* pathname;
#ifdef ARTC
	struct symflag* mode;
#else
	mode_t mode;
#endif

	/* Note: not reported by strace 4.7 */
	int flags;
};

struct newfstatat_attrs {
	struct fduse dirfd;
	char* pathname;
#ifdef ARTC
	struct symflag* flags;
#else
	int flags;
#endif
};

struct fchownat_attrs {
	struct fduse dirfd;
	char* pathname;
	uid_t owner;
	gid_t group;
#ifdef ARTC
	struct symflag* flags;
#else
	int flags;
#endif
};

struct unlinkat_attrs {
	struct fduse dirfd;
	char* pathname;
#ifdef ARTC
	struct symflag* flags;
#else
	int flags;
#endif
};

struct io_setup_attrs {
	unsigned nr_events;
#ifdef ARTC
	aio_context_t ctx;
#endif
	int ctx_id;
};

struct io_submit_attrs {
#ifdef ARTC
	aio_context_t ctx;
	long nr;
	struct artc_iocb* iocbs;
#else
	int infoidx;
	struct fduse fd;
#endif
	int ctx_id;
};

struct io_getevents_attrs {
#ifdef ARTC
	aio_context_t ctx;
	long min_nr, nr;
	struct artc_ioevent* ioevents;
#else
	int infoidx;
	struct fduse fd;
#endif
	int ctx_id;
};

struct io_destroy_attrs {
#ifdef ARTC
	aio_context_t ctx;
#endif
	int ctx_id;
};

struct threadspawn_attrs {
#ifdef ARTC
	int tid;
#else
	struct thread* thread;
#endif
};

struct threadcont_attrs {
#ifdef ARTC
#else
	struct event* next;
#endif
};

#ifdef ARTC
/*
 * Used to record a linked list of symbolic flag names (e.g. O_RDONLY,
 * MAP_SHARED, PROT_READ).
 */
struct symflag {
	struct symflag* next;
	char name[];
};
#endif

#ifdef ARTC
typedef enum {
	ST__INVALID_ = 0,

	ST_ENTRY,
	ST_RETURN,
	ST_COMMENT,
	ST_CALL,

	ST__MAX_ = ST_CALL,
} evsubtype_t;
#endif

struct filemd {
#ifdef ARTC
	struct pginfo* pgi;
#else
	int idx;
#endif
	struct event* lastuse;
};

struct depdelays {
	nstime_t thread;
	nstime_t fddep;
	nstime_t fdorder;
	nstime_t pathdep;
	nstime_t fileorder;
	nstime_t aiodep;
};

#define DEPDELAY_STATICINIT { .thread = -1, \
			.fddep = -1, \
			.fdorder = -1, \
			.pathdep = -1, \
			.fileorder = -1, \
			.aiodep = -1, }
#define DEPDELAY_INIT (struct depdelays) DEPDELAY_STATICINIT

/*
 * Records a reference to another event on which another event will be treated
 * as dependent for temporally-ordered replay.
 */
struct event_dep {
	struct event* event;
#ifdef ARTC
	struct event_dep* next;
#endif
};

struct event {
	int traceline, serial;

	struct depdelays predelays;
	float cpu_util;

	evid_t type;

	union {
		struct open_attrs open;
		struct shm_open_attrs shm_open;
		struct close_attrs close;
		struct dup_attrs dup;
		struct kqueue_attrs kqueue;
		struct socket_attrs socket;
		struct socketpair_attrs socketpair;
		struct read_attrs read;
		struct write_attrs write;
		struct pread_attrs pread;
		struct pwrite_attrs pwrite;
		struct aio_read_attrs aio_read;
		struct aio_error_attrs aio_error;
		struct aio_return_attrs aio_return;
		struct aio_suspend_attrs aio_suspend;
		struct lseek_attrs lseek;
		struct fsync_attrs fsync;
		struct fdatasync_attrs fdatasync;
		struct fcntl_attrs fcntl;
		struct ftruncate_attrs ftruncate;
		struct link_attrs link;
		struct symlink_attrs symlink;
		struct rmdir_attrs rmdir;
		struct unlink_attrs unlink;
		struct readlink_attrs readlink;
		struct rename_attrs rename;
		struct exchangedata_attrs exchangedata;
		struct mkdir_attrs mkdir;
		struct flock_attrs flock;
		struct mmap_attrs mmap;
		struct futimes_attrs futimes;
		struct utimes_attrs utimes;
		struct statfs_attrs statfs;
		struct statfs64_attrs statfs64;
		struct fstatfs64_attrs fstatfs64;
		struct fstat_attrs fstat;
		struct lstat_attrs lstat;
		struct stat_attrs stat;
		struct fstat64_attrs fstat64;
		struct lstat64_attrs lstat64;
		struct stat64_attrs stat64;
		struct setattrlist_attrs setattrlist;
		struct getattrlist_attrs getattrlist;
		struct getxattr_attrs getxattr;
		struct fgetxattr_attrs fgetxattr;
		struct listxattr_attrs listxattr;
		struct flistxattr_attrs flistxattr;
		struct access_attrs access;
		struct chown_attrs chown;
		struct chmod_attrs chmod;
		struct fsetxattr_attrs fsetxattr;
		struct fsgetpath_attrs fsgetpath;
		struct getdirentries64_attrs getdirentries64;
		struct getdirentriesattr_attrs getdirentriesattr;
		struct getfsstat64_attrs getfsstat64;
		struct getdtablesize_attrs getdtablesize;
		struct chdir_attrs chdir;
		struct fchdir_attrs fchdir;
		struct getcwd_attrs getcwd;
		struct kill_attrs kill;
		struct fchflags_attrs fchflags;
		struct fchmod_attrs fchmod;
		struct fchown_attrs fchown;
		struct removexattr_attrs removexattr;
		struct fremovexattr_attrs fremovexattr;
		struct setxattr_attrs setxattr;
#ifdef ARTC
		struct controller_attrs controller;
		struct resolver_attrs resolver;
		struct all_calls_attrs all_calls;
		struct setter_attrs setter;
		struct build_path_attrs build_path;
		struct namei_attrs namei;
		struct vn_create_attrs vn_create;
		struct vnode_pagein_attrs vnode_pagein;
		struct syscall_map_attrs syscall_map;
#endif
		struct hfs_vnop_pagein_attrs hfs_vnop_pagein;
		struct posix_spawn_attrs posix_spawn;
		struct execve_attrs execve;

		struct fadvise64_attrs fadvise64;
		struct fstatfs_attrs fstatfs;
		struct getdents_attrs getdents;
		struct epoll_create_attrs epoll_create;
		struct fallocate_attrs fallocate;
		struct utimensat_attrs utimensat;
		struct pipe_attrs pipe;
		struct dup2_attrs dup2;
		struct faccessat_attrs faccessat;
		struct newfstatat_attrs newfstatat;
		struct fchownat_attrs fchownat;
		struct unlinkat_attrs unlinkat;

		struct io_setup_attrs io_setup;
		struct io_submit_attrs io_submit;
		struct io_getevents_attrs io_getevents;
		struct io_destroy_attrs io_destroy;

		struct threadspawn_attrs threadspawn;
		struct threadcont_attrs threadcont;
	} attrs;

	union {
		int i;
		off_t o;
		void* p;
#ifdef ARTC
		/* Not strictly a *use*, but convenient. */
		struct fduse fd;
#endif
	} ret;

	struct {
		/* Lists terminated by a -1 entry (or NULL if empty) */
		int* consumed;
		int* produced;
		int* removed;

		/* Event that removes the pathgen we require to  */
		struct event* negdep;
	} paths;

#ifdef ARTC
	struct filemd* files;
#else
	struct filemd files[2];
#endif

	struct {
		nstime_t entry, exit;
	} time;

#ifdef ARTC
	int32_t thread;
	evsubtype_t subtype;
	struct event* next;
	struct event* prev;

	/* 0-based count of which event this is within its own thread. */
	int thread_evidx;
#endif

#ifndef ARTC
	/* RES_FUTURE before replay, RES_AVAIL after. */
	struct resstatus status;
#endif

	/*
	 * A linked list (in the ARTC case) or NULL-terminated array (!ARTC)
	 * of the last events in each thread whose exits occurred strictly
	 * before this event's entry.  In temporal-ordering mode, each of
	 * these must have completed (as indicated by its 'status' member)
	 * before this event can replay.
	 *
	 * It's not *all* other threads though, only those that have had an
	 * event complete since the start of the last event in this thread.
	 * (See also: threadtrace.thread_temporal_deps.)
	 */
	struct event_dep* temporal_deps;
};

#define evattr(type, ev, name) ((ev)->attrs.type.name)

/*
 * The maximum number of aiocbs we'll put in a single generated file, which in
 * turn needs to be known by artrun so it can look up an aiocb in the right
 * chunk of the top-level array that ties the chunks together.
 */
#define AIOCBS_PER_CHUNK 100000

#ifndef ARTC
struct thread {
	struct event* evtable;
	int tid;
};

/* Basic, generic AIO information */
typedef enum {
	GA_PREAD,
	GA_PWRITE,
} ga_op_t;

struct genaio {
	ga_op_t op;
	int fd;
	off_t offset;
	size_t len;
	void* buf;
};

struct aio_info {
	struct resstatus status;
	union {
		/* System-independent AIO request information */
		struct genaio gencb;

		/* POSIX AIO (aio_{read,error,return...}) */
		struct aiocb poscb;

#ifdef LINUX
		/* native Linux AIO (io_{submit,getevents}...) */
		struct iocb lincb;
#endif
	} u;

	/* Timestamps of... */
	struct aiotimes {
		nstime_t sub; /* ...io_submit() entry */
		nstime_t subdone; /* ...io_submit() return */
		nstime_t check; /* ...io_getevents() entry */
		nstime_t ret; /* ...io_getevents() return */
	} times;
};

struct benchmark {
	struct fd_info* fds;
	const unsigned int num_fds;

	const int* mystery_fds;
	const unsigned int num_mystery_fds;

	struct aio_info** aiocbs;
	const unsigned int num_aiocbs;

#ifdef LINUX
	io_context_t* aioctxs;
	const unsigned int num_aioctxs;

	const int* mystery_aioctxs;
	const unsigned int num_mystery_aioctxs;
#endif

	struct pathgen* pathgens;
	const unsigned int num_pathgens;

	struct resstatus* files;
	const unsigned int num_files;

	struct thread** threads;
	const unsigned int num_threads;
	struct thread* first_thread;

	struct thread* setup;

	unsigned long cpucount;

	const char* version_string;

	void (*visit_initpaths)(void (*fn)(const struct initpath* ip, void* arg),
	                        void* arg);

	/* Signature to match gperf-generated code */
	struct initpath* (*get_initpath)(register const char* str,
	                                 register unsigned int len);
};
extern const struct benchmark* bench;

static inline struct aio_info* get_aio_info(int idx)
{
	int chunkidx, infoidx;

	chunkidx = idx / AIOCBS_PER_CHUNK;
	infoidx = idx % AIOCBS_PER_CHUNK;

	return &bench->aiocbs[chunkidx][infoidx];
}
#endif

#ifdef ARTC

#define SETUP_TID 0

MAKE_ASSOC(ie_assoc, int, -1, struct event*, NULL);

struct threadtrace {
	int id;
	struct {
		struct event* head;
		struct event* tail;
	} evlist;
	size_t maxbuf; /* size of largest I/O buffer in thread trace */
	long length; /* Number of system calls in the trace */
	nstime_t lastreturn; /* timestamp of most recent return event */

	/*
	 * Maps *other* threads' tids to the last event of *that* thread that
	 * has been "seen" (i.e. temporally depended on) by an event in *this*
	 * thread.  This is so later threads in this thread don't have to
	 * explicitly record a temporal dependency on those events, since that
	 * dependency will be implicitly enforced by being later in the same
	 * threadtrace than a thread that already had that dependency, and
	 * instead only has to record a "delta" of new temporal dependencies
	 * that the previous event in the same thread did not have.
	 */
	ie_assoc_t thread_temporal_deps;
};

MAKE_ASSOC(ip_assoc, int, -1, struct pginfo*, NULL);
MAKE_ASSOC(it_assoc, int, INT_MIN, struct threadtrace*, NULL);
MAKE_ASSOC(ii_assoc, int, INT_MIN, int, INT_MIN);
MAKE_ASSOC(pi_assoc, uintptr_t, 0, int, -1);

typedef enum {
	TT_IBENCH,
	TT_STRACE,
} tracetype_t;

/* Used as a sentinel in fd_pgi_map */
#define NONFILE_PGINFO ((struct pginfo*)-1LL)

struct traceinfo {
	const char* path;
	tracetype_t type;
	const char* initfile_path;

	it_assoc_t threadassoc;
	ii_assoc_t pathidx_remap;
	ie_assoc_t mystery_fds;
	ip_assoc_t fd_pgi_map;
	pi_assoc_t aioctx_assoc;
	ii_assoc_t mystery_aioctxs;

	struct threadtrace setupthread;
	int numthreads;
	unsigned int num_fds;
	unsigned int num_mystery_fds;
	unsigned int num_mystery_aioctxs;
	unsigned int num_aio_infos;
	unsigned int num_aio_ctxs;
	unsigned int* fdidx_evcounts;
	unsigned int fdidx_evcounts_slots;

	unsigned int event_type_counts[EV__MAX_+1];
	struct fsmodel* fsm;

	unsigned long cpucount;
	struct {
		nstime_t start, end;
	} time;
};

extern struct traceinfo traceinfo;
extern int all_init_files;
extern int include_intrathread_deps;

struct parser_state {
	int linenum;
	struct event* events;
	struct event* last;
};

void append_event(struct event* ev, struct parser_state* state);
#endif

#endif /* TRACE_H */
