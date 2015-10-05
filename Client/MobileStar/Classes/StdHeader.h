#pragma once

#include "cocos2d.h"
#include "DefineHeader.h"
#include "MathManager.h"
#include "CameraManager.h"
#include "NetworkManager.h"
#include "IDManager.h"

USING_NS_CC;

//충돌함수
inline bool IsOverlaped(Vec2 pos, Sprite* sprite){
	Vec2 spritePos = sprite->getPosition();
	Size spriteSize = sprite->getContentSize();

	if (pos.x > spritePos.x - spriteSize.width / 2 &&
		pos.x < spritePos.x + spriteSize.width / 2 &&
		pos.y > spritePos.y - spriteSize.height / 2 &&
		pos.y < spritePos.y + spriteSize.height / 2)
	{
		return true;
	}
	return false;
}