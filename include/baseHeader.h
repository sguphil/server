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

//容器
#include <map>
#include <list>
#include <vector>
#include <set>
#include <sstream>

enum SESSION_TYPE
{
    eClient = 0,
    eGateWay = 1,
    eGameServer = 2,
};

using namespace std;

typedef long long Int64;
typedef long long int64;
typedef int Int32;
typedef int int32;
typedef char Byte;
typedef char Int8;
typedef char int8;
typedef unsigned long long uInt64;
typedef unsigned long long uint64;
typedef  long long Int64;
typedef  long long int64;
typedef unsigned int uInt32;
typedef unsigned int uint32;
typedef unsigned char uInt8;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned short uInt16;
typedef short int16;
typedef short Int16;

#define SAFE_DELETE(p) {if(NULL != (p)) { delete (p); (p) == NULL;}}

#endif // BASEHEADER_H_INCLUDED
