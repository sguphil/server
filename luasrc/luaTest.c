#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static void checktoptype(lua_State *L)
{
    if(lua_isnumber(L, -1))
        printf("number");
    else if(lua_isstring(L, -1))
        printf("string");
    else if(lua_isfunction(L, -1))
        printf("function");
    else if(lua_isboolean(L, -1))
        printf("boolean");
    else if(lua_isnil(L, -1))
        printf("nil");
    else if(lua_istable(L, -1))
        printf("table");
    else
        printf("userdata or thread");
}
 
static int l_print(lua_State *L)
{
    printf("call from lua");
    int ress[100];
    int i = 1;
    int n = 0;
    for(; i <= 100; i++){
        if(lua_isnil(L,-1)||!lua_isnumber(L, -1)){
            break;
        }
        ress[i-1] = lua_tonumber(L, -1);//??number,?????????
        lua_pop(L, 1);
        n++;
    }
    //????lua???lua??print
    lua_getglobal(L, "print");//????
    for(i = 0; i < n; i++){
        lua_pushnumber(L, ress[i]);//push??
        //printf("stack level = %d, n = %d\n", lua_gettop(L), n);
        lua_insert(L,2);//??????
    }
    if(lua_pcall(L, n, 0, 0)){
        printf("??,lua_pcall? %s",
            lua_tostring(L, -1));
        return 0;
    }
    return n;
}
 
static const struct luaL_Reg l_lib[] = {
    {"luaprint", l_print},
    {NULL, NULL}
};


int luaopen_tllib(lua_State *L)
{
    //luaL_openlib(L, "llib", l_lib, 0);//??????????????????
    //luaL_register(L, "llib", l_lib);
    
    //lua_getglobal(L, "llib");
    //if (lua_isnil(L, -1))
    //{
        //lua_pop(L, 1);
        //lua_newtable(L);
    //}

    //luaL_setfuncs(L, l_lib, 0);
    //lua_setglobal(L, "llib");

    luaL_newlib (L, l_lib);
    return 1;
}

