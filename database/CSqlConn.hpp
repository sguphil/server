#ifndef __CSQLCONN_H__
#define __CSQLCONN_H__
#include "../include/baseHeader.h"
#include <mysql/mysql.h>
class CResult;
class CQuery;
class CDBInstFactory;

#define USE_INST_FACTORY 1

class CSqlConn
{
public:
    CSqlConn() {};
    CSqlConn(int32 port, string ip, string user, string passwd, string dbname);
    ~CSqlConn();

    void connect(int32 port, string ip, string user, string passwd, string dbname);
    bool exequery(CQuery *queryobj, CResult * result);
    int exeUpdate(CQuery *queryobj);

    inline MYSQL* getMysql()
    {
        return m_pMysql;
    }

    inline bool getIsConnected()
    {
        return m_isConnected;
    }
/*
    void operator delete(void *p)
    {
           #if USE_INST_FACTORY
        ((CSqlConn*)p)->getFactory()->reuse(p);
           #else 
        free(p);
           #endif
    }
    */
    CDBInstFactory* getFactory()
    {
        return m_ptrInstFactory;
    }
    void setFactory(CDBInstFactory *fac)
    {
        m_ptrInstFactory = fac;
    }

private:
    MYSQL *m_pMysql;
    bool m_isConnected;
    CDBInstFactory *m_ptrInstFactory;
};

#endif 
