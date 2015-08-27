#ifndef __CONFIGBASE_H__
#define __CONFIGBASE_H__
#include "./baseHeader.h"

class CConfigBase
{
public:
    CConfigBase()
    {
    }
    virtual ~CConfigBase()
    {

    }

    virtual void parseXml() = 0;

public:
    std::string m_strFileName;
};

#endif
