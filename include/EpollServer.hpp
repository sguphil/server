#ifndef  __EpollServer_H__
#define __EpollServer_H__

#include "../include/baseHeader.h"
#include "../network/include/Acceptor.h"
#include "../network/include/CommonList.h"
#include "../network/include/Session.h"
#include "../network/include/Connector.h"
#include "../include/CSendThread.hpp"
#include "../include/acctTimeTool.hpp"
#include "../common/SIDGenerator.hpp"
#include "../session/ClientSession.h"
#include "../session/AccsvrSession.h"
#include "../session/DBSession.h"
#include "../session/LogicSession.h"
#include "../session/StrictClient.h"

class EpollServer
{
public:
    virtual ~EpollServer() { }
    virtual void start();
    virtual void updateSessionList();
    virtual void removeDeadSession();
    virtual void handleActiveSession();
    virtual void update();
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

    CSession* getBestServerSession(SESSION_TYPE type)
    {
        typedef std::multimap<SESSION_TYPE, CSession *>::iterator mapiter;
        int32 size = m_ServerSessionMap.size();
        if (size > 0)
        {
            for (mapiter it = m_ServerSessionMap.begin(); it != m_ServerSessionMap.end(); it++)
            {
                if (it->first == type)
                {
                    return it->second;
                }
            }
        }

        return NULL;
    }

protected:
    Acceptor m_acceptor;
    Connector m_connector;
    CommonList<CSession> m_waitSessionList;
    CommonList<CSession> m_activeSessionList;
    int m_ServerID;
    int32 m_epollfd; //for epoll in
    eSERVERTYPE m_svrType;
    int32 m_nCycleTick;
    int32 m_nNextTick;
    int32 m_nInterval;
    int32 m_epollSendfd;
    int32 m_nIoThreadNum;
    int32 m_nHandleCount;
    int32 m_nStatisticTick;
    int32 m_nSessionSwapTick;
    //CAccSvrConfig m_Config;
    std::multimap<SESSION_TYPE, CSession *> m_ServerSessionMap; //need to update session in loop
};

#endif
