#!/bin/sh
PATH=/home/fengkun/t3linux/lichee/out/sun8iw11p1/linux/common/buildroot/external-toolchain/bin/:$PATH

arm-linux-gnueabi-gcc -o fbinit  fbinit.c -DFB_INIT

