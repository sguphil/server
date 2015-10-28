#ifndef __ICPKGBUF_H__
#define __ICPKGBUF_H__

#include "../include/baseHeader.h"

class ICPkgBuf
{
    public:
        virtual ~ICPkgBuf()
        {
        }
        virtual void init() = 0;
        virtual int32 getbufLen() = 0;
        virtual int32 getcapacity() = 0;
        virtual int32 getPkgSize() = 0;
        virtual int32 getLeftPkgSize() = 0;
        virtual char* getPkgWritePos() = 0;
        virtual char* getPkgReadPos() = 0;
        virtual void  incPkgLen(int32 size) = 0;
        virtual int32 getHeadSize() = 0;
        virtual void incReadLen(int32 size) = 0;
        virtual int32 getReadLen() = 0;
        virtual ICPkgBuf* copyfrom(ICPkgBuf *buf) = 0;
};


#endif
