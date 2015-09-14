#include "CSqlConn.hpp"
#include "CQuery.hpp"
#include "CResult.hpp"
using std::string;

CSqlConn::CSqlConn(int32 port, string ip, string user, string passwd, string dbname)
{
    m_pMysql = mysql_init(NULL);
    if (NULL != m_pMysql)
    {
        connect(port, ip, user, passwd, dbname);
    }
    else
    {
        perror("mysql connect error");
        //assert(NULL != m_pMysql);
    }
}

CSqlConn::~CSqlConn()
{
    mysql_close(m_pMysql);
}

void CSqlConn::connect(int32 port, string ip, string user, string passwd, string dbname)
{
    const char *argip = ip.c_str();
    const char *arguser = user.c_str();
    const char *argpasswd = passwd.c_str();
    const char *argdbname = dbname.c_str();
    if (NULL == mysql_real_connect(m_pMysql, argip, arguser, argpasswd,argdbname,port, NULL, 0))
    {
        perror("mysql_real_connect error");
    }
}

bool CSqlConn::exequery(CQuery *queryobj, CResult * result)
{
    string qstr = queryobj->getQueryStr();
    mysql_real_query(m_pMysql, qstr.c_str(), queryobj->getQlen());
    MYSQL_RES *ptrRes = result->getRes();
    ptrRes = mysql_store_result(m_pMysql);
    if (NULL == ptrRes)
    {
        return false;
    }
    result->setRes(ptrRes);
    int32 col = mysql_num_fields(ptrRes);
    result->setCol(col);

    int32 row = mysql_num_rows(ptrRes);
    result->setRow(row);
    return true;
}

int CSqlConn::exeUpdate(CQuery *queryobj)
{
    if (0 != mysql_real_query(m_pMysql, queryobj->getQueryStr().c_str(), queryobj->getQlen()))
    {
        perror("mysql_real_query exeUpdate error");
        return -1;
    }
    return (int32)mysql_affected_rows(m_pMysql);
}
