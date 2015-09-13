#include "CSqlConn.hpp"
#include "CQuery.hpp"
#include "CResult.hpp"
using std::string;

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
    cout << "====33===dbinit:" << port << ip.c_str() << user << passwd << dbname << endl;
    const char *argip = ip.c_str();
    const char *arguser = user.c_str();
    const char *argpasswd = passwd.c_str();
    const char *argdbname = dbname.c_str();
    if (NULL == mysql_real_connect(m_mysql, argip, arguser, argpasswd,argdbname,port, NULL, false))
    {
        perror("mysql_real_connect error");
    }
}

bool CSqlConn::exequery(CQuery *queryobj, CResult * result)
{
    string qstr = queryobj->getQueryStr();
    mysql_real_query(m_mysql, qstr.c_str(), queryobj->getQlen());
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

int CSqlConn::exeUpdate(CQuery *queryobj)
{
    if (0 != mysql_real_query(m_mysql, queryobj->getQueryStr().c_str(), queryobj->getQlen()))
    {
        perror("mysql_real_query exeUpdate error");
        return -1;
    }
    return (int32)mysql_affected_rows(m_mysql);
}
