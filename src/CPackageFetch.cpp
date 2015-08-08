#include "../include/CPackageFetch.hpp"

CpackageFetch::CpackageFetch():
m_msgbuf(NULL),
m_nMsglen(0)
{

}

CpackageFetch::~CpackageFetch()
{
    if (NULL != m_msgbuf)
    {
        delete[] m_msgbuf;
        m_msgbuf = NULL;
    }
    m_nMsglen = 0;
}

CpackageFetch::CpackageFetch(CpackageFetch& inst)
{
    m_msgbuf = new char[inst.m_nMsglen];
    m_pkgHeader = inst.m_pkgHeader;
    m_msgHeader = inst.m_msgHeader;
    memcpy(m_msgbuf, inst.getMsgbuf(), inst.m_nMsglen);
}

CpackageFetch& CpackageFetch::operator=(CpackageFetch& inst)
{
    m_msgbuf = new char[inst.m_nMsglen];
    m_pkgHeader = inst.m_pkgHeader;
    m_msgHeader = inst.m_msgHeader;
    memcpy(m_msgbuf, inst.getMsgbuf(), inst.m_nMsglen);

    return *this;
}
