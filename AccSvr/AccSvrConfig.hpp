#ifndef __ACCSVRCONFIG_H__
#define __ACCSVRCONFIG_H__

#include "../include/baseHeader.h"
#include "../include/tinyxml.h"
#include "../include/tinystr.h"
#include "../include/ConfigStruct.hpp"
#include "../include/CConfigBase.hpp"

class CAccSvrConfig : public CConfigBase
{
public:
    CAccSvrConfig()
    {

    }

    CAccSvrConfig(string fileName)
    {
        m_strFileName = fileName;
        
    }
    ~CAccSvrConfig()
    {
        
    }

    void init(string fileName)
    {
        m_strFileName = fileName;
        
    }
    
    void parseXml();
	
public:
    std::vector<TAccLocalConfig> m_accConfigVec;
    std::vector<TDbsvrConfig> m_dbConfigVec;
    std::vector<TLogicConfig> m_logicConfigVec;
    std::vector<TMysqlConfig> m_mysqlConfigVec;
};

#endif