#ifndef __CQUERY_H__
#define __CQUERY_H__
#include "../include/baseHeader.h"
#include "CSqlConn.hpp"
#include "CDBInstFactory.hpp"
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
    CQuery(CSqlConn *sqlconn, string querystr, int32 len);
    CQuery(string querystr, int32 len);
    CQuery(CSqlConn * sqlconn);
    ~CQuery();
    bool exequery(CResult* res);
    int exeUpdate();

    uint32 escapeString(char *des, char *src, int32 srcLen);

    inline string getQueryStr()
    {
        return m_strQuery;
    }

    inline CSqlConn* getSqlConn()
    {
        return m_sqlconn;
    }

    inline int32 getQlen()
    {
        return m_queryStrLen;
    }

    void setSqlConn(CSqlConn *sqlconn);
    void setQueryStr(std::string, int32 len);
private:
    CSqlConn *m_sqlconn;
    string m_strQuery;
    int32 m_queryStrLen;
    bool m_bIsReady;
};
#endif 
