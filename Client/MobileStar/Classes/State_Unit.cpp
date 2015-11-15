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
    return true;
}
void State_Move::TouchMoved(Unit* unit, Touch* touch, Event* _event){
    
}
void State_Move::TouchCancelled(Unit* unit, Touch* touch, Event* _event){
    
}
void State_Move::TouchEnded(Unit* unit, Touch* touch, Event *_event){
    
}

/*----------------------------------------------------------------------------
 
 State_Attack
 
 ----------------------------------------------------------------------------*/
State_Attack* State_Attack::Instance(){
    static State_Attack instance;
    return &instance;
}

void State_Attack::Begin(Unit* unit){
    char buf[128];
    sprintf(buf,"Zergling%s%sAttack",StrMgr->GetPlayerFlagStr(unit->GetPlayerFlag()),StrMgr->GetUnitDirStr(unit->m_iDir));
    unit->m_pSprite->stopAllActions();
    auto pAnimate = Animate::create(AnimationCache::getInstance()->getAnimation(buf));
    auto pForever = RepeatForever::create(pAnimate);
    unit->m_pSprite->runAction(pForever);
}
void State_Attack::Update(Unit* unit,float eTime){
    //애니메이션 설정
    
    if(unit->m_iPrevDir != unit->m_iDir){
        char buf[128];
        sprintf(buf,"Zergling%s%sAttack",StrMgr->GetPlayerFlagStr(unit->GetPlayerFlag()),StrMgr->GetUnitDirStr(unit->m_iDir));
        unit->m_pSprite->stopAllActions();
        auto pAnimate = Animate::create(AnimationCache::getInstance()->getAnimation(buf));
        auto pForever = RepeatForever::create(pAnimate);
        unit->m_pSprite->runAction(pForever);
    }
}
void State_Attack::End(Unit* unit){
}
bool State_Attack::TouchBegan(Unit* unit, Touch* touch, Event* _event){
    return true;
}
void State_Attack::TouchMoved(Unit* unit, Touch* touch, Event* _event){
    
}
void State_Attack::TouchCancelled(Unit* unit, Touch* touch, Event* _event){
    
}
void State_Attack::TouchEnded(Unit* unit, Touch* touch, Event *_event){
    
}



