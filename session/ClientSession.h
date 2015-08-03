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
        int32 processSend(int16 sysid, int16 msgid, char *msgbuf, int32 bufsize);
        int32 testRefectSvr(char *msgbuf, int32 bufsize);
    protected:
    private:

};

#endif // CLIENTSESSION_H
