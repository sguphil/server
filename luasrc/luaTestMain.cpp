/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
};
*/
#include "luaTest.hpp"

int main()
{

    
    
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_llib(L);
    luaL_dofile(L, "luaTest.lua");
    
    lua_getglobal(L, "add");
    lua_pushinteger(L, 1);
    lua_pushinteger(L, 2);
    if (lua_pcall(L, 2, 1, 0) != 0)
        printf("error running function `f': %s", lua_tostring(L, -1));
    int ret = lua_tointeger(L, -1);
    printf("return is:%d\n", ret);
    lua_pop(L, 1);
    
    lua_close(L);
    return 0;
}
