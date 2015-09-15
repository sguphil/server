#ifndef __CDBSHANDLERFUNC_H__
#define __CDBSHANDLERFUNC_H__

#include "../../include/baseHeader.h"
#include "../../network/include/Session.h"
#include "../../protocol/testMsg.pb.h" // add all related protocol header here
#include "../DBSvr.h"
#include "../../database/CQuery.hpp"
#include "../../database/CResult.hpp"
#include "../../database/CSqlConn.hpp"

class CDBSHandlerFunc
{
public:
    static int32 testfunc(CSession *session, char *pMsg, int32 msglen);
    static int32 checkuser(CSession *session, char *pMsg, int32 msglen);
};
#endif
