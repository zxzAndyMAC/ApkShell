
#include "JniBootHelper.h"
#include "BootFileUtils.h"
#include "CPakReader.h"

#include <android/log.h>
#include <jni.h>

#include "DexMarcoDef.h"

#define  Dex_asstesPath "assets/OriginAPP.jar"
#define  Dex_NAME "OriginClasses.jar"
#define  Dex_RELEASEFLODER "OriginBoot"
#define  Dex_CACHEFLODER "OriginBootCache"

extern "C"
{
	JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
	{
		LOGD("JNI_OnLoad has been called.");
	    JniBootHelper::setJavaVM(vm);

	    return JNI_VERSION_1_4;
	}

	JNIEXPORT void JNI_OnUnLoad(JavaVM *vm, void *reserved)
	{
	    LOGD("JNI_OnUnLoad has been called.");
	}

	JNIEXPORT void Java_com_origingame_OriginShell_nativeLoadClass(JNIEnv*  env, jobject thiz, jobject context)
	{
		JniBootHelper::loadClass(context);
	}
	
	JNIEXPORT void Java_com_origingame_OriginShell_nativeattachBaseContext(JNIEnv*  env, jobject thiz, jobject context, jobject application)
	{
		JniBootHelper::attachBaseContext(context, application);
	}

	JNIEXPORT void Java_com_origingame_OriginShell_nativeBoot(JNIEnv*  env, jobject thiz, jobject context)
	{
		bool _initsuccess = true;

		if(!JniBootHelper::setassetmanager(context)){
			LOGE("setassetmanager failed!");
			_initsuccess = false;
		}

		if(!JniBootHelper::setFileDir(context)){
			LOGE("setFileDir failed!");
			_initsuccess = false;
		}

		if(!JniBootHelper::setCacheDir(context)){
			LOGE("setCacheDir failed!");
			_initsuccess = false;
		}

		if(_initsuccess)
		{
			LOGD("init success.");
			bool _dirExist = false;
			bool _exist = false;
			BootFileUtils* fileutils = BootFileUtils::getInstance();
			std::string _path = JniBootHelper::FileDir;
			_path.append("/");
			_path.append(Dex_RELEASEFLODER);
			LOGD("check dir: %s", _path.c_str());
			if(fileutils->isDirectoryExistInternal(_path))
			{
				_dirExist = true;
				std::string _file = JniBootHelper::FileDir;
				_file.append("/");
				_file.append(Dex_RELEASEFLODER);
				_file.append("/");
				_file.append(Dex_NAME);
				if(fileutils->isFileExistInternal(_file))
					_exist = true;
			}

			std::string _cachedirpath = JniBootHelper::FileDir;
			_cachedirpath.append("/");
			_cachedirpath.append(Dex_CACHEFLODER);

			if(!_dirExist)
			{
				LOGD("create dir.");
				std::string _filedirpath = JniBootHelper::FileDir;
				_filedirpath.append("/");
				_filedirpath.append(Dex_RELEASEFLODER);
				fileutils->createDirectory(_filedirpath);

				fileutils->createDirectory(_cachedirpath);
			}


			std::string _dexPath = JniBootHelper::FileDir;
			_dexPath.append("/");
			_dexPath.append(Dex_RELEASEFLODER);
			_dexPath.append("/");
			_dexPath.append(Dex_NAME);

			if(!_exist)
			{
				LOGD("needed file is not exist.");
				CPakReader* PakReader = CPakReader::Create(Dex_asstesPath, true);
				TFileBlock fb;
				PakReader->GetBlock(Dex_NAME, fb);
				
				LOGD("destfile: %s", _dexPath.c_str());
				fileutils->writeData2File(_dexPath.c_str(), fb.oData, fb.index.oSize);
				delete PakReader;
				BootFileUtils::destroyInstance();
			}
			else
				LOGD("needed file is exist.");

			
			JniBootHelper::loadDexFile(context, _dexPath.c_str(), _cachedirpath.c_str());
		}
	}
}