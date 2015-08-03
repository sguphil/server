#ifndef __CRECVBUF_H__
#define __CRECVBUF_H__

#include "baseHeader.h"
#include "CBuffQueue.hpp"
#include "packHeader.hpp"

class CRecvBuf
{
public:
    CRecvBuf();
    CRecvBuf(int32 size, int32 extraSize);
    ~CRecvBuf();
    inline bool init(int32 size, int32 extraSize)
    {
        return m_buffQueue.init(size, extraSize);
    }

    inline bool checkHead(PkgHeader *header) //check if buff large than header size
    {
        int32 size = sizeof(*header);
        return size > m_buffQueue.getBufLen();
    }

    inline bool checkMsg(PkgHeader *header) //check if buff is large than (sizeof(PkgHeader)+ msgsize)
    {
        int32 size = header->length;
        return size > m_buffQueue.getBufLen();
    }

    int32 getHead(PkgHeader *header);
    
    int32 getMsg(char *buf, int32 bufsize);

    inline CBuffQueue<char>* getBuffQueuePtr()
    {
        return &m_buffQueue;
    }
protected:
    CRecvBuf(CRecvBuf& bufqueue);

    CRecvBuf& operator=(CRecvBuf& bufqueue);
private:
    CBuffQueue<char> m_buffQueue;
};

#endif
