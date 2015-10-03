#pragma once

#include <vector>
#include <list>
#include "InvertedAABBox2D.h"

template<class Entity>
struct Cell{
    //이 셀에 내재하는 모든 사물들
    std::list<Entity> Members;
    
    //셀의 경계박스
    InvertedAABBox2D BBox;
    
    Cell(Vec2 topLeft, Vec2 botRight) :BBox(topLeft, botRight){}
};


/*--------------------------------------------------------------------------------
 
 CellSpace Class
 
 --------------------------------------------------------------------------------*/
template<class Entity>
class CellSpace{
private:
    //공간 내에서 요구된 개수의 셀들
    std::vector<Cell<Entity>>			m_Cells;
    
    //이것은 어떤 사물이 자신의 인근 공간을 탐색할 때 유효한 모든 모든이웃들을 저장하기 위해 사용된다.
    std::vector<Entity>		m_Neighbors;
    
    //이 iterator는 다음 메소드가 사용할 것이고 위와 같은
    //이웃 벡터들 모두를 방문하기 시작할 것이다.
    typename std::vector<Entity>::iterator m_curNeighbor;
    
    //사물들이 내재하고 있는 세계 공간의 높이와 폭
    float m_fSpaceWidth;
    float m_fSpaceHeight;
    
    //공간이 쪼개져 만들어질 셀의 개수
    int m_iNumCellsX;
    int m_iNumCellsY;
    
    //하나의 셀의 사이즈
    float m_fCellSizeX;
    float m_fCellSizeY;
    
    //게임 공간에서의 위치가 주어지면, 이 메소드는 상대적인 셀들의 색인을 결정한다.
    int PositionToIndex(const Vec2& pos)const;
public:
    CellSpace(float width,float height,int cellX,int cellY, int maxObjects);
    
    //사물을 적절한 셀에 할당함으로써 클래스에 추가하기
    void AddObject(Entity object);
    
    //사물의 갱신 메소드로부터 이 메소드를 호출함으로써 사물 셀을 갱신하기
    void UpdateObject(const Entity& object, Vec2 OldPos);
    
    //이 메소드는 목표물의 모든 이웃들을 계산하고 이웃 벡터에 이것들을 저장한다.
    //이 메소드를 호출한 뒤 이웃 벡터들을 반복해서 방문하기 위해
    //begin, next 및 end 메소드를 사용한다.
    void CalculateNeighbors(Vec2 TargetPos,float Radius);
    
    //사물을 셀에서 지운다.
    void RemoveObject(const Entity& pObject);
    
    //이웃 벡터의 앞에 있는 사물에 대한 참조 지점을 반환한다.
    Entity& begin(){ m_curNeighbor = m_Neighbors.begin(); return *m_curNeighbor; }
    
    //이웃 벡터 내의 다음 사물에 대한 참조 지점을 반환한다.
    Entity& next(){ ++m_curNeighbor; return *m_curNeighbor; }
    
    //이웃 벡터의 끝에 도달하면 참값을 반환한다.
    bool end(){return (m_curNeighbor == m_Neighbors.end()) || (*m_curNeighbor == NULL);}
    
    //셀에 있는 모든 사물을 비운다.
    void EmptyCells();
};
