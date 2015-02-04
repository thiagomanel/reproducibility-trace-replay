import sys
import os
import random
import subprocess
from deploy import *
from time import sleep

outdir = "/tmp"

strace = "strace -f -q -ttt -T -s -e trace=open,pread,pwrite -o"

target_mnt = "/data"
foreground_input = "/".join([target_mnt, "target/big_file"])

SSH_CMD = "ssh -p 2222 root@localhost"

bin_dir = "/home/user/scripts/capture/scripts"

load_scripts = {"rr": "/".join([bin_dir, "rr"]),
                "rw": "/".join([bin_dir, "rw"]),
                "seqr": "/".join([bin_dir, "seqr"]),
                "seqw": "/".join([bin_dir, "seqw"])}

background_path = "/".join([bin_dir, "background"])
check_path = "/".join([bin_dir, "check.sh"])

cpu_freq_path = "/".join([bin_dir, "cpu_freq.sh"])

stap_script = "/".join([bin_dir, "trace.stp"])

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

def cpu_util(base_out):
    outpath = ".".join([base_out, "cpu_util.out"])
    errpath = ".".join([base_out, "cpu_util.err"])
    cmd = " ".join(["nohup", "mpstat", "1",
                    ">", outpath, "2>", errpath, "<", "/dev/null", "&"])
    log(cmd + "\n")
    return remote(cmd, "localhost")

def kill_cpu_util():
    cmd = " ".join(["killall", "mpstat"])
    log(cmd + "\n")
    return remote(cmd, "localhost")

def cpu_freq(base_out):
    outpath = ".".join([base_out, "cpu_freq.out"])
    errpath = ".".join([base_out, "cpu_freq.err"])
    cmd = " ".join(["nohup", cpu_freq_path,
                    ">", outpath, "2>", errpath, "<", "/dev/null", "&"])
    log(cmd + "\n")
    return remote(cmd, "localhost")

def kill_cpu_freq():
    cmd = " ".join(["killall", "cpu_freq.sh"])
    log(cmd + "\n")
    return remote(cmd, "localhost")

def background():
    cmd = " ".join(["nohup", background_path, ">", "/dev/null", "2>",
                    "/dev/null", "<", "/dev/null", "&"])
    log(cmd + "\n")
    return remote(cmd, "localhost")

def killall_background():
    cmd = " ".join(["killall", "background"])
    log(cmd + "\n")
    return remote(cmd, "localhost")

def foreground(workload, nthreads, delay, nops, loader_input, blksize,
               storage_output_mode, trace_tool, base_out):

    def trace_cmd(cmd_to_trace, cmd_out, cmd_err):

        def strace():
            if storage_output_mode == "null":
                strace_out = "/dev/null"
            elif storage_output_mode == "same-disk":
                strace_out = ".".join([base_out, "strace"])

            strace_flags = "strace -e trace=open,read,write,pread64,pwrite64,close -T -s -q -ttt"
            return " ".join([strace_flags, "-o", strace_out, "-f", cmd_to_trace,
                             ">", cmd_out, "2>", cmd_err])
        def stap():
            if storage_output_mode == "null":
                strace_out = "/dev/null"
            elif storage_output_mode == "same-disk":
                strace_out = ".".join([base_out, "stap"])

            stap_flags = "stap -g -DSTP_NO_OVERLOAD -DMAXMAPENTRIES=10000"
            target_cmd = " ".join([cmd_to_trace, ">", cmd_out, "2>", cmd_err])

            return " ".join([stap_flags, stap_script, "-c",
                             "\"", target_cmd, "\"",
                             ">", strace_out])
        def baseline():
            return " ".join([cmd_to_trace, ">", cmd_out, "2>", cmd_err])

        if trace_tool == "stap":
            return stap()
        elif trace_tool == "strace":
            return strace()
        elif trace_tool == "baseline":
            return baseline()

    outpath = ".".join([base_out + ".out"])
    errpath = ".".join([base_out + ".err"])

    script_path = load_scripts[workload]
    script_args = " ".join([nthreads, delay, nops, loader_input, blksize,
                            "debug"])
    foreground_cmd = " ".join([script_path, script_args])

    cmd = trace_cmd(foreground_cmd, outpath, errpath)
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
    cmd = " ".join(["umount", target_mnt])
    log(cmd + "\n")
    return remote(cmd, "localhost")

def mount():
    cmd = " ".join(["mount", target_mnt])
    log(cmd + "\n")
    return remote(cmd, "localhost")

def main(workload, nthreads, delay, nops, blksize, nsamples, nbackground,
         storage_output_mode, trace_tool):

    #FIXME: append all factors to all output file names. factors including
    #vm_type, tracing_tool, storage
    def output_basename(out_dir, rand, sample): #out_dir/sample.random
        return os.path.join(out_dir, ".".join([str(sample), str(rand)]))

    for sample in range(nsamples):
        log("Running sample " + str(sample) + "\n")

        _random = int(random.random() * 10000000)

        base_out = ".".join([output_basename(outdir,
                                             _random,
                                             sample),
                             workload, nthreads, delay, nops, blksize, str(sample),
                             str(nbackground), storage_output_mode, "load"])

        local_base_out = ".".join([output_basename("/local/manel/doc/metrology/collect",
                                                   _random,
                                                   sample),
                                   workload, nthreads, delay, nops, blksize, str(sample),
                                   str(nbackground), storage_output_mode, "load"])

        log("Umount vm target fs\n")
        print umount()
        log("Drop vm page cache\n")
        drop_local_caches(local_base_out)
        drop_caches(base_out)
        log("Checking vm state before experiment\n")
        check(base_out)

        for i in range(nbackground):
             bout, berr, pcode = background()
             if bout:
                 log("background start cmd out: ", bout)
             if berr:
                 log("background start cmd err: ", berr)

#        log("Collecting cpu utilization\n")
#        cpu_util(base_out)

#        log("Collecting cpu_freq utilization\n")
#        cpu_freq(base_out)

        log("Mount vm target fs\n")
        print mount()

        log("Start load\n")
        sleep(10)
        pout, perr, pcode = foreground(workload, nthreads, delay, nops,
                                       foreground_input, blksize,
                                       storage_output_mode, trace_tool,
                                       base_out)

        sleep(5)
        log("Kill background\n")
        killall_background()

#        log("kill cpu utilization\n")
#        kill_cpu_util()

#        log("kill cpu_freq\n")
#        kill_cpu_freq()

        log("Collecting experiment output\n")
        collect(outdir, "/local/manel/doc/metrology/collect")

if __name__ == "__main__":

    if len(sys.argv) < 10:
        err("Usage: python coordinator.py workload nthreads delay nops blksize\
            num_samples num_background storage_output_mode trace_tool\n")
        sys.exit(-1)

    print sys.argv
    workload, nthreads, delay, nops, blksize, nsamples,\
             nbackground, storage_output_mode, trace_tool = sys.argv[1:10]

    main(workload, nthreads, delay, nops, blksize, int(nsamples),
         int(nbackground), storage_output_mode, trace_tool)
