#ifndef __CONFIGSTRUCT_H__
#define __CONFIGSTRUCT_H__
#include "baseHeader.h"

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
}TAccLocalConfig;

typedef struct dbsvrConfig
{
    int32 serverid;
    char ip[32];
    int32 port;
}TDbsvrConfig;


typedef struct logicConfig
{
    int8 serverid;
    char ip[32];
    int32 port;
}TLogicConfig;

typedef struct mysqlconfig
{
    int8 serverid;
    char ip[32];
    int32 port;
    int32 instNum;
}TMysqlConfig;
#endif
