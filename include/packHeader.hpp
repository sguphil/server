#ifndef __PACKHEADER_H__
#defien __PACKHEADER_H__
#include "baseHeader.h"

#param pack(1)
struct PkgHeader
{
    uint16 length;
    uint16 reserved;
};

struct MsgHeader
{
    uint16 sysId;
    uint16 msgType;
};

#endif 

