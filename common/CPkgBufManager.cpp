#include "CPkgBufManager.hpp"

CPkgbufManager::CPkgbufManager():m_CurPkg(NULL), m_ReadPkg(NULL)
{
    
}

CPkgbufManager::~CPkgbufManager()
{
    if (NULL != m_CurPkg)
    {
        delete m_CurPkg;
        m_CurPkg = NULL;
    }

    m_PkgList.lockList();
    for (std::list<ICPkgBuf *>::iterator it = m_PkgList.begin();
          it != m_PkgList.end(); it++)
    {
        delete *it;
    }
    m_PkgList.unLockList();
}

ICPkgBuf* CPkgbufManager::next()
{
    m_PkgList.lockList();
    if (m_PkgList.empty())
    {
        m_PkgList.unLockList();
        return NULL;
    }

    m_ReadPkg = m_PkgList.front();
    ICPkgBuf *pkg = m_ReadPkg;
    m_PkgList.pop_front();
    m_PkgList.unLockList();
    return pkg;
}

ICPkgBuf* CPkgbufManager::getCurPkg(int32 size)
{
    if (NULL == m_CurPkg)
    {
        m_CurPkg = CPkgBufFactory::getInstance()->alloc(size);
        assert(m_CurPkg != NULL);
    }

    if (m_CurPkg->getbufLen() >= m_CurPkg->getHeadSize())
    {
        int32 pkgsize = m_CurPkg->getPkgSize();
        if (pkgsize > m_CurPkg->getcapacity())
        {
            ICPkgBuf *pkg = CPkgBufFactory::getInstance()->alloc(pkgsize);
            assert(NULL != pkg);
            pkg->copyfrom(m_CurPkg);
            CPkgBufFactory::getInstance()->reuse(m_ReadPkg);
            m_ReadPkg = pkg;
            printf("=====realloc package===========%d\n", pkgsize);
        }
    }

    return m_CurPkg;
}

void CPkgbufManager::pushPkgToList(int32 size)
{
    if (NULL == m_CurPkg)
    {
        return;
    }
    
    m_CurPkg->incPkgLen(size);
    if (m_CurPkg->getbufLen() == m_CurPkg->getPkgSize())
    {
        m_PkgList.lockList();
        m_PkgList.push_back((m_CurPkg));
        m_PkgList.unLockList();
        m_CurPkg = NULL;
    }
}

void CPkgbufManager::readNReusePkg(int32 size)
{
    if (NULL == m_ReadPkg)
    {
        return;
    }
    
    ICPkgBuf *pm_ReadPkg = m_ReadPkg;
    pm_ReadPkg->incReadLen(size);
    if (pm_ReadPkg->getbufLen() == pm_ReadPkg->getReadLen())
    {
        CPkgBufFactory::getInstance()->reuse(pm_ReadPkg);
        m_ReadPkg = NULL;
    }
}

ICPkgBuf* CPkgbufManager::getReadPkg()
{
    if (NULL == m_ReadPkg)
    {
        return next();
    }
    
    return m_ReadPkg;
}


