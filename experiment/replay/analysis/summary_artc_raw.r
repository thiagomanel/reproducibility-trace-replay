library(plyr)

# It calculates some metrology definitions for artc replay

Args <- commandArgs(TRUE)

# tid call begin end response capture_src machine vm replay_sample replay_rand replayer sample rand workload nforeground delay nops blksize nbackground storage t_policy

samples <- read.table(Args[1], header = T)
samples$response = (samples$response)/1000
samples = subset(samples, samples$call %in% c("pwrite", "write", "pread", "read"))

data = ddply(samples, .(capture_src, machine, vm, replay_sample, replayer, workload, nforeground, delay, nops, blksize, nbackground, storage, t_policy),
	     summarize, mean=mean(response), dev=sd(response), median=median(response))

write.csv(data, file = paste(Args[1], "summary.csv", sep="_"))
