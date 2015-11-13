#ifndef __SERVERINCLUDE_H__
#define __SERVERINCLUDE_H__
#include "../Factory/BaseFactory.h"
#include "../include/packHeader.hpp"

//Define the global server utilities

CPackageMgr<accFuncStruct> *g_HandlerMgr;

int32 MAXPKGLEN = 1024*4;
int32 SESSIONBUFLEN = 1024*512;


#endif
