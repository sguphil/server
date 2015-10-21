#include "CLuaRegFuncConf.hpp"

namespace FuncHookConf
{

class Test_LuaHook
{
public:
    static int testa(lua_State *l, Test *a)
    {
        a->testa();
        lua_pushinteger(l, a->getNum());
        return 1;
    }
};

}


namespace RegFuncConfig
{

    RegType<Test> TestFuncMap[] =
    {
        {"testa",  &FuncHookConf::Test_LuaHook::testa},
        {NULL, NULL}
    };
    
    RegType<Test> *getTestFuncMap()
    {
        return RegFuncConfig::TestFuncMap;
    }
}
