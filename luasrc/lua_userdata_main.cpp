//g++  -o lua_userdata_main -g lua_userdata_main.cpp -llua -lm -ldl
/*#include<stdio.h>
#include<string.h>
#include<math.h>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
*/
#include "lua_userdata.cpp"

int main()
{
    lua_State *l = luaL_newstate();
    luaL_openlibs(l);
    luaopen_udata(l);
    luaL_dofile(l, "./lua_userdata.lua");
    

    //lua_getglobal(l, "test");
    //if (lua_pcall(l, 0, 0, 0) != 0)
        //printf("error running function f: %s", lua_tostring(l, -1));
    lua_close(l);
    return 0;
}
