#pragma once
#include <vector>
#include <string>
#include <list>
#include <vector>
#include "SparseGraph.h"
#include "cocos2d.h"
#include "Unit.h"
#include "CellSpace.h"
#include "TileNode.h"

class GameMap : public cocos2d::Layer
{
private:
    //그래프
    SparseGraph* m_pNavGraph;
    
    //셀 공간
    CellSpace<NavGraphNode*>* m_pCellSpace;
    
    //타일
    std::vector<TileNode*> m_Tiles;
    
    //이웃 탐색을 위한 셀 공간 탐색 Range
    float  m_fCellSpaceNeighborhoodRange;
    
    //타일의 갯수
    int m_iTileX;
    int m_iTileY;
    
    void  PartitionNavGraph();
    
    //미리계산된 그래프 탐색 길 비용
    std::vector<std::vector<float> >  m_PathCosts;
    
    //클리어 함수
    void Clear();
    
public:
    GameMap();
    ~GameMap();
    
    //파일에서 맵을 읽어온다.
    bool LoadMap(int tileX,int tileY);
    
    //두 노드 사이의 비용을 계산한다.
    float   CalculateCostToTravelBetweenNodes(int nd1, int nd2)const;
    
    //해당 노드의 인덱스 좌표를 얻어온다.
    int GetTileIndexFromPosition(const Vec2& position);
    
    //해당 위치로부터 가장 가까운 유효화된 노드 인덱스를 구한다.
    int GetClosestValidNodeFromPosition(Vec2 pos);
    
    //해당 타일 인덱스로부터 해당 거리만큼 안에있는 타일 인덱스들의 리스트를 반환한다.
    std::list<int> GetIndicesFromTileIndex(int iTileIndex, int iDistance);
    
    //게터 세터 함수
    SparseGraph&                       GetNavGraph()const{return *m_pNavGraph;}
    CellSpace<NavGraphNode*>* const    GetCellSpace()const{return m_pCellSpace;}
    
    int                                GetTileX()const{return m_iTileX;}
    int                                GetTileY()const{return m_iTileY;}
    float                              GetCellSpaceNeighborhoodRange()const{return m_fCellSpaceNeighborhoodRange;}
    
};
