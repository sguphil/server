
#ifndef __CFILEUTILS_H__
#define __CFILEUTILS_H__

#include <string>
#include <vector>
#include <map>
#include "./baseHeader.h"
#include <dirent.h>
#include <sys/stat.h>

class CFileUtils
{
public:
    
    /**
     *  Gets the instance of CFileUtils.
     *  @js getInstance
     */
    static CFileUtils* getInstance()
    {
        if (NULL == s_sharedFileUtils)
        {
            s_sharedFileUtils = new CFileUtils();
        }

        return s_sharedFileUtils;
    }
    /**
     *  Destroys the instance of CFileUtils.
     */
    static void purgeFileUtils();

    virtual ~CFileUtils();

    virtual void purgeCachedEntries();

    virtual unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize);

    virtual unsigned char* getFileDataFromZip(const char* pszZipFilePath, const char* pszFileName, unsigned long * pSize);

    virtual std::string getFileString(const char* pszFilePath);
    
    virtual std::string fullPathForFilename(const std::string& pszFileName);

    virtual const char* fullPathFromRelativeFile(const char *pszFilename, const char *pszRelativeFile);

    virtual void setSearchResolutionsOrder(const std::vector<std::string>& searchResolutionsOrder);

    virtual void addSearchResolutionsOrder(const char* order);

    virtual const std::vector<std::string>& getSearchResolutionsOrder();

    virtual void setSearchPaths(const std::vector<std::string>& searchPaths);

     virtual void addSearchPath(const char* path);

	 virtual void removeSearchPath(const char *path);

	 void removeAllPaths();

    virtual const std::vector<std::string>& getSearchPaths();

    virtual bool isAbsolutePath(const std::string& strPath);

    virtual void setPopupNotify(bool bNotify);
    virtual bool isPopupNotify();

    bool createDirectory(const char *path);
    
protected:
    CFileUtils();

    virtual bool init();

    virtual std::string getNewFilename(const std::string& pszFileName);

    virtual std::string getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath);

    virtual std::string getFullPathForDirectoryAndFilename(const std::string& strDirectory, const std::string& strFilename);
    
    std::vector<std::string> m_searchResolutionsOrderArray;
    
    std::vector<std::string> m_searchPathArray;

    std::string m_strDefaultResRootPath;

    std::map<std::string, std::string> m_fullPathCache;

    static CFileUtils* s_sharedFileUtils;
    
};

#endif    // __CFILEUTILS_H__
