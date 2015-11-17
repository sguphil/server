#include <iostream>
#include "TestClient.h"
#include "../Test/include/TestAccess.h"
#include "../Factory/BaseFactory.h"
#include "../include/baseHeader.h"
#include "../network/include/Acceptor.h"
#include "../network/include/Connector.h"
#include "../include/ServerInclude.hpp"
#include "./include/TCSessionHandler.hpp"

using namespace std;
extern void printItem(TestAccess *accessObj);

int main(int argc, char **argv)
{
    //multi thread ignore SIGPIPE
    sigset_t bset, oset;
    sigemptyset(&bset);
    sigaddset(&bset, SIGPIPE);
    if (pthread_sigmask(SIG_BLOCK, &bset, &oset) != 0) 
    {
         printf("set thread signal mask fail!\n");
    }

    TestClient* testClient = TestClient::GetInstance();
    cout << "Server construct! ServerID is:" << testClient->getServerID() << endl;
    
    g_HandlerMgr = new CTcHandlerMgr;
    g_HandlerMgr->addAllHandle();

    int32 count = 1;
    if (argc == 2)
    {
        count = atoi(argv[1]);
    }
    testClient->setConnectCount(count);
    testClient->start();
    testClient->update();

    return 0;
}
