#include "Thing.h"

Thing::Thing(GameWorld* pGame, Object_Type type, int zPosition)
 :m_pGameWorld(pGame)
 , m_ObjectType(type)
 ,m_bErase(false)
 ,m_zPosition(zPosition)
{
}
Thing::~Thing(){
}

void Thing::update(float eTime){
	Layer::update(eTime);
}

bool Thing::TouchBegan(Touch* touch, Event* _event){
	
	return true;
}
void Thing::TouchMoved(Touch* touch, Event* _event){

}
void Thing::TouchCancelled(Touch* touch, Event* _event){
	TouchEnded(touch, _event);
}
void Thing::TouchEnded(Touch* touch, Event *_event){

}