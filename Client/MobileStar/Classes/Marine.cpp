#include "Marine.h"
#include "GameWorld.h"


Marine::Marine(GameWorld* pGame, int playerFlag, int tileIndex)
 : Unit(pGame,playerFlag, tileIndex,100,0,1.5,1,6,1)
{
	m_pSprite = Sprite::create("Texture/Player.png");
	addChild(m_pSprite);

}
Marine::~Marine(){
}

void Marine::update(float eTime){
	Unit::update(eTime);

}

bool Marine::TouchBegan(Touch* touch, Event* _event){
    Unit::TouchBegan(touch,_event);
	return true;
}
void Marine::TouchMoved(Touch* touch, Event* _event){
    Unit::TouchMoved(touch, _event);
}
void Marine::TouchCancelled(Touch* touch, Event* _event){
    Unit::TouchCancelled(touch, _event);
}
void Marine::TouchEnded(Touch* touch, Event *_event){
    Unit::TouchEnded(touch, _event);
}