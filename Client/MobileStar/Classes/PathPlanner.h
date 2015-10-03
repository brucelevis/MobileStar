#pragma once
#include <list>
#include "cocos2d.h"
class Unit;
class SparseGraph;

class PathPlanner
{
private:

public:
    
private:
    
    //이 클래스 소유자에 대한 포인터
    Unit* m_pOwner;
    
    //Graph에 대한 포인터
    SparseGraph* m_pNavGraph;
    
    //Unit이 도달할 경로를 계획하기 위한 위치
    cocos2d::Vec2 m_vDestinationPos;
    
    //길 찾기 list
    std::list<cocos2d::Vec2> m_Path;
    
    //주어진위치까지 가장 가깝고 보이면서 방해받지 않는 그래프 노드의
    //인덱스를 반환한다. 발견되는 것이 없으면 -1을 반환한다.
    int   GetClosestNodeToPosition(cocos2d::Vec2 pos)const;
    
public:
    
    PathPlanner(Unit* owner);
    
    //유닛의 위치와 목표 위치 사이의 최소 비용 경로를 찾는다.
    //탐색이 성공적이어서 참을 반환하면 경로점들로 경로를 채운다.
    //실패하면 거짓을 반환한다.
    bool CreatePathToPosition(cocos2d::Vec2 TargetPos);
    
    std::list<cocos2d::Vec2>& GetPath(){return m_Path;}
    
    cocos2d::Vec2 GetDestination()const{return m_vDestinationPos;}
    void SetDestination(cocos2d::Vec2 NewPos){m_vDestinationPos = NewPos;}

};
