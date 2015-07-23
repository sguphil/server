#ifndef __COMMONLIST_H__
#define __COMMONLIST_H__
#include "../../include/baseHeader.h"
#include "../../Thread/Mutex.h"

template<typename T>
class CommonList: public std::list<T*>
{
public:
    CommonList(){}
    ~CommonList(){}

    void lockList()
    {
        m_mutex.lock();
    }

    void unLockList()
    {
        m_mutex.unLock();
    }

private:
    CMutex m_mutex;
};
#endif // __COMMONLIST_H__
