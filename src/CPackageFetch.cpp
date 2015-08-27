#include "../include/CPackageFetch.hpp"

CpackageFetch::CpackageFetch():
m_nMsglen(0)
{
    m_msgbuf = new char[MAXPKGLEN];
    assert(m_msgbuf != NULL);
}

CpackageFetch::~CpackageFetch()
{
    delete[] m_msgbuf;
    m_nMsglen = 0;
}

CpackageFetch::CpackageFetch(CpackageFetch& inst)
{
    m_pkgHeader = inst.m_pkgHeader;
    m_msgHeader = inst.m_msgHeader;
    m_nMsglen = inst.m_nMsglen;
    memcpy(m_msgbuf, inst.getMsgbuf(), inst.m_nMsglen);
}

CpackageFetch& CpackageFetch::operator=(CpackageFetch& inst)
{
    m_pkgHeader = inst.m_pkgHeader;
    m_msgHeader = inst.m_msgHeader;
    m_nMsglen = inst.m_nMsglen;
    memcpy(m_msgbuf, inst.getMsgbuf(), inst.m_nMsglen);

    return *this;
}
