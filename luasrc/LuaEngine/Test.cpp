#include "Test.hpp"

int Test::testa()
{
    printf("into the testa");
    m_testNum = 100;
    return 0;
}

int Test::testb(int num)
{
    m_testNum = num;
    printf("testb num is:%d\n", m_testNum);
    return m_testNum + 1;
}
