#ifndef __CSENDBUF_H__
#define __CSENDBUF_H__
#include "baseHeader.h"
#include "packHeader.hpp"

class CSendBuf
{
public:

private:
    CBuffQueue<char> m_sendbuf;
};

#endif 
