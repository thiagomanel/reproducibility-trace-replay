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
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

/* sync(2) with extreme prejudice. */

static void drop_caches(void)
{
	int fd = open("/proc/sys/vm/drop_caches", O_WRONLY);

	if (fd < 0) {
		perror("/proc/sys/vm/drop_caches");
		exit(1);
	}

	sync();
	if (write(fd, "3\n", 2) != 2) {
		fprintf(stderr, "drop_caches failed\n");
		exit(1);
	}

	close(fd);
}

int main(int argc, char** argv, char** envp)
{
	/* Twice, for good measure. */
	drop_caches();
	drop_caches();

	return 0;
}
