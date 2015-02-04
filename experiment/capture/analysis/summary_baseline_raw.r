library(plyr)
# It calculates some metrology definitions

Args <- commandArgs(TRUE)

#tid req begin end offset machine trace vm sample random workload nforeground delay nops blksize nbackground storage
samples <- read.table(Args[1], header = T)

samples$response = (samples$end - samples$begin)/1000

data = ddply(samples, .(machine, trace, vm, sample, random, workload, nforeground, delay, nops,
			blksize, nbackground, storage),
	     summarize, mean=mean(response), dev=sd(response), median=median(response))

write.csv(data, file = paste(Args[1], "raw.csv", sep="_"))
