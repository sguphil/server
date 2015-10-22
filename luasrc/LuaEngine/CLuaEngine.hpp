#ifndef __CLUAENGINE_H__
#define __CLUAENGINE_H__
#include <iostream>
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include "Test.hpp"

#include "CLuaScriptRegister.hpp"
/**
 * usage: 
 * to export c++ class to lua follow the steps: 
 * 1.if first time, open CLuaRegFuncConfig.hpp add 
 * "DECL_CLASS_LUAHOOK" and "DECL_CLASS_REGTYPE_GETTER"; open
 * CLuaRegFuncConfig.cpp add "__DEF_CLASS_LUAHOOK_BEGIN" chuck 
 * and add "__DEF_REGTYPE_ARRAY_BEGIN" chuck, or just open 
 * CLuaRegFuncConfig.cpp add item to "__DEF_CLASS_LUAHOOK_BEGIN" chuck 
 * and add "__DEF_REGTYPE_ARRAY_BEGIN" chuck. 
 *  
 * 2.if first time, open CLuaRegNameConf.hpp add 
 * "DECL_REG_CLASS_NAME" and then open CLuaRegNameConf.cpp add 
 * "DEF_REG_CLASS_NAME" with the argv classname, or nothing to 
 * do in this step. 
 */

class CLuaEngine
{
public:
    static CLuaEngine* getInstance()
    {
        static CLuaEngine engine;
        return &engine;
    }

    void init();
    void registerAll();
    int runFile(std::string fileName);

    inline lua_State* getLuaState()
    {
        return l;
    }

    template<typename T>
    int runFunc(T *obj, std::string funcName)
    {
        lua_getglobal(l, funcName.c_str());
        if (lua_isnil(l, -1))
        {
            luaL_error(l, "can not find func by name\n");
            return 0;
        }
        pushTest<T>(obj);
        if (0 != lua_pcall(l, 1, 1, 0))
        {
            luaL_error(l, "call %s error\n", funcName.c_str());
        }
        return 0;
    }

    template<typename T>
    int pushTest(T *obj, lua_State *vm = NULL)
    {
        if (NULL == obj)
        {
            luaL_error(l, "pushTest error!!!\n");
            return 0;
        }
        if (NULL == vm)
        {
            CLuaScriptRegister<Test>::pushobj(l, obj);
        }
        else
        {
            CLuaScriptRegister<Test>::pushobj(vm, obj);
        }
        return 1;
    }
    
private:
    CLuaEngine();
    bool m_isInit;
    lua_State *l;
    
};


#endif
