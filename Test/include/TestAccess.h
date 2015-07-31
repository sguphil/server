#ifndef TESTACCESS_H
#define TESTACCESS_H
#include "../../include/baseHeader.h"

class TestAccess
{
    friend void printItem(TestAccess *accessObj);
    friend ostream& operator<<(ostream &out, TestAccess& accObj);
    public:
        TestAccess();
        virtual ~TestAccess();
        int getNum();


    private:
        int m_nNum;
};

#endif // TESTACCESS_H
