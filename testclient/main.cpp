#include <iostream>
#include "TestClient.h"
#include "../Test/include/TestAccess.h"
#include "../Factory/BaseFactory.h"
#include "../Logic/Player.h"
#include "../include/baseHeader.h"
#include "../network/include/Acceptor.h"
#include "../network/include/Connector.h"
#include "../AccSvr/include/SessionHandler.hpp"
#include "../include/ServerInclude.hpp"

#define ULIMITSVR 1

using namespace std;
extern void printItem(TestAccess *accessObj);

int main(int argc, char **argv)
{
    signal(SIGPIPE, SIG_IGN);
    TestClient* testClient = TestClient::GetInstance();
    cout << "Hello world! ServerID is:" << testClient->getServerID() << endl;
    
    g_ClientNetWorkObjectFactory.init(1, 1);
    /*
    CBaseFactory<CPlayer> playerFactory;
    playerFactory.init(10, 10);
    CPlayer *player = playerFactory.allocate();
    cout << "actor type is:" << player->getActorType() << endl;
    */
    int32 count = 1;
    if (argc == 2)
    {
        count = atoi(argv[1]);
    }
    testClient->setConnectCount(count);
    testClient->start();
    //Connector conn;
    //conn.start();

    testClient->update();

    return 0;
}
