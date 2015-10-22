#ifndef __CLUAREGFUNCCONF_H__
#define __CLUAREGFUNCCONF_H__

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "Test.hpp"

#define DECL_CLASS_LUAHOOK(classname) \
class classname##_LuaHook

#define DECL_CLASS_REGTYPE_GETTER(classname) \
RegType< classname >* get##classname##FuncMap()

#define __DEF_CLASS_LUAHOOK_BEGIN(classname) \
class classname##_LuaHook\
{\
public:

#define  __DEF_CLASS_LUAHOOK_END(classname) };

#define __DEF_REGTYPE_ARRAY_BEGIN(classname) \
RegType < classname > classname##FuncMap[] = \
{\

#define __DEF_REGTYPE_ARRAY_END(classname) \
{NULL, NULL}\
};\
\
RegType< classname > *get##classname##FuncMap()\
{\
return RegFuncConfig::classname##FuncMap;\
}

#define DEF_REGTYPE_ARRAY_ITEM(classname, export_name, func_name) \
{#export_name,  &FuncHookConf::classname##_LuaHook::func_name},



namespace RegFuncConfig
{
template<typename T>
struct RegType
{
    const char *name;
    int (*pfunc)(lua_State *l, T *obj);
};
}


/**
 * THE BELOW DELARATIONS MUST BE MODIFY IF YOU WANT TO EXPORT 
 * FUNCTIONS TO LUA
 * ADD ALL EXPORT CLASS LUAHOOK DECLARE BELOW
 * @author root (10/21/2015)
 */
namespace FuncHookConf
{
DECL_CLASS_LUAHOOK(Test); //class Test_LuaHook;

}

/**
 * THE BELOW DELARATIONS MUST BE MODIFY IF YOU WANT TO EXPORT 
 * FUNCTIONS TO LUA
 * ADD ALL EXPORT CLASS REGTYPE STRUCT ARRAY GETTER DECLARE 
 * BELOW 
 * @author root (10/21/2015)
 */
namespace RegFuncConfig
{
DECL_CLASS_REGTYPE_GETTER(Test); //RegType<Test>* getTestFuncMap();
}



#endif
