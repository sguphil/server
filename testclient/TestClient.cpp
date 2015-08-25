#include "TestClient.h"
#include "../include/acctTimeTool.hpp"

TestClient::TestClient()
{
    m_nInterval = 10; //loop per Xms default
    m_nCycleTick = acct_time::getCurTimeMs();
    m_nNextTick = m_nCycleTick + m_nInterval;
    m_nSendTimes = 0;
    m_ServerID = 1;
    m_nIoThreadNum = 1;
    m_svrType = ACCSvr;
    m_epollfd = epoll_create(10);
    m_epollSendfd = epoll_create(10);
    m_bAlreadySend = false;
    if (m_epollfd <= 0 || m_epollSendfd <= 0)
    {
        printf("TestClient create epollfd error!!!");
        assert(false);
    }
}

TestClient::~TestClient()
{

}

void TestClient::start()
{
    //m_acceptor.init();
    //m_acceptor.startListen("127.0.0.1", 9997);
    //m_acceptor.start();
    m_connector.start();
    for (int i = 0; i < 1; i++)
    {
        m_connector.connect("127.0.0.1", 9997, eStrictClient);
        acct_time::sleepMs(2);
    }

    
    for (int i = 0; i < 1/*m_nIoThreadNum*/;i++)
    {
        CIoThread *newThread = new CIoThread(this);
        newThread->start();
    }
    
    for (int i = 0; i < 3;i++)
    {
        CSendThread *sendThread = new CSendThread(this);
        sendThread->start();
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

void TestClient::removeDeadSession()
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

        if (m_activeSessionList.size() <= 0)
        {
            cout << "no session!!! exit!!!" << endl;
            //assert(false);
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
                else if (m_nSendTimes++ == 0) //BIN DATA protocol test
                {
                    m_bAlreadySend = true;
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
                    if (session->send(buf, sendlen) < 0)
                    {
                        cout << "send buff is full!!!! stop!!!" << endl;
                        //session->setStatus(waitdel);
                        acct_time::sleepMs(10);
                    }
                    else
                    {
                        cout << "2ready to send msg:" << sendlen << endl;
                    }
                    
                }
                else //2 protobuf test
                {
                    m_nSendTimes = 1;
                    msghead.sysId = 1;
                    msghead.msgType = 3; //4;
                    test_package::testMsg tmsg;
                    tmsg.set_sendtime(acct_time::getCurTimeMs());
                    char *sendStr = (char*)"hello ulserver";
                    tmsg.set_msg(sendStr);
                    int32 bytelen = tmsg.ByteSize();
                    int32 msglen = sizeof(msghead) + bytelen;
                    sendlen = msglen + sizeof(header);
                    char protomsg[bytelen];
                    tmsg.SerializeToArray(protomsg, bytelen);

                    header.length = msglen;
                    char buf[sendlen];
                    encodepkg(buf, &header, &msghead, (char *)protomsg, (int16)bytelen);
                    if (session->send(buf, sendlen) < 0)
                    {
                        cout << "3send buff is full!!!! stop!!!" << endl;
                        //session->setStatus(waitdel);
                        acct_time::sleepMs(10);
                    }
                    else
                    {
                        cout << "3ready to send msg:" << sendlen << endl;
                    }
                }
            }
        }
    }
}

void TestClient::update()
{
    while (true)
    {
        while (acct_time::getCurTimeMs() >= m_nNextTick)
        {
            updateSessionList(); // handle new Session
            handleActiveSession();
            removeDeadSession();
            //m_nNextTick = acct_time::getCurTimeMs() + m_nInterval;// 30 ms per logic handle
            //cout << "into logic loop:" << acct_time::getCurTimeMs() << endl;
        }
        //cout << "out logic loop:" << acct_time::getCurTimeMs() << endl;
        acct_time::sleepMs(1000); // sleep 1ms per loop
    }
    
}
