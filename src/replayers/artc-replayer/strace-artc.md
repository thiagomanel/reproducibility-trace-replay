% strace-artc(1) ARTC
% Zev Weiss <zev@cs.wisc.edu>
% September 2013

# NAME

strace-artc - `strace`(1) wrapper for use with artc

# SYNOPSIS

strace-artc [FLAGS] *COMMAND*

strace-artc [FLAGS] -p *PID*

# DESCRIPTION

strace-artc runs `strace`(1) with the necessary flags to produce
output suitable for compilation by `artc`(1).  Like `strace`(1)
itself, `strace-artc` can either run a command as a child process or
attach to an existing process.

# OPTIONS

-o *OUTFILE*

  : Write trace output to *OUTFILE* instead of stderr.


-i *IOSTAT*

  : Run `iostat`(1) in the background during tracing, writing its
    output to *IOSTAT*.


-p *PID*

  : Instead of running *COMMAND*, attach to and trace the existing
    process *PID*.


-s

  : When using \`-p PID', send the target process a SIGSTOP before
    snapshotting its initial file descriptor state and generating the
    synthetic setup trace, then SIGCONT it when ready to start
    tracing.  This may be useful when attaching to a busy process
    that's actively changing its descriptor state, though it may not
    be the most reliable thing in the world.  Attaching to a
    relatively idle process and then prodding it into activity tends
    to work better if possible.

# ENVIRONMENT

*STRACE*: if set, the value of this environment variable is used as
 the path the `strace` binary to be called by strace-artc; by default
 strace-artc uses whatever `strace` is found first in $PATH.

# SEE ALSO

`strace`(1), `artc`(1), `geninit`(1)
