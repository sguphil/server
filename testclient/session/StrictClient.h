#ifndef __STRICTCLIENT_H__
#define __STRICTCLIENT_H__

#include "../../network/include/NetWorkObject.h"
#include "../../include/baseHeader.h"
#include "../../include/PackageHandler.hpp"
#include "../../include/packHeader.hpp"
#include "../../include/packageStruct.hpp"
#include "../../protocol/testMsg.pb.h"


class StrictClient : public NetWorkObject
{
    public:
        StrictClient();
        virtual ~StrictClient();

        #if 0
        int32 onRecv(PkgHeader *header, char *msgbuf, int32 buffsize);
        #endif

        int32 onRecv(PkgHeader *header, MsgHeader *msghead, char *msgbuf, int32 buffsize);
        int32 processSend(uint16 sysid, uint16 msgid, char *msgbuf, int32 bufsize);

        #if 0
        int32 testRefectSvr(char *msgbuf, int32 bufsize);
        #endif

        int32 testRefectSvr(MsgHeader *msghead, char *msgbuf, int32 bufsize);
        int32 testProtobuf(MsgHeader *msghead, char *msgbuf, int32 bufsize);
    protected:
    private:
        uint64 m_llpkgCount;
        int32 m_nNextTick;
};

#endif // StrictClient_H
