#ifndef __TEST_H__
#define __TEST_H__
#include <stdio.h>
#include <stdlib.h>

class Test
{
public:
    Test();
    int testa()
    {
        printf("into the testa");
        m_testNum = 100;
    }
    
    int getNum()
    {
        return m_testNum;
    }
private:
    int m_testNum;
};

#endif
