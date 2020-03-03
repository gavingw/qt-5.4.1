
include $(top_srcdir)/Makefile.inc

bin_PROGRAMS = logcat

logcat_SOURCES =   logcat.cpp

logcat_CFLAGS = $(KUN_CFLAGS)
logcat_CPPFLAGS = $(KUN_CPPFLAGS)
LOCAL_INCLUDE = -I$(top_srcdir)/include \
                -I$(top_srcdir)/

logcat_CFLAGS += $(LOCAL_INCLUDE) \
                 -D_XOPEN_SOURCE -D_GNU_SOURCE -DHAVE_FORKEXEC

logcat_CPPFLAGS += $(LOCAL_INCLUDE) \
                 -D_XOPEN_SOURCE -D_GNU_SOURCE -DHAVE_FORKEXEC

logcat_LDFLAGS = 

logcat_LDADD = $(top_srcdir)/libcutils/libcutils.la \
               $(top_srcdir)/liblog/liblog.la

LOCAL_LIB = -lpthread -ldl

logcat_LDFLAGS += $(LOCAL_LIB)

