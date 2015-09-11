#include "../include/PackageHandler.hpp"

void handlePackage(CSession *pSession, PkgHeader *header, char *msgbuf, int32 msgsize)
{
    MsgHeader *msgHead = (MsgHeader *)msgbuf;
    int16 sysid = msgHead->sysId;
    int16 msgtype = msgHead->msgType;
    
    NetWorkObject *networkObj = pSession->getNetWorkObject();
    if (NULL == networkObj)
    {
        if (sysid == eRegister_Message && msgtype == C_S_SISSION_REGISTER)
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

void handlePackage(CSession *pSession, PkgHeader *header, MsgHeader *msgHead, char *msgbuf, int32 msgsize)
{   
    NetWorkObject *networkObj = pSession->getNetWorkObject();
    if (NULL == networkObj)
    {
        if (msgHead->sysId == eRegister_Message && msgHead->msgType == C_S_SISSION_REGISTER)
        {
            pSession->defaultMsgHandle(msgHead, msgbuf, msgsize);
            return;
        }

        printf("session does not bind a networkObject!!!!!disconnect\n");
        pSession->setStatus(waitdel);
        return;
    }
    assert(NULL != networkObj->getSession());
    networkObj->onRecv(header, msgHead, msgbuf, msgsize);
}

void decodeMsgHead(MsgHeader &msghead, char *buf, int32 buffsize)
{
    memcpy(&msghead, buf, sizeof(msghead));
}

void encodepkg(char *buf, PkgHeader *head, MsgHeader *msgHead, char *msgbuf, int32 msgbufsize)
{
    int32 headsize = sizeof(*head) + sizeof(*msgHead);
    memcpy(buf, (char *)head, sizeof(*head));
    memcpy(buf + sizeof(*head), (char*)msgHead, sizeof(*msgHead));
    //c_s_refecttest *ret = (c_s_refecttest*)msgbuf;
    //memcpy(buf + headsize, (char*)&(ret->strlen), sizeof(ret->strlen));
    //memcpy(buf + headsize + sizeof(ret->strlen), (char*)&ret + sizeof(ret->strlen), ret->strlen);
    memcpy(buf + headsize, (char*)msgbuf, msgbufsize);
    //return pkgsize;
}
