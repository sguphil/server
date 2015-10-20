#ifndef __CLUAENGINE_H__
#define __CLUAENGINE_H__

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}


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
    int runFile(string fileName);

    template<typename T>
    int runFunc(T *obj, string funcName)
    {

    }
private:
    CLuaEngine() : m_isInit(false){ }
    bool m_isInit;
    Lua_State *l;
    
};


#endif
