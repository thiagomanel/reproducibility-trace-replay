#!/bin/bash

echo "#mem"
free -m
echo "#cpu"
cat /proc/cpuinfo
echo "#backgroud"
ps xau | grep background
echo "#rr"
ps xau | grep rr
echo "#rw"
ps xau | grep rw
echo "#seqr"
ps xau | grep seqr
echo "#seqw"
ps xau | grep seqw
