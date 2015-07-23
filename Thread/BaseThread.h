#ifndef __BASETHREAD_H__
#define __BASETHREAD_H__
#include "../include/baseHeader.h"
#include <pthread.h>
#include <sys/types.h>

class CBaseThread
{
public:
    friend void* common_thread(void *args);
public:
    CBaseThread(){}
    virtual ~CBaseThread(){terminate();}
    void start();
    void terminate();

protected:
    virtual void *threadRoutine(void *args);

private:
    pthread_attr_t m_threadAttr;
    pthread_t m_threadId;
};
#endif // __BASETHREAD_H__
