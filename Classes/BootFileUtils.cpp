
#include "BootFileUtils.h"
#include "jni.h"
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#include <stdlib.h>
#include "JniBootHelper.h"

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <vector>
#include <unordered_map>

#include "DexMarcoDef.h"

using namespace std;

BootFileUtils* BootFileUtils::s_sharedFileUtils = nullptr;

BootFileUtils* BootFileUtils::getInstance()
{
    if (s_sharedFileUtils == nullptr)
    {
        s_sharedFileUtils = new BootFileUtils();
    }
    return s_sharedFileUtils;
}

void BootFileUtils::destroyInstance()
{
    if(s_sharedFileUtils!=nullptr)
    {
    	delete s_sharedFileUtils;
    	s_sharedFileUtils=nullptr;
    }
}

unsigned char* BootFileUtils::getFileData(const std::string& filename, const char* mode, ssize_t * size)
{    
    unsigned char * data = 0;
    
    if ( filename.empty() || (! mode) )
    {
        return 0;
    }

    if (filename[0] != '/')
    {
        string relativePath = string();

        size_t position = filename.find("assets/");
        if (0 == position) {
            relativePath += filename.substr(strlen("assets/"));
        } else {
            relativePath += filename;
        }
        LOGD("relative path = %s", relativePath.c_str());

        AAssetManager* assetmanager = JniBootHelper::getAssetManager();

        if (nullptr == assetmanager) {
            LOGD("... assetmanager is nullptr");
            return nullptr;
        }

        // read asset data
        AAsset* asset =
            AAssetManager_open(assetmanager,
                               relativePath.c_str(),
                               AASSET_MODE_UNKNOWN);
        if (nullptr == asset) {
            LOGD("asset is nullptr");
            return nullptr;
        }

        off_t fileSize = AAsset_getLength(asset);

        data = (unsigned char*) malloc(fileSize);

        int bytesread = AAsset_read(asset, (void*)data, fileSize);
        if (size)
        {
            *size = bytesread;
        }

        AAsset_close(asset);
    }
    else
    {
        do
        {
            FILE *fp = fopen(filename.c_str(), mode);
            if(!fp)
            	break;
            
            long fileSize;
            fseek(fp,0,SEEK_END);
            fileSize = ftell(fp);
            fseek(fp,0,SEEK_SET);
            data = (unsigned char*) malloc(fileSize);
            fileSize = fread(data,sizeof(unsigned char), fileSize,fp);
            fclose(fp);
            
            if (size)
            {
                *size = fileSize;
            }
        } while (0);
    }
    
    if (! data)
    {
        std::string msg = "Get data from file(";
        msg.append(filename).append(") failed!");
        LOGD("%s", msg.c_str());
    }

    return data;
}

bool BootFileUtils::isDirectoryExistInternal(const std::string& dirPath) const
{
    struct stat st;
    if (stat(dirPath.c_str(), &st) == 0)
    {
        return S_ISDIR(st.st_mode);
    }
    return false;
}

bool BootFileUtils::createDirectory(const std::string& path)
{    
    if (isDirectoryExistInternal(path))
        return true;
    
    // Split the path
    size_t start = 0;
    size_t found = path.find_first_of("/\\", start);
    std::string subpath;
    std::vector<std::string> dirs;
    
    if (found != std::string::npos)
    {
        while (true)
        {
            subpath = path.substr(start, found - start + 1);
            if (!subpath.empty())
                dirs.push_back(subpath);
            start = found+1;
            found = path.find_first_of("/\\", start);
            if (found == std::string::npos)
            {
                if (start < path.length())
                {
                    dirs.push_back(path.substr(start));
                }
                break;
            }
        }
    }

    DIR *dir = NULL;

    // Create path recursively
    subpath = "";
    for (int i = 0; i < dirs.size(); ++i)
    {
        subpath += dirs[i];
        dir = opendir(subpath.c_str());
        
        if (!dir)
        {
            // directory doesn't exist, should create a new one
            
            int ret = mkdir(subpath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
            if (ret != 0 && (errno != EEXIST))
            {
                // current directory can not be created, sub directories can not be created too
                // should return
                return false;
            }
        }
        else
        {
            // directory exists, should close opened dir
            closedir(dir);
        }
    }
    return true;
}

bool BootFileUtils::isFileExistInternal(const std::string& strFilePath) const
{
    if (strFilePath.empty())
    {
        return false;
    }

    bool bFound = false;
    
    // Check whether file exists in apk.
    if (strFilePath[0] != '/')
    {
        const char* s = strFilePath.c_str();

        // Found "assets/" at the beginning of the path and we don't want it
        if (strFilePath.find("assets/") == 0) s += strlen("assets/");
        AAssetManager* assetmanager = JniBootHelper::getAssetManager();
        if (assetmanager) {
            AAsset* aa = AAssetManager_open(assetmanager, s, AASSET_MODE_UNKNOWN);
            if (aa)
            {
                bFound = true;
                AAsset_close(aa);
            }
        }
    }
    else
    {
        FILE *fp = fopen(strFilePath.c_str(), "r");
        if(fp)
        {
            bFound = true;
            fclose(fp);
        }
    }
    return bFound;
}

void BootFileUtils::writeData2File(const char* destFile, const unsigned char* data, ssize_t size)
{
	FILE *fp = fopen(destFile,"wb");
	if (!fp)
	{
		LOGD("can't create file: %s", destFile);
		return ;
	}
    LOGD("file2write: %s", destFile);
    LOGD("file2write size: %d", size);
	fseek(fp, 0, SEEK_SET);
	fwrite(data, size, 1, fp);
	fclose(fp);
}
