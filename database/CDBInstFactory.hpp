#ifndef __CDBINSTFACTORY_H__
#define __CDBINSTFACTORY_H__
#include "../include/baseHeader.h"
#include "../Factory/BaseFactory.h"
#include "CSqlConn.hpp"

class CDBInstFactory:public CBaseFactory<CSqlConn>
{
public:
    ~CDBInstFactory()
    {
        list<CSqlConn *>::iterator it = m_ItemList.begin();
        while (it != m_ItemList.end())
        {
            delete *it;
            it++;
        }
    }
    void init(Int32 ItemNum, Int32 accNum, string ip, int32 port, string user, string passwd, string dbname)
    {
        m_nAccNum = accNum;
        addItem(ItemNum, ip, port, user, passwd, dbname, true);
    }
private:
    void addItem(Int32 num, string ip, int32 port, string user, string passwd, string dbname, bool flashTotalNum = false)
    {
        if (num > 0)
        {
            for (int i=0; i<num; i++)
            {
                CSqlConn *addPlayer = new CSqlConn(port, ip, user, passwd, dbname);
                addPlayer->setFactory(this);
                m_ItemList.push_back(addPlayer);
                m_nItemLess++;
                if (flashTotalNum)
                    m_nTotalItem++;
            }
        }
    }
};
#endif 
