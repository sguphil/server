#ifndef __CPACKAGEFETCH_H__
#define __CPACKAGEFETCH_H__
#include "packHeader.hpp"
extern int32 MAXPKGLEN;
extern int32 SESSIONBUFLEN;

class CpackageFetch
{
public:
    CpackageFetch();
    ~CpackageFetch();
private:
    CpackageFetch(CpackageFetch &inst);
    CpackageFetch& operator=(CpackageFetch& inst);
public:
    inline int32 getPackageLen()
    {
        return sizeof(m_pkgHeader)+sizeof(m_msgHeader)+m_nMsglen;
    }

    inline int32 getMsgLen()
    {
        return m_nMsglen;
    }

    inline void setPkgHeader(PkgHeader& header)
    {
        m_pkgHeader = header;
        //m_pkgHeader.length = header.length;
        //m_pkgHeader.reserved = header.reserved;
    }

    inline void setMsgHeader(MsgHeader& msghead)
    {
        m_msgHeader = msghead;
        //m_msgHeader.sysId = msghead.sysId;
        //m_msgHeader.msgType = msghead.msgType;
    }

    inline int32 setMsgBuff(char* buff, int32 size)
    {
        if (size > MAXPKGLEN)
        {
            assert(size <= MAXPKGLEN);
            return -1;
        }
        memcpy(m_msgbuf,buff, size);
        m_nMsglen = size;
        return size;
    }

    inline char* getMsgbuf()
    {
        return m_msgbuf;
    }

public:// all can access the items
    PkgHeader m_pkgHeader;
    MsgHeader m_msgHeader;
    char *m_msgbuf;;
    int32 m_nMsglen;
};

#endif

