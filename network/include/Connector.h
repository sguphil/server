#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__
#include "../../include/baseHeader.h"
#include "CommonList.h"
#include "Session.h"
#include "../../Thread/BaseThread.h"
#include "../../Factory/BaseFactory.h"
#include "../../Thread/Mutex.h"

class EpollServer;

class Connector: public CBaseThread
{
public:
    Connector();
    ~Connector();
    void init()
    {
    }
    void init(void *confgStruct) {}
    bool connect(const char *szIp, Int32 Port, uint8 serverid);
    void *threadRoutine(void *args);
    void addToWaitList(CSession *session);
    void addToErrorList(CSession *session);
    bool preReConnect(CSession *session);
    void reConnectAll();
    bool getConnList(std::vector<CSession*> &retVec);

    inline CommonList<CSession>* getConnList()
    {
        return &m_connList;
    }

    inline void setServer(EpollServer *svr)
    {
        m_ptrServer = svr;
    }

    inline EpollServer* getServer()
    {
        return m_ptrServer;
    }

private:
    
    CommonList<CSession> m_waitList; //等待链接的队列
    CommonList<CSession> m_connList; //完成链接队列

    CommonList<CSession> m_connErrList; //保存链接错误session，用于重连操作
    CBaseFactory<CSession> m_sessionFactory;
    CMutex m_waitListLock;
    CMutex m_connListLock;
    CMutex m_connErrListLock;
    sem_t m_waitSem;
    pthread_cond_t m_waitCond;
    pthread_mutex_t m_mutex;
    EpollServer *m_ptrServer;
};
#endif // __CONNECTOR_H__
