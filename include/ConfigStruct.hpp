#ifndef __CONFIGSTRUCT_H__
#define __CONFIGSTRUCT_H__
#include "baseHeader.h"
//#pragma pack(1)
typedef struct acclocalconfig
{
    int32 serverid;
    char ip[32];
    int32 port;
    int32 maxclient;
    int32 recvThread;
    int32 sendThread;
    int32 sessionbuflen;
    int32 maxpkglen;
    int32 updateFps;
    acclocalconfig()
    {
        memset(ip, 0x00, sizeof(ip));
    }
}TAccLocalConfig;

typedef struct dbsvrConfig
{
    int32 serverid;
    char ip[32];
    int32 port;
    dbsvrConfig()
    {
        memset(ip, 0x00, sizeof(ip));
    }
}TDbsvrConfig;


typedef struct logicConfig
{
    int32 serverid;
    char ip[32];
    int32 port;
    logicConfig()
    {
        memset(ip, 0x00, sizeof(ip));
    }
}TLogicConfig;

typedef struct mysqlconfig
{
    int32 serverid;
    char ip[32];
    int32 port;
    int32 instNum;
    int32 instaccNum;
    char dbuserName[32];
    char dbpasswd[32];
    char dbname[32];
    mysqlconfig()
    {
        memset(ip, 0x00, sizeof(ip));
        memset(dbuserName, 0x00, sizeof(dbuserName));
        memset(dbpasswd, 0x00, sizeof(dbpasswd));
        memset(dbname, 0x00, sizeof(dbname));
    }
}TMysqlConfig;
#endif
