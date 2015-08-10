#ifndef ACCOUNTSVR_H
#define ACCOUNTSVR_H
#include "../include/Singleton.h"
#include "../network/include/Acceptor.h"
#include "../network/include/CommonList.h"
//#include "../network/include/Session.h"
#include "../include/CServerBase.hpp"
#include "../network/include/Connector.h"
#include "../include/CIoThread.hpp"
#include "../include/CSendThread.hpp"
#include "../include/acctTimeTool.hpp"

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
    
    inline int32 getSendEpollfd()
    {
        return m_epollSendfd;
    }

    inline eSERVERTYPE getServerType()
    {
        return m_svrType;
    }
    
    int32 addFdToRecvEpoll(CSession* session)
    {
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLONESHOT; // default EPOLLIN event
        if (m_nIoThreadNum == 1)
        {
            ev.events = EPOLLIN;
        }
        ev.data.ptr = session;
        return epoll_ctl(m_epollfd, EPOLL_CTL_ADD, session->getSocket(), &ev);
    }

    int32 addFdToSendEpoll(CSession* session)
    {
        struct epoll_event ev;
        ev.events = EPOLLOUT;// | EPOLLONESHOT; // default EPOLLOUT event , signal thread without EPOLLONESHOT
        ev.data.ptr = session;
        return epoll_ctl(m_epollSendfd, EPOLL_CTL_ADD, session->getSocket(), &ev);
    }

    inline Connector* getConnector()
    {
        return &m_connector;
    }

    inline int32 getIoThreadNum()
    {
        return m_nIoThreadNum;
    }

    inline void setIoThreadNum(int32 threadNum)
    {
        m_nIoThreadNum = threadNum;
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
    int32 m_nCycleTick;
    int32 m_nNextTick;
    int32 m_nInterval;
    int32 m_epollSendfd;
    int32 m_nIoThreadNum;
};

#endif // ACCOUNTSVR_H
