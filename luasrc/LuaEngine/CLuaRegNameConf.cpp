#include "CLuaRegNameConf.hpp"

namespace RegNameConf
{

template<typename T> const char* getType()
{
    return "unknow type";
}

template<> const char* getType<Test>()
{
    return "Test";
}

}

