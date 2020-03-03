adb push %~dp0libsdk.so /usr/lib/
adb push %~dp0libs\libsdk_compose.so /usr/lib/
adb push %~dp0libs\libsdk_disp.so /usr/lib/
adb push %~dp0libs\libsdk_dvr.so /usr/lib/
adb push %~dp0libs\libsdk_player.so /usr/lib/
adb push %~dp0libs\libsdk_tr.so /usr/lib/
adb push %~dp0run_auto_xplayer.sh /
adb push %~dp0sdktest /
adb push %~dp0xplayerdemo /
adb shell chmod 777 /run_auto_xplayer.sh
adb shell chmod 777 /xplayerdemo
adb shell chmod 777 /sdktest
pause