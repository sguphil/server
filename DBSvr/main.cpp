#include "../include/baseHeader.h"
#include "CSqlConn.hpp"
#include "CResult.hpp"
#include "CQuery.hpp"
#include "CDBInstFactory.hpp"

int main()
{
    CDBInstFactory* dbInstFact = new CDBInstFactory;
    dbInstFact->init(1, 5, "127.0.0.1", 3306, "root", "root", "test");
    CSqlConn *sqlconn = dbInstFact->allocate();
    //new CSqlConn(3306, "127.0.0.1", "root", "root", "test");
    const char *sql = "select a, b from tbl";
    CQuery query(sqlconn, sql, strlen(sql));
    CResult *res = new CResult();
    if (query.exequery(res))
    {
        int32 arg;
        while (res->hasNext())
        {
            if ((arg = res->getInt(0)) != -1)
            {
                printf("get result a:%d  b:%d\n", arg, res->getInt(1));
            }
            else
            {
                printf("getfield return NULL!!\n");
            }
        }
    }
    CQuery blobInsert(sqlconn);
    string insertsql("insert into tbl values(10, 11, '");
    char buf[1024] = { 0 };
    struct testdb
    {
        int32 a;
        int32 b;
    };
    struct testdb tsdata = {701, 702};
    int32 len = blobInsert.escapeString(buf, (char *)&tsdata, sizeof(tsdata));
    insertsql.append(buf, len);
    insertsql += "','test varchar";
    insertsql.append("')", strlen("')"));
    blobInsert.setQueryStr(insertsql, insertsql.length());
    CResult pres;
    int affectrow = blobInsert.exeUpdate();

    //test select 
    printf("insert affect row is:%d\n", affectrow);
    string selSql = "select a, b, blobdata, vchar from tbl where a = 10";
    CQuery queryblob(sqlconn, selSql, selSql.length());
    if (queryblob.exequery(&pres))
    {
        while (pres.hasNext())
        {
            //char databuf[100] = { 0 };
            char *databuf = pres.getString(2);
            //pres.getFieldbyT(databuf, 2);
            struct testdb *pdata = (struct testdb*)(databuf);
            
            printf("data is: %d, %d, %s\n", pdata->a, pdata->b, pres.getString(3));
        }
    }
    //delete sqlconn;
    dbInstFact->reuse(sqlconn);
    return 0;
}

