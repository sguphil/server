#ifndef __CQUERY_H__
#define __CQUERY_H__
#include "../include/baseHeader.h"
#include <mysql/mysql.h>
class CSqlConn;
class CResult;
/*
enum eQueryType
{
    default,
    specific
};
*/

class CQuery
{
public:
    CQuery(CSqlConn *sqlconn, string querystr);
    ~CQuery();
    bool exequery(CResult* res);
    string getQueryStr()
    {
        return m_strQuery;
    }

    CSqlConn* getSqlConn()
    {
        return m_sqlconn;
    }
private:
    CSqlConn *m_sqlconn;
    string m_strQuery;
};
#endif 
