
1~~~~~~~~~~~compile

$cd /home/youpatch/lichee/buildroot/target/user_rootfs_misc/sdk
$make -j32


then copy libsdk.so sdktest to you target board,youcan use a udisk


2~~~~~~~~~~~~~download small file to your target board


examples use secureCRT,
on your board type lrz,then choose you small file.
#lrz

3~~~~~~~~~~~~~upload small file from your target board to computer

#lsz filename


4~~~~~~~~~~~debug tips

this list some very helpful cmds for your debug using gdb

on your t2 board

#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:cdxlib/
#ulimit -c unlimited
#./sdktest &


on you service


$export PATH=/home/youpatch/lichee/out/linux/common/buildroot/external-toolchain/bin/:$PATH

$arm-linux-gnueabi-gdb sdktest core 

set solib-search-path /home/youpatch/lichee/buildroot/target/user_rootfs_misc/sdk
set solib-search-path /home/youpatch/lichee/out/linux/common/buildroot/target/lib/
set solib-search-path /home/youpatch/lichee/out/linux/common/buildroot/target/usr/lib
set solib-search-path /home/youpatch/lichee/buildroot/target/user_rootfs_misc/sdk/cedarx/

common gdb examples:
bt
l
p /x  aaaaa
p aaaaa

how to use debuglevelset to set debug level(level=0~6)
step1:   #cd user_rootfs_misc/sdk
step2:   #make
step3:   #adb push debuglevelset /
step4:   #adb shell chmod 777 /debuglevelset
step5:   #adb shell /debuglevelset 2          //it means we want to set debuglevel to 2







