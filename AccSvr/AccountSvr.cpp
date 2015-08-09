#include "AccountSvr.h"

AccountSvr::AccountSvr()
{
    m_nInterval = 100; //loop per 100ms
    m_nCycleTick = getSysTimeMs();
    m_nNextTick = m_nCycleTick + m_nInterval;
    m_ServerID = 1;
    m_nIoThreadNum = 1;
    m_svrType = ACCSvr;
    m_epollfd = epoll_create(10);
    m_epollSendfd = epoll_create(10);

    if (m_epollfd <= 0 || m_epollSendfd <= 0)
    {
        printf("AccountSvr create epollfd error!!!");
        assert(false);
    }
}

AccountSvr::~AccountSvr()
{

}

uint64 AccountSvr::getSysTimeMs()
{
    struct timeb t;
    ftime(&t);
    return 1000 * t.time + t.millitm;
}

void AccountSvr::start()
{
    m_acceptor.init();
    m_acceptor.startListen("127.0.0.1", 9997);
    m_acceptor.start();

    for (int i = 0; i < 1/*m_nIoThreadNum*/;i++)
    {
        CIoThread *newThread = new CIoThread(this);
        newThread->start();
    }

    CSendThread *sendThread = new CSendThread(this);
    sendThread->start();
}

void AccountSvr::updateSessionList()
{
    // 1.process accept session
    CommonList<CSession>* readList = m_acceptor.getReadSessionList();
    CommonList<CSession>::iterator it = readList->begin();
    for (; it != readList->end(); it++)
    {
        CSession *newSession = *it;
        newSession->setStatus(active);
        m_activeSessionList.push_back(newSession);
        //add to epoll event loop
        addFdToRecvEpoll(newSession);
        addFdToSendEpoll(newSession);
    }

    readList->clear();

    if (m_acceptor.getWriteSessionList()->size() > 0 )
    {
        m_acceptor.swapSessionList();
    }
    
    // 2.process connect session
    CommonList<CSession> *connSessionList =  m_connector.getConnList();
    if (connSessionList->size() > 0)
    {
        std::vector<CSession *> connectSessions;
        m_connector.getConnList(connectSessions);
        std::vector<CSession *>::iterator iter = connectSessions.begin();
        for (; iter != connectSessions.end(); iter++)
        {
            CSession *newSession = *iter;
            newSession->setStatus(active);
            m_activeSessionList.push_back(newSession);
            //add to epoll event loop
            addFdToRecvEpoll(newSession);
            addFdToSendEpoll(newSession);
        }
    }

}

void AccountSvr::removeDeadSession()
{
    if (m_activeSessionList.size() > 0)
    {
        CommonList<CSession>::iterator iter;
        for (iter = m_activeSessionList.begin(); iter != m_activeSessionList.end();)
        {
            CSession *session = *iter;
            if (session->getStatus() == waitdel)
            {
                session->delEpollEvent(m_epollfd);
                session->delEpollEvent(m_epollSendfd);
                session->clear();
                m_activeSessionList.erase(iter++);
                cout << "remove session===========" << endl;
                if (session->getType() == eClient)
                {
                    m_acceptor.sessionReUse(session);
                }
            }
            else
            {
                iter++;
            }
        }
    }
}

void AccountSvr::handleActiveSession()
{
    if (m_activeSessionList.size() > 0)
    {
        CommonList<CSession>::iterator iter = m_activeSessionList.begin();
        for (; iter != m_activeSessionList.end(); iter++)
        {
            CSession *session = *iter;
            session->processPacket();
        }
    }
}

void AccountSvr::update()
{
    while (true)
    {
        while (getSysTimeMs() >= m_nNextTick)
        {
            updateSessionList(); // handle new Session
            handleActiveSession();
            removeDeadSession();
            m_nNextTick = getSysTimeMs() + m_nInterval;// 100 ms
            //cout << "into logic loop" << endl;
        }
        usleep(1000);
    }
}
