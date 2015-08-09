#ifndef __CSENDTHREAD_H__
#define __CSENDTHREAD_H__

#include "../include/baseHeader.h"
#include "../Thread/BaseThread.h"
#include "../Thread/Mutex.h"
#include "../include/CServerBase.hpp"
#include "../network/include/Session.h"

class CSendThread: public CBaseThread
{
public:
    CSendThread(CServerBase* server): m_ptrServer(server)
    {
    }
    ~CSendThread()
    {
    }
    

    void *threadRoutine(void *args)
    {
        CSendThread *threadself = (CSendThread *)args;
        CServerBase *svr = threadself->getServerPtr();
        struct epoll_event epEvent;
        cout << "CSendThread start threadRoutine" << endl;
        //bool isRecvEvent = true;
        while (true)
        {
            //cout << "CSendThread infinity loop epollfd:"<< svr->getIoEpollfd() << endl;
            int32 evCount = epoll_wait(svr->getSendEpollfd(),&epEvent, 1, -1);//100ms wait timeout infinite wait just one event to one sockfd
            if (evCount > 0)
            {
                CSession *session =  (CSession*)epEvent.data.ptr;
                int32 oplen = 0;
                if (epEvent.events & EPOLLOUT) // send msg to client
                {
                    oplen = session->sendToSocket();
                    //isRecvEvent = false;
                
                    if (oplen >= 0) // normal 
                    {
                        if (oplen > 0)
                        {
                            cout << "CSendThread=======sendlen:" << oplen << endl;
                        }
                        //session->modEpollEvent(svr->getSendEpollfd(), isRecvEvent);//single thread do not need epolloneshoot
                        if (0 == oplen)
                        {
                            usleep(10000);
                        }
                    }
                    else // socket error wait to free session
                    {
                        session->delEpollEvent(svr->getSendEpollfd());
                        session->setStatus(waitdel);
                    }
                }
            }
            else if (0 == evCount) //epoll timeout
            {
                // handle timeout??
            }
            else // ret < 0 error or interrupt
            {
                if (evCount == -1 && errno == EINTR)
                {
                    continue;
                }
                usleep(10000);
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

};
#endif
