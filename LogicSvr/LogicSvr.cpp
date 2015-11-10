#include "LogicSvr.h"

LogicSvr::LogicSvr()
{
    m_Config.init("./config/LogicSvr.xml");
    m_Config.parseXml();

    m_nInterval = m_Config.m_accConfig.updateFps; //loop per Xms default
    m_nCycleTick = acct_time::getCurTimeMs(); 
    MAXPKGLEN = m_Config.m_accConfig.sessionbuflen;
    SESSIONBUFLEN = m_Config.m_accConfig.maxpkglen;

    m_nStatisticTick = m_nCycleTick;
    m_nHandleCount = 0;

    m_nNextTick = m_nCycleTick + m_nInterval;
    m_ServerID = m_Config.m_accConfig.serverid;
    m_nIoThreadNum = m_Config.m_accConfig.recvThread;
    LOGI("=====e======m_nIoThreadNum:" <<  m_nIoThreadNum);
    m_svrType = eGameServer;
    m_epollfd = epoll_create(10);
    m_epollSendfd = epoll_create(10);

    if (m_epollfd <= 0 || m_epollSendfd <= 0)
    {
        printf("LogicSvr create epollfd error!!!");
        assert(false);
    }
    m_dbInstFactory.init(m_Config.m_mysqlConfig.instNum, 1, m_Config.m_mysqlConfig.ip, m_Config.m_mysqlConfig.port, m_Config.m_mysqlConfig.dbuserName, m_Config.m_mysqlConfig.dbpasswd, m_Config.m_mysqlConfig.dbname);
}

LogicSvr::~LogicSvr()
{

}

void LogicSvr::start()
{
    //for acceptor
    m_acceptor.setSvrType(m_svrType);
    m_acceptor.setServer(this);
    m_acceptor.init(m_Config.m_accConfig.maxclient);
    m_acceptor.startListen(m_Config.m_accConfig.ip, m_Config.m_accConfig.port);
    m_acceptor.start();

    //getDBInstFactory();
    //getBestServerSession(eDBServer);

    for (int i = 0; i < m_nIoThreadNum;i++)
    {
        CIoThread *newThread = new CIoThread(this);
        newThread->start();
    }

    //CSendThread *sendThread = new CSendThread(this); //not start sendThread now 
    //sendThread->start();
     m_connector.start();

    m_connector.connect(m_Config.m_dbConfig.ip, m_Config.m_dbConfig.port, m_Config.m_dbConfig.serverid);
   
}

void LogicSvr::update()
{
    while (true)
    {
        while (acct_time::getCurTimeMs() >= m_nNextTick)
        {
            EpollServer::update();
            m_nNextTick = acct_time::getCurTimeMs() + m_nInterval;
            updateSessionList(); // handle new Session
            handleActiveSession();//process session message
            removeDeadSession(); //kick invalid session
        }
        acct_time::sleepMs(1); // sleep 1ms per loop
    }

}
