#include "CThreadPool.h"

volatile int threadId_count = 0;  //线程id生成

int GenerateThreadId()
{
    int i = ++threadId_count;
    return i;
}

CThreadPool ThreadPool;
#define THREAD_SERVER 10

CThreadPool::CThreadPool()
:m_nThreadExitSinceLastCheck(0),
m_nthreadsFreedSinceLastCheck(0),
m_nThreadsReqSinceLastCheck(0),
m_nThreadsEaten(0)
{
    //ctor
}

CThreadPool::~CThreadPool()
{
    //dtor
}

bool CThreadPool::ThreadExit(Thread *t)
{
    m_mutex.lock();
    m_ActiveThreadSet.erase(t);

    if (m_nThreadsToExit > 0) //线程数过多需要删除一些线程
    {
        --m_nThreadsToExit;
        ++m_nThreadExitSinceLastCheck;
        if (t->DeleteAfterExit)
            m_FreeThreadSet.erase(t);
        m_mutex.unLock();
        delete t;
        return false;
    }

    ++m_nThreadExitSinceLastCheck;
    ++m_nThreadsEaten;

    std::set<Thread*>::iterator iter = m_FreeThreadSet.find(t);
    if (iter != m_FreeThreadSet.end())
    {
        //need log module
        printf("duplicate thread: inset:%u outset:%u\n", (*iter)->controlInterface.getGenThreadId(), (*iter)->controlInterface.getGenThreadId());
    }

    m_FreeThreadSet.insert(t);
    //need log
    printf("threadpool recycle thread %u\n", t->controlInterface.getGenThreadId());
    m_mutex.unLock();
    return true;
}

void CThreadPool::ExecuteTask(ThreadBase* exeTarget)
{
    Thread* t;
    m_mutex.lock();

    ++m_nThreadsReqSinceLastCheck;
    --m_nThreadsEaten;

    if (m_FreeThreadSet.size())
    {
        t = *m_FreeThreadSet.begin();
        m_FreeThreadSet.erase(m_FreeThreadSet.begin());

        t->executeTarget = exeTarget;
        t->controlInterface.Resume(); //启动线程
        //need log
        printf("allocate thread: %u\n", t->controlInterface.getGenThreadId());
    }
    else
    {
        t = StartThread(exeTarget); //启动新线程并加入到队列中
    }

    //need log
    printf("threadpool thread:%u exe task at:%p.\n", t->controlInterface.getGenThreadId(), exeTarget);
    m_mutex.unLock();
}

void CThreadPool::StartUp()
{
    int i = 0;
    int nCount = THREAD_SERVER;
    for(i=0; i<nCount; i++)
    {
        StartThread(NULL);
    }

    //need log
    printf("threadpool startup %u threads.\n", nCount);
}

void CThreadPool::showStatus()
{
    m_mutex.lock();
    printf("Threadpool status: Active Threads:%u.\n", m_ActiveThreadSet.size());
    printf("Threadpool status: Suspended Threads:%u.\n", m_FreeThreadSet.size());
    //printf("Threadpool status: Requested-To-Free ratio: %.3f%% (%u%u).\n", );
    printf("Threadpool status: Eaten Count: %d (negative is bad!)\n", m_nThreadsEaten);
    m_mutex.unLock();
}


void CThreadPool::IntegrityCheck()
{
    m_mutex.lock();
    int32 gobbled = m_nThreadsEaten;

    if (gobbled < 0)
    {
        uint32 newThread = abs(gobbled) + THREAD_SERVER;
        m_nThreadsEaten = 0;
        for (uint32 i=0; i<newThread; i++)
        {
            StartThread(NULL);
        }

        //need log
        printf("ThreadPool gobbled<0, spawn %u threads.\n", newThread);
    }
    else if (gobbled<THREAD_SERVER)
    {
        uint32 newThread = (THREAD_SERVER - gobbled);
        for (uint32 i=0; i<newThread; i++)
        {
            StartThread(NULL);
        }

        //need log
        printf("ThreadPool gobbled<THREAD_SERVER, spawn %u threads.\n", newThread);
    }
    else if (gobbled > THREAD_SERVER)
    {
        uint32 killCount = gobbled - THREAD_SERVER;
        KillFreeThreads(killCount);
        m_nThreadsEaten -= killCount;
        //need log
        printf("ThreadPool gobbled>THREAD_SERVER, kill %u threads.\n", killCount);
    }
    else
    {
        //need log
        printf("ThreadPool working well.\n");
    }

    m_nThreadExitSinceLastCheck = 0;
    m_nThreadsReqSinceLastCheck = 0;
    m_nthreadsFreedSinceLastCheck = 0;
    m_mutex.unLock();
}

void CThreadPool::KillFreeThreads(uint32 count)
{
    //need log
    printf("Threadpool kill %u threads.\n", count);
    m_mutex.lock();
    Thread *t = NULL;
    ThreadSet::iterator iter;
    uint32 i = 0;

    for (i=0, iter=m_FreeThreadSet.begin();i<count && iter!=m_FreeThreadSet.end(); ++i, ++iter)
    {
        t = *iter;
        t->executeTarget = NULL;
        t->DeleteAfterExit = true;
        ++m_nThreadsToExit;
        t->controlInterface.Resume();
    }

    m_mutex.unLock();
}

void CThreadPool::Shutdown()
{
    m_mutex.lock();
    uint32 count = m_ActiveThreadSet.size() + m_FreeThreadSet.size();
    //need log
    printf("Threadpool total Threads is:%u\n", count);
    KillFreeThreads((uint32)m_FreeThreadSet.size());
    m_nThreadsToExit += (uint32)m_FreeThreadSet.size();

    ThreadSet::iterator iter;
    for(iter=m_ActiveThreadSet.begin(); iter!=m_ActiveThreadSet.end(); iter++)
    {
        Thread *t = *iter;
        if (NULL != t->executeTarget)
        {
            t->executeTarget->onShutdown();
        }
        else
        {
            t->controlInterface.Resume();
        }
    }
    m_mutex.unLock();

    for (int i=0; ; i++)
    {
        m_mutex.lock();
        if (m_ActiveThreadSet.size() || m_FreeThreadSet.size())
        {
            if (i!=0 && m_FreeThreadSet.size() != 0)
            {
                Thread *t;
                ThreadSet::iterator iter;
                for (iter=m_FreeThreadSet.begin(); iter!=m_FreeThreadSet.end(); ++iter)
                {
                    t = *iter;
                    t->controlInterface.Resume();
                }
            }
            //need log
            printf("Threadpool %u active and %u free threads remain\n", m_ActiveThreadSet.size(), m_FreeThreadSet.size());
            m_mutex.unLock();
            sleep(1000);
        }
        m_mutex.unLock();
        break;
    }
}


//定义线程默认回调及启动方法
static void* thread_proc(void *param)
{
    Thread* t = (Thread*)param;
    t->setupMutex.lock();
    //need log
    printf("thread:%u start\n", t->controlInterface.getGenThreadId());
    t->setupMutex.unLock();

    for (;;) //进入线程过程循环
    {
        if (t->executeTarget != NULL)
        {
            if (t->executeTarget->run()) //真正干的事情总在这run里面，run返回true表示线程跑完就退出销毁,否则只回收线程，不管线程target指针
                delete t->executeTarget;
            t->executeTarget = NULL;
        }

        if (!ThreadPool.ThreadExit(t))  //这里标志是否回收线程，如果需要退出线程则break出来
        {
            break;
        }
        else
        {
            t->controlInterface.suspend();
        }
    }
    pthread_exit(0);
}

Thread* CThreadPool::StartThread(ThreadBase* exeTarget)
{
    pthread_t threadId;
    Thread *t = new Thread;
    t->executeTarget = exeTarget;
    t->DeleteAfterExit = false;

    m_mutex.lock();
    t->setupMutex.lock();
    pthread_create(&threadId, NULL, &thread_proc, (void*)t);
    t->controlInterface.setUp(threadId);
    pthread_detach(threadId);
    t->setupMutex.unLock();
    m_mutex.unLock();
    return t;
}


