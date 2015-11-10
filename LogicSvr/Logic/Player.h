#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "../../include/baseHeader.h"
#include "Actor.h"

class CPlayer: public CActor
{
public:
    CPlayer():CActor(ePlayer){}
    ~CPlayer() {}

public:
    void loadData() {}
    void saveData() {}
    void dispatchMsg() {}


private:
    Int32 m_nGold;
    Int32 m_nCoin;
    Int32 m_nExp;

};

#endif // __PLAYER_H__
