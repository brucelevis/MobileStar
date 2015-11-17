#include "TargetSystem.h"
#include "GameWorld.h"


TargetSystem::TargetSystem(Thing* pOwner)
: m_pOwner(pOwner)
, m_pTarget(NULL)
{
    
}

TargetSystem::~TargetSystem(){
    m_AggroMap.clear();
}

void TargetSystem::Update(float eTime){
    
}
void TargetSystem::AddAggro(Thing* pThing,int aggro){
    //나 자신이면 종료한다.
    if(pThing == m_pOwner) return;
    
    m_AggroMap[pThing] = aggro;
}
void TargetSystem::RemoveAggro(Thing* pThing){
    m_AggroMap.erase(pThing);
}