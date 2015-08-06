#include "ClientSession.h"

ClientSession::ClientSession()
{
    m_llpkgCount = 0;
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
    char buf[(pmsg->strlen)+1];
    memset(buf, 0x00, sizeof(buf));
    snprintf(buf, (pmsg->strlen), "%s", (char *)pmsg + sizeof(pmsg->strlen));
    printf("server recv msg:%s\n", buf); //(char *)pmsg + sizeof(pmsg->strlen));
    cout << "=======================================================" << m_llpkgCount++ << endl;
    
    return processSend(msgHead->sysId, msgHead->msgType, (char *)msgbuf+sizeof(*msgHead), pkglen);
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

int32 ClientSession::processSend(uint16 sysid, uint16 msgid, char *msg, int32 msgsize)
{
    MsgHeader msgHead = {sysid, msgid};
    //c_s_refecttest *ret = (c_s_refecttest*)msgbuf;
    //int32 strlen = ret->strlen;
    uint16 msglen = sizeof(msgHead) + msgsize;
    uint16 pkglen = sizeof(msgHead) + msgsize + sizeof(PkgHeader);
    PkgHeader header = {msglen, 0};
    char buf[pkglen];
    encodepkg(buf, &header, &msgHead, msg, msgsize);
    return getSession()->send(buf, (int32)pkglen);
}
