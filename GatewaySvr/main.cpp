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

class CSession;

using namespace std;
extern void printItem(TestAccess *accessObj);

int main()
{
    using namespace zsummer::log4z;
    ILog4zManager::getRef().start();
    ILog4zManager::getRef().setLoggerLevel(LOG4Z_MAIN_LOGGER_ID,LOG_LEVEL_TRACE);

    //multi thread ignore SIGPIPE
    sigset_t bset, oset;
    sigemptyset(&bset);
    sigaddset(&bset, SIGPIPE);
    if (pthread_sigmask(SIG_BLOCK, &bset, &oset) != 0) 
    {
         printf("set thread signal mask fail!\n");
    }

    g_HandlerMgr = new CGwHandlerMgr;
    g_HandlerMgr->addAllHandle();

    GatewaySvr *GatewaySvr = GatewaySvr::GetInstance();
    LOGI("Server construct! ServerID is:" << GatewaySvr->getServerID());
        
    GatewaySvr->start(); //listen start
    GatewaySvr->update();

    delete g_HandlerMgr;
    return 0;
}
