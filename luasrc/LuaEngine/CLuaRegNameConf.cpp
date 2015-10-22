#include "CLuaRegNameConf.hpp"

#define DEF_REG_CLASS_NAME(classname) \
template<> const char* getType<classname>() \
{\
return #classname;\
}

namespace RegNameConf
{
template<typename T> const char* getType()
{
    return "unknow type";
}
}


/**
 * ADD ALL EXPORT CLASS NAME GETTER FUNCTION DEFINE BELOW
 * 
 * @author root (10/22/2015)
 */
namespace RegNameConf
{
DEF_REG_CLASS_NAME(Test);
}

