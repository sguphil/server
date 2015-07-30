#ifndef __CSQLCONN_H__
#define __CSQLCONN_H__
#include "../include/baseHeader.h"
#include <mysql/mysql.h>
class CResult;
class CQuery;

class CSqlConn
{
public:
    CSqlConn(int32 port, string ip, string user, string passwd, string dbname);
    ~CSqlConn();
    void connect(int32 port, string ip, string user, string passwd, string dbname);
    bool exequery(CQuery *queryobj, CResult * result);
private:
    MYSQL *m_mysql;
    bool m_isConnected;
};

#endif 
