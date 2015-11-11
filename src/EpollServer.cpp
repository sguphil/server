#include "../include/EpollServer.hpp"

EpollServer::EpollServer():m_nNextconnectTick(0)
{
}
void EpollServer::start()
{
   
}

void EpollServer::updateSessionList()
{
    // 1.process accept session
    CommonList<CSession>* readList = m_acceptor.getReadSessionList();
    if (!readList->empty())
    {
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
    }

    if (m_acceptor.getWriteSessionList()->size() > 0 )
    {
        m_acceptor.swapSessionList();
    }
    
    // 2.process connect session
    CommonList<CSession> *connSessionList =  m_connector.getConnList(); //all connector's session  connect to other server, we use mutilMap to record them
    if (!connSessionList->empty() )
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
                netobj = new ClientSession;
                assert(NULL != netobj);
                newSession->bindNetWorkObj(netobj);
                break;
            case 2:
                //netobj = new ClientSession;
                //assert(NULL != netobj);
                //newSession->bindNetWorkObj(netobj);
                break;
            case 3:
                //netobj = new ClientSession;
                //assert(NULL != netobj);
                //newSession->bindNetWorkObj(netobj);
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
                netobj = new StrictClient;
                assert(NULL != netobj);
                newSession->bindNetWorkObj(netobj);
                break;
            case 7:
                netobj = new AccsvrSession;
                assert(NULL != netobj);
                newSession->bindNetWorkObj(netobj);
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
                reg.sessionType =uint16(m_ServerID); // send self serverType 
                newSession->processSend((uint16)SYS_SESSION_REGISTER, (uint16)C_S_SISSION_REGISTER, (char*)&reg, sizeof(reg));
            }
        }
    }
}

void EpollServer::removeDeadSession()
{
    if (!m_rmSessionList.empty())
    {
        CommonList<CSession>::iterator iter;
        for (iter = m_rmSessionList.begin(); iter != m_rmSessionList.end();)
        {
            CSession *session = *iter;
            if (session->getStatus() == waitdel)
            {
                session->delEpollEvent(m_epollfd);
                session->delEpollEvent(m_epollSendfd);
                session->clear();
                m_rmSessionList.erase(iter++);
                cout << "remove session===========" << session->getSessionId() << endl;
                rmClientSessionFromMap(session->getSessionId());
                delClusterSession(session);
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
    if (!m_activeSessionList.empty())
    {
        CommonList<CSession>::iterator iter = m_activeSessionList.begin();
        for (; iter != m_activeSessionList.end();)
        {
            CSession *session = *iter;
            session->processPacket();
            if (session->getStatus() == waitdel)
            {
                m_rmSessionList.push_back(session);
                m_activeSessionList.erase(iter++);
                continue;
            }

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

            iter++;
        }
    }
}

void EpollServer::update()
{
    if (acct_time::getCurTimeMs() > m_nNextconnectTick) //reconnect 10 seconds
    {
        m_connector.reConnectAll();
        m_nNextconnectTick = acct_time::getCurTimeMs() + 10000;
    }
}
