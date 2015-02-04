import sys
from coordinator import *

vm_image_path = "/local/manel/doc/metrology/og-image-v2.qcow2"

vm_instance = {
    "small": (2, 4096),
    "large": (4, 8192)
}

def start(vm_type):
    vm_cpu, vm_mem = vm_instance[vm_type]
    (vcpu, vmem) = vm_instance[vm_type]
    cmd = " ".join(["qemu", vm_image_path, "-enable-kvm",
                    "-smp", str(vcpu),
                    "-m", str(vmem),
                    "-redir", "tcp:2222::22",
                    "-curses"])
    log(cmd + "\n")
    p = process(cmd, subprocess.PIPE, subprocess.STDOUT)
    _out, _err = p.communicate()
    return _out, _err, p.returncode

def stop():
    cmd = " ".join(["shutdown", "-h", "now"])
    log(cmd + "\n")
    return remote(cmd, "localhost")

if __name__ == "__main__":

    if sys.argv[1] == "stop":
        print stop()
    elif sys.argv[1] == "start":
        vm_type = sys.argv[2]
        if not vm_type in vm_instance:
            sys.stderr.write("Unknow vm instance: " + vm_type + "\n")
            exit(1)
        print start(vm_type)
    else:
        sys.stderr.write("Usage: $0 stop | start {vm_type}\n")
        exit(1)
