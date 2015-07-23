#ifndef __THREADBASE_H__
#define __THREADBASE_H__
#include "../include/baseHeader.h"
/**
    所有线程任务的基类，凡是需要使用线程执行的过程，都需要继承这个类并重写run函数，指向自己回调函数

*/

class ThreadBase
{
    public:
        ThreadBase();
        virtual ~ThreadBase();
        virtual bool run() = 0;
        virtual void onShutdown() {}
    private:
        pthread_t m_ThreadId;
};

#endif // __THREADBASE_H__
