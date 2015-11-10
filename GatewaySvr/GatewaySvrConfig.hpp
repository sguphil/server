#ifndef __GATEWAYSVRCONFIG_H__
#define __GATEWAYSVRCONFIG_H__

#include "../include/baseHeader.h"
#include "../include/tinyxml.h"
#include "../include/tinystr.h"
#include "../include/ConfigStruct.hpp"
#include "../include/CConfigBase.hpp"

class CGatewaySvrConfig : public CConfigBase
{
public:
    CGatewaySvrConfig()
    {
        m_accConfigVec.clear();
        m_dbConfigVec.clear();
        m_logicConfigVec.clear();
        m_mysqlConfigVec.clear();
    }

    CGatewaySvrConfig(string fileName)
    {
        m_strFileName = fileName;
        m_accConfigVec.clear();
        m_dbConfigVec.clear();
        m_logicConfigVec.clear();
        m_mysqlConfigVec.clear();
    }
    ~CGatewaySvrConfig()
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