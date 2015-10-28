#ifndef __DBSVR_H__
#define __DBSVR_H__
#include "../include/baseHeader.h"
#include "../include/Singleton.h"
#include "../network/include/Acceptor.h"
#include "../network/include/CommonList.h"
#include "../network/include/Session.h"
#include "../include/CServerBase.hpp"
#include "../network/include/Connector.h"
#include "../include/CIoThread.hpp"
#include "../include/CSendThread.hpp"
#include "../include/acctTimeTool.hpp"
#include "DBSvrConfig.hpp"
#include "../database/CDBInstFactory.hpp"

extern int32 MAXPKGLEN;
extern int32 SESSIONBUFLEN;

class DBSvr: public CServerBase, public base::Singleton<DBSvr>
{
public:
    DBSvr();
    virtual ~DBSvr();
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
            ev.events = EPOLLIN;// | EPOLLONESHOT;
        }
        ev.data.ptr = session;
        return epoll_ctl(m_epollfd, EPOLL_CTL_ADD, session->getSocket(), &ev);
    }

    int32 addFdToSendEpoll(CSession* session)
    {
        struct epoll_event ev;
        ev.events = EPOLLOUT | EPOLLONESHOT; // default EPOLLOUT event , single thread without EPOLLONESHOT
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

    inline int32 getSessionSwapTick()
    {
        return m_nSessionSwapTick;
    }

    inline void setSessionSwapTick(int32 tick)
    {
        m_nSessionSwapTick = tick;
    }

    inline bool checkRecord(CSession *session)
    {
        typedef std::multimap<SESSION_TYPE, CSession *>::iterator mapiter;
        //typedef std::pair<mapiter, mapiter> rangeBeginEnd;
        //rangeBeginEnd range = m_ServerSessionMap.equal_range(session->getType());
        for (mapiter it = m_ServerSessionMap.begin(); it != m_ServerSessionMap.end(); it++)
        {
            if (it->second == session)
            {
                return true;
            }
        }
        return false;
    }

    inline void delClusterSession(CSession *session)
    {
        typedef std::multimap<SESSION_TYPE, CSession *>::iterator mapiter;
        //typedef std::pair<mapiter, mapiter> rangeBeginEnd;
        //rangeBeginEnd range = m_ServerSessionMap.equal_range(session->getType());
        for (mapiter it = m_ServerSessionMap.begin(); it != m_ServerSessionMap.end(); )
        {
            if (it->second->getSessionId() == session->getSessionId())
            {
                m_ServerSessionMap.erase(it++);
                //put in connector errrolist, wait for reconnect...
                m_connector.addToErrorList(session);
                break;
            }
            else
            {
                 it++;
            }
        }
    }

    CDBInstFactory* getDBInstFactory()
    {
        return &m_dbInstFactory;
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
    int32 m_nHandleCount;
    int32 m_nStatisticTick;
    int32 m_nSessionSwapTick;
    std::multimap<SESSION_TYPE, CSession *> m_ServerSessionMap; //need to update session in loop
    CDBSvrConfig m_Config;
    CDBInstFactory m_dbInstFactory;
};


#endif 
