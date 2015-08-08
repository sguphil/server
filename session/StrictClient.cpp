#include "ClientSession.h"

StrictClient::StrictClient():m_llpkgCount(0)
{
    //ctor
}

StrictClient::~StrictClient()
{
    //dtor
}

int32 StrictClient::testRefectSvr(char *msgbuf, int32 bufsize)
{
    MsgHeader *msgHead = (MsgHeader *)msgbuf;
    struct c_s_refecttest *pmsg = (struct c_s_refecttest *)(msgbuf+sizeof(*msgHead));
    //int32 pkglen = bufsize - sizeof(*msgHead);
    char buf[(pmsg->strlen)+1];
    memset(buf, 0x00, sizeof(buf));
    snprintf(buf, (pmsg->strlen), "%s", (char *)pmsg + sizeof(pmsg->strlen));
    printf("server recv msg:%s\n", buf); //(char *)pmsg + sizeof(pmsg->strlen));
    
    return 0; //processSend(msgHead->sysId, msgHead->msgType, (char *)msgbuf, pkglen);
}

int32 StrictClient::testRefectSvr(MsgHeader *msghead, char *msgbuf, int32 bufsize)
{
    struct c_s_refecttest *pmsg = (struct c_s_refecttest *)(msgbuf);
    int32 pkglen = bufsize;
    char buf[(pmsg->strlen)+1];
    memset(buf, 0x00, sizeof(buf));
    snprintf(buf, (pmsg->strlen), "%s", (char *)pmsg + sizeof(pmsg->strlen));
    printf("server recv msg:%s\n", buf); //(char *)pmsg + sizeof(pmsg->strlen));
    cout << "===========================================%d" << getSession()->getSocket() << "============" << m_llpkgCount++ << endl;
    
    return processSend(msghead->sysId, msghead->msgType, (char *)msgbuf, pkglen);
}

int32 StrictClient::onRecv(PkgHeader *header, char *msgbuf, int32 buffsize)
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

int32 StrictClient::onRecv(PkgHeader *header, MsgHeader *msghead, char *msgbuf, int32 buffsize)
{
    int16 sysid = msghead->sysId;
    int16 msgtype = msghead->msgType;
    if (sysid == 1 and msgtype == 2)
    {
        testRefectSvr(msghead, msgbuf, buffsize);
    }
    return 0;
}

int32 StrictClient::processSend(uint16 sysid, uint16 msgid, char *msgbuf, int32 bufsize)
{
    MsgHeader msgHead = {sysid, msgid};
    //c_s_refecttest *ret = (c_s_refecttest*)msgbuf;
    //int32 strlen = ret->strlen;
    uint16 pkglen = sizeof(msgHead) + bufsize;
    PkgHeader header = {pkglen, 0};
    uint16 allPkgLen = pkglen + sizeof(header);
    char buf[allPkgLen];
    encodepkg(buf, &header, &msgHead, msgbuf, bufsize);
    return getSession()->send(buf, (int32)allPkgLen);
}
