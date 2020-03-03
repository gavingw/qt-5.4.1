adb push %~dp0gdb.tar /tmp
adb shell tar xvf /tmp/gdb.tar -C /tmp/
adb shell ln -s /tmp/gdbout/bin/gdb /gdb
adb shell rm -rf /tmp/gdb.tar
pause