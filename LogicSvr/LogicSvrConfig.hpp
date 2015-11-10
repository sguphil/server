#ifndef __LOGICSVRCONFIG_H__
#define __LOGICSVRCONFIG_H__

#include "../include/baseHeader.h"
#include "../include/tinyxml.h"
#include "../include/tinystr.h"
#include "../include/ConfigStruct.hpp"
#include "../include/CConfigBase.hpp"

class CLogicSvrConfig : public CConfigBase
{
public:
    CLogicSvrConfig()
    {

    }

    CLogicSvrConfig(string fileName)
    {
        m_strFileName = fileName;
        
    }
    ~CLogicSvrConfig()
    {
        
    }

    void init(string fileName)
    {
        m_strFileName = fileName;
        
    }
    
    void parseXml();
	
public:
    TAccLocalConfig m_accConfig;
    TDbsvrConfig m_dbConfig;
    TLogicConfig m_logicConfig;
    TMysqlConfig m_mysqlConfig;
};

#endif