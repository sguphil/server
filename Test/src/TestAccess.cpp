#include "TestAccess.h"

void printItem(TestAccess *accessObj)
{
    printf("output the private item:%d\n", accessObj->m_nNum);
}

TestAccess::TestAccess(): m_nNum(101)
{
    //ctor
}

TestAccess::~TestAccess()
{
    //dtor
}

Int32 TestAccess::getNum()
{
    return m_nNum;
}

ostream& operator<<(ostream &out, TestAccess &accObj)
{
    out << accObj.getNum();
    return out;
}
