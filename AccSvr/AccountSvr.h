#ifndef ACCOUNTSVR_H
#define ACCOUNTSVR_H
#include "../include/Singleton.h"
#include "../network/include/Acceptor.h"
#include "../network/include/CommonList.h"
#include "../network/include/Session.h"
#include "../include/CServerBase.hpp"
#include "../network/include/Connector.h"

class AccountSvr: public CServerBase, public base::Singleton<AccountSvr>
{
public:
    AccountSvr();
    virtual ~AccountSvr();
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
    
    int32 addFdToEpoll(int32 fd)
    {
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLOUT | EPOLLONESHOT;
        return epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &ev);
    }

    inline Connector* getConnector()
    {
        return &m_connector;
    }
    
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
};

#endif // ACCOUNTSVR_H
