#include "CLuaEngine.hpp"

CLuaEngine::CLuaEngine() : m_isInit(false)
{
    init();
}

void CLuaEngine::init()
{
    if (!m_isInit)
    {
        l = luaL_newstate();
        luaL_openlibs(l);
        registerAll();
        runFile("Test.lua");
        m_isInit = true;
    }
}

void CLuaEngine::registerAll()
{
    CLuaScriptRegister < Test > ::registClass(l);
}

int CLuaEngine::runFile(std::string fileName)
{
    luaL_loadfile(l, fileName.c_str());
    lua_pcall(l, 0, 0, 0);
    return 1;
}

