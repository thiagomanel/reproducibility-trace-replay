#!/bin/bash

while true
do
  cat /proc/cpuinfo | grep "cpu" | grep "MHz"
  sleep 1
done
