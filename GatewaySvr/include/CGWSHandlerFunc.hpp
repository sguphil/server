#ifndef __CGWSHANDLERFUNC_H__
#define __CGWSHANDLERFUNC_H__

#include "../../include/baseHeader.h"
#include "../../network/include/Session.h"
#include "../../protocol/testMsg.pb.h" // add all related protocol header here
#include "../GatewaySvr.h"

class CGWSHandlerFunc
{
public:
    static int32 testfunc(CSession *session, char *pMsg, int32 msglen);
    static int32 checkuser(CSession *session, char *pMsg, int32 msglen);
    static int32 dbcheckuserret(CSession *session, char *pMsg, int32 msglen);
};
#endif
