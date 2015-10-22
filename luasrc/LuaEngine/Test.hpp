#ifndef __TEST_H__
#define __TEST_H__
#include <stdio.h>
#include <stdlib.h>

class Test
{
public:
    Test() { }
    int testa();
 
    inline int getNum()
    {
        return m_testNum;
    }

    int testb(int num);
private:
    int m_testNum;
};

#endif
