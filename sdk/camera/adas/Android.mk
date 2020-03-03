ifeq ($(ADAS_ENABLE),1)
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_LDFLAGS += \
	$(LOCAL_PATH)/libmdas.a \

LOCAL_SHARED_LIBRARIES:= \
    libbinder \
    libutils \
    libcutils \
    libui
    
LOCAL_C_INCLUDES += 								\
	frameworks/base/core/jni/android/graphics 		\
	frameworks/native/include/media/openmax			\
	hardware/libhardware/include/hardware			\
	frameworks/native/include						\
	frameworks/native/include/media/hardware \
	frameworks/base/allwinnertech/include          \
	frameworks/av/media/liballwinner/LIBRARY/CODEC/VIDEO/DECODER \
	$(TARGET_HARDWARE_INCLUDE)

LOCAL_SRC_FILES := \
	AdasProcess.c \

LOCAL_MODULE:= libmdas
		
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)

endif
