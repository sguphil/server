#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include "../network/include/NetWorkObject.h"
#include "../include/baseHeader.h"
#include "../include/PackageHandler.hpp"
#include "../include/packHeader.hpp"
#include "../include/packageStruct.hpp"


class ClientSession : public NetWorkObject
{
    public:
        ClientSession();
        virtual ~ClientSession();
        int32 onRecv(PkgHeader *header, char *msgbuf, int32 buffsize);
        int32 processSend(uint16 sysid, uint16 msgid, char *msg, int32 msgsize);
        int32 testRefectSvr(char *msgbuf, int32 bufsize);
    protected:
    private:
        uint64 m_llpkgCount;
};

#endif // CLIENTSESSION_H
