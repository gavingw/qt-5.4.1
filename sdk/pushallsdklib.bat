adb push %~dp0libs/libsdk_compose.so /usr/lib/
adb push %~dp0libs/libsdk_disp.so  /usr/lib/
adb push %~dp0libs/libsdk_dvr.so  /usr/lib/
adb push %~dp0libs/libsdk_player.so  /usr/lib/
adb push %~dp0libs/libsdk_tr.so  /usr/lib/
adb push %~dp0libsdk.so /usr/lib/
adb push %~dp0sdktest /
adb push %~dp0debuglevelset /
adb shell chmod 777 /sdktest
adb shell chmod 777 /debuglevelset
pause