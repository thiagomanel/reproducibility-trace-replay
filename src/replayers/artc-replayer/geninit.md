% geninit(1) ARTC
% Zev Weiss <zev@cs.wisc.edu>
% September 2013

# NAME

geninit - generate a filesystem init-listing for use with artc

# SYNOPSIS

geninit [-a] *PATH* [*PATH*...]

# DESCRIPTION

geninit recursively traverses the *PATH*s passed to it as arguments
and writes to stdout an init-listing suitable for use with artc.  Each
argument *PATH* should be a filesystem subtree which the trace to be
compiled is expected to contain accesses to.  To ensure full coverage,
one can simply pass "/", though this may be unnecssarily slow if the
user knows the program in question is only going to access a small
area of the filesystem.

Do note, however, that most programs at startup will pull in shared
libraries from places like /lib and usually read a few things from
/etc, so you may want to include those so as to avoid warnings about
missing files during compilation and replay.

# OPTIONS

geninit accepts one optional flag.

-a

  : By default geninit will not cross filesystem boundaries; this flag
    disables that check.

# EXAMPLES

To write a full listing of everything in the filesystem to a file
`full-system.init` (excepting /proc, /sys, and /dev, which are always
skipped):

    geninit -a / > full-system.init

To write an init-listing likely to be suitable for a simple program
accessing files in the current working directory:

    geninit /etc /lib* . > abridged.init

# SEE ALSO

`strace-artc`(1), `artc`(1)

