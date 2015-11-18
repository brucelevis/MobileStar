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
    if(!m_pTarget)
        return;
    
    if(m_pTarget->IsDead()){
        ClearTarget();
        return;
    }
    
    //타겟이 너무 멀어지면 삭제
//    auto Graph = m_pOwner->GetGameWorld()->GetMap()->GetNavGraph();
//    float dist = Vec2DistanceSq(Graph.GetNode(m_pOwner->GetTileIndex()).getPosition(), Graph.GetNode(m_pTarget->GetTileIndex()).getPosition());
//    if(dist > 200 * 200){
//        ClearTarget();
//        return;
//    }
}
void TargetSystem::AddAggro(Thing* pThing,int aggro){
    //나 자신이면 종료한다.
    if(pThing == m_pOwner) return;
    
    m_AggroMap[pThing] = aggro;
}
void TargetSystem::RemoveAggro(Thing* pThing){
    m_AggroMap.erase(pThing);
}