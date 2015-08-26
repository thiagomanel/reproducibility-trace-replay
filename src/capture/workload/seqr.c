#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h> //usleep
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <stdint.h> //uint64_t
#include <stdlib.h> //rand

#define __STDC_FORMAT_MACRO
#include <inttypes.h>

#define ACCESS_PERMISSION 0777

//FIXME: add header and explain the parts we used from zev code
//FIXME: refactor workload code they share a lot

int debug;

//the sequential operation batch start at a random offset
long start_offset;

typedef struct thread_load {
    int thread_id;
    int fd;
    long file_size;
    ssize_t * rt_count;
    int nreq;
    useconds_t delay;
    char * buf;
    int blksize;
    uint64_t * begin;
    uint64_t * end;
} thread_load;

static uint64_t stamp (void) {
   struct timespec tspec;
   if (clock_gettime (CLOCK_MONOTONIC, &tspec)) {
       fprintf (stderr, "Error getting timestamp: %s\n", strerror(errno));
       exit (EXIT_FAILURE);
   }
   return (tspec.tv_sec * 1000000000ULL) + tspec.tv_nsec;
}

static void *request (void *arg) {

    int i;
    thread_load* load = arg;

    lseek (load->fd, start_offset, SEEK_SET);

    for (i = 0; i < load->nreq; i++) {
        if (load->delay > 0) {
	    usleep (load->delay);
	}

	if (debug) {
	    load->begin[i] = stamp ();
	}
        load->rt_count[i] = read (load->fd, load->buf, load->blksize);
	if (debug) {
	    load->end[i] = stamp ();
	}
    }

    return NULL;
}

int main (int argc, char* argv[]) {

    int i, j, fd;
    char pathbuf[256];
    struct stat st;

    int num_threads = atoi (argv[1]);
    int delay = atoi (argv[2]);
    int num_ops_per_thread = atoi (argv[3]);
    char* path = argv[4];
    int blksize = atoi (argv[5]);

    if (strcmp (argv[6], "debug") == 0) {
        debug = 1;
    } else if (strcmp (argv[6], "no-debug") == 0) {
    	debug = 0;
    } else {
  	fprintf (stderr, "Missing debug or no-debug flag\n");
	exit (EXIT_FAILURE);
    }

    srand(time(NULL));

    thread_load* load = (thread_load*) malloc (sizeof (thread_load) * num_threads);
    for (i = 0; i < num_threads; i++) {

	snprintf (pathbuf, sizeof pathbuf, "%s%d", path, i);
	fd = open (pathbuf, O_RDWR, ACCESS_PERMISSION);
	if (fd < 0) {
  	    fprintf (stderr, "Error opening file: %s\n", strerror (errno));
	    exit (EXIT_FAILURE);
	}
    	stat (pathbuf, &st);

	load[i].file_size = st.st_size;
	load[i].fd = fd;
        load[i].thread_id = i;
	load[i].nreq = num_ops_per_thread;
	load[i].delay = delay;
	load[i].blksize = blksize;
	load[i].buf = (char*) malloc (sizeof (char) * blksize);
	load[i].rt_count = (ssize_t*) calloc (num_ops_per_thread, sizeof(ssize_t));

	if (debug) {
	    load[i].begin = (uint64_t*) calloc (num_ops_per_thread, sizeof(uint64_t));
	    load[i].end = (uint64_t*) calloc (num_ops_per_thread, sizeof(uint64_t));
	} else {
	    load[i].begin = NULL;
	    load[i].end = NULL;
	}
    }

    //safe random offset function, we are going to execute nops sequential
    //requests, each requesting blksize bytes
    start_offset = (long) ((rand() / (double) RAND_MAX) *
		    (load[0].file_size - (blksize * num_ops_per_thread)));

    pthread_t *requesters = (pthread_t*) malloc (sizeof (pthread_t) * num_threads);
    for (i = 0; i < num_threads; i++) {
        pthread_create (&requesters[i], NULL, request, (void *) &load[i]);
    }

   for (i = 0; i < num_threads; i++) {
       pthread_join (requesters[i], NULL);
    }
    if (debug) {
        for (i = 0; i < num_threads; i++) {
	    for (j = 0; j < load[i].nreq; j++) {
    	        printf ("%d %d %" PRIu64 " %" PRIu64" %d %d\n", i, j, load[i].begin[j],
				load[i].end[j], (start_offset + (blksize * j)), load[i].rt_count[j]);
	    }
	}
    }

    return 0;
}

