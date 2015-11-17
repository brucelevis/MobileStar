#pragma once
#include "Thing.h"
#include "StateMachine.h"
#include "PathPlanner.h"

class GameWorld;

class Unit : public Thing{
    
protected:
    //유닛의 방향 구조체
    struct Dir{
        enum{
            Left,
            LeftLeftUp,
            LeftUp,
            LeftUpUp,
            Up,
            RightUpUp,
            RightUp,
            RightRightUp,
            Right,
            RightRightDown,
            RightDown,
            RightDownDown,
            Down,
            LeftDownDown,
            LeftDown,
            LeftLeftDown
        };
    };
    
public:
    //유닛의 방향을 담은 변수
    int m_iPrevDir;
    int m_iDir;
protected:
    
    
    
    /*----------------------------------- AI관련 변수 --------------------------------*/
    
    //유한 상태 기계 머신
    StateMachine<Unit>* m_pFSM;
    
    //길 찾기 플래너
    PathPlanner* m_pPathPlanner;
    
    //유닛이 바라보는 방향
    Vec2 m_vHeading;
    
    
    /*-----------------------------------------------------------------------------------*/
    
    /*----------------------------------- Network관련 변수 --------------------------------*/
    
    //현재 등록된 AutoTaskPacket;
    int m_iAutoTaskPacket;
    
    /*-----------------------------------------------------------------------------------*/
    
    Sprite* m_pSprite;
    
    Animation m_pAnimation[16];
    
public:
    Unit(GameWorld* pGame, int playerFlag, int tileIndex, int hp, int mp, float speed, float attackSpeed, int dmg, int armor);
    ~Unit();
    
    void update(float eTime);
    void updateDir();
    
    /*								터치 관련 함수								*/
    
    bool TouchBegan(Touch* touch, Event* _event);
    void TouchMoved(Touch* touch, Event* _event);
    void TouchEnded(Touch* touch, Event *_event);
    
    /*								유틸리티 함수								*/
    
    //현재 Path.front에 저장되어 있는 타일 인덱스로 이동한다. 만약 Path가 비어있으면 false를 반환한다.
    bool MoveToPathFront(int CurrentPacket);
    
    //AutoTaskMove의 packet 값이 유효한지?
    bool IsValidAutoTask(int packet){ return m_iAutoTaskPacket == packet; }
    
    void AttackTarget(int CurrentPacket);
    
    void FindTarget();
    
    /*								게터세터 함수								*/
    
    
    
    
    Vec2	GetHeading()const{ return m_vHeading; }
    void	SetHeading(Vec2 new_heading);
    
    void    SetAutoTaskPacket(int packet){ m_iAutoTaskPacket = packet;}
    int     GetAutoTaskPacket()const{return m_iAutoTaskPacket;}
    
    StateMachine<Unit>* GetFSM()const{return m_pFSM;}
    
    PathPlanner* GetPathPlanner()const{return m_pPathPlanner; }
    
    friend class State_Idle;
    friend class State_Move;
    friend class State_Attack;
    
};

inline bool operator<(const Unit& c1, const Unit& c2){
    return c1.getPosition().y < c1.getPosition().y;
}