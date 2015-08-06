#include "TestClient.h"

TestClient::TestClient()
{
    m_nInterval = 100; //loop per 100ms
    m_nCycleTick = getSysTimeMs();
    m_nNextTick = m_nCycleTick + m_nInterval;
    m_nSendTimes = 0;
    m_ServerID = 1;
    m_svrType = ACCSvr;
    m_epollfd = epoll_create(10);
    if (m_epollfd <= 0)
    {
        printf("TestClient create epollfd error!!!");
        exit(1);
    }
}

TestClient::~TestClient()
{

}

uint64 TestClient::getSysTimeMs()
{
    struct timeb t;
    ftime(&t);
    return 1000 * t.time + t.millitm;
}

void TestClient::start()
{
    //m_acceptor.init();
    //m_acceptor.startListen("127.0.0.1", 9997);
    //m_acceptor.start();
    m_connector.start();
    m_connector.connect("127.0.0.1", 9997, eStrictClient);

    for (int i = 0; i < 1;i++)
    {
        CIoThread *newThread = new CIoThread(this);
        newThread->start();
    }
}

void TestClient::updateSessionList()
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
        addFdToEpoll(newSession);
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
            addFdToEpoll(newSession);
        }
    }

}

void TestClient::removeDeadSession()
{
    if (m_activeSessionList.size() > 0)
    {
        CommonList<CSession>::iterator iter = m_activeSessionList.begin();
        for (; iter != m_activeSessionList.end(); iter++)
        {
            CSession *session = *iter;
            if (session->getStatus() == waitdel)
            {
                session->delEpollEvent(m_epollfd);
                session->clear();
                if (session->getType() == eClient)
                {
                    m_acceptor.sessionReUse(session);
                }
            }
        }
    }
}

void TestClient::handleActiveSession()
{
    if (m_activeSessionList.size() > 0)
    {
        CommonList<CSession>::iterator iter = m_activeSessionList.begin();
        for (; iter != m_activeSessionList.end(); iter++)
        {
            CSession *session = *iter;
            session->processPacket();

            SESSION_TYPE sessionType = session->getType();

            if (sessionType == eStrictClient && m_nSendTimes < 2)
            {
                //m_nSendTimes++; // test ---send times
                MsgHeader msghead;
                int32 sendlen = 0;
                PkgHeader header;
                struct c_s_registersession reg;
                struct c_s_refecttest testStr;
                
                if (session->getStatus() != registered)
                {
                    msghead.sysId = 1;
                    msghead.msgType = 1;
                    reg.sessionType = 1;
                    sendlen = sizeof(msghead) + sizeof(reg);
                    header.length = sendlen;
                    int32 totallen = sendlen +sizeof(header);
                    char buf[totallen];
                    encodepkg(buf, &header, &msghead, (char *)&reg, (int16)sizeof(reg));
                    session->send(buf, totallen);
                    cout << "ready to send msg:" << totallen << endl;
                    session->setStatus(registered);
                }
                else
                {
                    msghead.sysId = 1;
                    msghead.msgType = 2;
                    char *sendStr = (char*)"hello ulserver";
                    testStr.strlen = (int16)strlen(sendStr);
                    testStr.buf = sendStr;
                    int32 msglen = sizeof(msghead) + sizeof(testStr.strlen) + testStr.strlen;
                    sendlen = msglen + sizeof(header);
                    header.length = msglen;
                    char buf[sendlen];
                    //encodepkg(buf, &header, &msghead, (char *)&testStr, (int16)sizeof(testStr.strlen)+testStr.strlen);
                    memcpy(buf, (char *)&header, sizeof(header));
                    memcpy(buf+sizeof(header), (char *)&msghead, sizeof(msghead));
                    memcpy(buf+sizeof(header)+sizeof(msghead), (char *)&(testStr.strlen), sizeof(testStr.strlen));
                    memcpy(buf+sizeof(header)+sizeof(msghead)+sizeof(testStr.strlen), (char *)sendStr, testStr.strlen);
                    session->send(buf, sendlen);
                    cout << "2ready to send msg:" << sendlen << endl;
                }
            }
        }
    }
}

void TestClient::update()
{
    while (true)
    {
        while (getSysTimeMs() >= m_nNextTick)
        {
            updateSessionList(); // handle new Session
            handleActiveSession();
            removeDeadSession();
            //m_nNextTick = getSysTimeMs() + m_nInterval*10000;
            usleep(100);
            cout << "into logic loop" << endl;
        }
        usleep(200);
    }
    
}
