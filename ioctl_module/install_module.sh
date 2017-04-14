#!/bin/sh
set -x
# WARNING: this script doesn't check for errors, so you have to enhance it in case any of the commands
# below fail.
lsmod
rmmod ioctl_module
mknod /dev/ioctl_module c 100 0
insmod ioctl_module.ko
lsmod
