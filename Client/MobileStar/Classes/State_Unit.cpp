#include "State_Unit.h"
#include "GameWorld.h"
#include "Unit.h"
/*----------------------------------------------------------------------------
 
 State_Idle
 
 ----------------------------------------------------------------------------*/
State_Idle* State_Idle::Instance(){
    static State_Idle instance;
    return &instance;
}

void State_Idle::Begin(Unit* unit){
}
void State_Idle::Update(Unit* unit,float eTime){
}
void State_Idle::End(Unit* unit){
}
bool State_Idle::TouchBegan(Unit* unit, Touch* touch, Event* _event){
    Vec2 MovePos = unit->GetGameWorld()->GetCameraLayer()->convertTouchToNodeSpace(touch);
    unit->GetPathPlanner()->CreatePathToPosition(MovePos);
    unit->GetFSM()->ChangeState(State_Move::Instance());
    return true;
}
void State_Idle::TouchMoved(Unit* unit, Touch* touch, Event* _event){
    
}
void State_Idle::TouchCancelled(Unit* unit, Touch* touch, Event* _event){
    
}
void State_Idle::TouchEnded(Unit* unit, Touch* touch, Event *_event){
    
}
/*----------------------------------------------------------------------------
 
 State_Move
 
 ----------------------------------------------------------------------------*/
State_Move* State_Move::Instance(){
    static State_Move instance;
    return &instance;
}

void State_Move::Begin(Unit* unit){
}
void State_Move::Update(Unit* unit,float eTime){
    //만약 길 찾기 플래너의 Path가 비어있으면, Idle 상태로 전환한다.
    std::list<Vec2>& Path = unit->GetPathPlanner()->GetPath();
    
    if(Path.empty()){
        unit->GetFSM()->ChangeState(State_Idle::Instance());
    }else{
        Vec2 Heading = Path.front() - unit->getPosition();
        Heading = Vec2Normalize(Heading) * unit->GetSpeed();
        
        //유닛을 이동시킨다.
        unit->addPosition(Heading * eTime);
        
        //유닛이 해당 노드에 다다랐다면, 지운다.
        const float Dist = 100;
        if( Vec2DistanceSq(unit->getPosition(), Path.front()) < Dist ){
            Path.pop_front();
        }
    }
}
void State_Move::End(Unit* unit){
}
bool State_Move::TouchBegan(Unit* unit, Touch* touch, Event* _event){
    Vec2 MovePos = unit->GetGameWorld()->GetCameraLayer()->convertTouchToNodeSpace(touch);
    unit->GetPathPlanner()->CreatePathToPosition(MovePos);
    return true;
}
void State_Move::TouchMoved(Unit* unit, Touch* touch, Event* _event){
    
}
void State_Move::TouchCancelled(Unit* unit, Touch* touch, Event* _event){
    
}
void State_Move::TouchEnded(Unit* unit, Touch* touch, Event *_event){
    
}


