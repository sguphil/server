#ifndef __MUTEX_H__
#define __MUTEX_H__
#include <pthread.h>
#include <sys/types.h>
#include "../include/baseHeader.h"

class CMutex
{
public:
    CMutex();
    ~CMutex();
    void lock();
    void unLock();
private:
    pthread_mutex_t m_mutex;
};

class AutoLock
{
public:
    AutoLock(CMutex *mutex):m_mutex(mutex)
    {
        m_mutex->lock();
    }
    ~AutoLock()
    {
        m_mutex->unLock();
    }

private:
    CMutex *m_mutex;
};
#endif // __MUTEX_H__
