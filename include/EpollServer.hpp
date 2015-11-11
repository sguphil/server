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

#define MAX_EPOLL_IN_FD 16 // SET MAX EPOLLIN FD TO 16 

class EpollServer
{
public:
    EpollServer();
    virtual ~EpollServer() { }
    virtual void start();
    virtual void updateSessionList();
    virtual void removeDeadSession();
    virtual void handleActiveSession();
    virtual void update();
    inline uint8 getServerID()
    {
        return m_ServerID;
    }

    inline int32 getIoEpollfd(int idx)
    {
        return m_epollfd[idx];
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
        //ev.events = EPOLLIN | EPOLLONESHOT; // default EPOLLIN event
        //if (m_nIoThreadNum == 1)
        //{
            ev.events = EPOLLIN;// | EPOLLONESHOT;
        //}
        ev.data.ptr = session;
        int32 epfd_idx = session->getSocket() % m_nIoThreadNum;
        return epoll_ctl(m_epollfd[epfd_idx], EPOLL_CTL_ADD, session->getSocket(), &ev);
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

    bool checkRecord(CSession *session)
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

    void recordServerSession(CSession *newSession)
    {
         if (!checkRecord(newSession))
         {
             m_ServerSessionMap.insert(std::make_pair<SESSION_TYPE, CSession *>(newSession->getType(), newSession));
         }
    }

    void delClusterSession(CSession *session)
    {
        //typedef std::multimap<SESSION_TYPE, CSession *>::iterator mapiter;
        std::multimap<SESSION_TYPE, CSession *>::iterator it;
        for (it = m_ServerSessionMap.begin(); it != m_ServerSessionMap.end(); )
        {
            if (it->second->getSessionId() == session->getSessionId())
            {
                m_ServerSessionMap.erase(it++);
                //put in connector errrolist, wait for reconnect... !!!!todo.....verify not connector session
                if (session->getIsFromSelf())
                {
                    m_connector.addToErrorList(session);
                }
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
        //typedef std::multimap<SESSION_TYPE, CSession *>::iterator mapiter;
        std::multimap<SESSION_TYPE, CSession *>::iterator it;
        int32 size = m_ServerSessionMap.size();
        if (size > 0)
        {
            for (it = m_ServerSessionMap.begin(); it != m_ServerSessionMap.end(); it++)
            {
                if (it->first == type)
                {
                    return it->second;
                }
            }
        }

        return NULL;
    }

    void recordClientSession(CSession *session)
    {
        m_ClientSessionMap.insert(std::make_pair<uint32, CSession *>(session->getSessionId(), session));
    }

    CSession* getClientSession(uint32 sessionid)
    {
        CSession *session = NULL;
        std::map<uint32, CSession *>::iterator it = m_ClientSessionMap.find(sessionid);
        if (it != m_ClientSessionMap.end())
        {
            session = it->second;
        }
        return session;
    }

    void rmClientSessionFromMap(uint32 sessionid)
    {
        std::map<uint32, CSession *>::iterator it = m_ClientSessionMap.begin();
        while (it != m_ClientSessionMap.end())
        {
            if (it->first == sessionid)
            {
                m_ClientSessionMap.erase(it++);
                break;
            }
            else
            {
                it++;
            }
        }
    }

protected:
    Acceptor m_acceptor;
    Connector m_connector;
    CommonList<CSession> m_waitSessionList;
    CommonList<CSession> m_activeSessionList;
    CommonList<CSession> m_rmSessionList; //waitdel sessions store in this list
    uint8 m_ServerID;
    int32 m_epollfd[MAX_EPOLL_IN_FD]; //for epoll in
    eSERVERTYPE m_svrType;
    uint32 m_nCycleTick;
    uint32 m_nNextTick;
    uint32 m_nInterval;
    int32 m_epollSendfd;
    int32 m_nIoThreadNum;
    int32 m_nHandleCount;
    int32 m_nStatisticTick;
    int32 m_nSessionSwapTick;
    uint32 m_nNextconnectTick;
    //CAccSvrConfig m_Config;
    std::multimap<SESSION_TYPE, CSession *> m_ServerSessionMap; //need to update session in loop
    std::map<uint32, CSession *> m_ClientSessionMap; //sessionid to CSession_ptr
};

#endif
