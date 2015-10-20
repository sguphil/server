#ifndef __CLUAREGFUNCCONF_H__
#define __CLUAREGFUNCCONF_H__

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
    static testa(Lua_State *l, Test *a)
    {
    }
};

}

#endif
