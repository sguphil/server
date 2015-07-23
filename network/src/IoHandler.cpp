#include "IoHandler.h"
#include "ClientSession.h"

IoHandler::IoHandler()
:m_acceptor(eGameServer)
{

}

IoHandler::~IoHandler()
{

}

void IoHandler::init()
{
    //read config
    //m_acceptor.init(&configStruct);
    m_acceptor.init();
    //m_acceptor.startListen("127.0.0.1", 9997);
    //m_connector.init(&connConf);
    m_connector.init(); //根据配置创建好需要连接的ip端口
    //m_connector.connect("127.0.0.1", 9997)
}

void IoHandler::run()
{
    m_acceptor.startListen("127.0.0.1", 9997);
    //m_connector.connect("127.0.0.1", 9997);
}

void IoHandler::update()
{


}

void IoHandler::processActiveSessionList()
{
    std::list<CSession*>::iterator iter = m_ActiveSessionList.begin();
    for (iter; iter!=m_ActiveSessionList.end(); iter++)
    {
        //这里需要加入判断断开的连接处理

        CSession *pSession = *iter;
        pSession->processPacket();
    }
}

void IoHandler::processAcceptList()
{
    m_tempSessionList.clear();
    m_acceptor.getAcceptList(m_tempSessionList);
    std::list<CSession*>::iterator iter = m_tempSessionList.begin();
    int ItemCount = 0;
    for (iter; iter!=m_tempSessionList.end();)
    {
        CSession *tmpSession = *iter;
        if (NULL != tmpSession)
        {
            ++ItemCount;
            if (m_ActiveSessionList.size()+ItemCount < m_acceptor.getMaxAcc())
            {
                NetWorkObject *pNetWorkObj = new ClientSession();
                tmpSession->bindNetWorkObj(pNetWorkObj);
            }
            else  //超出链接限制
            {
                printf("超出连接限制，销毁session");
                m_tempSessionList.erase(iter++);
                m_acceptor.sessionReUse(tmpSession);
                continue;
            }
        }
        iter++;
    }

    if (!m_tempSessionList.empty())
        m_ActiveSessionList.splice(m_ActiveSessionList.end(),m_tempSessionList);

}











