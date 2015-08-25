#include <iostream>
#include "AccountSvr.h"
#include "../Test/include/TestAccess.h"
#include "../Factory/BaseFactory.h"
#include "../Logic/Player.h"
#include "../include/baseHeader.h"
#include "../network/include/Acceptor.h"
#include "../network/include/Connector.h"
#include "../session/ClientSession.h"
#include "../protocol/testMsg.pb.h"
#include "./include/SessionHandler.hpp"
#include "../include/ServerInclude.hpp"

#define ULIMITSVR 1

using namespace std;
extern void printItem(TestAccess *accessObj);

int main()
{
    signal(SIGPIPE, SIG_IGN);
    AccountSvr* accountSvr = AccountSvr::GetInstance();
    cout << "Hello world! ServerID is:" << accountSvr->getServerID() << endl;
    g_ClientNetWorkObjectFactory.init(10000, 50);

    g_AccHandlerMgr.addAllHandle();
    //TestAccess acsObj;
    //printItem(&acsObj);
    //cout << acsObj << endl;
    test_package::testMsg tmsg;
    tmsg.set_sendtime(123);
    tmsg.set_msg("protobuf hello world!!");
    
    int32 buflen = tmsg.ByteSize();
    
    char buf[buflen];
    tmsg.SerializeToArray(buf, buflen);

    test_package::testMsg after;
    after.ParseFromArray(buf, buflen);
    cout << "after:" << after.sendtime() << "  msg:" << after.msg() << endl;
    
    //CBaseFactory<CPlayer> playerFactory;
    //playerFactory.init(10, 10);
    //CPlayer *player = playerFactory.allocate();
    //cout << "actor type is:" << player->getActorType() << endl;

    //Acceptor acc(eClient);
    //acc.init();
    //acc.startListen("127.0.0.1", 9997);
    //acc.start();
    accountSvr->start(); //listen start
    //Connector conn;
    //conn.start();
    
    accountSvr->update();
    while (false)
    {
        cout << "flash main thread" << endl;
        //conn.connect("127.0.0.1", 9997, eClient);
        sleep(1);
    }
    return 0;
}
