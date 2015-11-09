#include "../include/EpollServer.hpp"

void EpollServer::start()
{
   
}

void EpollServer::updateSessionList()
{
    // 1.process accept session
    CommonList<CSession>* readList = m_acceptor.getReadSessionList();
    CommonList<CSession>::iterator it = readList->begin();
    for (; it != readList->end(); it++)
    {
        CSession *newSession = *it;
        newSession->setStatus(active);
        newSession->setSessionId(SIDGenerator::getInstance()->generatorSid());
        m_activeSessionList.push_back(newSession);
        //add to epoll event loop
        addFdToRecvEpoll(newSession); //pollin and pollout
        //addFdToSendEpoll(newSession);
    }

    readList->clear();

    if (m_acceptor.getWriteSessionList()->size() > 0 )
    {
        m_acceptor.swapSessionList();
    }
    
    // 2.process connect session
    CommonList<CSession> *connSessionList =  m_connector.getConnList(); //all connector's session  connect to other server, we use mutilMap to record them
    if (connSessionList->size() > 0)
    {
        std::vector<CSession *> connectSessions;
        m_connector.getConnList(connectSessions);
        std::vector<CSession *>::iterator iter = connectSessions.begin();
        for (; iter != connectSessions.end(); iter++)
        {
            CSession *newSession = *iter;
            newSession->setStatus(active);
            NetWorkObject *netobj = NULL;
            int32 sessionT = newSession->getType();

            switch (sessionT)
            {
            case 1:

                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                netobj = new LogicSession;
                assert(NULL != netobj);
                newSession->bindNetWorkObj(netobj);
                break;
            case 5:
                netobj = new DBSession;
                assert(NULL != netobj);
                newSession->bindNetWorkObj(netobj);
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            default:
                break;
            }
            m_activeSessionList.push_back(newSession);
            newSession->setSessionId(SIDGenerator::getInstance()->generatorSid());
            if (!checkRecord(newSession))
            {
                m_ServerSessionMap.insert(std::make_pair<SESSION_TYPE, CSession *>(newSession->getType(), newSession));
            }
            //add to epoll event loop
            addFdToRecvEpoll(newSession);
            //addFdToSendEpoll(newSession);

            if (newSession->getStatus() != registered)
            {
                //send first package to register session
                struct c_s_registersession reg;
                reg.sessionType = int16(m_svrType);
                newSession->processSend((uint16)eRegister_Message, (uint16)C_S_SISSION_REGISTER, (char*)&reg, sizeof(reg));
            }
        }
    }
}

void EpollServer::removeDeadSession()
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
                cout << "remove session===========" << session->getSessionId() << endl;
                if (session->getType() == eClient)
                {
                    m_acceptor.sessionReUse(session);
                }
                else
                {
                    if (checkRecord(session))
                    {
                        delClusterSession(session);
                    }
                }
            }
            else
            {
                iter++;
            }
        }
    }
}

void EpollServer::handleActiveSession()
{
    if (m_activeSessionList.size() > 0)
    {
        CommonList<CSession>::iterator iter = m_activeSessionList.begin();
        for (; iter != m_activeSessionList.end(); iter++)
        {
            CSession *session = *iter;
            session->processPacket();
            if ((acct_time::getCurTimeMs() - m_nStatisticTick)>1000) //1s
            {
                m_nStatisticTick = acct_time::getCurTimeMs() + 1000;
                m_nHandleCount = 0;
            }
            m_nHandleCount++;
            SESSION_TYPE type = session->getType();
            if (type != eUndefineSessionType)
            {
                if (eGateWay == type || eOtherSvr == type || eGameServer == type || eAccountSvr == type) //record server cluster
                {
                    if (registered == session->getStatus() && !checkRecord(session))
                    {
                        m_ServerSessionMap.insert(std::make_pair<SESSION_TYPE, CSession *>(session->getType(), session));
                    }
                }
            }
        }
    }
}

void EpollServer::update()
{
    
}
