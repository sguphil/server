#include "../include/CRecvBuf.hpp"

CRecvBuf::CRecvBuf()
{
    
}

CRecvBuf::CRecvBuf(int32 size,int32 extraSize)
{
    init(size, extraSize);
}

CRecvBuf::~CRecvBuf()
{

}

int32 CRecvBuf::getHead(PkgHeader *header)
{
    if (!checkHead(header))
    {
        return -1;
    }
    
    memcpy(header, m_buffQueue.getReadPtr(sizeof(*header)), sizeof(*header));
    return sizeof(*header);
}

int32 CRecvBuf::getMsg(char *buf, int32 bufsize)
{
    PkgHeader header;
    header.length = bufsize;
    if (!checkMsg(&header))
    {
        return -1;
    }
    m_buffQueue.popMsg((char *)&header, sizeof(header));
    return m_buffQueue.popMsg(buf, bufsize);
}
