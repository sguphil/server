#ifndef __CPKGBUF_H__
#define __CPKGBUF_H__

#include "ICPkgBuf.hpp"
#include "../include/packHeader.hpp"

template<int32 SIZE, int32 headSize>
class CPkgBuf : public ICPkgBuf
{
public:
    CPkgBuf():
        m_nBufLen(0),
        m_nCapacity(SIZE),
        m_nHeadSize(headSize),
        m_nReadPos(0)
    {
    }

    ~CPkgBuf()
    {
        m_nReadPos = 0;
        m_nBufLen = 0;
        m_nCapacity = SIZE;
        m_nHeadSize = headSize;
    }

    void init()
    {
        m_nReadPos = 0;
        m_nBufLen = 0;
        m_nCapacity = SIZE;
        m_nHeadSize = headSize;
    }

    int32 getbufLen()
    {
        return m_nBufLen;
    }

    int32 getcapacity()
    {
        return m_nCapacity;
    }

    int32 getHeadSize()
    {
        return m_nHeadSize;
    }

    //return 0 means head is not recv complete
    int32 getPkgSize() 
    {
        int32 allLen = 0;
        if (m_nBufLen >= m_nHeadSize)
        {
            PkgHeader *head = (PkgHeader *)m_msgbuf;
            allLen = head->length;
        }
        return allLen;
    }

    //return 0 means head is not recv complete
    int32 getLeftPkgSize()
    {
        int32 leftLen = 0;
        if (m_nBufLen >= m_nHeadSize)
        {
            PkgHeader *head = (PkgHeader *)m_msgbuf;
            leftLen = head->length - m_nBufLen;
        }
        return leftLen;
    }

    char* getPkgWritePos()
    {
        return m_msgbuf + m_nBufLen;
    }

    char* getPkgReadPos()
    {
        return m_msgbuf + m_nReadPos;
    }
    
    void incReadLen(int32 size)
    {
        m_nReadPos += size;
    }

    void  incPkgLen(int32 size)
    {
        m_nBufLen += size;
    }

    int32 getReadLen()
    {
        return m_nReadPos;
    }

    ICPkgBuf* copyfrom(ICPkgBuf *buf)
    {
        if (this->m_nCapacity < buf->getcapacity())
        {
            return NULL;
        }

        this->m_nBufLen = buf->getbufLen();
        this->m_nHeadSize = buf->getHeadSize();
        this->m_nReadPos = buf->getReadLen();
        memcpy(this->m_msgbuf, buf->getPkgWritePos(), this->m_nBufLen);
        return this;
    }

private:
    CPkgBuf(CPkgBuf &buf)//no copy func
    {
    }
private:
    char m_msgbuf[SIZE];
    int32 m_nBufLen;
    int32 m_nCapacity;
    int32 m_nHeadSize;
    int32 m_nReadPos; //send/read buf only
};

#endif
