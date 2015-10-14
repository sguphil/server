//gcc lua_call_so.c -fPIC -shared -o call_so111.so

#include <math.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static int hello_sin(lua_State *L){
    double d = luaL_checknumber(L, 1);
    lua_pushnumber(L, sin(d));
    return 1;
}

static const struct luaL_Reg lua_call_so[] = {
    {"hello_sin" , hello_sin},
    {NULL, NULL}
};

int luaopen_call_so111(lua_State *L){
    luaL_newlib(L, lua_call_so);
    //luaL_register(L, "hello_lib",hello_lib); // lua 5.1
    return 1;
}

