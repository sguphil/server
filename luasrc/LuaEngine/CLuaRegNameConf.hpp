#ifndef __CLUAREGNAMECONF_H__
#define __CLUAREGNAMECONF_H__

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

#endif
