#ifndef __CTHREADPOOL_H__
#define __CTHREADPOOL_H__

#include "../include/baseHeader.h"
#include "ThreadBase.h"
#include "Mutex.h"

/**
    线程池提供了一个管理系统所有线程的场所，凡是需要启动线程的服务部件都将从线程池中获取线程进行运作
    线程池提供了线程的挂起，增删等功能
*/

/**
    线程控制器, 控制线程的停止，启动
*/
int GenerateThreadId();

class ThreadController
{
public:
    void setUp(pthread_t thread)
        {
            m_RealThreadId = thread;
            pthread_mutex_init(&m_mutex, NULL);
            pthread_cond_init(&m_cond, NULL);
            m_nGenThreadId = GenerateThreadId();
        }

        ~ThreadController()
        {
            pthread_mutex_destroy(&m_mutex);
            pthread_cond_destroy(&m_cond);
        }

        void suspend()
        {
            pthread_cond_wait(&m_cond, &m_mutex);
        }

        void Resume()
        {
            pthread_cond_signal(&m_cond);
        }

        void Join()
        {
            pthread_join(m_RealThreadId, NULL);
        }

        inline int getGenThreadId()
        {
            return (uint32)m_nGenThreadId;
        }

private:
    pthread_cond_t m_cond;
    pthread_mutex_t m_mutex;
    int m_nGenThreadId;
    pthread_t m_RealThreadId;
};

/**
    线程对象，线程池中保存一个线程队列，当无任务需要运行时，线程处于挂起状态，当有任务需要线程运作时从队列中拿出线程并启动线程运行指定任务
    需要线程执行的任务都需要集成于ThreadBase，重写run函数，使用多态的方式实现线程函数调用。
*/
struct Thread
{
    ThreadBase *executeTarget;
    ThreadController controlInterface;
    CMutex setupMutex;
    bool DeleteAfterExit;
};

typedef std::set<Thread*> ThreadSet;


class CThreadPool
{
    public:
        CThreadPool();
        ~CThreadPool();

        void IntegrityCheck();

        //启动线程池
        void StartUp();

        //停止
        void Shutdown();

        //线程退出 返回：true 放回到可用队列中  false:删除线程实例，清理内存
        bool ThreadExit(Thread *t);

        //启动一个线程返回线程指针，线程未启动
        Thread* StartThread(ThreadBase *exeTarget);

        //创建并执行一个线程
        void ExecuteTask(ThreadBase* exeTarget);

        //打印状态信息
        void showStatus();

        //杀死指定个数的线程
        void KillFreeThreads(uint32 count);

        //设置当前空闲值
        inline void Gobble() { m_nThreadsEaten = (uint32)m_FreeThreadSet.size();}

        inline uint32 GetActiveThreadCount() {return (uint32)m_ActiveThreadSet.size();}
        inline uint32 GetFreeThreadCount() {return (uint32)m_FreeThreadSet.size();}

    private:
        uint32 m_nThreadsReqSinceLastCheck;
        uint32 m_nthreadsFreedSinceLastCheck;
        uint32 m_nThreadExitSinceLastCheck;
        uint32 m_nThreadsToExit; //需要删除线程数
        uint32 m_nThreadsEaten;
        CMutex m_mutex;
        ThreadSet m_ActiveThreadSet;
        ThreadSet m_FreeThreadSet;

};

extern CThreadPool ThreadPool;
#endif // __CTHREADPOOL_H__
