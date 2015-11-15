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
    m_svrType = eAccountSvr;
}

AccountSvr::~AccountSvr()
{

}


void AccountSvr::DestructNetWorkObj(NetWorkObject *netobj)
{
    if (NULL == netobj)
    {
        return;
    }

    CSession *session = netobj->getSession();
    if (NULL == session)
    {
        return;
    }

    close(session->getSocket()); //close socket
    

    SESSION_TYPE type = session->getType();
    if ( eClient == type)
    {
        m_ClientNetWorkObjectFactory.reuse((ClientSession*)netobj);
    }
    else
    {
        delete netobj;
        session->setNetWorkObj(NULL);
    }
}

NetWorkObject* AccountSvr::CreateNetWorkObj(SESSION_TYPE type)
{
    NetWorkObject *netobj = NULL;
    switch (type)
    {
    case 1: // client
        netobj = m_ClientNetWorkObjectFactory.allocate(); //new ClientSession;
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

void AccountSvr::start()
{
    EpollServer::start(); //create all epollfd

    m_ClientNetWorkObjectFactory.init(50, 10);

    m_acceptor.setSvrType(m_svrType);
    m_acceptor.setServer(this);
    m_acceptor.init(m_Config.m_accConfigVec[0].maxclient);
    m_acceptor.startListen(m_Config.m_accConfigVec[0].ip, m_Config.m_accConfigVec[0].port);
    m_acceptor.start();

    for (int i = 0; i < m_nIoThreadNum; i++)
    {
        CIoThread *newThread = new CIoThread(this);
        newThread->setStartID(i);
        newThread->start();
    }

    //CSendThread *sendThread = new CSendThread(this);
    //sendThread->start();
    m_connector.setServer(this);
    m_connector.start();

    for (uint32 i = 0; i < m_Config.m_dbConfigVec.size(); i++)
    {
        m_connector.connect(m_Config.m_dbConfigVec[i].ip, m_Config.m_dbConfigVec[i].port, m_Config.m_dbConfigVec[i].serverid);
    }
    int32 size = m_Config.m_logicConfigVec.size();
    for (int32 i = 0; i < size; i++)
    {
        m_connector.connect(m_Config.m_logicConfigVec[i].ip, m_Config.m_logicConfigVec[i].port, m_Config.m_logicConfigVec[i].serverid);
    }

}

void AccountSvr::update()
{
    while (true)
    {
        EpollServer::update();
        if (acct_time::getCurTimeMs() >= m_nNextTick)
        {
            m_nNextTick = acct_time::getCurTimeMs() + m_nInterval;
            updateSessionList(); // handle new Session
            handleActiveSession();
            removeDeadSession();
        }
        acct_time::sleepMs(10); // sleep 1ms per loop
    }
}

