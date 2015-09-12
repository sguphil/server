#include "../include/CACCSHandlerFunc.hpp"

int32 CACCSHandlerFunc::testfunc(CSession *session, char *pMsg, int32 msglen)
{
    test_package::testMsg recvmsg;
    recvmsg.ParseFromArray(pMsg, msglen);
    LOGFMTI("protocol==sendtime:%d====server recv:%s\n", recvmsg.sendtime(), recvmsg.msg().c_str());
    return session->getNetWorkObject()->processSend(1, 4, pMsg, msglen);
}

int32 CACCSHandlerFunc::checkuser(CSession *session, char *pMsg, int32 msglen)
{
    test_package::client_2_acc_checkuser recvmsg;    
    test_package::acc_2_dbs_checkuser sendmsg;
    recvmsg.ParseFromArray(pMsg, msglen);
    sendmsg.set_name(recvmsg.name());
    sendmsg.set_passwd(recvmsg.passwd());
    int32 sendlen = sendmsg.ByteSize();
    char pmsg[sendlen];
    sendmsg.SerializeToArray(pmsg, sendlen);
    //LOGFMTI("protocol==sendtime:%d====server recv:%s\n", recvmsg.sendtime(), recvmsg.msg().c_str());
    CSession *dbsession = AccountSvr::GetInstance()->getBestServerSession(eDBServer);
    if (NULL == dbsession)
    {
        LOGFMTI("CACCSHandlerFunc::checkuser====find no dbsession\n");
        return -1;
    }
    return session->getNetWorkObject()->processSend(1, 4, pmsg, sendlen);
}
