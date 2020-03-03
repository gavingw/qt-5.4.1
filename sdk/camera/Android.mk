
CAMERA_HAL_LOCAL_PATH := $(call my-dir)
include $(call all-subdir-makefiles)
include $(CLEAR_VARS)
LOCAL_PATH := $(CAMERA_HAL_LOCAL_PATH)

LOCAL_PREBUILT_LIBS := libcnr.so libfacedetection/libfacedetection.so libfacedetection/libsmileeyeblink.so libfacedetection/libapperceivepeople.so

LOCAL_MODULE_TAGS := optional
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)

LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw

LOCAL_LDLIBS := -llog

LOCAL_SHARED_LIBRARIES:= \
    libbinder \
    libutils \
    libcutils \
    libcamera_client \
    libui \

# cedarx libraries
LOCAL_SHARED_LIBRARIES += \
    libfacedetection \
    libMemAdapter    \
    libcnr \
    libsmileeyeblink \
    libvencoder \
    libvdecoder

ifeq ($(ADAS_ENABLE),1)
LOCAL_SHARED_LIBRARIES += \
	libmdas \
	
LOCAL_C_INCLUDES += \
	 $(LOCAL_PATH)/adas \
	 frameworks/base/allwinnertech/include          \
	 
endif
LOCAL_SHARED_LIBRARIES += \
    libwater_mark


LOCAL_C_INCLUDES +=                                 \
    frameworks/base/core/jni/android/graphics       \
    frameworks/native/include/media/openmax         \
    hardware/libhardware/include/hardware           \
    frameworks/native/include                       \
    frameworks/av/media/liballwinner/LIBRARY/CODEC/VIDEO/ENCODER/include \
    frameworks/av/media/liballwinner/LIBRARY/CODEC/VIDEO/DECODER/include \
    frameworks/native/include/media/hardware \
    system/core/include/camera \
    $(LOCAL_PATH)/allwinnertech/include \
    $(LOCAL_PATH)/allwinnertech/water_mark \
    $(TARGET_HARDWARE_INCLUDE)

LOCAL_SRC_FILES := \
    HALCameraFactory.cpp \
    PreviewWindow.cpp \
    CallbackNotifier.cpp \
    CCameraConfig.cpp \
    BufferListManager.cpp \
    OSAL_Mutex.c \
    OSAL_Queue.c \
    scaler.c \
    Libve_Decoder2.c \
    CameraList.cpp

# choose hal for new driver or old
SUPPORT_NEW_DRIVER := Y

ifeq ($(SUPPORT_NEW_DRIVER),Y)
LOCAL_CFLAGS += -DSUPPORT_NEW_DRIVER
LOCAL_SRC_FILES += \
    CameraHardware2.cpp \
    V4L2CameraDevice2.cpp
else
LOCAL_SRC_FILES += \
    CameraHardware.cpp \
    V4L2CameraDevice.cpp
endif

ifneq ($(filter nuclear%,$(TARGET_DEVICE)),)
LOCAL_CFLAGS += -D__SUN5I__
endif

ifneq ($(filter crane%,$(TARGET_DEVICE)),)
LOCAL_CFLAGS += -D__SUN4I__
endif

ifneq ($(filter fiber%,$(TARGET_DEVICE)),)
LOCAL_CFLAGS += -D__SUN6I__
endif

ifneq ($(filter wing%,$(TARGET_DEVICE)),)
LOCAL_CFLAGS += -D__SUN7I__
endif

ifneq ($(filter jaws%,$(TARGET_DEVICE)),)
LOCAL_CFLAGS += -D__SUN9I__
endif

LOCAL_MODULE := camera.$(TARGET_BOARD_PLATFORM)

LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)
