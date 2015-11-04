#pragma once
#include "Unit.h"

class GameWorld;
USING_NS_CC;

class Marine : public Unit{
private:
public:
	Marine(GameWorld* pGame, int playerFlag, int tileIndex);
	~Marine();

	void update(float eTime);

	/*							터치관련 함수							*/
	bool TouchBegan(Touch* touch, Event* _event);
	void TouchMoved(Touch* touch, Event* _event);
	void TouchCancelled(Touch* touch, Event* _event);
	void TouchEnded(Touch* touch, Event *_event);

	/*								유틸리티 함수								*/


	/*								게터세터 함수								*/

};
