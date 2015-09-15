#include "../include/CSendBuf.hpp"

CSendBuf::CSendBuf()
{
}

CSendBuf::~CSendBuf()
{
}

bool CSendBuf::init(int32 size, int32 extraSize)
{
    return m_sendbuf.init(size, extraSize);
}

int32 CSendBuf::putMsg(PkgHeader *header, char* msg, int32 msgSize)
{
    char buf[msgSize + sizeof(*header)];
    memcpy(buf, (char*)header, sizeof(*header));
    memcpy(buf + sizeof(*header), msg, msgSize);
    return m_sendbuf.pushMsg(buf,msgSize+sizeof(*header));
}
