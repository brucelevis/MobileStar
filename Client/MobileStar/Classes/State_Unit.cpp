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
//    //플래너의 Path를 가져온다.
//    std::list<int>& Path = unit->GetPathPlanner()->GetPath();
//    
//    //만약 Path가 비었다면 Idle 상태로 전환한다.
//    if(Path.empty()){
//        unit->GetFSM()->ChangeState(State_Idle::Instance());
//        return;
//    }
//    
//    //Path의 front로 이동한다.
//    int MoveIndex = Path.front();
//    Path.pop_front();
//    
//    //AutoTask에 다음 Path.front를 넣는다.
//    //NetMgr->PushAutoTask(AutoTaskMove());
//    
//    //이동시킨다.
//    unit->SetTileIndex(MoveIndex);
//    Vec2 PathFrontPosition = unit->GetGameWorld()->GetMap()->GetNavGraph().GetNode(MoveIndex).getPosition();
//    printf("%d : %.f %.f \n ",MoveIndex, PathFrontPosition.x,PathFrontPosition.y);
//    auto move = MoveTo::create(1.0f,PathFrontPosition);
//    
//    unit->runAction(move);
}
void State_Move::Update(Unit* unit,float eTime){
//    //플래너의 Path를 가져온다.
//    std::list<int>& Path = unit->GetPathPlanner()->GetPath();
//
//    //만약 길 찾기 플래너의 Path가 비어있으면, Idle 상태로 전환한다.
//    if(Path.empty()){
//        unit->GetFSM()->ChangeState(State_Idle::Instance());
//    }else{
//        //만약 Path가 있으면 앞에서부터 하나씩 꺼내 Action을 취한다.
//        Vec2 PathFrontPosition = unit->GetGameWorld()->GetMap()->GetNavGraph().GetNode(Path.front()).getPosition();
//        Vec2 Heading = PathFrontPosition - unit->getPosition();
//        Heading = Vec2Normalize(Heading) * unit->GetSpeed();
//        
//        //유닛을 이동시킨다.
//        unit->addPosition(Heading * eTime);
//        unit->SetTileIndex(Path.front());
//        
//        //유닛이 해당 노드에 다다랐다면, 지운다.
//        const float Dist = 100;
//        if( Vec2DistanceSq(unit->getPosition(),PathFrontPosition) < Dist ){
//            Path.pop_front();
//        }
//    }
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


