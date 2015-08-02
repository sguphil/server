#ifndef ACCOUNTSVR_H
#define ACCOUNTSVR_H
#include "../include/Singleton.h"
#include "../network/include/Acceptor.h"
#include "../network/include/CommonList.h"
#include "../network/include/Session.h"

class AccountSvr: public base::Singleton<AccountSvr>, CServerBase
{
public:
    AccountSvr();
    virtual ~AccountSvr();
    void start();
    void update()
    {
    }
    inline int getServerID()
    {
        return m_ServerID;
    }

    int32 getIoEpollfd()
    {
        return m_epollfd;
    }
protected:
    
private:
    Acceptor m_acceptor;
    CommonList<CSession> m_waitSessionList;
    CommonList<CSession> m_activeSessionList;
    //CommonList<Connector*> m_activeConnectorList;
    //CommonList<CSession*> 
    int m_ServerID;
    int32 m_epollfd;

};

#endif // ACCOUNTSVR_H
