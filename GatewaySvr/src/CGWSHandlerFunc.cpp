#include "../include/CGWSHandlerFunc.hpp"

int32 CGWSHandlerFunc::testfunc(CSession *session, char *pMsg, int32 msglen)
{
    test_package::testMsg recvmsg;
    recvmsg.ParseFromArray(pMsg, msglen);
    LOGFMTI("protocol==sendtime:%d====server recv:%s\n", recvmsg.sendtime(), recvmsg.msg().c_str());
    return session->processSend(eServerMessage_AccSvr, ACCS_DBS_TEST, recvmsg);
}

int32 CGWSHandlerFunc::checkuser(CSession *session, char *pMsg, int32 msglen)
{
    test_package::client_2_acc_checkuser recvmsg;    
    test_package::acc_2_dbs_checkuser sendmsg;
    recvmsg.ParseFromArray(pMsg, msglen);
    sendmsg.set_name(recvmsg.name());
    sendmsg.set_passwd(recvmsg.passwd());
    sendmsg.set_sessionid(session->getSessionId());
    int32 sendlen = sendmsg.ByteSize();
    char pmsg[sendlen];
    sendmsg.SerializeToArray(pmsg, sendlen);
    //LOGFMTI("protocol==sendtime:%d====server recv:%s\n", recvmsg.sendtime(), recvmsg.msg().c_str());
    GatewaySvr *gw = GatewaySvr::GetInstance();
    CSession *dbsession = gw->getBestServerSession(eDBServer);
    if (NULL == dbsession)
    {
        LOGFMTI("CGWSHandlerFunc::checkuser====find no dbsession\n");
        return -1;
    }
    return dbsession->processSend(eServerMessage_AccSvr, ACCS_DBS_CHECKLOGINUSER, sendmsg);
}

int32 CGWSHandlerFunc::dbcheckuserret(CSession *session, char *pMsg, int32 msglen)
{
    test_package::dbs_2_acc_checkuser recvmsg;
    test_package::acc_2_client_checkuser sendmsg;
    recvmsg.ParseFromArray(pMsg, msglen);
    uint32 clientSessionid = recvmsg.sessionid();
    //find clientSessionId and send the db-search result 
    
    //LOGFMTI("CGWSHandlerFunc::dbcheckuserret====recvmsg retcode:%d\n", recvmsg.retcode());
    return 0;
}
