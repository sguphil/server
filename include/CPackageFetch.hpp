#ifndef __CPACKAGEFETCH_H__
#define __CPACKAGEFETCH_H__
#include "packHeader.hpp"

class CpackageFetch
{
public:
    CpackageFetch();
    ~CpackageFetch();
    CpackageFetch(CpackageFetch &inst);
    CpackageFetch& operator=(CpackageFetch& inst);
    
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
        if (size > m_nMsglen && m_msgbuf != NULL)
        {
            m_msgbuf = (char*)realloc((char*)m_msgbuf, sizeof(char)*size);
        }
        else if (NULL == m_msgbuf)
        {
            m_msgbuf = (char*)malloc(sizeof(char)*size);
        }

        if (NULL == m_msgbuf)
        {
            printf("getpackage error!!!");
            assert(NULL != m_msgbuf);
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
    char *m_msgbuf;
    int32 m_nMsglen;
};

#endif

