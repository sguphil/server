#include "../include/Connector.h"

Connector::Connector()
{
    m_sessionFactory.init(2,2);
    pthread_mutex_init(&m_mutex, NULL);
    pthread_cond_init(&m_waitCond, NULL);
    sem_init(&m_waitSem, 0, 0);
}

Connector::~Connector()
{
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_waitCond);
    sem_destroy(&m_waitSem);
}

void Connector::addToWaitList(CSession *session)
{
    //pthread_mutex_lock(&m_mutex);
    m_waitList.push_back(session);
    sem_post(&m_waitSem);
    //pthread_cond_signal(&m_waitCond);
    //pthread_mutex_unlock(&m_mutex);
}

bool Connector::connect(const char *szIp, Int32 Port, SESSION_TYPE type)
{
    CSession *pSession = m_sessionFactory.allocate();
    if (NULL == pSession)
    {
        printf("connector can not alloc Session\n");
        return false;
    }

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
    pSession->setType(type);

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
        cout << "first while connect thread=======" << endl;
        /*pthread_mutex_lock(&m_mutex);
        while(m_waitList.empty())
        {
            cout << "connect thread=======list empty" << endl;
            pthread_cond_wait(&m_waitCond, &m_mutex);
        }*/
        sem_wait(&m_waitSem);
        cout << "connect thread=======" << endl;
        while(!m_waitList.empty())
        {
            CSession *pSession = m_waitList.front();
            m_waitList.pop_front();
            Int32 connResult = ::connect(pSession->getSocket(), (struct sockaddr*)(&(pSession->getSockAddr())), sizeof(struct sockaddr_in));
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

        //pthread_mutex_unlock(&m_mutex);
    }
    return NULL;
}
