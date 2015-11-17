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
    char buf[128];
    sprintf(buf,"Zergling%s%sIdle",StrMgr->GetPlayerFlagStr(unit->GetPlayerFlag()),StrMgr->GetUnitDirStr(unit->m_iDir));
    unit->m_pSprite->stopAllActions();
    auto pAnimate = Animate::create(AnimationCache::getInstance()->getAnimation(buf));
    auto pForever = RepeatForever::create(pAnimate);
    unit->m_pSprite->runAction(pForever);
}
void State_Idle::Update(Unit* unit,float eTime){
}
void State_Idle::End(Unit* unit){
}
bool State_Idle::TouchBegan(Unit* unit, Touch* touch, Event* _event){
    //터치하였을 때, 네트워크 메시지를 보낸다.
    //만약 나의 플레이어가 아니었을 경우, 하지 않는다.
//    if(unit->GetPlayerFlag() != NetMgr->GetPlayerFlag())
//        return false;
//    
//    Vec2 MovePos = unit->GetGameWorld()->GetCameraLayer()->convertTouchToNodeSpace(touch);
//    int TileIndex = unit->GetGameWorld()->GetMap()->GetTileIndexFromPosition(MovePos);
//    
//    auto tel = new TelegramMove(TileIndex);
//    tel->PushBackUnitCode(unit->GetID());
//    NetMgr->PushBackMessage(tel);
    
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
    char buf[128];
    sprintf(buf,"Zergling%s%sRun",StrMgr->GetPlayerFlagStr(unit->GetPlayerFlag()),StrMgr->GetUnitDirStr(unit->m_iDir));
    unit->m_pSprite->stopAllActions();
    auto pAnimate = Animate::create(AnimationCache::getInstance()->getAnimation(buf));
    auto pForever = RepeatForever::create(pAnimate);
    unit->m_pSprite->runAction(pForever);
}
void State_Move::Update(Unit* unit,float eTime){
    //애니메이션 설정
    
    if(unit->m_iPrevDir != unit->m_iDir){
        char buf[128];
        sprintf(buf,"Zergling%s%sRun",StrMgr->GetPlayerFlagStr(unit->GetPlayerFlag()),StrMgr->GetUnitDirStr(unit->m_iDir));
        unit->m_pSprite->stopAllActions();
        auto pAnimate = Animate::create(AnimationCache::getInstance()->getAnimation(buf));
        auto pForever = RepeatForever::create(pAnimate);
        unit->m_pSprite->runAction(pForever);
    }
}
void State_Move::End(Unit* unit){
}
bool State_Move::TouchBegan(Unit* unit, Touch* touch, Event* _event){
    //터치하였을 때, 네트워크 메시지를 보낸다.
    //만약 나의 플레이어가 아니었을 경우, 하지 않는다.
//    if(unit->GetPlayerFlag() != NetMgr->GetPlayerFlag())
//        return false;
//    
//    Vec2 MovePos = unit->GetGameWorld()->GetCameraLayer()->convertTouchToNodeSpace(touch);
//    int TileIndex = unit->GetGameWorld()->GetMap()->GetTileIndexFromPosition(MovePos);
//    
//    auto tel = new TelegramMove(TileIndex);
//    tel->PushBackUnitCode(unit->GetID());
//    NetMgr->PushBackMessage(tel);
    return true;
}
void State_Move::TouchMoved(Unit* unit, Touch* touch, Event* _event){
    
}
void State_Move::TouchCancelled(Unit* unit, Touch* touch, Event* _event){
    
}
void State_Move::TouchEnded(Unit* unit, Touch* touch, Event *_event){
    
}


