
#include "JniBootHelper.h"
#include <android/log.h>
#include <string.h>
#include <pthread.h>

#include "DexMarcoDef.h"

static pthread_key_t g_key;


void _detachCurrentThread(void* a) {
    LOGD("DetachCurrentThread");
    JniBootHelper::getJavaVM()->DetachCurrentThread();
}


JavaVM* JniBootHelper::_psJavaVM = nullptr;
std::string JniBootHelper::FileDir = "";
std::string JniBootHelper::CacheDir = "";
AAssetManager* JniBootHelper::_assetmanager = nullptr;
jmethodID JniBootHelper::_loadclassMethod_methodID = nullptr;
jobject JniBootHelper::_classloader = nullptr;

bool JniBootHelper::setassetmanager(jobject activityinstance) {
    JniMethodInfo _getassetsMethod;
    if (!JniBootHelper::getMethodInfo(_getassetsMethod,
                                                     "android/content/Context",
                                                     "getAssets",
                                                     "()Landroid/content/res/AssetManager;")) {
        LOGE("getmethod getAssets() failed.");
        return false;
    }

    jobject _am = JniBootHelper::getEnv()->CallObjectMethod(activityinstance,
                                                                _getassetsMethod.methodID);

    if (nullptr == _am) {
        LOGE("CallObjectMethod getAssets() failed.");
        return false;
    }

    JniBootHelper::_assetmanager = AAssetManager_fromJava(JniBootHelper::getEnv(), _am);
    return true;
}

bool JniBootHelper::setFileDir(jobject activityinstance)
{
    JniMethodInfo _getFileDirMethod;
    if (!JniBootHelper::getMethodInfo(_getFileDirMethod,
                                                     "android/content/Context",
                                                     "getFilesDir",
                                                     "()Ljava/io/File;")) {
        LOGE("getmethod getFilesDir() failed.");
        return false;
    }
    jobject _f = JniBootHelper::getEnv()->CallObjectMethod(activityinstance,
                                                                _getFileDirMethod.methodID);
    if (nullptr == _f) {
        LOGE("CallObjectMethod getFilesDir() failed.");
        return false;
    }
    JniMethodInfo _getFilePathMethod;
    if (!JniBootHelper::getMethodInfo(_getFilePathMethod,
                                                     "java/io/File",
                                                     "getAbsolutePath",
                                                     "()Ljava/lang/String;")) {
        LOGE("getmethod getAbsolutePath() failed.");
        return false;
    }
    jstring _p = (jstring)JniBootHelper::getEnv()->CallObjectMethod(_f,
                                                                _getFilePathMethod.methodID);
    if (nullptr == _p) {
        LOGE("CallObjectMethod getAbsolutePath() failed.");
        return false;
    }

    JniBootHelper::FileDir.assign(JniBootHelper::jstring2string(_p));
    LOGD("apk FileDir : %s", JniBootHelper::FileDir.c_str());
    JniBootHelper::getEnv()->DeleteLocalRef(_p);
    return true;
}

bool JniBootHelper::setCacheDir(jobject activityinstance)
{
    JniMethodInfo _getFileDirMethod;
    if (!JniBootHelper::getMethodInfo(_getFileDirMethod,
                                                     "android/content/Context",
                                                     "getCacheDir",
                                                     "()Ljava/io/File;")) {
        LOGE("getmethod getCacheDir() failed.");
        return false;
    }
    jobject _f = JniBootHelper::getEnv()->CallObjectMethod(activityinstance,
                                                                _getFileDirMethod.methodID);
    if (nullptr == _f) {
        LOGE("CallObjectMethod getCacheDir() failed.");
        return false;
    }
    JniMethodInfo _getFilePathMethod;
    if (!JniBootHelper::getMethodInfo(_getFilePathMethod,
                                                     "java/io/File",
                                                     "getAbsolutePath",
                                                     "()Ljava/lang/String;")) {
        LOGE("getmethod getAbsolutePath() failed.");
        return false;
    }
    jstring _p = (jstring)JniBootHelper::getEnv()->CallObjectMethod(_f,
                                                                _getFilePathMethod.methodID);
    if (nullptr == _p) {
        LOGE("CallObjectMethod getAbsolutePath() failed.");
        return false;
    }

    JniBootHelper::CacheDir.assign(JniBootHelper::jstring2string(_p));
    LOGD("apk CacheDir : %s", JniBootHelper::CacheDir.c_str());
    JniBootHelper::getEnv()->DeleteLocalRef(_p);
    return true;
}

bool JniBootHelper::loadClass(jobject context)
{
    JNIEnv *env = JniBootHelper::getEnv();
    if (!env) {
        return false;
    }

    jstring _jstrClassName = env->NewStringUTF("com/origingame/InApplicationMethod");
    jclass _classid = (jclass) env->CallObjectMethod(JniBootHelper::_classloader,
                                                   JniBootHelper::_loadclassMethod_methodID,
                                                   _jstrClassName);
    if(!_classid)
    {
        LOGE("can not find class com/origingame/InApplicationMethod");
    }
    jmethodID _mid = env->GetStaticMethodID(_classid, "onCreate", "(Landroid/content/Context;)V");
    if(!_mid)
    {
        LOGE("failed to find methodID onCreate");
    }
    env->CallStaticVoidMethod(_classid, _mid, context);
    env->DeleteLocalRef(_jstrClassName);
    env->DeleteGlobalRef(JniBootHelper::_classloader);
}

bool JniBootHelper::attachBaseContext(jobject context, jobject application)
{
    JNIEnv *env = JniBootHelper::getEnv();
    if (!env) {
        return false;
    }

    jstring _jstrClassName = env->NewStringUTF("com/origingame/InApplicationMethod");
    jclass _classid = (jclass) env->CallObjectMethod(JniBootHelper::_classloader,
                                                   JniBootHelper::_loadclassMethod_methodID,
                                                   _jstrClassName);
    if(!_classid)
    {
        LOGE("can not find class com/origingame/InApplicationMethod");
    }
    jmethodID _mid = env->GetStaticMethodID(_classid, "attachBaseContext", "(Landroid/content/Context;Landroid/app/Application;)V");
    if(!_mid)
    {
        LOGE("failed to find methodID attachBaseContext");
    }
    env->CallStaticVoidMethod(_classid, _mid, context, application);
    env->DeleteLocalRef(_jstrClassName);
    //env->DeleteGlobalRef(JniBootHelper::_classloader);
}

bool JniBootHelper::loadDexFile(jobject context,
                                const char* dexPath, 
                                const char* dexOptDir)
{
    JNIEnv *env = JniBootHelper::getEnv();
    if (!env) {
        return false;
    }
    
    jstring dexPathString, dexOptDirString;
    dexPathString = env->NewStringUTF(dexPath);
    dexOptDirString = env->NewStringUTF(dexOptDir);

    jclass native_clazz = env->GetObjectClass(context);
    jmethodID methodID_func = env->GetMethodID(native_clazz, 
                                               "getPackageName", 
                                               "()Ljava/lang/String;");
    jstring packagename = (jstring) (env->CallObjectMethod(context, methodID_func));
    std::string _packagename = JniBootHelper::jstring2string(packagename);
    LOGD("packagename: %s",_packagename.c_str());
    char libsDir[256];
    sprintf(libsDir, "/data/data/%s/lib", _packagename.c_str());
    jstring jlibsDir = env->NewStringUTF(libsDir);

    
    jclass jActivityThread = env->FindClass("android/app/ActivityThread");
    if(!jActivityThread)
    {
        LOGE("can not find class android/app/ActivityThread");
    }
    jmethodID jcATmid = env->GetStaticMethodID(jActivityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
    jobject currentActivityThread = env->CallStaticObjectMethod(jActivityThread, jcATmid);
    
    
    jclass class_hashmap = env->FindClass("java/util/Map");

    
    jobject obj_hashmap = JniBootHelper::getFieldOjbect(env, 
                                                "android.app.ActivityThread", 
                                                "mPackages",
                                                currentActivityThread);
    /*
    jmethodID map_size = env->GetMethodID(class_hashmap, 
                                          "size", 
                                          "()I");
    jint size = (jint)env->CallObjectMethod(obj_hashmap, 
                                      map_size
                                      );
    LOGD("map size : %d", (int)size);
    */
    if(!obj_hashmap)
    {
        LOGE("obj_hashmap is null");
    }

    jclass class_WeakReference = env->FindClass("java/lang/ref/WeakReference");
    if (!class_WeakReference) 
    {
        LOGE("class_WeakReference Not Found ");
    }

    jmethodID WeakReference_get_method = env->GetMethodID(class_WeakReference, 
                                                          "get", 
                                                          "()Ljava/lang/Object;");
    if (!WeakReference_get_method) 
    {
        LOGE("WeakReference_get_method Not Found ");
    }

    jmethodID get_func = env->GetMethodID(class_hashmap, 
                                          "get", 
                                          "(Ljava/lang/Object;)Ljava/lang/Object;");
    if (!get_func) {
        LOGE("method get Not Found ");
    }
    
    jobject get_obj = env->CallObjectMethod(obj_hashmap, 
                                            get_func, 
                                            packagename);
    if (!get_obj) {
        LOGE("get_obj Not Found ");
    }
    
    jobject get_get_obj = env->CallObjectMethod(get_obj, 
                                                WeakReference_get_method);
    if (!get_get_obj) {
        LOGE("get_get_obj Not Found ");
    }
    
    
    jclass class_DexClassloader = env->FindClass("dalvik/system/DexClassLoader");
    if (!class_DexClassloader) {
        LOGE("class_DexClassloader Not Found ");
    }

    jmethodID DexClassloader_construct_method = env->GetMethodID(class_DexClassloader, 
                                                                 "<init>",
                                                                 "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");
    if (!DexClassloader_construct_method) {
        LOGE("DexClassloader_construct_method Not Found ");
    }
    
    jobject obj_DexClassloader = env->NewObject(class_DexClassloader,
                                                DexClassloader_construct_method, 
                                                dexPathString, 
                                                dexOptDirString,
                                                jlibsDir,
                                                JniBootHelper::getFieldOjbect(env, "android.app.LoadedApk", "mClassLoader", get_get_obj));
    if (!obj_DexClassloader) {
        LOGE("obj_DexClassloader Not Found ");
    }
    
	/*
    jclass class_DexClassloader = env->FindClass("dalvik/system/PathClassLoader");
    if (!class_DexClassloader) {
        LOGE("class_DexClassloader Not Found ");
    }

    jmethodID DexClassloader_construct_method = env->GetMethodID(class_DexClassloader, 
                                                                 "<init>",
                                                                 "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");
    if (!DexClassloader_construct_method) {
        LOGE("DexClassloader_construct_method Not Found ");
    }
    
    jobject obj_DexClassloader = env->NewObject(class_DexClassloader,
                                                DexClassloader_construct_method, 
                                                dexPathString,
                                                jlibsDir,
                                                JniBootHelper::getFieldOjbect(env, "android.app.LoadedApk", "mClassLoader", get_get_obj));
    if (!obj_DexClassloader) {
        LOGE("obj_DexClassloader Not Found ");
    }
	*/

    JniBootHelper::_classloader = env->NewGlobalRef(obj_DexClassloader);
    JniBootHelper::_loadclassMethod_methodID = env->GetMethodID(class_DexClassloader,
                                                                "loadClass",
                                                                "(Ljava/lang/String;)Ljava/lang/Class;");

    JniBootHelper::setFieldOjbect(env, "android.app.LoadedApk", "mClassLoader", get_get_obj, obj_DexClassloader);
    env->DeleteLocalRef(dexPathString);
    env->DeleteLocalRef(dexOptDirString);
    env->DeleteLocalRef(jlibsDir);
    LOGD("finish.");
    return true;
}

JavaVM* JniBootHelper::getJavaVM() {
    pthread_t thisthread = pthread_self();
    LOGD("JniBootHelper::getJavaVM(), pthread_self() = %ld", thisthread);
    return _psJavaVM;
}

void JniBootHelper::setJavaVM(JavaVM *javaVM) {
    pthread_t thisthread = pthread_self();
    LOGD("JniBootHelper::setJavaVM(%p), pthread_self() = %ld", javaVM, thisthread);
    _psJavaVM = javaVM;

    pthread_key_create(&g_key, _detachCurrentThread);
}

JNIEnv* JniBootHelper::cacheEnv(JavaVM* jvm) {
    JNIEnv* _env = nullptr;
    // get jni environment
    jint ret = jvm->GetEnv((void**)&_env, JNI_VERSION_1_4);
    
    switch (ret) {
    case JNI_OK :
        // Success!
        pthread_setspecific(g_key, _env);
        return _env;
            
    case JNI_EDETACHED :
        // Thread not attached
        if (jvm->AttachCurrentThread(&_env, nullptr) < 0)
            {
                LOGE("Failed to get the environment using AttachCurrentThread()");

                return nullptr;
            } else {
            // Success : Attached and obtained JNIEnv!
            pthread_setspecific(g_key, _env);
            return _env;
        }
            
    case JNI_EVERSION :
        // Cannot recover from this error
        LOGE("JNI interface version 1.4 not supported");
    default :
        LOGE("Failed to get the environment using GetEnv()");
        return nullptr;
    }
}

JNIEnv* JniBootHelper::getEnv() {
    JNIEnv *_env = (JNIEnv *)pthread_getspecific(g_key);
    if (_env == nullptr)
        _env = JniBootHelper::cacheEnv(_psJavaVM);
    return _env;
}

bool JniBootHelper::getMethodInfo(JniMethodInfo &methodinfo,
                                  const char *className,
                                  const char *methodName,
                                  const char *paramCode) 
{
    if ((nullptr == className) ||
        (nullptr == methodName) ||
        (nullptr == paramCode)) {
        return false;
    }

    JNIEnv *env = JniBootHelper::getEnv();
    if (!env) {
        return false;
    }

    jclass classID = env->FindClass(className);
    if (! classID) {
        LOGE("Failed to find class %s", className);
        env->ExceptionClear();
        return false;
    }

    jmethodID methodID = env->GetMethodID(classID, methodName, paramCode);
    if (! methodID) {
        LOGE("Failed to find method id of %s", methodName);
        env->ExceptionClear();
        return false;
    }

    methodinfo.classID = classID;
    methodinfo.env = env;
    methodinfo.methodID = methodID;

    return true;
}

std::string JniBootHelper::jstring2string(jstring jstr) {
    if (jstr == nullptr) {
        return "";
    }
    
    JNIEnv *env = JniBootHelper::getEnv();
    if (!env) {
        return nullptr;
    }

    const char* chars = env->GetStringUTFChars(jstr, nullptr);
    std::string ret(chars);
    env->ReleaseStringUTFChars(jstr, chars);

    return ret;
}

jobject JniBootHelper::invokeStaticMethod(JNIEnv *env, 
                                          const char* className, 
                                          const char* methodName, 
                                          jobjectArray pareTyple, 
                                          jobjectArray pareVaules)
{
    jstring jclassName, jmethodName;
    jclassName = env->NewStringUTF(className);
    jmethodName = env->NewStringUTF(methodName);
    
    jclass _class = env->FindClass("java/lang/Class");
    if(!_class)
    {
        LOGE("invokeStaticMethod: Failed to find class java/lang/Class");
        env->ExceptionClear();
        return nullptr;
    }
    
    jmethodID forname_func = env->GetStaticMethodID(_class, 
                                                    "forName",
                                                    "(Ljava/lang/String;)Ljava/lang/Class;");
    if(!forname_func)
    {
        LOGE("invokeStaticMethod: Failed to find method forName");
        env->ExceptionClear();
        return nullptr;
    }
    
    jobject class_obj = env->CallStaticObjectMethod(_class, 
                                                    forname_func,
                                                    jclassName);
    
    jclass class_java = env->GetObjectClass(class_obj);
    
    jmethodID getmethod_func = env->GetMethodID(class_java, 
                                                "getMethod",
                                                "(Ljava/lang/String;[Ljava/lang/Class;)Ljava/lang/reflect/Method;");
    if(!getmethod_func)
    {
        LOGE("invokeStaticMethod: Failed to find method getMethod");
        env->ExceptionClear();
        return nullptr;
    }
                                              
    jobject method_obj = env->CallObjectMethod(class_obj, 
                                               getmethod_func,
                                               jmethodName, 
                                               pareTyple);
    if(!method_obj)
    {
        LOGE("invokeStaticMethod: Failed to CallObjectMethod.%s,   %s",className, methodName);
        env->ExceptionClear();
        return nullptr;
    }
    
    jclass class_method_obj = env->GetObjectClass(method_obj);
    
    jmethodID invoke_func = env->GetMethodID(class_method_obj, 
                                             "invoke",
                                             "(Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;");
                                             
    if(!invoke_func)
    {
        LOGE("invokeStaticMethod: Failed to find method invoke");
        env->ExceptionClear();
        return nullptr;
    }
    
    jobject invoke_obj = env->CallObjectMethod(method_obj, 
                                               invoke_func, 
                                               NULL,
                                               pareVaules);
    
    env->DeleteLocalRef(jclassName);
    env->DeleteLocalRef(jmethodName);
    env->DeleteLocalRef(class_java);
    env->DeleteLocalRef(method_obj);
    LOGD("invokeStaticMethod finish.");
    return invoke_obj;
}

jobject JniBootHelper::getFieldOjbect(JNIEnv *env, 
                                      const char* className, 
                                      const char* fieldName,
                                      jobject obj)
{
    jstring jclassName, jfieldName;
    jclassName = env->NewStringUTF(className);
    jfieldName = env->NewStringUTF(fieldName);

    jclass _class = env->FindClass("java/lang/Class");
    jmethodID forname_func = env->GetStaticMethodID(_class, 
                                                    "forName",
                                                    "(Ljava/lang/String;)Ljava/lang/Class;");
    if(!forname_func)
    {
        LOGE("getFieldOjbect: Failed to find method forName");
        env->ExceptionClear();
        return nullptr;
    }
    jobject class_obj = env->CallStaticObjectMethod(_class, 
                                                    forname_func,
                                                    jclassName);
    jclass class_java = env->GetObjectClass(class_obj);

    jmethodID getfield_func = env->GetMethodID(class_java, 
                                               "getDeclaredField",
                                               "(Ljava/lang/String;)Ljava/lang/reflect/Field;");
    if(!getfield_func)
    {
        LOGE("getFieldOjbect: Failed to find method getDeclaredField");
        env->ExceptionClear();
        return nullptr;
    }
    jobject method_obj = env->CallObjectMethod(class_obj, 
                                               getfield_func,
                                               jfieldName);
    jclass class_method_obj = env->GetObjectClass(method_obj);

    jmethodID setaccess_func = env->GetMethodID(class_method_obj,
                                                "setAccessible", 
                                                "(Z)V");
    if(!setaccess_func)
    {
        LOGE("getFieldOjbect: Failed to find method setAccessible");
        env->ExceptionClear();
        return nullptr;
    }
    env->CallVoidMethod(method_obj, 
                        setaccess_func, 
                        true);

    jmethodID get_func = env->GetMethodID(class_method_obj, 
                                          "get",
                                          "(Ljava/lang/Object;)Ljava/lang/Object;");
    if(!get_func)
    {
        LOGE("getFieldOjbect: Failed to find method get");
        env->ExceptionClear();
        return nullptr;
    }
    jobject get_obj = env->CallObjectMethod(method_obj, 
                                            get_func, 
                                            obj);

    env->DeleteLocalRef(class_java);
    env->DeleteLocalRef(method_obj);
    env->DeleteLocalRef(jclassName);
    env->DeleteLocalRef(jfieldName);
    return get_obj;
}

void JniBootHelper::setFieldOjbect(JNIEnv *env, 
                   const char* className, 
                   const char* fieldName, 
                   jobject obj, 
                   jobject filedVaule)
{
    jstring jclassName, jfieldName;
    jclassName = env->NewStringUTF(className);
    jfieldName = env->NewStringUTF(fieldName);

    jclass _class = env->FindClass("java/lang/Class");
    if(!_class)
    {
        LOGE("setFieldOjbect: Failed to find java/lang/Class");
        env->ExceptionClear();
        return;
    }
    jmethodID forName_func = env->GetStaticMethodID(_class, 
                                                    "forName",
                                                    "(Ljava/lang/String;)Ljava/lang/Class;");
    if(!forName_func)
    {
        LOGE("setFieldOjbect: Failed to find method forname");
        env->ExceptionClear();
        return;
    }
    jobject class_obj = env->CallStaticObjectMethod(_class, 
                                                    forName_func,
                                                    jclassName);
    jclass class_java = env->GetObjectClass(class_obj);

    jmethodID getField_func = env->GetMethodID(class_java, 
                                               "getDeclaredField",
                                                "(Ljava/lang/String;)Ljava/lang/reflect/Field;");
    if(!getField_func)
    {
        LOGE("setFieldOjbect: Failed to find method getDeclaredField");
        env->ExceptionClear();
        return;
    }
    jobject method_obj = env->CallObjectMethod(class_obj, 
                                               getField_func,
                                               jfieldName);
    jclass class_method_obj = env->GetObjectClass(method_obj);

    jmethodID setaccess_func = env->GetMethodID(class_method_obj,
                                                "setAccessible", 
                                                "(Z)V");
    if(!setaccess_func)
    {
        LOGE("setFieldOjbect: Failed to find method setAccessible");
        env->ExceptionClear();
        return;
    }
    env->CallVoidMethod(method_obj, setaccess_func, true);

    jmethodID set_func = env->GetMethodID(class_method_obj, 
                                          "set",
                                          "(Ljava/lang/Object;Ljava/lang/Object;)V");
    if(!set_func)
    {
        LOGE("setFieldOjbect: Failed to find method set");
        env->ExceptionClear();
        return;
    }
    env->CallVoidMethod(method_obj, 
                        set_func, 
                        obj, 
                        filedVaule);

    env->DeleteLocalRef(class_java);
    env->DeleteLocalRef(method_obj);
    env->DeleteLocalRef(jclassName);
    env->DeleteLocalRef(jfieldName);
}
