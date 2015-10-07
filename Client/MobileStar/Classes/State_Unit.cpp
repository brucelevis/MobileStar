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
    //터치하였을 때, 네트워크 메시지를 보낸다.
    Vec2 MovePos = unit->GetGameWorld()->GetCameraLayer()->convertTouchToNodeSpace(touch);
    int TileIndex = unit->GetGameWorld()->GetMap()->GetTileIndexFromPosition(MovePos);
    
    auto tel = new TelegramMove(TileIndex);
    printf("index: %d\n",TileIndex);
    tel->PushBackUnitCode(unit->GetID());
    NetMgr->PushBackMessage(tel);
    
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
}
void State_Move::End(Unit* unit){
}
bool State_Move::TouchBegan(Unit* unit, Touch* touch, Event* _event){
    Vec2 MovePos = unit->GetGameWorld()->GetCameraLayer()->convertTouchToNodeSpace(touch);
    int TileIndex = unit->GetGameWorld()->GetMap()->GetTileIndexFromPosition(MovePos);
    
    auto tel = new TelegramMove(TileIndex);
    tel->PushBackUnitCode(unit->GetID());
    printf("index: %d\n",TileIndex);
    NetMgr->PushBackMessage(tel);
    return true;
}
void State_Move::TouchMoved(Unit* unit, Touch* touch, Event* _event){
    
}
void State_Move::TouchCancelled(Unit* unit, Touch* touch, Event* _event){
    
}
void State_Move::TouchEnded(Unit* unit, Touch* touch, Event *_event){
    
}


