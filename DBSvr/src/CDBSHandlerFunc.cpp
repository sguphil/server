#include "../include/CDBSHandlerFunc.hpp"

int32 CDBSHandlerFunc::testfunc(CSession *session, char *pMsg, int32 msglen)
{
    test_package::testMsg recvmsg;
    
    recvmsg.ParseFromArray(pMsg, msglen);
    //LOGFMTI("protocol==sendtime:%d====server recv:%s\n", recvmsg.sendtime(), recvmsg.msg().c_str());
    //return session->getNetWorkObject()->processSend(1, 4, pMsg, msglen);
    return session->processSend< test_package::testMsg >(1, 4, recvmsg);
}

int32 CDBSHandlerFunc::checkuser(CSession *session, char *pMsg, int32 msglen)
{
    test_package::acc_2_dbs_checkuser recvmsg;
    
    recvmsg.ParseFromArray(pMsg, msglen);
    DBSvr* dbsvr = DBSvr::GetInstance();
    CDBInstFactory *dbf = dbsvr->getDBInstFactory();
    CSqlConn *sqlInst = dbf->allocate();
    if (NULL == sqlInst)
    {
        printf("error get sql instance!!!\n");
        assert(NULL != sqlInst);
    }
    std::string sql = "select a, b from tbl where a ='";
    sql.append(recvmsg.name());
    sql.append("' and b = '");
    sql.append(recvmsg.passwd());
    sql.append("'");
    CQuery query(sqlInst, sql, sql.length());
    printf("query sql is:%s\n", sql.c_str());
    CResult res;
    int32 retcode = 1;
    if (query.exequery(&res))
    {
        while (res.hasNext())
        {
            char *name = res.getString(0);
            char *passwd = res.getString(1);
            printf("db query result name:%s, passwd:%s\n", name, passwd);
            retcode = 0;
            break;
        }
    }
    //LOGFMTI("check printing log==sendtime:====server recv:%d\n", retcode);

    test_package::dbs_2_acc_checkuser sendmsg;
    sendmsg.set_retcode(retcode);
    printf("checkuser db retcode is:%d\n", retcode);
    int32 sendlen = sendmsg.ByteSize();
    char pmsg[sendlen];
    sendmsg.SerializeToArray(pmsg, sendlen);
    dbf->reuse(sqlInst);
    //return session->processSend((uint16)eServerMessage_DBServer, (uint16)DBS_ACCS_CHECKLOGINUSER_RET, pmsg, sendlen);
    return session->processSend<test_package::dbs_2_acc_checkuser>((uint16)eServerMessage_DBServer, (uint16)DBS_ACCS_CHECKLOGINUSER_RET, sendmsg);
}
