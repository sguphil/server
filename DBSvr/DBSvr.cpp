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

void DBSvr::DestructNetWorkObj(NetWorkObject * netobj)
{
    delete netobj;
}

NetWorkObject* DBSvr::CreateNetWorkObj(SESSION_TYPE type)
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

void DBSvr::start()
{
    EpollServer::start(); //create all epollfd
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
        newThread->setStartID(i);
        newThread->start();
    }

    m_connector.setServer(this);
    //CSendThread *sendThread = new CSendThread(this); //not start sendThread now 
    //sendThread->start();
}

void DBSvr::update()
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
