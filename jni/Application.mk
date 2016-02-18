APP_STL := gnustl_static
NDK_TOOLCHAIN_VERSION = 4.9
APP_PLATFORM := android-19
APP_CPPFLAGS := -frtti -std=c++11 -fsigned-char -fexceptions
APP_LDFLAGS := -latomic
APP_ABI := all
#armeabi x86

ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DORIGIN_BOOT_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif