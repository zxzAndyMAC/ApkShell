LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := origin_shared

LOCAL_MODULE_FILENAME := liboriginshell

LOCAL_SRC_FILES := ../Classes/BootMain.cpp \
				   ../Classes/JniBootHelper.cpp \
				   ../Classes/CPakReader.cpp \
				   ../Classes/BootFileUtils.cpp \
				   ../Classes/Base/e32std.cpp \
				   ../Classes/Base/e32des16.cpp \
				   ../Classes/Base/e32des8.cpp \
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../Classes

LOCAL_LDLIBS := -lz \
                -llog \
                -landroid
            
include $(BUILD_SHARED_LIBRARY)