import subprocess
import os

def rollback(server, backup_path, dst_path):

    src = backup_path
    dst = dst_path
    rollback_cmd = " ".join(["rsync", "-progtl", "--delete", src + "/", dst])
    if server:
        return remote(rollback_cmd, server)
    else:
        return execute(rollback_cmd)

def process(command, out=subprocess.PIPE, err=subprocess.STDOUT):
    return subprocess.Popen(command, shell=True, stdout=out, stderr=err,
                            preexec_fn=os.setsid)

def execute(command, out=subprocess.PIPE, err=subprocess.STDOUT, block=True):
    p = process(command, out, err)
    if block:
        _out, _err = p.communicate()
        return _out, _err, p.returncode

def remote(command, machine_addr, out=subprocess.PIPE, err=subprocess.STDOUT):
    remote_cmd = " ".join(["ssh", "root@"+machine_addr, command])
    p = process(remote_cmd, out, err)
    _out, _err = p.communicate()
    return _out, _err, p.returncode
