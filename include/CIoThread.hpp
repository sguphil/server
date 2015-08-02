#ifndef __CIOTHREAD_H__
#define __CIOTHREAD_H__

#include "../include/baseHeader.h"
#include "../Thread/BaseThread.h"
#include "../Thread/Mutex.h"

template<typename ServerType>
class CIoThread: public CBaseThread
{
public:
    CIoThread(ServerType* server): m_ptrServer(server)
    {
    }
    ~CIoThread()
    {
    }
    

    void *threadRoutine(void *args)
    {
        CServerBase *svr = (ServerBase*)arg;
        struct epoll_event epEvent;
        cout << "CIoThread start threadRoutine" << endl;
        while (true)
        {
            cout << "CIoThread infinity loop" << endl;
            int32 evCount = epoll_wait(svr->getIoEpollfd(),&epEvent, 1, -1);//infinite wait just one event to one sockfd
            if (evCount > 0)
            {
                CSession *session =  (CSession*)epEvent.data.ptr;
                if (epEvent.events & EPOLLIN)
                {
                    
                }
            }
            else
            {
                printf("CIoThread error!!! epoll_wait return:%d", evCount);
            }
        }
    }

    inline ServerType* getServerPtr()
    {
        return m_ptrServer;
    }
private:
    ServerType* m_ptrServer;

};
#endif
