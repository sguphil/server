#ifndef ACCOUNTSVR_H
#define ACCOUNTSVR_H
#include "../include/baseHeader.h"
#include "../include/Singleton.h"
#include "../network/include/Acceptor.h"
#include "../network/include/CommonList.h"
#include "../network/include/Session.h"
#include "../network/include/Connector.h"
#include "../include/CIoThread.hpp"
#include "../include/CSendThread.hpp"
#include "../include/acctTimeTool.hpp"
#include "AccSvrConfig.hpp"
#include "../include/packHeader.hpp"
#include "../common/SIDGenerator.hpp"
#include "../include/EpollServer.hpp"

#include "./session/ClientSession.h"
#include "./session/AccsvrSession.h"
#include "./session/DBSession.h"
#include "./session/LogicSession.h"
#include "./session/GatewaySession.h"
#include "./session/StrictClient.h"

extern int32 MAXPKGLEN;
extern int32 SESSIONBUFLEN;
using std::multimap;

class AccountSvr: public EpollServer, public base::Singleton<AccountSvr>
{
public:
    AccountSvr();
    virtual ~AccountSvr();
    void start();
    void update(); 

    void DestructNetWorkObj(NetWorkObject *netobj);
    NetWorkObject* CreateNetWorkObj(SESSION_TYPE type);

private:
    CAccSvrConfig m_Config;
    CBaseFactory<ClientSession> m_ClientNetWorkObjectFactory;
};

#endif // ACCOUNTSVR_H
