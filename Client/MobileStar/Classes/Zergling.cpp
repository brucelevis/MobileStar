#include "Zergling.h"
#include "GameWorld.h"


Zergling::Zergling(GameWorld* pGame, int playerFlag, int tileIndex)
: Unit(pGame,playerFlag, tileIndex,35,0,2,1,6,1)
{
    char buf[128];
    sprintf(buf, "Texture/Zergling/%s/%s/Ani_00.png",StrMgr->GetPlayerFlagStr(playerFlag),StrMgr->GetUnitDirStr(10));
    m_pSprite = Sprite::create(buf);
    addChild(m_pSprite);
    
    SetTileIndex(tileIndex);
}
Zergling::~Zergling(){
}

void Zergling::update(float eTime){
    Unit::update(eTime);
    
}

bool Zergling::TouchBegan(Touch* touch, Event* _event){
    Unit::TouchBegan(touch,_event);
    return true;
}
void Zergling::TouchMoved(Touch* touch, Event* _event){
    Unit::TouchMoved(touch, _event);
}
void Zergling::TouchCancelled(Touch* touch, Event* _event){
    Unit::TouchCancelled(touch, _event);
}
void Zergling::TouchEnded(Touch* touch, Event *_event){
    Unit::TouchEnded(touch, _event);
}