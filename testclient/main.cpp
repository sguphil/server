#include <iostream>
#include "TestClient.h"
#include "../Test/include/TestAccess.h"
#include "../Factory/BaseFactory.h"
#include "../Logic/Player.h"
#include "../include/baseHeader.h"
#include "../network/include/Acceptor.h"
#include "../network/include/Connector.h"

#define ULIMITSVR 1

using namespace std;
extern void printItem(TestAccess *accessObj);

CBaseFactory<ClientSession> m_NetWorkObjectFactory;

int main()
{
    signal(SIGPIPE, SIG_IGN);
    TestClient* testClient = TestClient::GetInstance();
    cout << "Hello world! ServerID is:" << testClient->getServerID() << endl;
    
    m_NetWorkObjectFactory.init(1, 1);
    /*
    CBaseFactory<CPlayer> playerFactory;
    playerFactory.init(10, 10);
    CPlayer *player = playerFactory.allocate();
    cout << "actor type is:" << player->getActorType() << endl;
    */
    testClient->start();
    //Connector conn;
    //conn.start();

    testClient->update();

    return 0;
}
