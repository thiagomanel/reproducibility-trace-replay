import sys
import json
import random

TARGET_BASENAME = "/data/target/big_file"

def to_artc(workload, op_type, count, file_size, working_dir):
    """
        It converts from baseline data to strace-artc format. Baseline format follown below
        standard:
            tid op_id begin end
        In this particular capture we have a single payload operation, plus its related open and
        close calls. Each thread handles operations to a single file.
    """
    def strace_stamp(usecs):
        return "{:.6f}".format(float(usecs)/1000000)
        #return str(float(usecs)/1000000)

    def emit_close_ops(tids, fd_tid, close_stamps, next_id, workflow):
        #14696 1383677049.076619 close(3)        = 0 <0.000077>
        def emit_close(tid, begin, elapsed, fd, _id):
            op_plus_fd = "".join(["close", "(", str(fd), ")"])

            elapsed_stamp = strace_stamp(elapsed)
            elapsed_token = "".join(["<", str(elapsed_stamp), ">"])
            begin_stamp = strace_stamp(begin)

            return {
                    "probe": " ".join([str(tid), begin_stamp, op_plus_fd, "=", "0", elapsed_token]),
                    "id": _id
                   }

        _id = next_id
        for tid in tids:
            begin = close_stamps[tid]
            elapsed = 1

            fd = fd_tid[tid]
            close_call = emit_close(tid, begin, elapsed, fd, _id)
            workflow[tid].append(close_call)
            _id = _id + 1

        return _id

    def emit_control_ops(working_dir):
        return "0 0.000000 chdir(\"" + working_dir + "\") = 0 <0.000000>"

    def emit_open_ops(tids, fd_tid, path_tid, open_stamps, next_id, workflow):

        #14696 1383677049.558185 open("/path", O_RDONLY) = 3 <0.000013>
        def emit_open(tid, begin, elapsed, fd, path, _id):

            begin_stamp = strace_stamp(begin)

            call_plus_path = "".join(["open", "(", "\"", path, "\"", ","])
            flag_token = "".join(["O_RDONLY", ")"])

            elapsed_stamp = strace_stamp(elapsed)
            elapsed_token = "".join(["<", elapsed_stamp, ">"])

            return {
                    "probe": " ".join([str(tid), begin_stamp, call_plus_path, flag_token, "=",
                                       str(fd), elapsed_token]),
                    "id": _id
                   }

        elapsed = 1
        _id = next_id

        for tid in tids:
            fd = fd_tid[tid]
            path = path_tid[tid]
            begin = open_stamps[tid]
            call = emit_open(tid, begin, elapsed, fd, path, _id)
            workflow[tid].append(call)
            _id = _id + 1

        return _id

    def emit_payload_ops(op_type, file_size, count, tids, fd_tid, path_tid, next_id, workflow):

        def emit_read(tid, begin, elapsed, fd, path, count, _id):
            pass
        def emit_write(tid, begin, elapsed, fd, path, count, _id):
            pass

        def emit_pread(tid, begin, elapsed, fd, path, count, offset, _id):
            # 14696 1383677049.559108 pread(3, "............, 1000, 404503000) = 1000 <0.027891>
            strace_args_stub = "\"................................................\"...,"

            call_plus_fd = "".join(["pread", "(", str(fd), ","])
            count_token = "".join([str(count), ","])
            offset_token = "".join([str(offset), ")"])

            elapsed_stamp = strace_stamp(elapsed)
            elapsed_token = "".join(["<", elapsed_stamp, ">"])

            begin_stamp = strace_stamp(begin)

            return {
                    "probe": " ".join([str(tid), begin_stamp, call_plus_fd,
                                       strace_args_stub, count_token,
                                       offset_token, "=", str(count),
                                       elapsed_token]),
                    "id": _id
                   }

        def emit_pwrite(tid, begin, elapsed, fd, path, count, offset, _id):
            pass

        _id = next_id
        for tid in tids:
            for probe in workload[tid]:
                tid, op_id, _, _, begin_relative, end_relative = probe
                fd = fd_tid[tid]
                path = path_tid[tid]

                if op_type == "rr":
                    offset = random.randint(0, file_size - count)
                    elapsed = end_relative - begin_relative
                    call = emit_pread(tid, begin_relative, elapsed, fd, path,
                                      count, offset, _id)
                elif op_type == "rw":
                    offset = random.randint(0, file_size - count)
                    elapsed = end_relative - begin_relative
                    call = emit_pwrite(tid, begin_relative, elapsed, fd, path,
                                       count, offset, _id)
                elif op_type == "seqr":
                    elapsed = end_relative - begin_relative
                    call = emit_read(tid, begin_relative, elapsed, fd, path,
                                     count, _id)
                elif op_type == "seqw":
                    elapsed = end_relative - begin_relative
                    call = emit_write(tid, begin_relative, elapsed, fd, path,
                                      count, _id)
                else:
                    raise Exception("Unknow op_type: " + op_type)

                workflow[tid].append(call)
                _id = _id + 1

        return _id

    def fd_map(tids):
        fdmap = {}
        for (tid, i) in zip(tids, range(len(tids))):
            fdmap[tid] = tid + 1
        return fdmap

    def path_map(tids, path_basename):
        pmap = {}
        for (tid, i) in zip(tids, range(len(tids))):
            pmap[tid] = "".join([path_basename, str(i)])
        return pmap

    # 1. control call
    # 2. open calls
    # 3. payload calls
    # 4. close call

    # 1. control calls
    emit_control_ops(working_dir)

    tids = workload.keys()
    workflow = {}
    for tid in tids:
        workflow[tid] = []

    #assign the corresponding path and fd to each tid
    path_tid_map = path_map(tids, TARGET_BASENAME)
    fd_tid_map = fd_map(tids)

    # 2. open calls
    # we assign to the open calls the same stamps 2 microsecs earlier from
    # the first payload calls and the elapsed time of 1 microsecond
    open_stamps = {}
    for tid in tids:
        first_call = workload[tid][0]
        open_stamps[tid] = begin_relative = first_call[4] - 2

    next_id = 1
    next_id = emit_open_ops(tids, fd_tid_map, path_tid_map, open_stamps,
                            next_id, workflow)

    # 3. payload calls
    next_id = emit_payload_ops(op_type, file_size, count, tids, fd_tid_map,
                               path_tid_map, next_id, workflow)
    # 4. close calls

    #close calls start after the end of the last payload calls
    close_stamps = {}
    for tid in tids:
        _, _, _, _, _, last_call_end_relative = workload[tid][-1]
        close_stamps[tid] = last_call_end_relative + 1

    next_id = emit_close_ops(tids, fd_tid_map, close_stamps, next_id, workflow)

    # takes all values from the map and put in a list to ease sorting
    probes = []
    for tid in tids:
        for probe in workflow[tid]:
            probes.append(probe)
    return sorted(probes, key=lambda probe: int(probe["id"]))

if __name__ == "__main__":
    """
        This scripts converts data from trace capture to replayer input files.
        Usage: python $0 operation_type [rw, rr, seqw, seqr] < capture_data > output
    """
    op_type = sys.argv[1]

    workload = {}
    min_stamp = sys.maxint

    # exclude header
    sys.stdin.readline()

    for line in sys.stdin:
        tid, op_id, begin, end = map(long, line.split())
        tid = tid + 1000
        if not tid in workload:
            workload[tid] = []
        #baseline stamps are nanoseconds. we require microseconds
        begin = begin / 1000
        end = end / 1000
        min_stamp = min(min_stamp, begin)
        workload[tid].append([tid, op_id, begin, end])

    for tid, values in workload.iteritems():
        for probe in values:
            tid, op_id, begin, end = probe
            probe.append(begin - min_stamp + 2)
            probe.append(end - min_stamp + 2)

    op_count = 4096
    file_size = 1073741824
    working_dir = "/data/target"

    workflow = to_artc(workload, op_type, op_count, file_size, working_dir)
    for probe in workflow:
        print probe["probe"]
