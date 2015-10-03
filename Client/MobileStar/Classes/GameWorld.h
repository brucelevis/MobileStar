#pragma once
#include "StdHeader.h"
#include "Marine.h"
#include "AStarAlgorithm.h"
#include "GameMap.h"

class GameWorld : public cocos2d::Layer
{
private:
    //카메라 레이어
    Layer* m_pCameraLayer;
    
    //UI 레이어
    Layer* m_pUILayer;
    
    //유닛들
    Vector<Unit*> m_Units;
    
    GameMap* m_pMap;
    
    Sprite* m_pTile[30][30];
public:
    static cocos2d::Scene* createScene();
    
    GameWorld();
    ~GameWorld();
    
    void onEnter();
    
    CREATE_FUNC(GameWorld);
    
    void update(float eTime);
    
    void updateNetwork(float eTime);
    
    bool TouchBegan(Touch* touch, Event* _event);
    void TouchMoved(Touch* touch, Event* _event);
    void TouchCancelled(Touch* touch, Event* _event);
    void TouchEnded(Touch* touch, Event* _event);
    
    GameMap* GetMap()const{return m_pMap;}
    
    Layer* GetCameraLayer()const{ return m_pCameraLayer; }
    Layer* GetUILayer()const{ return m_pUILayer; }
    
    Vector<Unit*>& GetUnits(){return m_Units;}
};
