#include "CLuaEngine.hpp"
#include "Test.hpp"
#include <stdio.h>
#include <iostream>

int main()
{
    CLuaEngine *g_engine = CLuaEngine::getInstance();
    g_engine->init();
    Test t;
    g_engine->runFunc<Test>(&t, "runTest");
    int a = 10;
    int b = 20;
    //a = a ^ b; b = a ^ b; a = a ^ b;
    a = a ^ a;
    printf("\na=%d b=%d\n", -1>>1, b);
    return 0;
}
