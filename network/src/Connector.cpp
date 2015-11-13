#include "../include/Connector.h"
#include "../../common/SIDGenerator.hpp"

//static sem_t m_waitSem;

Connector::Connector()
{
    m_sessionFactory.init(2,2);
    pthread_mutex_init(&m_mutex, NULL);
    pthread_cond_init(&m_waitCond, NULL);
    printf("Connector constructer\n");
    if (sem_init(&m_waitSem, 0, 0) < 0)
    {
        perror("sem_init error");
        assert(false);
    }
}

Connector::~Connector()
{
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_waitCond);
    sem_destroy(&m_waitSem);
}

void Connector::addToWaitList(CSession *session)
{
    pthread_mutex_lock(&m_mutex);
    m_waitList.push_back(session);
    pthread_mutex_unlock(&m_mutex);
    if (sem_post(&m_waitSem) < 0)
    {
        perror("sem_post error");
        assert(false);
    }
}

void Connector::addToErrorList(CSession *session)
{
    AutoLock lock(&m_connErrListLock);
    m_connErrList.push_back(session);
}

void Connector::reConnectAll()
{
    m_connErrListLock.lock();
    std::vector<CSession *> sessionvec;
    
    if (!m_connErrList.empty())
    {
        sessionvec.reserve(m_connErrList.size());
        CommonList<CSession>::iterator iter;
        for (iter = m_connErrList.begin(); iter != m_connErrList.end(); )
        {
            if (preReConnect(*iter))
            {
                sessionvec.push_back(*iter);
                m_connErrList.erase(iter++);
            }
        }
    }
    m_connErrListLock.unLock();
    for (uint32 i = 0; i < sessionvec.size(); i++)
    {
        addToWaitList(sessionvec[i]);
    }
}

bool Connector::preReConnect(CSession *session)
{
    close(session->getSocket());//close the old socket
    Int32 cliSock = socket(AF_INET, SOCK_STREAM, 0); //create a new socket
    if (cliSock < 0)
    {
        printf(" pre reconnect create sock error\n");
        return false;
    }

    session->setSocket(cliSock);
    return true;
}

bool Connector::connect(const char *szIp, Int32 Port, uint8 serverid)
{
    CSession *pSession = m_sessionFactory.allocate();
    if (NULL == pSession)
    {
        printf("connector can not alloc Session\n");
        return false;
    }

    pSession->setType(SIDGenerator::getInstance()->getServerTypeBySvrID(serverid));
    pSession->setConnectSvrID(serverid);
    pSession->setIsFromSelf(true);//means this connection is create from server itself

    Int32 cliSock = socket(AF_INET, SOCK_STREAM, 0);
    if (cliSock < 0)
    {
        printf("connector create sock error\n");
        return false;
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(szIp);
    addr.sin_port = htons(Port);

    pSession->setSocket(cliSock);
    pSession->setSockAddr(addr);
    pSession->setIp(szIp);
    pSession->setPort(Port);

    addToWaitList(pSession);
    return true;
}

bool Connector::getConnList(std::vector<CSession*> &retVec)
{
    AutoLock autoLock(&m_connListLock);
    std::list<CSession*>::iterator iter = m_connList.begin();
    for(; iter!=m_connList.end(); iter++)
    {
        retVec.push_back(*iter);
    }
    m_connList.clear();
    return true;
}

void* Connector::threadRoutine(void *args)
{
    if (false)
    {
        return NULL;
    }

    while (true)
    {
        if (sem_wait(&m_waitSem) < 0)
        {
            perror("sem_wait error");
            assert(false);
        }

        pthread_mutex_lock(&m_mutex);
        while(!m_waitList.empty())
        {
            CSession *pSession = m_waitList.front();
            m_waitList.pop_front();
            Int32 connResult = ::connect(pSession->getSocket(), (struct sockaddr*)(&(pSession->getSockAddr())), sizeof(struct sockaddr_in));
            cout << "connect thread=======ip:" << pSession->getIp() << " port:" << pSession->getPort() << endl;
            if(connResult<0)
            {
                printf("connect error\n");
                m_connErrListLock.lock();
                m_connErrList.push_back(pSession);
                m_connErrListLock.unLock();
            }
            else
            {
                printf("connect success:%s\n", pSession->getIp());
                m_connListLock.lock();
                m_connList.push_back(pSession);
                m_connListLock.unLock();
            }
        }

        pthread_mutex_unlock(&m_mutex);
    }
    return NULL;
}
