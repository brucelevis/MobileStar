#include "CellSpace.h"
#include "Thing.h"
#include "GameWorld.h"


/*--------------------------------------------------------------------------------
 
 생성자
 
 --------------------------------------------------------------------------------*/

template<class Entity>
CellSpace<Entity>::CellSpace(float width,float height,int cellX,int cellY, int maxObjects)
: m_fSpaceWidth(width)
, m_fSpaceHeight(height)
, m_iNumCellsX(cellX)
, m_iNumCellsY(cellY)
, m_Neighbors(maxObjects,NULL)
{
    m_fCellSizeX = width / cellX;
    m_fCellSizeY = height/ cellY;
    
    //셀 초기화
    for(int y=0;y<m_iNumCellsY;y++){
        for(int x=0;x<m_iNumCellsX;x++){
            float left = x*m_fCellSizeX;
            float right = left + m_fCellSizeX;
            float top = y * m_fCellSizeY;
            float bot = top + m_fCellSizeY;
            
            m_Cells.push_back(Cell<Entity>(Vec2(left, top), Vec2(right, bot)));
        }
    }
}

/*--------------------------------------------------------------------------------
 
 셀 함수
 
 --------------------------------------------------------------------------------*/
template<class Entity>
void CellSpace<Entity>::CalculateNeighbors(Vec2 TargetPos, float Radius){
    //반복자를 생성해서 이웃 벡터 리스트의 시작으로 값을 설정한다.
    typename std::vector<Entity>::iterator curNbor = m_Neighbors.begin();
    
    //목표물의 질의 지역의 경계박스인 질의 박스를 생성한다.
    InvertedAABBox2D QueryBox(TargetPos - Vec2(Radius, Radius),
                              TargetPos + Vec2(Radius, Radius));
    
    //각 셀들을 반복해서 방문해서 이것들의 경계 박스가 질의 박스와 겹치는지를
    //알아보기 위한 검사를 한다. 만약 겹치고 사물들을 포함하고 있으면,
    //근접고 검사를 추가로 실시한다.
    typename std::vector<Cell<Entity>>::iterator curCell;
    for(curCell = m_Cells.begin(); curCell != m_Cells.end(); curCell++){
        //현재 셀이 멤버들을 가지고 있는지와 질의 박스와 겹치는지를
        //알아보기 위한 검사를 한다.
        if(curCell->BBox.IsOverlappedWith(QueryBox) &&
           !curCell->Members.empty())
        {
            //질의 반경 이내에서 발견된 모든 사물들을 이웃 벡터 리스트에 추가한다.
            typename std::list<Entity>::iterator iter = curCell->Members.begin();
            for( ;iter!= curCell->Members.end();iter++){
                if(Vec2DistanceSq((*iter)->getPosition(),TargetPos) <
                   Radius*Radius)
                {
                    *curNbor++ = *iter;
                }
            }
        }
    }
    //벡터 리스트의 끝을 NULL값으로 표시한다.
    *curNbor = NULL;
}

/*--------------------------------------------------------------------------------
 
 셀을 비운다.
 
 --------------------------------------------------------------------------------*/

template<class Entity>
void CellSpace<Entity>::EmptyCells(){
    typename std::vector<Cell<Entity>>::iterator iter = m_Cells.begin();
    
    for( ; iter != m_Cells.end(); iter++){
        iter->Members.clear();
    }
}

/*--------------------------------------------------------------------------------
 
 해담 지점을 인덱스로 돌린다.
 
 --------------------------------------------------------------------------------*/

template<class Entity>
int CellSpace<Entity>::PositionToIndex(const Vec2& pos)const{
    int idx = (int)(pos.x / m_fCellSizeX)
    +((int)(pos.y / m_fCellSizeY) * m_iNumCellsX);
    
    if( idx > (int)m_Cells.size()-1 ) idx = (int)m_Cells.size() - 1;
        
    return idx;
}


/*--------------------------------------------------------------------------------
 
 해당 사물을 셀에 추가한다.
 
 --------------------------------------------------------------------------------*/
template<class Entity>
void CellSpace<Entity>::AddObject(Entity object){
    
    int idx = PositionToIndex(object->getPosition());
    m_Cells[idx].Members.push_back(object);
}

/*--------------------------------------------------------------------------------
 
 사물의 갱신 메소드로부터 이 메소드를 호출함으로써 사물 셀을 갱신하기
 
 --------------------------------------------------------------------------------*/
template<class Entity>
void CellSpace<Entity>::UpdateObject(const Entity& object,Vec2 OldPos){
    int OldIdx = PositionToIndex(OldPos);
    int NewIdx = PositionToIndex(object->getPosition());
    
    if(NewIdx == OldIdx) return;
    
    m_Cells[OldIdx].Members.remove(object);
    m_Cells[NewIdx].Members.push_back(object);
}
/*--------------------------------------------------------------------------------
 
 사물을 셀에서 지운다.
 
 --------------------------------------------------------------------------------*/
template<class Entity>
void CellSpace<Entity>::RemoveObject(const Entity& pObject){
    int idx = PositionToIndex(pObject->getPosition());
    m_Cells[idx].Members.remove(pObject);
}

template class CellSpace<NavGraphNode*>;
template class CellSpace<Thing*>;