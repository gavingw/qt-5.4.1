#configuration
CONFIG +=  cross_compile shared qpa no_mocdepend release qt_no_framework
host_build {
    QT_ARCH = x86_64
    QT_TARGET_ARCH = arm
} else {
    QT_ARCH = arm
    QMAKE_DEFAULT_LIBDIRS = /home/skyliqf/workhome/lichee_A40i/out/sun8iw11p1/linux/common/buildroot/external-toolchain/lib/gcc/arm-linux-gnueabi/4.6.3 /home/skyliqf/workhome/lichee_A40i/out/sun8iw11p1/linux/common/buildroot/external-toolchain/lib/gcc/arm-linux-gnueabi /home/skyliqf/workhome/lichee_A40i/out/sun8iw11p1/linux/common/buildroot/external-toolchain/lib/gcc /home/skyliqf/workhome/lichee_A40i/out/sun8iw11p1/linux/common/buildroot/external-toolchain/arm-linux-gnueabi/lib /home/skyliqf/workhome/lichee_A40i//out/sun8iw11p1/linux/common/buildroot/host/usr/arm-unknown-linux-gnueabi/sysroot/lib/arm-linux-gnueabi /home/skyliqf/workhome/lichee_A40i//out/sun8iw11p1/linux/common/buildroot/host/usr/arm-unknown-linux-gnueabi/sysroot/lib /home/skyliqf/workhome/lichee_A40i//out/sun8iw11p1/linux/common/buildroot/host/usr/arm-unknown-linux-gnueabi/sysroot/usr/lib/arm-linux-gnueabi /home/skyliqf/workhome/lichee_A40i//out/sun8iw11p1/linux/common/buildroot/host/usr/arm-unknown-linux-gnueabi/sysroot/usr/lib
    QMAKE_DEFAULT_INCDIRS = /home/skyliqf/workhome/lichee_A40i/buildroot/dl/qt-everywhere-opensource-src-5.4.1/qtbase/src/3rdparty/angle/include . /home/skyliqf/workhome/lichee_A40i/out/sun8iw11p1/linux/common/buildroot/external-toolchain/arm-linux-gnueabi/include/c++/4.6.3 /home/skyliqf/workhome/lichee_A40i/out/sun8iw11p1/linux/common/buildroot/external-toolchain/arm-linux-gnueabi/include/c++/4.6.3/arm-linux-gnueabi /home/skyliqf/workhome/lichee_A40i/out/sun8iw11p1/linux/common/buildroot/external-toolchain/arm-linux-gnueabi/include/c++/4.6.3/backward /home/skyliqf/workhome/lichee_A40i/out/sun8iw11p1/linux/common/buildroot/external-toolchain/lib/gcc/arm-linux-gnueabi/4.6.3/include /home/skyliqf/workhome/lichee_A40i/out/sun8iw11p1/linux/common/buildroot/external-toolchain/lib/gcc/arm-linux-gnueabi/4.6.3/include-fixed /home/skyliqf/workhome/lichee_A40i/out/sun8iw11p1/linux/common/buildroot/external-toolchain/arm-linux-gnueabi/include /home/skyliqf/workhome/lichee_A40i//out/sun8iw11p1/linux/common/buildroot/host/usr/arm-unknown-linux-gnueabi/sysroot/usr/include/arm-linux-gnueabi /home/skyliqf/workhome/lichee_A40i//out/sun8iw11p1/linux/common/buildroot/host/usr/arm-unknown-linux-gnueabi/sysroot/usr/include
}
QT_CONFIG +=  minimal-config small-config medium-config large-config full-config evdev tslib linuxfb accessibility egl eglfs opengl opengles2 shared qpa reduce_exports clock-gettime clock-monotonic posix_fallocate mremap getaddrinfo ipv6ifname getifaddrs inotify eventfd png freetype harfbuzz zlib dbus rpath alsa concurrent audio-backend release

#versioning
QT_VERSION = 5.4.1
QT_MAJOR_VERSION = 5
QT_MINOR_VERSION = 4
QT_PATCH_VERSION = 1

#namespaces
QT_LIBINFIX = 
QT_NAMESPACE = 

# pkgconfig
PKG_CONFIG_SYSROOT_DIR = /home/skyliqf/workhome/lichee_A40i/buildroot/dl/qt-everywhere-opensource-src-5.4.1/../../..//out/sun8iw11p1/linux/common/buildroot/host/usr/arm-unknown-linux-gnueabi/sysroot
PKG_CONFIG_LIBDIR = /home/skyliqf/workhome/lichee_A40i/buildroot/dl/qt-everywhere-opensource-src-5.4.1/../../..//out/sun8iw11p1/linux/common/buildroot/host/usr/arm-unknown-linux-gnueabi/sysroot/usr/lib/pkgconfig:/home/skyliqf/workhome/lichee_A40i/buildroot/dl/qt-everywhere-opensource-src-5.4.1/../../..//out/sun8iw11p1/linux/common/buildroot/host/usr/arm-unknown-linux-gnueabi/sysroot/usr/share/pkgconfig:/home/skyliqf/workhome/lichee_A40i/buildroot/dl/qt-everywhere-opensource-src-5.4.1/../../..//out/sun8iw11p1/linux/common/buildroot/host/usr/arm-unknown-linux-gnueabi/sysroot/usr/lib/arm-linux-gnueabi/pkgconfig

# sysroot
!host_build {
    QMAKE_CFLAGS    += --sysroot=$$[QT_SYSROOT]
    QMAKE_CXXFLAGS  += --sysroot=$$[QT_SYSROOT]
    QMAKE_LFLAGS    += --sysroot=$$[QT_SYSROOT]
}

QMAKE_RPATHDIR +=  "/usr/lib"
QT_GCC_MAJOR_VERSION = 4
QT_GCC_MINOR_VERSION = 6
QT_GCC_PATCH_VERSION = 3
