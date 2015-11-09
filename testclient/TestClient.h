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

class TestClient: public EpollServer, public base::Singleton<TestClient>
{
public:
    TestClient();
    virtual ~TestClient();
    void start();
    //void updateSessionList();
    //void removeDeadSession();
    void handleActiveSession();
    void update();

    void setConnectCount(int32 count)
    {
        m_ConnCount = count;
    }

private:
    //Acceptor m_acceptor;
    //Connector m_connector;
    //CommonList<CSession> m_waitSessionList;
    //CommonList<CSession> m_activeSessionList;
    //CommonList<Connector*> m_activeConnectorList;
    //CommonList<CSession*> 
    //int m_ServerID;
    //int32 m_epollfd;
    //eSERVERTYPE m_svrType;
    int32 m_nCycleTick;
    int32 m_nNextTick;
    int32 m_nInterval;
    int32 m_nSendTimes;
    int32 m_epollSendfd;
    bool m_bAlreadySend;
    int32 m_nIoThreadNum;
    int32 m_ConnCount;
};

#endif // __TESTCLIENT_H__
