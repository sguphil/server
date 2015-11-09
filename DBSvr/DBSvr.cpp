#include "DBSvr.h"

DBSvr::DBSvr()
{
    m_Config.init("./config/DBSvr.xml");
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
    m_svrType = eDBServer;
    m_epollfd = epoll_create(10);
    m_epollSendfd = epoll_create(10);

    if (m_epollfd <= 0 || m_epollSendfd <= 0)
    {
        printf("DBSvr create epollfd error!!!");
        assert(false);
    }
    m_dbInstFactory.init(m_Config.m_mysqlConfig.instNum, 1, m_Config.m_mysqlConfig.ip, m_Config.m_mysqlConfig.port, m_Config.m_mysqlConfig.dbuserName, m_Config.m_mysqlConfig.dbpasswd, m_Config.m_mysqlConfig.dbname);
}

DBSvr::~DBSvr()
{
    /*
    if (NULL != m_dbInstFactory)
    {
        delete m_dbInstFactory;
        m_dbInstFactory = NULL;
    }
    */
}

void DBSvr::start()
{
    //for acceptor
    m_acceptor.setSvrType(m_svrType);
    m_acceptor.init(m_Config.m_accConfig.maxclient);
    m_acceptor.startListen(m_Config.m_accConfig.ip, m_Config.m_accConfig.port);
    m_acceptor.start();

    getDBInstFactory();
    

    for (int i = 0; i < m_nIoThreadNum;i++)
    {
        CIoThread *newThread = new CIoThread(this);
        newThread->start();
    }

    //CSendThread *sendThread = new CSendThread(this); //not start sendThread now 
    //sendThread->start();
}

void DBSvr::update()
{
    while (true)
    {
        while (acct_time::getCurTimeMs() >= m_nNextTick)
        {
            m_nNextTick = acct_time::getCurTimeMs() + m_nInterval;
            updateSessionList(); // handle new Session
            handleActiveSession();//process session message
            removeDeadSession(); //kick invalid session
        }
        acct_time::sleepMs(1); // sleep 1ms per loop
    }

}
