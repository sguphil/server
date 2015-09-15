#ifndef __CIOTHREAD_H__
#define __CIOTHREAD_H__

#include "../include/baseHeader.h"
#include "../Thread/BaseThread.h"
#include "../Thread/Mutex.h"
#include "../include/CServerBase.hpp"
#include "../network/include/Session.h"
#include "../include/acctTimeTool.hpp"

class CIoThread: public CBaseThread
{
public:
    CIoThread(CServerBase* server): m_ptrServer(server)
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
        CServerBase *svr = threadself->getServerPtr();
        struct epoll_event epEvent[10];
        cout << "CIoThread start threadRoutine" << endl;
        bool isRecvEvent = true;
        while (true)
        {
            //cout << "CIoThread infinity loop epollfd:"<< svr->getIoEpollfd() << endl;
            int32 evCount = epoll_wait(svr->getIoEpollfd(),epEvent, 10, -1);//100ms wait timeout infinite wait just one event to one sockfd
            if (evCount > 0)
            {
                for (int i = 0; i < evCount;i++)
                {
                    CSession *session =  (CSession *)epEvent[i].data.ptr;
                    int32 oplen = 0;
                    if (epEvent[i].events & EPOLLIN) // recv msg
                    {
                        oplen = session->recv();
                        isRecvEvent = true;
                        if (oplen > 0) // normal 
                        {
                            #if 0
                            if (oplen > 0 )
                            {
                                if ((acct_time::getCurTimeMs() - m_nNextTick)>1000) //1s
                                {
                                    m_nNextTick = acct_time::getCurTimeMs() + 1000;
                                    cout << "===========================================================" << m_llpkgCount++ << endl;
                                    m_llpkgCount = 0;
                                }
                            
                                m_llpkgCount++;
                                //cout << "CIoThread=======recvlen:" << oplen << endl;
                            }
                            #endif 

                            if (svr->getIoThreadNum() > 1)
                            {
                                session->modEpollEvent(svr->getIoEpollfd(), isRecvEvent);
                            }
                        }
                        else if (0 == oplen)
                        {
                            acct_time::sleepMs(200);
                        } 
                        else // socket error wait to free session
                        {
                            session->delEpollEvent(svr->getIoEpollfd());
                            session->setStatus(waitdel);
                        }
                    }

                }
            }
            else if (0 == evCount) //epoll timeout
            {
                acct_time::sleepMs(100);
                // handle timeout??
                //printf("CIoThread epoll timeout!!! epoll_wait return:%d\n", evCount);
            }
            else // ret < 0 error or interrupt
            {
                if (evCount == -1 && errno == EINTR)
                {
                    //acct_time::sleepMs(200);
                    printf("CIoThread epoll timeout!!! epoll_wait return:%d\n", evCount);
                    continue;
                }
                //acct_time::sleepMs(300);
                perror("epoll_wait error!!!");
                printf("CIoThread error!!! epoll_wait return:%d\n", evCount);
            }
        }

        return NULL;
    }

    inline CServerBase* getServerPtr()
    {
        return m_ptrServer;
    }
private:
    CServerBase* m_ptrServer;

    int32 m_nNextTick;
    int32 m_llpkgCount;
};
#endif
