#ifndef __CLUAREGFUNCCONF_H__
#define __CLUAREGFUNCCONF_H__

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "Test.hpp"
namespace FuncHookConf
{

class Test_LuaHook;

}


namespace RegFuncConfig
{
    template<typename T>
        struct RegType{
        const char *name;
        int (*pfunc)(lua_State *l, T *obj);
    };

    RegType<Test>* getTestFuncMap();
}



#endif
