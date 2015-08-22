#include "../include/CIoBuff.hpp"

CIoBuff::CIoBuff()
{

}

CIoBuff::CIoBuff(int32 size, int32 extraSize)
{
    init(size, extraSize);
    m_nBuffSwapTick = acct_time::getCurTimeMs();
    m_nSwapFPS = 500;
}

CIoBuff::~CIoBuff()
{

}
