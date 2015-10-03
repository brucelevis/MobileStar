#include "GameMap.h"
#include "NavGraphNode.h"

//Static 멤버 변수 선언
int GraphNode::NodeID;

//생성자
GameMap::GameMap()
: m_pNavGraph(NULL)
, m_pCellSpace(NULL)
, m_iTileX(0)
, m_iTileY(0)
, m_iNodeX(0)
, m_iNodeY(0)
, m_fCellSpaceNeighborhoodRange(1024)
{
    
}

//소멸자
GameMap::~GameMap()
{
    Clear();
}


//맵에서 로드된 것들을 삭제한다.
void GameMap::Clear()
{
    //NavGraph를 제거한다.
    delete m_pNavGraph;
    
    //CellSpace를 제거한다.
    delete m_pCellSpace;
}


//맵을 불러온다.
bool GameMap::LoadMap(int tileX,int tileY)
{
    Clear();
    
    m_pNavGraph = new SparseGraph();
    
    //맵을 임시로 생성한다.
    
    m_iTileX = tileX;
    m_iTileY = tileY;
    m_iNodeX = tileX;
    m_iNodeY = tileY;
    
    //타일 생성
    for(int i=0;i<m_iTileY;i++){
        for(int j=0;j<m_iTileX;j++){
            auto tile = new TileNode();
            tile->setPosition(Vec2((j-i) * TILE_WIDTH_SIZE / 2,(j+i) * TILE_HEIGHT_SIZE / 2));
            addChild(tile);
            m_Tiles.push_back(tile);
        }
    }
    
    //노드 생성
    GraphNode::InitialNodeID();
    for(int i=0;i<m_iNodeY;i++){
        for(int j=0;j<m_iNodeX;j++){
            NavGraphNode node;
            node.setPosition(Vec2((j-i) * TILE_WIDTH_SIZE / 2,(j+i) * TILE_HEIGHT_SIZE / 2));
            m_pNavGraph->AddNode(node);
        }
    }
    
    m_pNavGraph->AddAllEdgeFromPresentNode( m_iNodeX, m_iNodeY);
    
    PartitionNavGraph();
    
    return false;
}

//미리 계산되어 있는 비용 테이블에서 비용을 가져온다.
float
GameMap::CalculateCostToTravelBetweenNodes(int nd1, int nd2)const
{
    assert (nd1>=0 && nd1<m_pNavGraph->NumNodes() &&
            nd2>=0 && nd2<m_pNavGraph->NumNodes() &&
            "<GameMap::CalculateCostToTravelBetweenNodes>: invalid index");
    
    return m_PathCosts[nd1][nd2];
}


int GameMap::GetTileIndexFromPosition(const Vec2& position){
    Vec2 MovePos = position + Vec2(0,TILE_HEIGHT_SIZE/2);
    int tileX = (MovePos.x / (TILE_WIDTH_SIZE/2) + MovePos.y / (TILE_HEIGHT_SIZE/2)) / 2;
    int tileY = (MovePos.y / (TILE_HEIGHT_SIZE/2) - (MovePos.x / (TILE_WIDTH_SIZE/2))) / 2;
    
    return tileY * m_iTileX + tileX;
}

//공간을 분할한다.
void GameMap::PartitionNavGraph()
{
    if (m_pCellSpace) delete m_pCellSpace;
    
    m_pCellSpace = new CellSpace<NavGraphNode*>(TILE_WIDTH_SIZE * TILE_WIDTH_NUM, TILE_HEIGHT_SIZE * TILE_HEIGHT_NUM, TILE_WIDTH_NUM / 4, TILE_HEIGHT_NUM / 4, m_pNavGraph->NumNodes());
    
    //셀 공간에 노드를 집어 넣는다.
    SparseGraph::NodeIterator NodeItr(*m_pNavGraph);
    for (NavGraphNode* pN=NodeItr.begin();!NodeItr.end();pN=NodeItr.next())
    {
        m_pCellSpace->AddObject(pN);
    }
}

