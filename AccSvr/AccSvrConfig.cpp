#include "AccSvrConfig.hpp"

void CAccSvrConfig::parseXml()
{
    TiXmlDocument doc(m_strFileName.c_str());
    bool bLoadRet = doc.LoadFile();
    if (!bLoadRet)
    {
        perror("load cfg file error!");
        printf("load config file Error:%s\n", m_strFileName.c_str());
        exit(1);
    }
			
    TiXmlElement *root = doc.RootElement();
    TiXmlNode *item = NULL;

    //accsvr local config
    item = root->FirstChild("localconfig");
    for (;NULL != item; item = item->NextSibling("localconfig"))
    {
        if (NULL == item)
        {
            printf("can not find localconfig, please check the config file!!!\n");
            exit(1);
        }
        TAccLocalConfig m_accConfig;
        TiXmlNode *child = item->FirstChild("serverid");
        const char *serverid = child->ToElement()->GetText();
        m_accConfig.serverid = atoi(serverid);
        
        child = item->FirstChild("ip");
        const char *ip = child->ToElement()->GetText();
        strncpy(m_accConfig.ip, ip, 32);
        
        child = item->FirstChild("port");
        const char *port = child->ToElement()->GetText();
        m_accConfig.port = atoi(port);

        child = item->FirstChild("maxclient");
        const char *maxclient = child->ToElement()->GetText();
        m_accConfig.maxclient = atoi(maxclient);

        child = item->FirstChild("recvThread");
        const char *recvThread = child->ToElement()->GetText();
        m_accConfig.recvThread = atoi(recvThread);

        child = item->FirstChild("sendThread");
        const char *sendThread = child->ToElement()->GetText();
        m_accConfig.sendThread = atoi(sendThread);

        child = item->FirstChild("sessionbuflen");
        const char *sessionbuflen = child->ToElement()->GetText();
        m_accConfig.sessionbuflen = atoi(sessionbuflen);

        child = item->FirstChild("maxpkglen");
        const char *maxpkglen = child->ToElement()->GetText();
        m_accConfig.maxpkglen = atoi(maxpkglen);

        child = item->FirstChild("updateFps");
        const char *updateFps = child->ToElement()->GetText();
        m_accConfig.updateFps = atoi(updateFps);
        m_accConfigVec.push_back(m_accConfig);
    }
    
    //mysql config 
    item = root->FirstChild("mysql");
    for (; NULL != item; item = item->NextSibling("mysql"))
    {
        if (NULL == item)
        {
            printf("can not find mysql config, please check the config file!!!\n");
            exit(1);
        }
        TMysqlConfig m_mysqlConfig;
        TiXmlNode *child = item->FirstChild("serverid");
        const char *serverid = child->ToElement()->GetText();
        m_mysqlConfig.serverid = atoi(serverid);
            
        child = item->FirstChild("ip");
        const char *ip = child->ToElement()->GetText();
        strncpy(m_mysqlConfig.ip, ip, 32);

        child = item->FirstChild("port");
        const char *port = child->ToElement()->GetText();
        m_mysqlConfig.port = atoi(port);

        child = item->FirstChild("instNum");
        const char *instNum = child->ToElement()->GetText();
        m_mysqlConfig.instNum = atoi(instNum);   
        m_mysqlConfigVec.push_back(m_mysqlConfig);
    }

    //logic config 
    item = root->FirstChild("logicSvr");
    for (; NULL != item; item = item->NextSibling("logicSvr"))
    {
        if (NULL == item)
        {
            printf("can not find logicSvr config, please check the config file!!!\n");
            exit(1);
        }
        
        TLogicConfig m_logicConfig;
        TiXmlNode *child = item->FirstChild("serverid");
        const char *serverid = child->ToElement()->GetText();
        m_logicConfig.serverid = atoi(serverid);
            
        child = item->FirstChild("ip");
        const char *ip = child->ToElement()->GetText();
        strncpy(m_logicConfig.ip, ip, 32);

        child = item->FirstChild("port");
        const char *port = child->ToElement()->GetText();
        m_logicConfig.port = atoi(port);
        cout << "config size1:" << sizeof(m_logicConfig) << endl;
        m_logicConfigVec.push_back(m_logicConfig);
        cout << "config size2:" << m_logicConfigVec.size() << endl;
        int size = m_logicConfigVec.size();
        int size2 = size;
    }
    
    //dbSvr config 
    item = root->FirstChild("dbSvr");
    if (NULL == item)
    {
        printf("can not find dbSvr config, please check the config file!!!\n");
        exit(1);
    }
    TDbsvrConfig m_dbConfig;
    TiXmlNode *child = item->FirstChild("serverid");
    const char *serverid = child->ToElement()->GetText();
    m_dbConfig.serverid = atoi(serverid);
        
    child = item->FirstChild("ip");
    const char *ip = child->ToElement()->GetText();
    strncpy(m_dbConfig.ip, ip, 32);

    child = item->FirstChild("port");
    const char *port = child->ToElement()->GetText();
    m_dbConfig.port = atoi(port);
    m_dbConfigVec.push_back(m_dbConfig);

}
