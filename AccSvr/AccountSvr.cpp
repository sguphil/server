#include "AccountSvr.h"

AccountSvr::AccountSvr()
{
    m_Config.init("./config/accountSvr.xml");
    m_Config.parseXml();

    m_nInterval = m_Config.m_accConfigVec[0].updateFps; //loop per Xms default
    m_nCycleTick = acct_time::getCurTimeMs(); 
    MAXPKGLEN = m_Config.m_accConfigVec[0].sessionbuflen;
    SESSIONBUFLEN = m_Config.m_accConfigVec[0].maxpkglen;

    m_nStatisticTick = m_nCycleTick;
    m_nHandleCount = 0;

    m_nNextTick = m_nCycleTick + m_nInterval;
    m_ServerID = m_Config.m_accConfigVec[0].serverid;
    m_nIoThreadNum = m_Config.m_accConfigVec[0].recvThread;
    LOGI("=====e======m_nIoThreadNum:" <<  m_nIoThreadNum);
    m_svrType = eACCSvr;
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

void AccountSvr::start()
{
    m_acceptor.setSvrType(m_svrType);
    m_acceptor.init(m_Config.m_accConfigVec[0].maxclient);
    m_acceptor.startListen(m_Config.m_accConfigVec[0].ip, m_Config.m_accConfigVec[0].port);
    m_acceptor.start();

    for (int i = 0; i < m_nIoThreadNum;i++)
    {
        CIoThread *newThread = new CIoThread(this);
        newThread->start();
    }

    CSendThread *sendThread = new CSendThread(this);
    sendThread->start();
    
    m_connector.start();
    
    for (uint32 i = 0; i < m_Config.m_dbConfigVec.size(); i++)
    {
        m_connector.connect(m_Config.m_dbConfigVec[i].ip, m_Config.m_dbConfigVec[i].port, eDBServer);
    }
    int32 size = m_Config.m_logicConfigVec.size();
    for (int32 i = 0; i < size; i++)
    {
        m_connector.connect(m_Config.m_logicConfigVec[i].ip, m_Config.m_logicConfigVec[i].port, eGameServer);
    }
    
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
            if (!checkRecord(newSession))
            {
                m_ServerSessionMap.insert(std::make_pair<SESSION_TYPE, CSession *>(newSession->getType(), newSession));
            }
            //add to epoll event loop
            addFdToRecvEpoll(newSession);
            addFdToSendEpoll(newSession);

            if (newSession->getStatus() != registered)
            {
                //send first package to register session
                struct c_s_registersession reg;
                reg.sessionType = int16(eAccountSvr);
                newSession->processSend((uint16)eRegister_Message, (uint16)C_S_SISSION_REGISTER, (char*)&reg, sizeof(reg));
            }
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
                cout << "remove session===========" << session->getSessionId() << endl;
                if (session->getType() == eClient)
                {
                    m_acceptor.sessionReUse(session);
                }
                else
                {
                    typedef std::multimap<SESSION_TYPE, CSession *>::iterator mapiter;
                    typedef std::pair<mapiter, mapiter> rangeBeginEnd;
                    rangeBeginEnd range = m_ServerSessionMap.equal_range(session->getType());
                    for (mapiter be = range.first; be != range.second; be++)
                    {
                        if (be->second->getSessionId() == session->getSessionId())
                        {
                            m_ServerSessionMap.erase(be);
                            //put in connector errrolist, wait for reconnect...
                            m_connector.addToErrorList(session);
                            break;
                        }
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

void AccountSvr::handleActiveSession()
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
                //cout << "========================================session=====" << ++m_nHandleCount << endl;
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

void AccountSvr::update()
{
    while (true)
    {
        while (acct_time::getCurTimeMs() >= m_nNextTick)
        {
            m_nNextTick = acct_time::getCurTimeMs() + m_nInterval;
            updateSessionList(); // handle new Session
            handleActiveSession();
            removeDeadSession();
        }
        acct_time::sleepMs(10); // sleep 1ms per loop
    }
}
