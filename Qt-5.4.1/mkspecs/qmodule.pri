CONFIG +=  cross_compile compile_examples qpa largefile nostrip precompile_header pcre
QT_BUILD_PARTS += libs examples
QT_NO_DEFINES =  CUPS EGL_X11 FONTCONFIG GLIB ICONV IMAGEFORMAT_JPEG NIS OPENSSL OPENVG PULSEAUDIO SSL STYLE_GTK XRENDER
QT_QCONFIG_PATH = 
host_build {
    QT_CPU_FEATURES.x86_64 =  mmx sse sse2
} else {
    QT_CPU_FEATURES.arm = 
}
QT_COORD_TYPE = double
QT_LFLAGS_ODBC   = -lodbc
styles += mac fusion windows
DEFINES += QT_NO_MTDEV
DEFINES += QT_NO_LIBUDEV
QMAKE_X11_PREFIX = /usr
QMAKE_RPATHDIR +=  "/usr/lib"
sql-drivers = 
sql-plugins =  sqlite
