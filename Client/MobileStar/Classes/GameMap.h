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
    
    TileNode** tileMap;
    
    //이웃 탐색을 위한 셀 공간 탐색 Range
    float  m_fCellSpaceNeighborhoodRange;
    
    //타일의 갯수
    int m_iTileX;
    int m_iTileY;
    
    //노드의 갯수
    int m_iNodeX;
    int m_iNodeY;
    
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
    
    float   CalculateCostToTravelBetweenNodes(int nd1, int nd2)const;
    
    //게터 세터 함수
    SparseGraph&                       GetNavGraph()const{return *m_pNavGraph;}
    CellSpace<NavGraphNode*>* const    GetCellSpace()const{return m_pCellSpace;}
   
    int                                GetTileX()const{return m_iTileX;}
    int                                GetTileY()const{return m_iTileY;}
    
    int                                GetNodeX()const{return m_iNodeX;}
    int                                GetNodeY()const{return m_iNodeY;}
    float                              GetCellSpaceNeighborhoodRange()const{return m_fCellSpaceNeighborhoodRange;}
    
};
