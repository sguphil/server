#include <iostream>
#include "AccountSvr.h"
#include "../Test/include/TestAccess.h"
#include "../Factory/BaseFactory.h"
#include "../include/baseHeader.h"
#include "../network/include/Acceptor.h"
#include "../network/include/Connector.h"
#include "../protocol/testMsg.pb.h"
#include "./include/ACCSessionHandler.hpp"
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

    g_HandlerMgr = new CAccHandlerMgr;
    g_HandlerMgr->addAllHandle();

    AccountSvr *accountSvr = AccountSvr::GetInstance();
    LOGI("Server construct! ServerID is:" << accountSvr->getServerID());
    
    accountSvr->start(); //listen start
    accountSvr->update();

    delete g_HandlerMgr;
    return 0;
}
