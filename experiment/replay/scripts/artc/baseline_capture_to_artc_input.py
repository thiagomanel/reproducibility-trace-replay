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
            flag_token = "".join(["O_RDWR|O_CREAT|O_LARGEFILE", ")"])

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

    def emit_lseek_ops(tids, fd_tid, offset_tid, seek_stamps, next_id, workflow):

        def emit_lseek(tid, begin, elapsed, fd, offset, _id):
            #28910 1413985190.308693 lseek(3, 3, SEEK_SET) = 3 <0.000024>
            begin_stamp = strace_stamp(begin)

            call_plus_fd = "".join(["lseek", "(", str(fd), ","])
            offset_token = str(offset) + ","
            seek_token = "SEEK_SET)"

            elapsed_stamp = strace_stamp(elapsed)
            elapsed_token = "".join(["<", elapsed_stamp, ">"])

            return {
                    "probe": " ".join([str(tid), begin_stamp, call_plus_fd,
                                       offset_token, seek_token, "=",
                                       str(offset), elapsed_token]),
                    "id": _id
                   }

        elapsed = 1
        _id = next_id

        for tid in tids:
            fd = fd_tid[tid]
            offset = offset_tid[tid]
            begin = seek_stamps[tid]
            call = emit_lseek(tid, begin, elapsed, fd, offset, _id)
            workflow[tid].append(call)
            _id = _id + 1

        return _id

    def emit_payload_ops(op_type, file_size, count, tids, fd_tid, path_tid, next_id, workflow):

        strace_args_stub = "\"................................................\"...,"

        def emit_readwrite(callname, tid, begin, elapsed, fd, path, count, _id):
            #13266 1383671715.997552 read(3, "", 1024) = 0 <0.000067>
            #13266 1383671716.284703 write(2, "", 28) = 28 <0.00001>
            call_plus_fd = "".join([callname, "(", str(fd), ","])
            count_token = "".join([str(count), ")"])

            elapsed_stamp = strace_stamp(elapsed)
            elapsed_token = "".join(["<", elapsed_stamp, ">"])

            begin_stamp = strace_stamp(begin)

            return {
                    "probe": " ".join([str(tid), begin_stamp, call_plus_fd, strace_args_stub,
                                       count_token, "=", str(count), elapsed_token]),
                    "id": _id
                   }

        def emit_preadwrite(callname, tid, begin, elapsed, fd, path, count, offset, _id):
            # 14696 1383677049.559108 pread(3, "............, 1000, 404503000) = 1000 <0.027891>
            # 14696 1383677049.559108 pwrite(3, "............, 1000, 404503000) = 1000 <0.027891>
            call_plus_fd = "".join([callname, "(", str(fd), ","])
            count_token = "".join([str(count), ","])
            offset_token = "".join([str(offset), ")"])

            elapsed_stamp = strace_stamp(elapsed)
            elapsed_token = "".join(["<", elapsed_stamp, ">"])

            begin_stamp = strace_stamp(begin)

            return {
                    "probe": " ".join([str(tid), begin_stamp, call_plus_fd, strace_args_stub,
                                       count_token, offset_token, "=", str(count), elapsed_token]),
                    "id": _id
                   }

        _id = next_id
        for tid in tids:
            for probe in workload[tid]:
                fd = fd_tid[tid]
                path = path_tid[tid]

                begin_relative = probe["begin_relative"]
                end_relative = probe["end_relative"]
                elapsed = end_relative - begin_relative

                count = probe["count"]
                offset = probe["offset"]

                if op_type == "rr":
                    call = emit_preadwrite("pread", tid, begin_relative,
                                           elapsed, fd, path, count, offset, _id)
                elif op_type == "rw":
                    call = emit_preadwrite("pwrite", tid, begin_relative,
                                           elapsed, fd, path, count, offset, _id)
                elif op_type == "seqr":
                    call = emit_readwrite("read", tid, begin_relative, elapsed,
                                          fd, path, count, _id)
                elif op_type == "seqw":
                    call = emit_readwrite("write", tid, begin_relative, elapsed,
                                          fd, path, count, _id)
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

    #assign the corresponding path, fd and first offsets to each tid
    path_tid_map = path_map(tids, TARGET_BASENAME)
    fd_tid_map = fd_map(tids)

    # 2. open calls
    # we assign to the open calls the same stamps 4 microsecs earlier from the first payload calls
    # and the elapsed time of 1 microsecond
    open_stamps = {}
    for tid in tids:
        first_call = workload[tid][0]
        open_stamps[tid] = first_call["begin_relative"] - 4

    next_id = 1
    next_id = emit_open_ops(tids, fd_tid_map, path_tid_map, open_stamps, next_id, workflow)

    #seeking the file. now, 2 microsecs ealier the first payload calls and 1
    #microsecs elapsed time.
    if op_type == "seqr" or op_type == "seqw":
        offset_tid_map = {}
        for tid in tids:
            first_call = workload[tid][0]
            offset_tid_map[tid] = first_call["offset"]

        seek_stamps = {}
        for tid in tids:
            first_call = workload[tid][0]
            seek_stamps[tid] = first_call["begin_relative"] - 2

        next_id = emit_lseek_ops(tids, fd_tid_map, offset_tid_map, seek_stamps, next_id, workflow)

    # 3. payload calls
    next_id = emit_payload_ops(op_type, file_size, count, tids, fd_tid_map, path_tid_map, next_id,
                               workflow)
    # 4. close calls

    #close calls start after the end of the last payload calls
    close_stamps = {}
    for tid in tids:
        last_call = workload[tid][-1]
        close_stamps[tid] = last_call["end_relative"] + 1

    next_id = emit_close_ops(tids, fd_tid_map, close_stamps, next_id, workflow)

    # takes all values from the map and put in a list to ease sorting
    probes = []
    for tid in tids:
        for probe in workflow[tid]:
            probes.append(probe)
    return sorted(probes, key=lambda probe: int(probe["id"]))

def new_probe(tid, op_id, begin, end, offset, count):
    return {"tid": tid,
            "op_id": op_id,
            "begin": begin,
            "end": end,
            "offset": offset,
            "count": count}

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
        tid, op_id, begin, end, offset, count = map(long, line.split())
        tid = tid + 1000
        if not tid in workload:
            workload[tid] = []
        #baseline stamps are nanoseconds. we require microseconds
        begin = begin / 1000
        end = end / 1000
        min_stamp = min(min_stamp, begin)
        workload[tid].append(new_probe(tid, op_id, begin, end, offset, count))

    new_workload = {}
    for tid, values in workload.iteritems():

        if not tid in new_workload:
            new_workload[tid] = []

        #we start correction from beyond first probe
        first_probe = values[0]
        new_workload[tid].append(first_probe)

        for i in range(1, len(values)):
            orig_curr_probe = values[i]
            orig_prev_probe = values[i - 1]

            orig_iat = orig_curr_probe["begin"] - orig_prev_probe["end"]

            if (orig_iat < 0):
                raise Exception("prev_probe: " + orig_prev_probe +\
                                " curr_probe: " + orig_curr_probe)

            if (orig_iat == 0):
                new_iat = 1
            else:
                new_iat = orig_iat

            #building the corresponding new probe
            new_prev_probe = new_workload[tid][i - 1]
            new_begin = new_prev_probe["end"] + new_iat

            orig_elapsed = orig_curr_probe["end"] - orig_curr_probe["begin"]
            new_end = new_begin + orig_elapsed

            new_curr_probe = new_probe(orig_curr_probe["tid"],
                                       orig_curr_probe["op_id"],
                                       new_begin,
                                       new_end,
                                       orig_curr_probe["offset"],
                                       orig_curr_probe["count"])

            new_workload[tid].append(new_curr_probe)

    #adding relative stamps
    for tid, values in new_workload.iteritems():
        for probe in values:
            probe["begin_relative"] = probe["begin"] - min_stamp + 4
            probe["end_relative"] = probe["end"] - min_stamp + 4

    op_count = 4096
    file_size = 1073741824
    working_dir = "/data/target"

    workflow = to_artc(new_workload, op_type, op_count, file_size, working_dir)
    for probe in workflow:
        print probe["probe"]
