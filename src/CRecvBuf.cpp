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
    return m_buffQueue.getHead(header);
}

int32 CRecvBuf::getMsg(char *buf, int32 bufsize)
{
    return m_buffQueue.getMsg(buf, bufsize);
}
/*
int32 CRecvBuf::recvToBuff(int32 socket, int32 recvlen)
{
    return getBuffQueuePtr()->recvFromSocket(socket, recvlen);
}
*/
