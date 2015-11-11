#ifndef __PACKAGESTRUCT_H__
#define __PACKAGESTRUCT_H__
#include "baseHeader.h"

#pragma pack(1)

//c->s 1-1  --> SYS_SESSION_REGISTER  C_S_SISSION_REGISTER
struct c_s_registersession
{
    uint16 sessionType; // 1: client 2:gateway 3:other accsvr 4:gameserver 5:dbserver 6:strictClient 7:accsvr   serverid  -->  8bit for serverid 
};

//========test package below==========
//s-c 1-1 return
struct s_c_registersession
{
    int16 retcode; // 0:success >0: error code
};

//c-s 1-2  
struct c_s_refecttest
{
    int16 strlen;
    char *buf;
};

//s-c 1-2 ret = c_s_refecttest
/*
struct c_s_refecttest
{
    int16 strlen;
    char *buf;
};
*/

// c-s 1-3  proto: test_package:testMsg
// s-c 1-3  proto: test_package:testMsg

#pragma pack()
#endif
