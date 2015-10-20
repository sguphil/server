#ifndef __CLUAREGFUNCCONF_H__
#define __CLUAREGFUNCCONF_H__
#include "Test.hpp"

namespace RegFuncConfig
{
    template<typename T>
        struct RegType{
        char *name;
        int (*pfunc)(LuaState *l, T *obj);
    };

    RegType<Test> *getFuncMap()
    {
        return TestFuncMap;
    }

    RegType<Test> TestFuncMap =
    {
        {"testa",  &FuncHookConf::Test_LuaHook::testa};
        {NULL, NULL};
    }

}

namespace FuncHookConf
{

class Test_LuaHook
{
    static int testa(Lua_State *l, Test *a)
    {
        a->testa();
        lua_pushinteger(l, a->getNum());
        return 1;
    }
};

}

#endif
