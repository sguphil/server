#ifndef __CLUAREGNAMECONF_H__
#define __CLUAREGNAMECONF_H__
#include "Test.hpp"

namespace RegNameConf
{

template<typename T> const char* getType();

template<> const char* getType<Test>();

}

#endif
