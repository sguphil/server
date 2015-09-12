#ifndef __CDBINSTMGR_H__
#define __CDBINSTMGR_H__
#include "../include/baseHeader.h"
#include "CDBInstFactory.hpp"
#include "CSqlConn.hpp"

class CDBInstMgr
{
public:
    CDBInstMgr();
    ~CDBInstMgr();



private:
    CDBInstFactory m_dbFactory;
};
#endif
