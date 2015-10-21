#ifndef __CLUASCRIPTREGISTER_H__
#define __CLUASCRIPTREGISTER_H__

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "CLuaRegNameConf.hpp"
#include "CLuaRegFuncConf.hpp"

template<typename T>
class CLuaScriptRegister
{
public:
    
    typedef struct R{
        const char *name;
        int (*pfunc)(lua_State *l, T *obj);
    }RegType;
    
    static void registClass(lua_State *l)
    {
        lua_newtable(l);
        int methods = lua_gettop(l);

        luaL_newmetatable(l, RegNameConf::getType<T>());
        int metatable = lua_gettop(l);


        lua_pushvalue(l, methods);
        lua_setglobal(l, RegNameConf::getType<T>());

        luaL_newmetatable(l, "obj_gc_record"); // record the obj not to delete,save metable in LUA_REGISTRYINDEX
        lua_pop(l, 1);

        lua_pushvalue(l, methods);
        lua_setfield(l, metatable, "__metatable"); //set metatable to methods,so when call objfunc it will find it's metatable and finally it just find methods table,and protect the metatable to be modified

        lua_pushcfunction(l, index);
        lua_setfield(l, metatable, "__index");

        //we need gc method to handle userdata to delete or not
        lua_pushcfunction(l, gc);
        lua_setfield(l, metatable, "__gc");

        //we don't need to new class do not add __call

        for (RegType *handle = (RegType*)RegFuncConfig::getTestFuncMap(); handle->name != NULL;  handle++)
        {
            lua_pushstring(l, handle->name);
            lua_pushlightuserdata(l, (void *)handle);
            lua_pushcclosure(l, regfunc, 1);
            lua_settable(l, methods);
        }

        lua_pop(l, 2);
    }

    static int pushobj(lua_State *l, T *obj, bool gc = false)
    {
        if (NULL == obj)
        {
            return 0;
        }

        luaL_getmetatable(l, RegNameConf::getType<T>());
        int protoidx = lua_gettop(l);

        if (lua_isnil(l, -1))
        {
            luaL_error(l, "error no metatable");
            return 0;
        }

        T **ppobj = static_cast<T **>(lua_newuserdata(l, sizeof(T **)));
        if (NULL == ppobj)
        {
            luaL_error(l, "error to new userdata");
            return 0;
        }
        int udataidx = lua_gettop(l);

        *ppobj = obj;
        lua_pushvalue(l, -2);
        lua_setmetatable(l, -2);

        if (!gc)
        {
            char pobjstr[30] = { 0 };
            sprintf(pobjstr, "%p", obj);
            luaL_getmetatable(l, "obj_gc_record");
            if (lua_isnil(l, -1))
            {
                luaL_newmetatable(l, "obj_gc_record");
                lua_pop(l, 1);
            }
            luaL_getmetatable(l, "obj_gc_record");
            lua_pushstring(l, pobjstr);
            lua_pushboolean(l, 1);
            lua_settable(l, -3);
        }

        lua_settop(l, udataidx);
        lua_insert(l, protoidx);
        lua_settop(l, protoidx);
        return 1;
    }

    static T* checkobj(lua_State *l, int idx)
    {
        T **obj = static_cast<T **>(lua_touserdata(l, idx));
        if (NULL == obj)
        {
            return NULL;
        }

        return *obj;
    }

private:
    static int regfunc(lua_State *l)
    {
        //stack content is: T *obj, args 
        T *obj = checkobj(l, 1); //get this pointer like cpp
        lua_remove(l, 1); //remove self from stack, args left in stack only
        RegType *handle = static_cast<RegType *>(lua_touserdata(l, lua_upvalueindex(1)));
        return handle->pfunc(l, obj);
    }

    static int index(lua_State *l)
    {// when obj call func in lua, it will find the method in obj first, if not found,find it's metatable,we don't set metatable to golbal methods 

        lua_getglobal(l, RegNameConf::getType<T>());
        if (lua_istable(l, -1))
        {
            lua_pushvalue(l, 2);
            lua_rawget(l, -2);
            if (lua_isnil(l, -1)) //if nil we can check metatable, but we don't have now, so just return nil
            {
            }
        }
        else
        {
            lua_pushnil(l);
        }

        lua_insert(l, 1);
        lua_settop(l, 1);
        return 1;
    }

    static int gc(lua_State *l)
    {
        lua_getfield(l, LUA_REGISTRYINDEX, "obj_gc_record");
        T *obj = checkobj(l, 1);
        if (NULL == obj)
        {
            return 0;
        }
        
        if (lua_istable(l, -1))
        {
            char ptrstr[30] = { 0 };
            getObjPtrStr(obj, ptrstr);
            lua_getfield(l, -1, ptrstr);
            if (lua_isnil(l, -1))
            {
                delete obj;
                obj = NULL;
            }
        }
        return 0;
    }

    static void getObjPtrStr(T *obj, char *ptrstr)
    {
        sprintf(ptrstr, "%p", obj);
    }
};


#endif


