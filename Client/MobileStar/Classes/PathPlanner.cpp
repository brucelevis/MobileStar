#include "PathPlanner.h"
#include "GameWorld.h"
#include "SparseGraph.h"
USING_NS_CC;

int PathPlanner::GetClosestNodeToPosition(cocos2d::Vec2 pos)const{
    double ClosestSoFar = MathMgr->MaxDouble;
    int    ClosestNode  = -1;
    
    //셀 이웃 탐색 Range를 얻어온다.
    const float range = m_pOwner->GetGameWorld()->GetMap()->GetCellSpaceNeighborhoodRange();
    
    //셀 공간에서 이웃들을 계산한다.
    m_pOwner->GetGameWorld()->GetMap()->GetCellSpace()->CalculateNeighbors(pos, range);
    
    //이웃들을 순회하면서 가까운 노드를 찾는다.
//    for (NavGraphNode* pN = m_pOwner->GetGameWorld()->GetMap()->GetCellSpace()->begin();
//         !m_pOwner->GetGameWorld()->GetMap()->GetCellSpace()->end();
//         pN = m_pOwner->GetGameWorld()->GetMap()->GetCellSpace()->next())
    SparseGraph::NodeIterator NodeItr(*m_pNavGraph);
    for (NavGraphNode* pN=NodeItr.begin();!NodeItr.end();pN=NodeItr.next())
    {
        float dist = Vec2DistanceSq(pos, pN->getPosition());
        
        //가장 가까운 거리와 노드 인덱스를 저장해둔다.
        if (dist < ClosestSoFar)
        {
            ClosestSoFar = dist;
            ClosestNode  = pN->GetIndex();
        }
    
    }
    
    return ClosestNode;
}


PathPlanner::PathPlanner(Unit* owner)
: m_pOwner(owner)
, m_pNavGraph(&owner->GetGameWorld()->GetMap()->GetNavGraph())
{
    
}
bool PathPlanner::CreatePathToPosition(int TargetIndex){
    //목표 위치에 대한 기록을 남긴다.
    m_iDestinationIndex = TargetIndex;
    
    //유닛이 서있는 타일 인덱스를 가져온다.
    int ClosestNodeToUnit = m_pOwner->GetTileIndex();
    
    //아무런 가시 노드도 찾을 수 없으면 실패를 반환한다.
    //유닛이 갖힌 경우이다.
    if(ClosestNodeToUnit == -1)
        return false;
    
    //목표로부터 가지 노드를 찾을 때 문제가 생기면 실패를 반환한다.
    //이런 종류의 일은 위에서 보다 자주 발생한다.
    //예를 들어, 사용자가 벽으로 둘러싸인 지역이나 객체의 내부를 클릭하는 경우이다.
    if(TargetIndex == -1)
        return false;
    
    //만약 목적 타일 인덱스가 비어있지 않다면, 그 주변의 유효화된 타일 인덱스를 찾아 이동한다.
    if(!m_pNavGraph->GetNode(TargetIndex).IsEmpty()){
        Vec2 pos = m_pNavGraph->GetNode(TargetIndex).getPosition();
        int Index = m_pOwner->GetGameWorld()->GetMap()->GetClosestValidNodeFromPosition(pos);
        
        if(Index == -1)
            return false;
        else
            TargetIndex = Index;
    }
    
    //유닛이 가장 가까운 노드와 목표 위치에 가장 가까운 노드 사이의 경로를 찾기위해
    //A* 탐색 클래스의 인스턴스를 생성한다.
    //이 A* 탐색은 유클리디언 직선 휴리스틱을 이용할 것이다.
    Graph_SearchAStar Search(m_pNavGraph, ClosestNodeToUnit, TargetIndex);
    
    //경로를 잡는다.
    m_Path = Search.GetPathToTarget();
    
    //탐색이 성공이면 참을 반환한다.
    if(!m_Path.empty()){
        m_Path.pop_front();
        return true;
    }
    else
        return false;
}
