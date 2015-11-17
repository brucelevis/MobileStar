#pragma once
#include "StdHeader.h"
#include "Zergling.h"
#include "AStarAlgorithm.h"
#include "GameMap.h"
#include <map>

class GameWorld : public cocos2d::Layer
{
private:
    //카메라 레이어
    Layer* m_pCameraLayer;
    
    //UI 레이어
    Layer* m_pUILayer;
    
    //유닛들
    std::map<int,Unit*> m_Units;
    
    //게임 맵
    GameMap* m_pMap;
    
    //게임이 시작된 초
    float m_fStartFrame;
    
    //터치 상태
    int m_iTouchCnt;
    bool m_bDoubleTouch;
    
    //터치를 시작한 포지션
    Vec2 m_vTouchPosition;
    Vec2 m_vFirstTouchPosition;
    
    //내가 현재 담고 있는 유닛
    std::list<Unit*> m_TouchedUnits;
    
    //유닛을 선택할때 나오는 초록색 사각형
    DrawNode* m_pSelectDrawNode;
    Rect m_SelectRect;
    
    //디버그용
    Label* m_pDebugLabel;
    
    //초기화
    void Init();
public:
    static cocos2d::Scene* createScene();
    
    GameWorld();
    ~GameWorld();
    
    void onEnter();
    
    CREATE_FUNC(GameWorld);
    
    void update(float eTime);
    
    void updateNetwork(float eTime);
    
    bool TouchesBegan(const std::vector<Touch*>& touches, Event* _event);
    void TouchesMoved(const std::vector<Touch*>& touches, Event* _event);
    void TouchesCancelled(const std::vector<Touch*>& touches, Event* _event);
    void TouchesEnded(const std::vector<Touch*>& touches, Event* _event);
    
    //해당 유닛의 주변 타일에 있는 유닛들의 리스트를 가져온다
    std::list<Thing*> GetNearThings(Thing* pOwner,int iDistance);
    
    //게터세터
    
    GameMap* GetMap()const{return m_pMap;}
    
    Layer* GetCameraLayer()const{ return m_pCameraLayer; }
    Layer* GetUILayer()const{ return m_pUILayer; }
    
    std::map<int,Unit*>& GetUnits(){return m_Units;}
    
    float GetStartFrame()const{return m_fStartFrame;}
    void SetStartFrame(int _frame){m_fStartFrame = _frame;}
};
