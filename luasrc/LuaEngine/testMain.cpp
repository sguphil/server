#include "CLuaEngine.hpp"
#include "Test.hpp"
#include <stdio.h>
#include <iostream>

int main()
{
    CLuaEngine *g_engine = CLuaEngine::getInstance();
    g_engine->init();
    Test a;
    g_engine->runFunc<Test>(&a, "runTest");
    return 0;
}
