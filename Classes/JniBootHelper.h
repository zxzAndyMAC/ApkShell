
#ifndef __ANDROID_JNI_BOOT_HELPER_H__
#define __ANDROID_JNI_BOOT_HELPER_H__

#include <jni.h>
#include <string>
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"

typedef struct JniMethodInfo_
{
    JNIEnv *    env;
    jclass      classID;
    jmethodID   methodID;
} JniMethodInfo;

class JniBootHelper
{
public:
    static void             setJavaVM(JavaVM *javaVM);
    static JavaVM*          getJavaVM();
    static JNIEnv*          getEnv();

    static bool             setassetmanager(jobject activityinstance);
    static AAssetManager*   getAssetManager() { return _assetmanager; }

    static bool             setFileDir(jobject activityinstance);
    static bool             setCacheDir(jobject activityinstance);

    static bool             loadDexFile(jobject context,
                                        const char* dexPath, 
                                        const char* dexOptDir);

    static bool             loadClass(jobject context);
	static bool             attachBaseContext(jobject context, jobject application);

    static jobject          invokeStaticMethod(JNIEnv *env, 
                                                const char* className, 
                                                const char* methodName, 
                                                jobjectArray pareTyple, 
                                                jobjectArray pareVaules);

    static jobject          getFieldOjbect(JNIEnv *env, 
                                           const char* className, 
                                           const char* fieldName,
                                           jobject obj);
    static void             setFieldOjbect(JNIEnv *env, 
                                           const char* className, 
                                           const char* fieldName, 
                                           jobject obj, 
                                           jobject filedVaule);

    static std::string      jstring2string(jstring str);

    static std::string FileDir;
    static std::string CacheDir;

private:
    static JNIEnv*          cacheEnv(JavaVM* jvm);

    static bool             getMethodInfo(JniMethodInfo &methodinfo,
                                        const char *className,
                                        const char *methodName,
                                        const char *paramCode);

    static JavaVM*          _psJavaVM;
    static AAssetManager*   _assetmanager;
    static jmethodID        _loadclassMethod_methodID;
    static jobject          _classloader;
};

#endif // __ANDROID_JNI_BOOT_HELPER_H__
