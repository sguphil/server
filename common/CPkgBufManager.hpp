#ifndef __CPKGBUFMANAGER_H__
#define __CPKGBUFMANAGER_H__

#include "../include/baseHeader.h"
#include "CPkgBuf.hpp"
#include "CPkgBufFactory.hpp"
#include "../network/include/CommonList.h"

class CPkgbufManager
{
public:
    CPkgbufManager();
    /*
    CPkgbufManager* getInstance()
    {
        static CPkgbufManager instance;
        return &instance;
    }
    */
    ~CPkgbufManager();
    ICPkgBuf* next();

    inline void setCurPkg(ICPkgBuf *pkg)
    {
        m_CurPkg = pkg;
    }

    ICPkgBuf* getCurPkg(int32 size = 0);

    void pushPkgToList(int32 size);

    void readNReusePkg(int32 size);
    
    ICPkgBuf* getReadPkg();

    void swapRWList();

private:
    ICPkgBuf *m_CurPkg; //for recv
    ICPkgBuf *m_ReadPkg;//for read/send
    CommonList<ICPkgBuf> m_PkgList;
    CommonList<ICPkgBuf> m_PkgList_sec; //double list to avoid lock competition
    CommonList<ICPkgBuf> *m_ptrRead;
    CommonList<ICPkgBuf> *m_ptrWrite;
    CMutex m_listSwapLock;
};

#endif
