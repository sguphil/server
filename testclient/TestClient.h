#ifndef __TESTCLIENT_H__
#define __TESTCLIENT_H__
#include "../include/Singleton.h"
#include "../network/include/Acceptor.h"
#include "../network/include/CommonList.h"
#include "../include/EpollServer.hpp"
#include "../network/include/Connector.h"
#include "../include/CIoThread.hpp"
#include "../include/PackageHandler.hpp"
#include "../include/CSendThread.hpp"
#include "../protocol/testMsg.pb.h"

#include "./session/ClientSession.h"
#include "./session/AccsvrSession.h"
#include "./session/DBSession.h"
#include "./session/LogicSession.h"
#include "./session/GatewaySession.h"
#include "./session/StrictClient.h"


class TestClient: public EpollServer, public base::Singleton<TestClient>
{
public:
    TestClient();
    virtual ~TestClient();
    void start();
    void handleActiveSession();
    void update();

    void DestructNetWorkObj(NetWorkObject *netobj);
    NetWorkObject* CreateNetWorkObj(SESSION_TYPE type);

    void setConnectCount(int32 count)
    {
        m_ConnCount = count;
    }

private:
    int32 m_nSendTimes;
    bool m_bAlreadySend;
    int32 m_ConnCount;
};

#endif // __TESTCLIENT_H__
