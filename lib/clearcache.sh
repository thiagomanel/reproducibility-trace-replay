#!/bin/bash

echo "umounting $1..."
umount $1

sync
echo 3 > /proc/sys/vm/drop_caches

echo "mounting $1..."
mount $1
