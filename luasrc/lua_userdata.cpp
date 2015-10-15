//gcc lua_userdata.cpp -fPIC -shared -o udata.so
#include<stdio.h>
#include<string.h>
#include<math.h>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>


typedef struct info
{
    int a;
    int b;
}INFO;

static int geta(lua_State *l)
{
    INFO *info = (INFO*)lua_touserdata(l, 1);
    lua_pushinteger(l, info->a);
    return 1;
}

static int getb(lua_State *l)
{
    INFO *info = (INFO*)lua_touserdata(l, 1);
    lua_pushinteger(l, info->b);
    return 1;
}

static int newinfo(lua_State *l)
{
    INFO *a = (INFO*)lua_newuserdata(l, sizeof(INFO));
    a->a = 1;
    a->b = 2;
    return 1;
}

static const struct luaL_Reg udata[] =
{
    { "new", newinfo },
    { "geta", geta },
    { "getb", getb },
    { NULL, NULL }
};

#if 0
int luaopen_udata(lua_State *l)
{
    luaL_newlib(l, udata);
    return 1;
}
#else
int luaopen_udata(lua_State *l)
{
    for (struct luaL_Reg* reg = const_cast<struct luaL_Reg*>(udata); reg->name; reg++)
    {
        lua_register(l, reg->name, reg->func);
    }
}
#endif
}
