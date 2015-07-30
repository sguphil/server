#include "CQuery.hpp"
#include "CSqlConn.hpp"

CQuery::CQuery(CSqlConn* sqlconn, string querystr):m_sqlconn(sqlconn),m_strQuery(querystr)
{

}

CQuery::~CQuery()
{

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
