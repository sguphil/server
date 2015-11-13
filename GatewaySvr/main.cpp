#include <iostream>
#include "GatewaySvr.h"
#include "../Test/include/TestAccess.h"
#include "../Factory/BaseFactory.h"
#include "../include/baseHeader.h"
#include "../network/include/Acceptor.h"
#include "../network/include/Connector.h"
#include "../protocol/testMsg.pb.h"
#include "./include/GWSessionHandler.hpp"
#include "../include/ServerInclude.hpp"
#include "../include/log4z.h"

#define ULIMITSVR 1

class CSession;

using namespace std;
extern void printItem(TestAccess *accessObj);

int main()
{
    using namespace zsummer::log4z;
    ILog4zManager::getRef().start();
    ILog4zManager::getRef().setLoggerLevel(LOG4Z_MAIN_LOGGER_ID,LOG_LEVEL_TRACE);
    signal(SIGPIPE, SIG_IGN);
    g_HandlerMgr = new CGwHandlerMgr;
    g_HandlerMgr->addAllHandle();

    GatewaySvr *GatewaySvr = GatewaySvr::GetInstance();
    LOGI("Hello world! ServerID is:" << GatewaySvr->getServerID());
    
    test_package::testMsg tmsg;
    tmsg.set_sendtime(123);
    tmsg.set_msg("protobuf hello world!!");
    
    int32 buflen = tmsg.ByteSize();
    
    char buf[buflen];
    tmsg.SerializeToArray(buf, buflen);

    test_package::testMsg after;
    after.ParseFromArray(buf, buflen);

    
    LOGI("after:" << after.sendtime() << "  msg:" << after.msg());
    
    //CBaseFactory<CPlayer> playerFactory;
    //playerFactory.init(10, 10);
    //CPlayer *player = playerFactory.allocate();
    //cout << "actor type is:" << player->getActorType() << endl;

    //Acceptor acc(eClient);
    //acc.init();
    //acc.startListen("127.0.0.1", 9997);
    //acc.start();
    GatewaySvr->start(); //listen start
    //Connector conn;
    //conn.start();
    
    GatewaySvr->update();

    while (false)
    {
        LOGI("flash main thread");
        //conn.connect("127.0.0.1", 9997, eClient);
        sleep(1);
    }
    delete g_HandlerMgr;
    return 0;
}
