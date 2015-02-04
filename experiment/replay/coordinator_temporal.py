import sys
import os
import random
import subprocess
from deploy import *
from time import sleep

OUTDIR = "/tmp"

TARGET_MNT = "/data"
SSH_CMD = "ssh -p 2222 root@localhost"

bin_dir = "/home/user/scripts"

BEEFS_REPL_DIR = "/".join([bin_dir, "beefs_repl"])
BEEFS_REPL_BIN = "/".join([BEEFS_REPL_DIR, "Beefs-trace-replayer", "beefs_replayer"])

ARTC_DIR = "/".join([bin_dir, "artc"])
ARTC_RUN_BIN = "/".join([ARTC_DIR, "compiler", "artrun"])

check_path = "/".join([bin_dir, "check.sh"])

#I avoid using scripts with redirections in ssh commands, so I'll keep this
#in a proper script
drop_cache_path = "/".join([bin_dir, "drop_cache.sh"])
drop_local_cache_path = "/".join(["scripts", "drop_cache.sh"])

def log(message):#TODO: replace by proper logging ?
    print message
def err(message):
    sys.stderr.write(message)

def remote(command, machine_addr, out=subprocess.PIPE, err=subprocess.STDOUT):
    remote_cmd = " ".join(["ssh", "-p", "2222", "root@"+machine_addr, "'",
                           command, "'"])
    log(remote_cmd + "\n")
    p = process(remote_cmd, out, err)
    _out, _err = p.communicate()
    return _out, _err, p.returncode

def background():
    cmd = " ".join(["nohup", background_path, ">", "/dev/null", "2>",
                    "/dev/null", "<", "/dev/null", "&"])
    log(cmd + "\n")
    return remote(cmd, "localhost")

def replay(replayer, trace_input, timing, num_workers, base_out):

    def replay_cmd(out_path, err_path):

        def artc():
            #$artcrun --predelay-multiplier=0 $workload/bench.so $target_dir 1>
            #$outdir.artc.root.out 2> $outdir/artc.root.err
            compiled_workload = "/".join([trace_input, "bench.so"])
            target_dir = "/data/target"
            return " ".join([ARTC_RUN_BIN, "--predelay-multiplier=0 --disable-constraints=all --enable-constraints=temporal",
                             compiled_workload, target_dir,
                             ">", out_path,
                             "2>", err_path])

        def beefs_rpl():
            #beefs_replayer $replay_input $timing_policy $num_workers $add_delay_us [debug]
            add_delay_us = 0
            return " ".join([BEEFS_REPL_BIN, trace_input, timing,
                             str(num_workers),
                             str(add_delay_us), "debug",
                             ">", out_path,
                             "2>", err_path])

        if replayer == "compile":
            return artc()
        elif replayer == "event":
            return beefs_rpl()

    outpath = ".".join([base_out + ".out"])
    errpath = ".".join([base_out + ".err"])

    cmd = replay_cmd(outpath, errpath)
    log(cmd + "\n")
    return remote(cmd, "localhost")

def collect(remote_src, local_dst):
    src = ":".join(["root@localhost", remote_src + "/*"])
    rsync_cmd = " ".join(["rsync", "--remove-source-files", "-ave",
                          "\"ssh -p 2222\"", src, local_dst])
    log(rsync_cmd + "\n")
    p = process(rsync_cmd, subprocess.PIPE, subprocess.STDOUT)
    _out, _err = p.communicate()
    return _out, _err, p.returncode

def drop_caches(base_out):
    outpath = ".".join([base_out + ".dropcache.remote.out"])
    errpath = ".".join([base_out + ".dropcache.remote.err"])
    cmd = " ".join([drop_cache_path, ">", outpath, "2>", errpath])
    log(cmd + "\n")
    return remote(cmd, "localhost")

def drop_local_caches(base_out):
    outpath = ".".join([base_out + ".dropcache.local.out"])
    errpath = ".".join([base_out + ".dropcache.local.err"])
    cmd = " ".join([drop_local_cache_path, ">", outpath, "2>", errpath])
    log(cmd + "\n")
    return execute(cmd)

def check(base_out):
    outpath = ".".join([base_out + ".check"])
    cmd = " ".join([check_path, ">", outpath])
    log(cmd + "\n")
    return remote(cmd, "localhost")

def umount():
    cmd = " ".join(["umount", TARGET_MNT])
    log(cmd + "\n")
    return remote(cmd, "localhost")

def mount():
    cmd = " ".join(["mount", TARGET_MNT])
    log(cmd + "\n")
    return remote(cmd, "localhost")

def main(replayer, trace_input, timing, nthreads_replayer, nsamples):

    def output_basename(out_dir, rand, sample):#out_dir/sample.random
        return os.path.join(out_dir, ".".join([str(sample), str(rand)]))

    for sample in range(nsamples):
        log("Running sample " + str(sample) + "\n")

        _random = int(random.random() * 10000000)

        base_out = ".".join([output_basename(OUTDIR, _random, sample),
                             replayer,
                             os.path.basename(trace_input),
                             timing,
                             str(nthreads_replayer),
                             str(sample),
                             "replay"])

        local_base_out =  ".".join([output_basename("/local/manel/doc/metrology/collect",
                                                    _random,
                                                    sample),
                                    replayer,
                                    os.path.basename(trace_input),
                                    timing,
                                    str(nthreads_replayer),
                                    str(sample),
                                    "replay"])

        log("Umount vm target fs\n")
        print umount()

        log("Drop vm page cache\n")
        drop_local_caches(local_base_out)
        drop_caches(base_out)

        log("Checking vm state before experiment\n")
        check(base_out)

        log("Mount vm target fs\n")
        print mount()

        log("Start load\n")
        sleep(10)
        pout, perr, pcode = replay(replayer, trace_input, timing,\
                                   nthreads_replayer, base_out)

        sleep(5)
        log("Collecting experiment output\n")
        collect(OUTDIR, "/local/manel/doc/metrology/collect")

if __name__ == "__main__":

    if not len(sys.argv) == 6:
        err("Usage: python coordinator.py replayer trace_input timing nthreads samples\n")
        sys.exit(-1)

    print sys.argv
    replayer, trace_input, timing, nthreads_replayer, nsamples = sys.argv[1:6]
    main(replayer, trace_input, timing, int(nthreads_replayer), int(nsamples))
