LOCAL_PATH := $(call my-dir)

# ==========================================
# 1. Dobby (Prebuilt)
# ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE    := dobby
LOCAL_SRC_FILES := Dobby/$(TARGET_ARCH_ABI)/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)

# ==========================================
# 2. Main Project (BloodShot / crashlytics-common)
# ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE           := crashlytics-common
LOCAL_CFLAGS           := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS         := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -Werror -s -std=c++17 -Wno-error=c++11-narrowing -fms-extensions -fno-rtti -fno-exceptions -fpermissive
LOCAL_LDFLAGS          += -Wl,--gc-sections,--strip-all, -llog
LOCAL_LDLIBS           := -llog -landroid -lEGL -lGLESv3 -lGLESv2 -lGLESv1_CM -lz

# ลิงก์ Dobby
LOCAL_STATIC_LIBRARIES := dobby

LOCAL_C_INCLUDES       += $(LOCAL_PATH) \
                          $(LOCAL_PATH)/Bs4484 \
                          $(LOCAL_PATH)/ZygModule \
                          $(LOCAL_PATH)/Misc \
                          $(LOCAL_PATH)/Dobby \
                          $(LOCAL_PATH)/Misc/unity \
                          $(LOCAL_PATH)/Misc/update/xdl/include \
                          $(LOCAL_PATH)/Misc/update \
                          $(LOCAL_PATH)/Misc/update/xdl \
                          $(LOCAL_PATH)/Misc/imgui \
                          $(LOCAL_PATH)/Misc/imgui/fonts

FILE_LIST              := $(wildcard $(LOCAL_PATH)/Misc/imgui/*.c*) \
                          $(wildcard $(LOCAL_PATH)/Misc/update/*.c*) \
                          $(wildcard $(LOCAL_PATH)/Misc/update/xdl/*.c*) \
                          $(wildcard $(LOCAL_PATH)/ZygModule/*.c*)

LOCAL_SRC_FILES        := $(FILE_LIST:$(LOCAL_PATH)/%=%)

include $(BUILD_SHARED_LIBRARY)

