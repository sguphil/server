#include <iostream>
#include "DBSvr.h"
#include "../Test/include/TestAccess.h"
#include "../Factory/BaseFactory.h"
#include "../Logic/Player.h"
#include "../include/baseHeader.h"
#include "../network/include/Acceptor.h"
#include "../network/include/Connector.h"
#include "../session/ClientSession.h"
#include "../protocol/testMsg.pb.h"
#include "./include/DBSessionHandler.hpp"
#include "../include/ServerInclude.hpp"
#include "../include/log4z.h"

#define ULIMITSVR 1

using namespace std;
extern void printItem(TestAccess *accessObj);

int main()
{
    using namespace zsummer::log4z;
    ILog4zManager::getRef().start();
    ILog4zManager::getRef().setLoggerLevel(LOG4Z_MAIN_LOGGER_ID,LOG_LEVEL_TRACE);
    g_HandlerMgr = new CDBHandlerMgr;
    signal(SIGPIPE, SIG_IGN);
    DBSvr* dbsvr = DBSvr::GetInstance();
    LOGI("Hello world! ServerID is:" << dbsvr->getServerID());
    g_ClientNetWorkObjectFactory.init(10, 10);
    g_HandlerMgr->addAllHandle();

    test_package::testMsg tmsg;
    tmsg.set_sendtime(123);
    tmsg.set_msg("protobuf hello world!!");
    int32 buflen = tmsg.ByteSize();
    char buf[buflen];
    tmsg.SerializeToArray(buf, buflen);
    test_package::testMsg after;
    after.ParseFromArray(buf, buflen);
    LOGI("after:" << after.sendtime() << "  msg:" << after.msg());
    
    dbsvr->start(); //listen start
    dbsvr->update();
    while (false)
    {
        LOGI("flash main thread");
        //conn.connect("127.0.0.1", 9997, eClient);
        sleep(1);
    }
    delete g_HandlerMgr;
    return 0;
}
