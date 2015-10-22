#include "CLuaEngine.hpp"
#include "Test.hpp"
#include <stdio.h>
#include <iostream>
/*
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
*/

int main()
{
    CLuaEngine *g_engine = CLuaEngine::getInstance();
    g_engine->init();
    Test t;
    g_engine->runFunc<Test>(&t, "runTest");
    g_engine->runFunc<Test>(&t, "runTestb");

    int a = 10;
    int b = 20;
    //a = a ^ b; b = a ^ b; a = a ^ b;
    a = a ^ a;
    printf("\na=%d b=%d\n", -1>>1, b);
    return 0;
}
