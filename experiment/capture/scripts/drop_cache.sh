#!/bin/bash

sync
echo 3 > /proc/sys/vm/drop_caches
#blockdev --flushbufs /dev/sda
#hdparm -F /dev/sda3
