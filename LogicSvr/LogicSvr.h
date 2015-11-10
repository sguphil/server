#ifndef __LOGICSVR_H__
#define __LOGICSVR_H__

#include "LogicSvrConfig.hpp"
#include "../database/CDBInstFactory.hpp"
#include "../include/EpollServer.hpp"
#include "../include/CSendThread.hpp"
#include "../include/CIoThread.hpp"
#include "../include/acctTimeTool.hpp"
#include "../include/Singleton.h"

extern int32 MAXPKGLEN;
extern int32 SESSIONBUFLEN;

class LogicSvr: public EpollServer, public base::Singleton<LogicSvr>
{
public:
    LogicSvr();
    virtual ~LogicSvr();
    void start();
    void update();

    CDBInstFactory* getDBInstFactory()
    {
        return &m_dbInstFactory;
    }

private:
    CLogicSvrConfig m_Config;
    CDBInstFactory m_dbInstFactory;
};


#endif 

