#ifndef __CLUASCRIPTREGISTER_H__
#define __CLUASCRIPTREGISTER_H__

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include "CLuaRegNameConf.hpp"

template<typename T>
class CLuaScriptRegister
{
public:
    static void register(lua_State *l)
    {
        lua_newtable(l);
        int methods = lua_gettop(l);

        luaL_newmetatable(l, RegName::getType<T>());
        int metatable = lua_gettop(l);



        lua_setglobal(l, RegName::getType<T>());
        
    }
};


#endif


