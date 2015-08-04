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

int main()
{
    TestClient* testClient = TestClient::GetInstance();
    cout << "Hello world! ServerID is:" << testClient->getServerID() << endl;


    CBaseFactory<CPlayer> playerFactory;
    playerFactory.init(10, 10);
    CPlayer *player = playerFactory.allocate();
    cout << "actor type is:" << player->getActorType() << endl;

    testClient->start();
    Connector conn;
    conn.start();
    int i = 0;
    while (i>0)
    {
        cout << "flash main thread" << endl;
        conn.connect("127.0.0.1", 9997, eStrictClient);
        sleep(1);
        i--;
    }

    testClient->update();

    return 0;
}
