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
    int16 strlen = pmsg->strlen;
    printf("server recv msg:%s\n", pmsg->buf);
    
    return processSend(msgHead->sysId, msgHead->msgType, pmsg, sizeof(*pmsg));
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

int32 ClientSession::processSend(int16 sysid, int16 msgid, char *msgbuf, int32 bufsize)
{
    MsgHeader msgHead = {sysid, msgid};
    int32 pkglen = sizeof(msgHead) + buffsize;
    PkgHeader header = {pkglen, 0};
    char buf[pkglen];
    encodepkg(buf, header, msgHead, msgbuf, bufsize);
    getSession()->send(buf, pkglen);
}
