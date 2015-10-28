#ifndef __CSENDTHREAD_H__
#define __CSENDTHREAD_H__

#include "../include/baseHeader.h"
#include "../Thread/BaseThread.h"
#include "../Thread/Mutex.h"
#include "../include/CServerBase.hpp"
#include "../network/include/Session.h"
#include "../include/acctTimeTool.hpp"

class CSendThread: public CBaseThread
{
public:
    CSendThread(CServerBase* server): m_ptrServer(server)
    {
        m_nNextTick = 0;
        m_llpkgCount = 0;
    }
    ~CSendThread()
    {
    }
    

    void *threadRoutine(void *args)
    {
        CSendThread *threadself = (CSendThread *)args;
        CServerBase *svr = threadself->getServerPtr();
        struct epoll_event epEvent[30];
        cout << "CSendThread start threadRoutine" << endl;
        //bool isRecvEvent = true;
        while (true)
        {
            //cout << "CSendThread infinity loop epollfd:"<< svr->getIoEpollfd() << endl;
            int32 evCount = epoll_wait(svr->getSendEpollfd(),epEvent, 30, -1);//100ms wait timeout infinite wait just one event to one sockfd
            if (evCount > 0)
            {
                for (int i = 0; i < evCount; i++)
                {
                    CSession *session =  (CSession *)epEvent[i].data.ptr;
                    int32 oplen = 0;
                    if (epEvent[i].events & EPOLLOUT) // send msg to client
                    {
                        oplen = session->sendToSocket();
                        //isRecvEvent = false;
                        
                        if (oplen > 0) // normal 
                        {
                            //session->modEpollEvent(svr->getSendEpollfd(), isRecvEvent);//single thread do not need epolloneshoot
                        }
                        else if (oplen == -1)// socket error wait to free session
                        {
                            session->delEpollEvent(svr->getSendEpollfd());
                            session->setStatus(waitdel);
                        }
                        else // EAGAIN
                        {
                            acct_time::sleepMs(100);
                        }
                    }
                    
                }
            }
            else if (0 == evCount) //epoll timeout
            {
                acct_time::sleepMs(100);
                // handle timeout??
                //printf("CSendThread epoll timeout!!! epoll_wait return:%d\n", evCount);
            }
            else // ret < 0 error or interrupt
            {
                if (evCount == -1 && errno == EINTR)
                {
                    acct_time::sleepMs(200);
                    printf("CSendThread epoll EINTR!!! epoll_wait return:%d\n", evCount);
                    continue;
                }
                acct_time::sleepMs(300);
                perror("epoll_wait error!!!");
                printf("CSendThread error!!! epoll_wait return:%d\n", evCount);
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
