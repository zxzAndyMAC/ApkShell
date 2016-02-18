
#ifndef __CC_BOOT_FILEUTILS_ANDROID_H__
#define __CC_BOOT_FILEUTILS_ANDROID_H__


#include <string>

class BootFileUtils
{
public:

    static BootFileUtils* s_sharedFileUtils;

    static BootFileUtils* getInstance();
    static void destroyInstance();

    unsigned char* getFileData(const std::string& filename, const char* mode, ssize_t * size);
    bool isDirectoryExistInternal(const std::string& dirPath) const;
    bool createDirectory(const std::string& path);
    bool isFileExistInternal(const std::string& strFilePath) const;
    void writeData2File(const char* destFile, const unsigned char* data, ssize_t size);
};

#endif // __CC_BOOT_FILEUTILS_ANDROID_H__