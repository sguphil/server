#ifndef __CRECVBUF_H__
#define __CRECVBUF_H__

#include "baseHeader.h"
#include "CBuffQueue.hpp"
#include "packHeader.hpp"

class CRecvBuf
{
public:
    CRecvBuf(int32 size, int32 extraSize);
    ~CRecvBuf();
    bool checkHead(PkgHeader *header) //check if buff large than header size
    {
        int32 size = sizeof(*header);
        return size > m_buffQueue.getBufLen();
    }

    bool checkMsg(PkgHeader *header) //check if buff is large than (sizeof(PkgHeader)+ msgsize)
    {
        int32 size = sizeof(*header) + header->length;
        return size > m_buffQueue.getBufLen();
    }

    inline int32 getHead(PkgHeader *header, int32 bufsize)
    {
        PkgHeader header;
        if (!checkHead(&header))
        {
            return -1;
        }

        return m_buffQueue.GetMsg(header, sizeof(*header));
    }

    inline int32 getMsg(char *buf, int32 bufsize)
    {
        PkgHeader header;
        if (!checkMsg(&header))
        {
            return -1;
        }

        return m_buffQueue.GetMsg(buf, buffsize);
    }

    int32 pushMsg(char *buf, int32 bufsize)
    {
            return m_buffQueue.pushMsg(buf, bufsize)
    }

private:
    CBuffQueue<char> m_buffQueue;
};

#endif
