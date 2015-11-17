#pragma once
#include "cocos2d.h"

class GameWorld;

class CameraLayer : public cocos2d::Layer{
private:
    GameWorld* m_pGameWorld;
public:
    CameraLayer(GameWorld* pGame);
    ~CameraLayer();
};