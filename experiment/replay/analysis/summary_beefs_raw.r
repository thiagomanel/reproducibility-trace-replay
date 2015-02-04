library(plyr)
# It calculates some metrology definitions

Args <- commandArgs(TRUE)

#begin end tid capture_src machine vm replay_sample replay_rand replayer sample rand workload nforeground delay nops blksize nbackground storage t_policy
samples <- read.table(Args[1], header = T)
samples$response = (samples$end - samples$begin)

data = ddply(samples, .(capture_src, machine, vm, replay_sample, replayer, workload, nforeground, delay, nops, blksize, nbackground, storage, t_policy),
	     summarize, mean=mean(response), dev=sd(response), median=median(response))

write.csv(data, file = paste(Args[1], "summary.csv", sep="_"))
