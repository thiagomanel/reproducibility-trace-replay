import sys
import json
import random

TARGET_BASENAME = "/data/target/big_file"

def join(parent, child):
    if child:
        if not child["id"] in parent["children"]:
            parent["children"].append(child["id"])
        if not parent["id"] in child["parents"]:
            child["parents"].append(parent["id"])

def apply_fs_policy(workflow):
    for tid, probes in workflow.iteritems():
        for i in range(len(probes) - 1):
            join(probes[i], probes[i + 1])

def apply_timestamp_policy(workflow):

    def finished_before(probes, current_pos):
        """ probes are stamp-begin sorted, we return the last which finished
            before stamp begin.
        """
        current_stamp_begin = probes[current_pos]["stamp"]["begin"]
        for pos in xrange(current_pos - 1, -1, -1):
            probe = probes[pos]
            candidate_end = probe["stamp"]["begin"] + probe["stamp"]["elapsed"]
            if (candidate_end < current_stamp_begin):
                return probe
        return None

    probes = []
    for probes_by_tid in workflow.values():
        for probe in probes_by_tid:
            probes.append(probe)
    probes = sorted(probes, key=lambda probe: int(probe["stamp"]["begin"]))

    for pos in range(len(probes)):
        current_probe = probes[pos]
        parent_probe = finished_before(probes, pos)
        if parent_probe:
            join(parent_probe, current_probe)

def baseline_to_beefsrepl(workload, op_type, order_policy, count, file_size):
    """
        It converts from baseline data to beefs_replayer workload. Baseline
        format follown below standard:
            tid op_id begin end
        In this particular capture we have a single payload operation, plus its
        related open and close calls. Each thread handles operations to a
        single file.
    """
    def emit_open(tid, begin, elapsed, fd, path, _id):
        #flags = 32834#O_RDWR|O_CREAT|O_LARGEFILE mode = 511#0777
        return {
                "args": [path, "32834", "511"], "parents": [],
                "stamp" : {"begin": begin, "elapsed": elapsed},
                "call" : "open", "rvalue": fd,
                "caller": {"exec": "fake", "tid": str(tid), "pid": "1", "uid": "1"},
                "id": _id, "session_id": -1, "children" : []
               }

    def emit_close(tid, begin, elapsed, fd, _id):
        return {
                "args": [str(fd)],  "parents": [],
                "stamp" : {"begin": begin, "elapsed": elapsed},
                "call" : "close", "rvalue": 0,
                "caller": {"exec": "fake", "tid": str(tid), "pid": "1", "uid": "1"},
                "id": _id, "session_id": -1, "children" : []
               }

    def emit_llseek(tid, begin, elapsed, fd, path, offset, _id):
        return {
                "args": [path, str(fd), str(0), str(offset), "SEEK_SET"], "parents": [],
                "stamp": {"begin": begin, "elapsed": elapsed},
                "call": "llseek", "rvalue": 0,
                "caller": {"exec": "fake", "tid": str(tid), "pid": "1", "uid": "1"},
                "id": _id, "session_id": -1, "children" : []
                }

    def emit_read(tid, begin, elapsed, fd, path, count, _id):
        # read(int fd, void *buf, size_t count, off_t offset);
        return {
                "args": [path, str(fd), str(count)], "parents": [],
                "stamp": {"begin": begin, "elapsed": elapsed},
                "call": "read", "rvalue": count,
                "caller": {"exec": "fake", "tid": str(tid), "pid": "1", "uid": "1"},
                "id": _id, "session_id": -1, "children" : []
                }

    def emit_write(tid, begin, elapsed, fd, path, count, _id):
        # write(int fd, void *buf, size_t count, off_t offset);
        return {
                "args": [path, str(fd), str(count)], "parents": [],
                "stamp": {"begin": begin, "elapsed": elapsed},
                "call": "write", "rvalue": count,
                "caller": {"exec": "fake", "tid": str(tid), "pid": "1", "uid": "1"},
                "id": _id, "session_id": -1, "children" : []
                }

    def emit_pread(tid, begin, elapsed, fd, path, count, offset, _id):
        # pread(int fd, void *buf, size_t count, off_t offset);
        return {
                "args": [path, str(fd), str(count), str(offset)], "parents": [],
                "stamp": {"begin": begin, "elapsed": elapsed},
                "call": "pread", "rvalue": count,
                "caller": {"exec": "fake", "tid": str(tid), "pid": "1", "uid": "1"},
                "id": _id, "session_id": -1, "children" : []
                }

    def emit_pwrite(tid, begin, elapsed, fd, path, count, offset, _id):
        return {
                "args": [path, str(fd), str(count), str(offset)], "parents": [],
                "stamp": {"begin": begin, "elapsed": elapsed},
                "call": "pwrite", "rvalue": count,
                "caller": {"exec": "fake", "tid": str(tid), "pid": "1", "uid": "1"},
                "id": _id, "session_id": -1, "children" : []
                }

    workflow = {}
    path_tid_map = {}
    fd_tid_map = {}

    #assign the corresponding path and fd to each tid
    tids = workload.keys()
    for (tid, i) in zip(tids, range(len(tids))):
        workflow[tid] = []
        path_tid_map[tid] = "".join([TARGET_BASENAME, str(i)])
        #we just avoid a zero fd
        fd_tid_map[tid] = tid + 1

    #we assign to the open calls the same stamps from the first pread calls
    open_stamps = {}
    for tid in tids:
        first_call = workload[tid][0]
        open_stamps[tid] = first_call[2]

    #open calls
    _id = 1
    for tid in tids:
        #tid and fd are equal, since they do not repeat, then it is ok
        fd = fd_tid_map[tid]
        path = path_tid_map[tid]
        begin = open_stamps[tid]
        elapsed = 0
        call = emit_open(tid, begin, elapsed, fd, path, _id)
        workflow[tid].append(call)
        begin = begin + 1
        _id = _id + 1

    #seeek calls
    for tid in tids:
        #the first opeation, for seqw and seqr, must be a seek call; this is
        #because our capture workload performed a seek operation but we missed
        #it in trace. Then, we have to infer them from the offset field.
        if op_type == "seqw" or op_type == "seqr":
            fd = fd_tid_map[tid]
            path = path_tid_map[tid]
            begin = open_stamps[tid]
            elapsed = 0

            _, _, _, _, offset = first_call = workload[tid][0]
            call = emit_llseek(tid, begin, elapsed, fd, path, offset, _id)
            workflow[tid].append(call)

            begin = begin + 1
            _id = _id + 1

    #payload calls
    for tid in tids:
        for probe in workload[tid]:
            tid, op_id, begin, end, offset = probe
            fd = fd_tid_map[tid]
            path = path_tid_map[tid]

            if op_type == "rr":
                elapsed = end - begin
                call = emit_pread(tid, begin, elapsed, fd, path, count, offset, _id)
            elif op_type == "rw":
                elapsed = end - begin
                call = emit_pwrite(tid, begin, elapsed, fd, path, count, offset, _id)
            elif op_type == "seqr":
                elapsed = end - begin
                call = emit_read(tid, begin, elapsed, fd, path, count, _id)
            elif op_type == "seqw":
                elapsed = end - begin
                call = emit_write(tid, begin, elapsed, fd, path, count, _id)
            else:
                raise Exception("Unknow op_type: " + op_type)

            workflow[tid].append(call)
            _id = _id + 1

    #close calls
    for tid in tids:
        #close follows the last call for this tid
        last_call = workflow[tid][-1]
        begin = last_call["stamp"]["begin"] + last_call["stamp"]["elapsed"]
        elapsed = 1

        fd = fd_tid_map[tid]
        close_call = emit_close(tid, begin, elapsed, fd, _id)
        workflow[tid].append(close_call)
        _id = _id + 1

    #creating workflow dependencies
    if order_policy == "fs":
        apply_fs_policy(workflow)
    elif order_policy == "timestamp":
        apply_timestamp_policy(workflow)
    else:
        raise Exception("unknown policy <" + order_policy + ">")

    probes = []
    for tid in tids:
        for probe in workflow[tid]:
            probes.append(probe)

    return sorted(probes, key=lambda probe: int(probe["id"]))

if __name__ == "__main__":
    """
        This scripts converts data from trace capture to replayer input files.
        Usage: python $0 operation_type [rw, rr, seqw, seqr] order_policy [fs, timestamp] < capture_data > output
    """
    op_type = sys.argv[1]
    order_policy = sys.argv[2]

    workload = {}
    for line in sys.stdin:
        if not "debug" in line:
            tid, op_id, begin, end, offset, count = map(long, line.split())
            if not tid in workload:
                workload[tid] = []
            #baseline stamps are nanoseconds. we require microseconds
            workload[tid].append([tid, op_id, begin/1000, end/1000, offset])

    op_count = 4096
    file_size = 1073741824
    workflow = baseline_to_beefsrepl(workload, op_type, order_policy, op_count, file_size)
    print len(workflow)
    for w in workflow:
        print json.dumps(w).strip()
