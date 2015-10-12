#include "../include/CFileUtils.hpp"
#include <stack>
#include <algorithm>

using namespace std;

CFileUtils* CFileUtils::s_sharedFileUtils = NULL;

void CFileUtils::purgeFileUtils()
{
    if (NULL != s_sharedFileUtils)
	{
		delete s_sharedFileUtils;
		s_sharedFileUtils = NULL;
	}
}

CFileUtils::CFileUtils()
{
}

CFileUtils::~CFileUtils()
{
   
}

bool CFileUtils::init()
{
    m_searchPathArray.push_back(m_strDefaultResRootPath);
    m_searchResolutionsOrderArray.push_back("");
    return true;
}

void CFileUtils::purgeCachedEntries()
{
    m_fullPathCache.clear();
}

unsigned char* CFileUtils::getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize)
{
    unsigned char * pBuffer = NULL;
    *pSize = 0;
    do
    {
        // read the file from hardware
        std::string fullPath = fullPathForFilename(pszFileName);
        FILE *fp = fopen(fullPath.c_str(), pszMode);
        if (!fp) break;
        
        fseek(fp,0,SEEK_END);
        *pSize = ftell(fp);
        fseek(fp,0,SEEK_SET);
        pBuffer = new unsigned char[*pSize];
        *pSize = fread(pBuffer,sizeof(unsigned char), *pSize,fp);
        fclose(fp);
    } while (0);
    
    if (! pBuffer)
    {
        std::string msg = "Get data from file(";
        msg.append(pszFileName).append(") failed!");
		printf("ERROR=========%s\n", msg.c_str());
    }
    return pBuffer;
}

unsigned char* CFileUtils::getFileDataFromZip(const char* pszZipFilePath, const char* pszFileName, unsigned long * pSize)
{
    unsigned char * pBuffer = NULL;
    return pBuffer;
}

std::string CFileUtils::getFileString(const char* pszFilePath)
{
    std::string data;
    
    unsigned long pSize = 0;
    unsigned char* pData = this->getFileData(pszFilePath, "r", &pSize);
    
    data.resize(pSize, sizeof(char));
    for (unsigned int i=0; i<data.size(); i++)
    {
        data[i] = pData[i];
    }
    data += '\0';
    
    return data;
}

std::string CFileUtils::getNewFilename(const std::string& pszFileName)
{
    std::string pszNewFileName = "";
    return pszNewFileName;
}

std::string CFileUtils::getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath)
{
    std::string file = filename;
    std::string file_path = "";
    size_t pos = filename.find_last_of("/");
    if (pos != std::string::npos)
    {
        file_path = filename.substr(0, pos+1);
        file = filename.substr(pos+1);
    }
    
    // searchPath + file_path + resourceDirectory
    std::string path = searchPath;
    path += file_path;
    path += resolutionDirectory;
    
    path = getFullPathForDirectoryAndFilename(path, file);
    
    return path;
}


std::string CFileUtils::fullPathForFilename(const std::string& pszFileName)
{
    std::string strFileName = std::string(pszFileName);
    if (isAbsolutePath(pszFileName))
    {
        //CCLOG("Return absolute path( %s ) directly.", pszFileName);
        return pszFileName;
    }
    // Already Cached ?
    std::map<std::string, std::string>::iterator cacheIter = m_fullPathCache.find(pszFileName);
    if (cacheIter != m_fullPathCache.end())
    {
        //CCLOG("Return full path from cache: %s", cacheIter->second.c_str());
        return cacheIter->second;
    }
    // Get the new file name.
    std::string newFilename = getNewFilename(pszFileName);
    
    string fullpath = "";
    
    std::vector<std::string>::iterator searchPathsIter;
    for (searchPathsIter = m_searchPathArray.begin();
         searchPathsIter != m_searchPathArray.end();
         ++searchPathsIter)
    {
        std::vector<std::string>::iterator resOrderIter;
        for (resOrderIter = m_searchResolutionsOrderArray.begin();
             resOrderIter != m_searchResolutionsOrderArray.end();
             ++resOrderIter)
        {
            fullpath = this->getPathForFilename(newFilename, *resOrderIter, *searchPathsIter);
            
            if (fullpath.length() > 0)
            {
                // Using the filename passed in as key.
                m_fullPathCache.insert(std::pair<std::string, std::string>(pszFileName, fullpath));
                return fullpath;
            }
        }
    }
    
    return pszFileName;
}

const char* CFileUtils::fullPathFromRelativeFile(const char *pszFilename, const char *pszRelativeFile)
{
    return "";
}

void CFileUtils::setSearchResolutionsOrder(const std::vector<std::string>& searchResolutionsOrder)
{
    
}

void CFileUtils::addSearchResolutionsOrder(const char* order)
{
    m_searchResolutionsOrderArray.push_back(order);
}

const std::vector<std::string>& CFileUtils::getSearchResolutionsOrder()
{
    return m_searchResolutionsOrderArray;
}

const std::vector<std::string>& CFileUtils::getSearchPaths()
{
    return m_searchPathArray;
}

void CFileUtils::setSearchPaths(const std::vector<std::string>& searchPaths)
{
    bool bExistDefaultRootPath = false;

    m_fullPathCache.clear();
    m_searchPathArray.clear();
    for (std::vector<std::string>::const_iterator iter = searchPaths.begin(); iter != searchPaths.end(); ++iter)
    {
        std::string strPrefix;
        std::string path;
        if (!isAbsolutePath(*iter))
        { // Not an absolute path
            strPrefix = m_strDefaultResRootPath;
        }
        path = strPrefix+(*iter);
        if (path.length() > 0 && path[path.length()-1] != '/')
        {
            path += "/";
        }
        if (!bExistDefaultRootPath && path == m_strDefaultResRootPath)
        {
            bExistDefaultRootPath = true;
        }
        m_searchPathArray.push_back(path);
    }
    
    if (!bExistDefaultRootPath)
    {
        m_searchPathArray.push_back(m_strDefaultResRootPath);
    }
}

void CFileUtils::addSearchPath(const char* path_)
{
    std::string strPrefix;
    std::string path(path_);
    if (!isAbsolutePath(path))
    { // Not an absolute path
        strPrefix = m_strDefaultResRootPath;
    }
    path = strPrefix + path;
    if (path.length() > 0 && path[path.length()-1] != '/')
    {
        path += "/";
    }
    m_searchPathArray.push_back(path);
}

void CFileUtils::removeSearchPath(const char *path_)
{
	std::string strPrefix;
	std::string path(path_);
	if (!isAbsolutePath(path))
	{ // Not an absolute path
		strPrefix = m_strDefaultResRootPath;
	}
	path = strPrefix + path;
	if (path.length() > 0 && path[path.length()-1] != '/')
	{
		path += "/";
	}
	std::vector<std::string>::iterator iter = std::find(m_searchPathArray.begin(), m_searchPathArray.end(), path);
	m_searchPathArray.erase(iter);
}

void CFileUtils::removeAllPaths()
{
	m_searchPathArray.clear();
}




std::string CFileUtils::getFullPathForDirectoryAndFilename(const std::string& strDirectory, const std::string& strFilename)
{
    std::string ret = strDirectory;
    if (strDirectory.size() && strDirectory[strDirectory.size()-1] != '/')
    {
        ret += '/';
    }
    ret += strFilename;
    
    if (!access(ret.c_str(), 0)) {
        ret = "";
    }
    return ret;
}

bool CFileUtils::isAbsolutePath(const std::string& strPath)
{
    return strPath[0] == '/' ? true : false;
}

//////////////////////////////////////////////////////////////////////////
// Notification support when getFileData from invalid file path.
//////////////////////////////////////////////////////////////////////////
static bool s_bPopupNotify = true;

void CFileUtils::setPopupNotify(bool bNotify)
{
    s_bPopupNotify = bNotify;
}

bool CFileUtils::isPopupNotify()
{
    return s_bPopupNotify;
}

/*
 * Create a direcotry is platform depended.
 */
bool CFileUtils::createDirectory(const char *path)
{
    DIR *dir=NULL;
    dir = opendir(path);
    if (dir) return false;
    
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    return true;
}


