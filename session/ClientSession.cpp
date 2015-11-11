#include "ClientSession.h"

extern CPackageMgr<accFuncStruct> *g_HandlerMgr;

ClientSession::ClientSession()
{
    m_llpkgCount = 0;
    m_nStatistic = 0;
    m_nNextTick = acct_time::getCurTimeMs();
    //ctor
}

ClientSession::~ClientSession()
{
    //dtor
}
#if 0
int32 ClientSession::testRefectSvr(char *msgbuf, int32 bufsize)
{
    MsgHeader *msgHead = (MsgHeader *)msgbuf;
    struct c_s_refecttest *pmsg = (struct c_s_refecttest *)(msgbuf+sizeof(*msgHead));
    int32 pkglen = bufsize - sizeof(*msgHead);
    char buf[(pmsg->strlen)+1];
    memset(buf, 0x00, sizeof(buf));
    snprintf(buf, (pmsg->strlen), "%s", (char *)pmsg + sizeof(pmsg->strlen));
    #if 0
    printf("server recv msg:%s\n", buf); //(char *)pmsg + sizeof(pmsg->strlen));
    #endif

    if ((acct_time::getCurTimeMs() - m_nNextTick)>1000) //1s
    {
        m_nNextTick = acct_time::getCurTimeMs() + 1000;
        cout << "=========socket:" << getSession()->getSocket() << "============" << m_llpkgCount++ << endl;
        m_llpkgCount = 0;
    }

    m_llpkgCount++;
    return processSend(msgHead->sysId, msgHead->msgType, (char *)msgbuf+sizeof(*msgHead), pkglen);
}
#endif 

int32 ClientSession::testRefectSvr(MsgHeader *msghead, char *msgbuf, int32 bufsize)
{
    struct c_s_refecttest *pmsg = (struct c_s_refecttest *)(msgbuf);
    int32 pkglen = bufsize;
    char buf[(pmsg->strlen)+1];
    memset(buf, 0x00, sizeof(buf));
    snprintf(buf, (pmsg->strlen), "%s", (char *)msgbuf + sizeof(pmsg->strlen));
    return processSend(msghead->sysId, msghead->msgType, (char *)msgbuf, pkglen);
}

int32 ClientSession::testProtobuf(MsgHeader *msghead, char *msgbuf, int32 bufsize)
{
    test_package::testMsg recvmsg;
    recvmsg.ParseFromArray(msgbuf, bufsize);
    return processSend(msghead->sysId, msghead->msgType, msgbuf, bufsize);
}

#if 0
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
#endif

int32 ClientSession::onRecv(PkgHeader *header, MsgHeader *msghead, char *msgbuf, int32 buffsize)
{
    int16 sysid = msghead->sysId;
    int16 msgtype = msghead->msgType;
    /*
    if (sysid == 1 and msgtype == 2)
    {
        testRefectSvr(msghead, msgbuf, buffsize);
    }
    else if (sysid == 1 and msgtype ==3 )
    {
        testProtobuf(msghead, msgbuf, buffsize);
    }
    else
    {
    */
        #if 1
        int32 delay = acct_time::getCurTimeMs() - m_nNextTick;
        if (delay > 0) //1s
        {
            cout << "curms:" << acct_time::getCurTimeMs() << "  nextick:" << m_nNextTick << " delay:" << delay << " socket:" << getSession()->getSocket() << "============" << m_llpkgCount++ << endl;
            m_nNextTick = acct_time::getCurTimeMs() + 1000;
            m_llpkgCount = 0;
        }
        
        m_llpkgCount++;
        #endif
        
        int32 key = PKGFUNCBASE::makeKey(sysid, msgtype);
        accFuncStruct *funcStruct = g_HandlerMgr->findFuncStruct(key);
        if (NULL == funcStruct)
        {
            printf("find no func by sysid:%d and msgtype:%d\n", sysid, msgtype);
        }
        else
        {
            funcStruct->handler(this->getSession(), msgbuf, buffsize);
        }
        
    //}
    return 0;
}

int32 ClientSession::processSend(uint16 sysid, uint16 msgid, char *msg, int32 msgsize)
{
    //MsgHeader msgHead = {sysid, msgid};
    //c_s_refecttest *ret = (c_s_refecttest*)msgbuf;
    //int32 strlen = ret->strlen;
    //uint16 msglen = sizeof(msgHead) + msgsize;
    //uint16 pkglen = sizeof(msgHead) + msgsize + sizeof(PkgHeader);
    //PkgHeader header = {msglen, 0};
    //char buf[pkglen];
    //encodepkg(buf, &header, &msgHead, msg, msgsize);
    return getSession()->processSend(sysid, msgid, (char *)&msg, msgsize);//send(buf, (int32)pkglen);
}

