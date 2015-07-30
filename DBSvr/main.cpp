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
            if (res->getFieldbyT(arg, 1) != NULL)
            {
                printf("get result:%d\n", *(res->getFieldbyT<int32>(arg, 1)));
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
    insertsql.append("')", strlen("')"));
    blobInsert.setQueryStr(insertsql, insertsql.length());
    CResult pres;
    int affectrow = blobInsert.exeUpdate();

    //test select 
    printf("insert affect row is:%d\n", affectrow);
    string selSql = "select a, b, blobdata from tbl where a = 10";
    CQuery queryblob(sqlconn, selSql, selSql.length());
    if (queryblob.exequery(&pres))
    {
        while (pres.hasNext())
        {
            //char databuf[100] = { 0 };
            string databuf;
            pres.getFieldbyT(databuf, 2);
            struct testdb *pdata = (struct testdb *)databuf.c_str();
            printf("data is: %d, %d \n", pdata->a, pdata->b);
        }
    }
    //delete sqlconn;
    dbInstFact->reuse(sqlconn);
    return 0;
}

