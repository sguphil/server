#ifndef BASEHEADER_H_INCLUDED
#define BASEHEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <string.h>
#include <sys/epoll.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/timeb.h>
#include <assert.h>
#include <signal.h>
#include <sys/time.h>
#include <semaphore.h>

//容器
#include <map>
#include <list>
#include <vector>
#include <set>
#include <sstream>
//#include <cstdint>


//
#include "./log4z.h"
//#include "../database/CQuery.hpp"
//#include "../database/CResult.hpp"

using namespace std;

typedef long long Int64;
typedef long long int64;
typedef int Int32;
typedef int int32;
typedef char Byte;
typedef unsigned char uint8;
typedef signed char int8;
//typedef uint8_t uint8;
//typedef int8_t int8;
typedef unsigned long long uInt64;
typedef unsigned long long uint64;
typedef  long long Int64;
typedef  long long int64;
typedef unsigned int uInt32sem, i;
typedef unsigned int uint32;
typedef unsigned char uInt8;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned short uInt16;
typedef short int16;
typedef short Int16;

#define SAFE_DELETE(p) {if(NULL != (p)) { delete (p); (p) == NULL;}}

enum SESSION_TYPE
{
    eClient = 1,  //serverid 61-75
    eGateWay = 2, //serverid 46-60
    eOtherSvr = 3,//serverid 76-90
    eGameServer = 4, //serverid 31-45
    eDBServer = 5, // serverid 16-30
    eStrictClient = 6,//serverid >90 && <128
    eAccountSvr = 7,  //serverid 1-15
    eUndefineSessionType = 8,
};

typedef SESSION_TYPE eSERVERTYPE;

enum eSESSIONSTATUS
{
    active = 1, // active io
    waitactive = 2, // need to active io 
    waitdel = 3, // deactive, need to remove
    registered = 4,//registered
    sock_SIGPIPE = 5, //recv signal pipe
};


#endif // BASEHEADER_H_INCLUDED
