#ifndef __CIOTHREAD_H__
#define __CIOTHREAD_H__

#include "../include/baseHeader.h"
#include "../Thread/BaseThread.h"
#include "../Thread/Mutex.h"
#include "../include/EpollServer.hpp"
#include "../network/include/Session.h"
#include "../include/acctTimeTool.hpp"

class CIoThread: public CBaseThread
{
public:
    CIoThread(EpollServer* server): m_ptrServer(server)
    {
        m_llpkgCount = 0;
        m_nNextTick = 0;
    }
    ~CIoThread()
    {
    }
    

    void *threadRoutine(void *args)
    {
        CIoThread *threadself = (CIoThread *)args;
        EpollServer *svr = threadself->getServerPtr();
        struct epoll_event epEvent[10];
        cout << "CIoThread start threadRoutine" << endl;
        bool isRecvEvent = false;
        while (true)
        {
            int32 evCount = epoll_wait(svr->getIoEpollfd(m_nStartID),epEvent, 10, -1);//100ms wait timeout infinite wait just one event to one sockfd
            if (evCount > 0)
            {
                for (int i = 0; i < evCount;i++)
                {
                    isRecvEvent = false;
                    CSession *session =  (CSession *)epEvent[i].data.ptr;
                    int32 oplen = 0;
                    if (epEvent[i].events & EPOLLIN) // recv msg
                    {
                        oplen = session->onRecv();
                        
                        if (oplen >= 0) // normal 
                        {
                            isRecvEvent = true;
                        }
                        else // socket error wait to free session
                        {
                            isRecvEvent = false;
                            session->delEpollEvent(svr->getIoEpollfd(m_nStartID));
                            session->setStatus(waitdel);
                        }
                    }
                    
                    #if 1

                    if (epEvent[i].events & EPOLLOUT) // send msg
                    {
                        oplen = session->sendToSocket();
                        if (0 == oplen)
                        {
                            isRecvEvent = true;
                        }
                        else if (1 == oplen) //still have data
                        {
                            isRecvEvent = false;
                        }
                        else if (oplen < 0)
                        {
                            isRecvEvent = false;
                            session->delEpollEvent(svr->getIoEpollfd(m_nStartID));
                            session->setStatus(waitdel);
                        }
                    }
                    #endif
                    /* //now there is single thread wait for each epollfd
                    if (svr->getIoThreadNum() > 1 &&  oplen >= 0)
                    {
                        session->modEpollEvent(svr->getIoEpollfd(), isRecvEvent);
                    }
                    */
                }
            }
            else if (0 == evCount) //epoll timeout
            {
                acct_time::sleepMs(100);
                // handle timeout??
            }
            else // ret < 0 error or interrupt
            {
                if (evCount == -1 && errno == EINTR)
                {
                    printf("CIoThread epoll timeout!!! epoll_wait return:%d\n", evCount);
                    continue;
                }
                perror("epoll_wait error!!!");
                printf("CIoThread error!!! epoll_wait return:%d\n", evCount);
            }
        }

        return NULL;
    }

    inline EpollServer* getServerPtr()
    {
        return m_ptrServer;
    }

    inline int32 getStartID()
    {
        return m_nStartID;
    }

    inline void setStartID(int32 id)
    {
        m_nStartID = id;
    }
private:
    EpollServer* m_ptrServer;

    int32 m_nNextTick;
    int32 m_llpkgCount;
    int32 m_nStartID;
};
#endif
