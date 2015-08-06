#ifndef __NETWORKOBJECT_H__
#define __NETWORKOBJECT_H__
//#include "Session.h"
#include "../../include/baseHeader.h"
#include "../../include/packHeader.hpp"

class CSession;
//struct PkgHeader;

class NetWorkObject
{
public:
    NetWorkObject();
    virtual ~NetWorkObject();

    virtual int32 onRecv() { return 0; }
    virtual int32 onRecv(PkgHeader *header, char *msgbuf, int32 buffsize) { return 0; }
    virtual int32 processSend(int16 sysid, int16 msgid, char *msg, int32 msgsize) { return 0; }

    inline void setSesion(CSession* session)
    {
        m_pSession = session;
    }
    inline CSession* getSession()
    {
        return m_pSession;
    }
private:
    CSession *m_pSession;
};
#endif // __NETWORKOBJECT_H__
