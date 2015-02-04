import sys

def parse_thread_log(line):
    #thread_log(1001): open:4771093:23805 ...
    tokens = line.split()
    tid = tokens[0].split("(")[1].split(")")[0]

    probes = []
    for token in tokens[1:]:
        # open:4771093:23805
        probe_tokens = token.split(":")
        probes.append(probe_tokens)

    return (tid, probes)

if __name__ == "__main__":

    workload = {}
    for line in sys.stdin:
        if "thread_log" in line:
            tid, probes = parse_thread_log(line)
            workload[tid] = probes

    for tid, probes in workload.iteritems():
        for probe in probes:
            call, begin, elapsed = probe
            end = long(begin) + long(elapsed)
            print " ".join([tid, call, begin, str(end), elapsed])
