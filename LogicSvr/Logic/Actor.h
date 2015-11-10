#ifndef __ACTOR_H__
#define __ACTOR_H__
#include "../../include/baseHeader.h"

class CActor
{
public:
    enum ActorType
    {
        ePlayer = 0,
        eMonster = 1,
    };

public:
    CActor() {}
    CActor(ActorType type){ m_eActorType = type;}

public:
    virtual void actorMove() {}
    virtual void actorDie() {}
    virtual void actorEnter(Int32 sceneId, Int32 x, Int32 y) {m_nSceneId = sceneId; m_nx = x; m_ny = y;}

public:
    ActorType getActorType() {return m_eActorType;}

private:
    Int32 m_nx;
    Int32 m_ny;
    uInt64 m_uId;
    Int32 m_nSceneId;
    ActorType m_eActorType;

};
#endif // __ACTOR_H__
