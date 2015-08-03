#ifndef __PACKAGEHANDLER_H__
#define __PACKAGEHANDLER_H__

#include "packHeader.hpp"
#include "../network/include/Session.h"

enum sysid
{
    SYS_SESSION_REGISTER = 1,

};

enum msgid
{
    C_S_SISSION_REGISTER = 1,
};

void handlePackage(CSession *pSession, PkgHeader *header, char *msgbuf, int32 msgsize)
{
    MsgHeader *msgHead = (MsgHeader *)msgbuf;
    int16 sysid = msgHead->sysId;
    int16 msgtype = msgHead->msgType;
    
    NetWorkObject *networkObj = pSession->getNetWorkObject();
    if (NULL == networkObj)
    {
        if (sysid == SYS_SESSION_REGISTER && msgtype == C_S_SISSION_REGISTER)
        {
            pSession->defaultMsgHandle(sysid, msgtype, msgbuf, msgsize);
            return;
        }

        printf("session does not bind a networkObject!!!!!disconnect\n");
        pSession->setStatus(waitdel);
        return;
    }
    networkObj->onRecv(header, msgbuf, msgsize);
}

void decodeMsgHead(MsgHeader &msghead, char *buf, int32 buffsize)
{
    memcpy(&msghead, buf, sizeof(msghead));
}

void encodepkg(char *buf, PkgHeader *head, MsgHeader *msgHead, char *msgbuf, int32 msgbufsize)
{
    int32 headsize = sizeof(*head) + sizeof(*msgHead);
    memcpy(buf, (char *)head, sizeof(*head));
    memcpy(buf + sizeof(*head), msgHead, sizeof(*msgHead));
    memcpy(buf + headsize, msgbuf, msgbufsize);
    //return pkgsize;
}
#endif
