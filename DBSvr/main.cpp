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
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/reader.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/stringbuffer.h"
#include "../include/CFileUtils.hpp"

#define ULIMITSVR 1

using namespace std;
extern void printItem(TestAccess *accessObj);

int main()
{
    using namespace zsummer::log4z;
    ILog4zManager::getRef().start();
    ILog4zManager::getRef().setLoggerLevel(LOG4Z_MAIN_LOGGER_ID,LOG_LEVEL_TRACE);
    
    signal(SIGPIPE, SIG_IGN);
    DBSvr* dbsvr = DBSvr::GetInstance();
    LOGI("Hello world! ServerID is:" << dbsvr->getServerID());
    g_ClientNetWorkObjectFactory.init(10, 10);
    g_HandlerMgr = new CDBHandlerMgr;
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
    
    //==========test
    while (true)
    {
        LOGI("flash main thread");
        //conn.connect("127.0.0.1", 9997, eClient);
        sleep(1);
        //=====test= rapidjson=======
        const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
        //Document d;
        //d.Parse(json);
        //Value s = d["stars"];
        std::string filecontent = CFileUtils::getInstance()->getFileString("./config/DBSvr.xml");
        cout << "myf=======test" << filecontent;
        break;
    }
    //==========test

    dbsvr->start(); //listen start
    dbsvr->update();
    delete g_HandlerMgr;
    return 0;
}


