#include "TestClient.h"
#include "../include/acctTimeTool.hpp"
#include "./include/MessageDef.hpp"

TestClient::TestClient()
{
    m_nInterval = 10; //loop per Xms default
    m_nCycleTick = acct_time::getCurTimeMs();
    m_nNextTick = m_nCycleTick + m_nInterval;
    m_nSendTimes = 0;
    m_ServerID = 61;
    m_nIoThreadNum = 1;
    m_svrType = eClient;
}

TestClient::~TestClient()
{

}

void TestClient::DestructNetWorkObj(NetWorkObject * netobj)
{
    if (NULL == netobj)
    {
        return;
    }
    
    {
        delete netobj;
        netobj = NULL;
    }
}

NetWorkObject* TestClient::CreateNetWorkObj(SESSION_TYPE type)
{
    NetWorkObject *netobj = NULL;
    switch (type)
    {
    case 1: // client
        netobj = new ClientSession;
        assert(NULL != netobj);
        cout << "got Client msg" << endl;
        break;
    case 2: // gateway
        netobj = new GatewaySession;
        assert(NULL != netobj);
        cout << "got GatewaySvr msg" << endl;
        break;
    case 3: // other account svr
        cout << "got other server msg" << endl;
        break;
    case 4: // gameserver/logicServer
        netobj = new LogicSession;
        assert(NULL != netobj);
        cout << "got LogicSvr msg" << endl;
        break;
    case 5: // dbserver
        netobj = new DBSession;
        assert(NULL != netobj);
        cout << "got dbserver msg" << endl;
        break;
    case 6: // strict client for test
        netobj = new StrictClient;
        assert(NULL != netobj);
        break;
    case 7: //Account server
        netobj = new AccsvrSession;
        assert(NULL != netobj);
        cout << "got accsvr msg" << endl;
        break;
    default:
        break;
    }

    return netobj;
}

void TestClient::start()
{
    EpollServer::start(); //create all epollfd
    //m_acceptor.init();
    //m_acceptor.startListen("127.0.0.1", 9997);
    //m_acceptor.start();
    m_connector.start();
    for (int i = 0; i < m_ConnCount; i++)
    {
        m_connector.connect("127.0.0.1", 9997, 1); //account serverid
        acct_time::sleepMs(2);
    }

    
    for (int i = 0; i < m_nIoThreadNum;i++)
    {
        CIoThread *newThread = new CIoThread(this);
        newThread->setStartID(i);
        newThread->start();
    }
    
    for (int i = 0; i < 1;i++)
    {
        //CSendThread *sendThread = new CSendThread(this);
        //sendThread->start();
    }

}

#if 0 
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
        //addFdToSendEpoll(newSession);
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
            //addFdToSendEpoll(newSession);
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
                cout << "remove session===========" << session->getSessionId() << endl;
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
#endif

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

            if (sessionType == eAccountSvr )//&& m_nSendTimes < 200)
            {
                //m_nSendTimes++; // test ---send times
                MsgHeader msghead;
                int32 sendlen = 0;
                PkgHeader header;
                //struct c_s_registersession reg;
                struct c_s_refecttest testStr;
                
                if (session->getStatus() != registered)
                {
                    /*
                    msghead.sysId = (uint16)eRegister_Message;
                    msghead.msgType = (uint16)C_S_SISSION_REGISTER;
                    reg.sessionType = m_ServerID;
                    sendlen = sizeof(msghead) + sizeof(reg);
                    //header.length = sendlen;
                    int32 totallen = sendlen +sizeof(header);
                    //char buf[totallen];
                    //encodepkg(buf, &header, &msghead, (char *)&reg, (int16)sizeof(reg));
                    //session->send(buf, totallen);
                    session->processSend((uint16)eRegister_Message, (uint16)C_S_SISSION_REGISTER, (char *)&reg, (int16)sizeof(reg));
                    
                    cout << "ready to send msg:" << totallen << endl;
                    */
                    session->setStatus(registered);
                }/*
                else if (m_nSendTimes++ == 0) //BIN DATA protocol test
                {
                    m_bAlreadySend = true;
                    msghead.sysId = (uint16)eServerMessage_Client;
                    msghead.msgType = (uint16)CLI_ACCS_TESTBINPKG;
                    char *sendStr = (char*)"hello ulserver";
                    testStr.strlen = (int16)strlen(sendStr);
                    testStr.buf = sendStr;
                    int32 msglen = sizeof(msghead) + sizeof(testStr.strlen) + testStr.strlen;
                    sendlen = msglen + sizeof(header);
                    header.length = msglen;
                    char buf[sendlen];
                    //encodepkg(buf, &header, &msghead, (char *)&testStr, (int16)sizeof(testStr.strlen)+testStr.strlen);
                    //memcpy(buf, (char *)&header, sizeof(header));
                    //memcpy(buf+sizeof(header), (char *)&msghead, sizeof(msghead));
                    memcpy(buf, (char *)&(testStr.strlen), sizeof(testStr.strlen));
                    memcpy(buf+sizeof(testStr.strlen), (char *)sendStr, testStr.strlen);
                    //if (session->send(buf, sendlen) < 0)
                    if (session->processSend((uint16)eServerMessage_Client, (uint16)CLI_ACCS_TESTBINPKG, buf, sizeof(testStr.strlen)+testStr.strlen) < 0 )
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
                else if (m_nSendTimes++ == 2)//2 protobuf test
                {
                    //m_nSendTimes = 1;
                    m_nSendTimes = 3;
                    msghead.sysId = (uint16)eServerMessage_Client;
                    msghead.msgType = (uint16)CLI_ACCS_TESTPROBUFPKG;
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
                    //char buf[sendlen];
                    //encodepkg(buf, &header, &msghead, (char *)protomsg, (int16)bytelen);
                    //if (session->send(buf, sendlen) < 0)
                    if (session->processSend((uint16)eServerMessage_Client, (uint16)CLI_ACCS_TESTPROBUFPKG, (char *)protomsg, (int16)bytelen) < 0)
                    {
                        cout << "3send buff is full!!!! stop!!!" << endl;
                        //session->setStatus(waitdel);
                        acct_time::sleepMs(10);
                    }
                    else
                    {
                        cout << "3ready to send msg:" << sendlen << endl;
                    }
                }*/
                else //if (m_nSendTimes == 3)//2 dbServer test
                {
                    //m_nSendTimes = 1;
                    msghead.sysId = (uint16)eServerMessage_Client;
                    msghead.msgType = (uint16)CLI_ACCS_CHECKLOGINUSER;
                    test_package::client_2_acc_checkuser tmsg;
                    tmsg.set_name("10");
                    tmsg.set_passwd("11");
                    int32 bytelen = tmsg.ByteSize();
                    int32 msglen = sizeof(msghead) + bytelen;
                    sendlen = msglen + sizeof(header);
                    char protomsg[bytelen];
                    tmsg.SerializeToArray(protomsg, bytelen);

                    header.length = msglen;
                    //char buf[sendlen];
                    //encodepkg(buf, &header, &msghead, (char *)protomsg, (int16)bytelen);
                    //if (session->send(buf, sendlen) < 0)
                    if (session->processSend((uint16)eServerMessage_Client, (uint16)CLI_ACCS_CHECKLOGINUSER, (char *)protomsg, (int16)bytelen) < 0)
                    {
                        //cout << "4send buff is full!!!! stop!!!" << endl;
                        //session->setStatus(waitdel);
                        acct_time::sleepMs(10);
                    }
                    else
                    {
                        //cout << "4ready to send msg:" << sendlen << endl;// no output
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
        if (true || acct_time::getCurTimeMs() >= m_nNextTick)
        {
            m_nNextTick = acct_time::getCurTimeMs() + 100;
            updateSessionList(); // handle new Session
            handleActiveSession();
            removeDeadSession();
        }
        acct_time::sleepMs(100); // sleep 1ms per loop
    }
    
}
