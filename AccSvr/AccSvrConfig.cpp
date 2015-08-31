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

    {
        //accsvr local config
        item = root->FirstChild("localconfig");
        if (NULL == item)
        {
            printf("can not find localconfig, please check the config file!!!\n");
            exit(1);
        }
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
    }
    {
        //mysql config 
        item = root->FirstChild("mysql");
        if (NULL == item)
        {
            printf("can not find mysql config, please check the config file!!!\n");
            exit(1);
        }
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
    }
    
    {
        //dbSvr config 
        item = root->FirstChild("dbSvr");
        if (NULL == item)
        {
            printf("can not find dbSvr config, please check the config file!!!\n");
            exit(1);
        }
        TiXmlNode *child = item->FirstChild("serverid");
        const char *serverid = child->ToElement()->GetText();
        m_dbConfig.serverid = atoi(serverid);
        
        child = item->FirstChild("ip");
        const char *ip = child->ToElement()->GetText();
        strncpy(m_dbConfig.ip, ip, 32);

        child = item->FirstChild("port");
        const char *port = child->ToElement()->GetText();
        m_dbConfig.port = atoi(port);

    }
    
    {
        //logic config 
        item = root->FirstChild("logicSvr");
        if (NULL == item)
        {
            printf("can not find logicSvr config, please check the config file!!!\n");
            exit(1);
        }
        TiXmlNode *child = item->FirstChild("serverid");
        const char *serverid = child->ToElement()->GetText();
        m_logicConfig.serverid = atoi(serverid);
        
        child = item->FirstChild("ip");
        const char *ip = child->ToElement()->GetText();
        strncpy(m_logicConfig.ip, ip, 32);

        child = item->FirstChild("port");
        const char *port = child->ToElement()->GetText();
        m_logicConfig.port = atoi(port);

    }
}