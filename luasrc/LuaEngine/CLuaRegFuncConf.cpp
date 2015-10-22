#include "CLuaRegFuncConf.hpp"

/**
 * ADD ALL DEFINIYIONS BELOW WHICH DEFINED IN HEADER FILE
 * add class definetion of each exported class
 * @author root (10/21/2015)
 */
namespace FuncHookConf
{
__DEF_CLASS_LUAHOOK_BEGIN(Test) //BEGIN DEFINITION OF CLASS Test
static int testa(lua_State *l, Test *a)
{
    a->testa();
    lua_pushinteger(l, a->getNum());
    return 1;
}
// ... ...
__DEF_CLASS_LUAHOOK_END(Test) //END DEFINITION OF CLASS Test

}

/**
 * ADD ALL DEFINIYIONS BELOW WHICH DEFINED IN HEADER FILE
 * add RegType function array and getter definition below
 * @author root (10/21/2015)
 */
namespace RegFuncConfig
{
__DEF_REGTYPE_ARRAY_BEGIN(Test)
DEF_REGTYPE_ARRAY_ITEM(Test, testa, testa)
// ... ...
__DEF_REGTYPE_ARRAY_END(Test)

}
