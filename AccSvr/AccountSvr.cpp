#include "AccountSvr.h"

AccountSvr::AccountSvr()
{
    m_ServerID = 1;
    m_svrType = ACCSvr;
    m_epollfd = epoll_create(10);
    if (m_epollfd < 0)
    {
        printf("AccountSvr create epollfd error!!!");
        exit(1);
    }
}

AccountSvr::~AccountSvr()
{

}

void AccountSvr::start()
{
    m_acceptor.init();
    m_acceptor.startListen("127.0.0.1", 9997);
    m_acceptor.start();
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
        addFdToEpoll(newSession->getSocket());
    }

    readList->clear();

    if (m_acceptor.getWriteSessionList()->size() > 0 )
    {
        m_acceptor.swapSessionList();
    }
    
    // 2.process connect session
    CommonList<CSession> *connSessionList =  m_connector.getConnList()->getConnList();
    if (connSessionList->size() > 0)
    {
        std::vector<CSession *> connectSessions;
        m_connector.getConnList(connectSessions);
        std::vector<CSession *>::iterator iter = connSessionList->begin();
        for (; iter != connectSessions.end(); iter++)
        {
            CSession *newSession = *iter;
            newSession->setStatus(active);
            m_activeSessionList.push_back(newSession);
            //add to epoll event loop
            addFdToEpoll(newSession->getSocket());
        }
    }

}

void AccountSvr::removeDeadSession()
{

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
    updateSessionList(); // handle new Session
    handleActiveSession();
}
