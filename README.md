# reproducibility-trace-replay
File system trace replay reproducibility.

In this project, we aim to know the effects of the difference between the performance of trace environment and replay environment. That is, how trace replayers (beefsrepl and artc) behave when they reproduce a trace in an environment that is different from the capture environment.

copin=INF062:2013.1-2

The experiment is realized in three steps

**Step 1: capture**

This step captures baseline logs for rr, rw, seqr and seqw

Here you need to run the capture/bin/run-with-baseline.sh

**Step 2: trace-converter**

This step converts the baseline traces to beefs_repl input format

Here you need to run trace-converter/baseline-to-beefs_repl.sh

**Step 3: beefs-replayer**

This step executes the beefs_repl with the converted traces as input

Here you need to run beefs-replayer/bin/run-replayer.sh

**General information**

Remeber to enter the directory where the script you want to run is.

conf/ - some configurations valid for all the steps
scripts/ - Some scripts used to drop cache, check hardware profile etc. (valid for all the steps)
