#include "../include/CPackageFetch.hpp"

CpackageFetch::CpackageFetch():
m_nMsglen(0)
{

}

CpackageFetch::~CpackageFetch()
{
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
