#include "CSqlConn.hpp"
#include "CQuery.hpp"
#include "CResult.hpp"

CSqlConn::CSqlConn(int32 port, string ip, string user, string passwd, string dbname)
{
    m_mysql = mysql_init(m_mysql);
    if (NULL != m_mysql)
    {
        connect(port, ip, user, passwd, dbname);
    }
    else
    {
        perror("mysql connect error");
        //assert(NULL != m_mysql);
    }
}

CSqlConn::~CSqlConn()
{
    mysql_close(m_mysql);
}

void CSqlConn::connect(int32 port, string ip, string user, string passwd, string dbname)
{
    if (NULL == mysql_real_connect(m_mysql, ip.c_str(), user.c_str(), passwd.c_str(),dbname.c_str(),port, NULL, false))
    {
        perror("mysql_real_connect error");
    }
}

bool CSqlConn::exequery(CQuery *queryobj, CResult * result)
{
    string qstr = queryobj->getQueryStr();
    mysql_real_query(m_mysql, qstr.c_str(), qstr.length());
    MYSQL_RES *ptrRes = result->getRes();
    ptrRes = mysql_store_result(m_mysql);
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
