#include "CQuery.hpp"
#include "CSqlConn.hpp"

CQuery::CQuery(CSqlConn* sqlconn, string querystr, int32 len):
m_sqlconn(sqlconn),
m_strQuery(querystr),
m_queryStrLen(len),
m_bIsReady(true)
{

}

void CQuery::setQueryStr(string querystr, int32 len)
{
    m_strQuery = querystr;
    m_queryStrLen = len;
    if (NULL != m_sqlconn)
    {
        m_bIsReady = true;
    }
    else
    {
        m_bIsReady = false;
    }
}

CQuery::CQuery(CSqlConn *sqlconn):
m_sqlconn(sqlconn),
m_bIsReady(false)
{

}

void CQuery::setSqlConn(CSqlConn *sqlconn)
{
    m_sqlconn = sqlconn;
}

CQuery::~CQuery()
{
    m_sqlconn->getFactory()->reuse(m_sqlconn);  
    m_sqlconn = NULL;
}

bool CQuery::exequery(CResult* res)
{
    if (NULL == m_sqlconn)
    {
        printf("error mysqlconn instance is null!!");
        return false;
    }

    return m_sqlconn->exequery(this, res);
    
}

int CQuery::exeUpdate()
{
    return m_sqlconn->exeUpdate(this);
}

uint32 CQuery::escapeString(char *des, char *src, int32 srcLen)
{
    return mysql_real_escape_string(m_sqlconn->getMysql(), des, src, srcLen);
}
