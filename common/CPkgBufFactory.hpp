#ifndef __CPKGBUFFACTORY_H__
#define __CPKGBUFFACTORY_H__

#include "CPkgBuf.hpp"
#include "ICPkgBuf.hpp"
#include "../include/packHeader.hpp"
#include "../network/include/CommonList.h"

class CPkgBufFactory
{
public:
    static CPkgBufFactory* getInstance()
    {
        static CPkgBufFactory factory;
        return &factory;
    }

    ICPkgBuf* alloc()
    {
        return new CPkgBuf<32, sizeof(PkgHeader)>();
    }

    ICPkgBuf* alloc(int32 size)
    {
        if (size <= 0 )
        {
            return NULL;
        }
        int32 bufcapacity = 0;
        CommonList<ICPkgBuf> *pkgList = NULL;
        if (size <= 32)
        {
            pkgList = &m_bufListSz32;
            bufcapacity = 32;
        }
        else if (size <= 64)
        {
            pkgList = &m_bufListSz64;
            bufcapacity = 64;
        }
        else if (size <= 128)
        {
            pkgList = &m_bufListSz128;
            bufcapacity = 128;
        }
        else if (size <= 256)
        {
            pkgList = &m_bufListSz256;
            bufcapacity = 256;
        }
        else if (size <= 512)
        {
            pkgList = &m_bufListSz512;
            bufcapacity = 512;
        }
        else if (size <= 1024)
        {
            pkgList = &m_bufListSz1024;
            bufcapacity = 1024;
        }
        else if (size <= 2048)
        {
            pkgList = &m_bufListSz2048;
            bufcapacity = 2048;
        }
        else if (size <= 4096)
        {
            pkgList = &m_bufListSz4096;
            bufcapacity = 4096;
        }
        else if (size <= 8192)
        {
            pkgList = &m_bufListSz8192;
            bufcapacity = 8192;
        }

        if (NULL == pkgList)
        {
            return NULL;
        }
        else
        {
            pkgList->lockList();
            if (!pkgList->empty())
            {
                ICPkgBuf *pkg = pkgList->front();
                pkgList->pop_front();
                pkgList->unLockList();
                return pkg;
            }
            else
            {
                pkgList->unLockList();
                ICPkgBuf *newpkg = NULL;
                switch (bufcapacity)
                {
                case 32:
                    newpkg = new CPkgBuf<32,  sizeof(PkgHeader)>();
                    break;
                case 64:
                    newpkg = new CPkgBuf<64,  sizeof(PkgHeader)>();
                    break;
                case 128:
                    newpkg = new CPkgBuf<128,  sizeof(PkgHeader)>();
                    break;
                case 256:
                    newpkg = new CPkgBuf<256,  sizeof(PkgHeader)>();
                    break;
                case 512:
                    newpkg = new CPkgBuf<512,  sizeof(PkgHeader)>();
                    break;
                case 1024:
                    newpkg = new CPkgBuf<1024,  sizeof(PkgHeader)>();
                    break;
                case 2048:
                    newpkg = new CPkgBuf<2048,  sizeof(PkgHeader)>();
                    break;
                case 4096:
                    newpkg = new CPkgBuf<4096,  sizeof(PkgHeader)>();
                    break;
                case 8192:
                    newpkg = new CPkgBuf<8192,  sizeof(PkgHeader)>();
                    break;
                default:
                    break;
                }
                return newpkg;
            }
        }
    }

    void reuse(ICPkgBuf *pkg)
    {
        int size = pkg->getcapacity();
        CommonList<ICPkgBuf> *pkgList = NULL;
        if (size <= 32)
        {
            pkgList = &m_bufListSz32;
        }
        else if (size <= 64)
        {
            pkgList = &m_bufListSz64;
        }
        else if (size <= 128)
        {
            pkgList = &m_bufListSz128;
        }
        else if (size <= 256)
        {
            pkgList = &m_bufListSz256;
        }
        else if (size <= 512)
        {
            pkgList = &m_bufListSz512;
        }
        else if (size <= 1024)
        {
            pkgList = &m_bufListSz1024;
        }
        else if (size <= 2048)
        {
            pkgList = &m_bufListSz2048;
        }
        else if (size <= 4096)
        {
            pkgList = &m_bufListSz4096;
        }
        else if (size <= 8192)
        {
            pkgList = &m_bufListSz8192;
        }
        
        if (NULL == pkgList)
        {
            delete pkg;
            pkg = NULL;
        }
        else
        {
            pkgList->lockList();
            pkg->init();
            pkgList->push_back(pkg);
            pkgList->unLockList();
        }
    }

private:
    CPkgBufFactory()
    {
        //not to be used
    }
private:
    CommonList<ICPkgBuf> m_bufListSz32;
    CommonList<ICPkgBuf> m_bufListSz64;
    CommonList<ICPkgBuf> m_bufListSz128;
    CommonList<ICPkgBuf> m_bufListSz256;
    CommonList<ICPkgBuf> m_bufListSz512;
    CommonList<ICPkgBuf> m_bufListSz1024;
    CommonList<ICPkgBuf> m_bufListSz2048;
    CommonList<ICPkgBuf> m_bufListSz4096;
    CommonList<ICPkgBuf> m_bufListSz8192; //max package size
};

#endif
