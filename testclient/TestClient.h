#ifndef __TESTCLIENT_H__
#define __TESTCLIENT_H__
#include "../include/Singleton.h"
#include "../network/include/Acceptor.h"
#include "../network/include/CommonList.h"
#include "../include/CServerBase.hpp"
#include "../network/include/Connector.h"
#include "../include/CIoThread.hpp"
#include "../include/PackageHandler.hpp"


class TestClient: public CServerBase, public base::Singleton<TestClient>
{
public:
    TestClient();
    virtual ~TestClient();
    void start();
    void updateSessionList();
    void removeDeadSession();
    void handleActiveSession();
    void update();
    inline int32 getServerID()
    {
        return m_ServerID;
    }

    inline int32 getIoEpollfd()
    {
        return m_epollfd;
    }

    inline eSERVERTYPE getServerType()
    {
        return m_svrType;
    }
    
    int32 addFdToEpoll(CSession* session)
    {
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLOUT | EPOLLONESHOT;
        ev.data.ptr = session;
        return epoll_ctl(m_epollfd, EPOLL_CTL_ADD, session->getSocket(), &ev);
    }

    inline Connector* getConnector()
    {
        return &m_connector;
    }
    uint64 getSysTimeMs();
private:
    Acceptor m_acceptor;
    Connector m_connector;
    CommonList<CSession> m_waitSessionList;
    CommonList<CSession> m_activeSessionList;
    //CommonList<Connector*> m_activeConnectorList;
    //CommonList<CSession*> 
    int m_ServerID;
    int32 m_epollfd;
    eSERVERTYPE m_svrType;
    uint32 m_nCycleTick;
    uint32 m_nNextTick;
    uint32 m_nInterval;
};

#endif // __TESTCLIENT_H__
