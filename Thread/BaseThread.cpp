#include "BaseThread.h"
void* common_thread(void *args)
{
    ((CBaseThread*)args)->threadRoutine(args);
    return NULL;
}

void CBaseThread::start()
{
    pthread_attr_init(&m_threadAttr);
    //pthread_attr_setdetachstate(&m_threadAttr, 1);
    pthread_create(&m_threadId, &m_threadAttr, common_thread, this);
    //pthread_join(m_threadId, NULL);
}

void CBaseThread::terminate()
{
    pthread_attr_destroy(&m_threadAttr);
}

void* CBaseThread::threadRoutine(void *args)
{
    for(int i=0; i<10; i++)
        cout << "basethread routine" << endl;

    return NULL;
}
