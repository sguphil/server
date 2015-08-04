#include "ClientSession.h"

ClientSession::ClientSession()
{
    //ctor
}

ClientSession::~ClientSession()
{
    //dtor
}

int32 ClientSession::testRefectSvr(char *msgbuf, int32 bufsize)
{
    MsgHeader *msgHead = (MsgHeader *)msgbuf;
    struct c_s_refecttest *pmsg = (struct c_s_refecttest *)(msgbuf+sizeof(*msgHead));
    int32 pkglen = bufsize - sizeof(*msgHead);
    printf("server recv msg:%s\n", (char*)pmsg+(sizeof(pmsg->strlen)));
    
    return processSend(msgHead->sysId, msgHead->msgType, (char*)pmsg, pkglen);
}

int32 ClientSession::onRecv(PkgHeader *header, char *msgbuf, int32 buffsize)
{
    MsgHeader *msghead = (MsgHeader *)msgbuf;
    int16 sysid = msghead->sysId;
    int16 msgtype = msghead->msgType;
    if (sysid == 1 and msgtype == 2)
    {
        testRefectSvr(msgbuf, buffsize);
    }
    return 0;
}

int32 ClientSession::processSend(uint16 sysid, uint16 msgid, char *msgbuf, int32 bufsize)
{
    MsgHeader msgHead = {sysid, msgid};
    uint16 pkglen = sizeof(msgHead) + bufsize;
    PkgHeader header = {pkglen, 0};
    char buf[pkglen];
    encodepkg(buf, &header, &msgHead, msgbuf, bufsize);
    return getSession()->send(buf, (int32)pkglen);
}
