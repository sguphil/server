#ifndef __CLUAREGNAMECONF_H__
#define __CLUAREGNAMECONF_H__
#include "Test.hpp"

#define DECL_REG_CLASS_NAME(classname) \
template<> const char* getType<classname>()

namespace RegNameConf
{
template<typename T> const char* getType();
}


/**
 * ADD ALL EXPORT CLASS NAME DECLARE BELOW
 * 
 * @author root (10/22/2015)
 */
namespace RegNameConf
{
DECL_REG_CLASS_NAME(Test); //template<> const char* getType<Test>();
}

#endif
