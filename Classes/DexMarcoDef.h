
#ifndef __DEX_MARCO_H__
#define __DEX_MARCO_H__

#include <android/log.h>

#define  DEX_LOG_TAG    "JniBootHelper"

#if !defined(ORIGIN_BOOT_DEBUG) || ORIGIN_BOOT_DEBUG == 0
#define  LOGD(...)       do {} while (0)
#define  LOGE(...)       do {} while (0)
#define  LOGI(...)       do {} while (0)
#elif ORIGIN_BOOT_DEBUG == 1
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,DEX_LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,DEX_LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif

#endif //__DEX_MARCO_H__