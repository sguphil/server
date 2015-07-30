#include "../include/baseHeader.h"
#include "CSqlConn.hpp"
#include "CResult.hpp"
#include "CQuery.hpp"

int main()
{
    CSqlConn *sqlconn = new CSqlConn(3306, "127.0.0.1", "root", "root", "test");

    CQuery query(sqlconn, "select a, b from tbl");
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
    delete sqlconn;
    return 0;
}
