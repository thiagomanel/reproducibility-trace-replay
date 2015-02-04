#!/bin/bash

echo "#mem"
free -m

echo "#cpu"
cat /proc/cpuinfo

echo "ps"
ps xau
