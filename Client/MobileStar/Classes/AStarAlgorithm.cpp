#include "AStarAlgorithm.h"

void Graph_SearchAStar::Search()
{
    //노드들의 색인화된 우선 순위 큐를 생성한다.
    //그 큐는 낮은 F비용(F=G+H)을 가진 노드에게 우선순위를 줄 것이다.
    IndexedPriorityQLow pq(m_FCosts, m_pGraph->NumNodes());
    
    //큐에 근원 노드를 삽입한다.
    pq.insert(m_iSource);
    
    //큐가 비어있지 않는 한
    while (!pq.empty())
    {
        //큐에서 최소 비용의 노드를 가져온다.
        int NextClosestNode = pq.Pop();
        
        //변방에서 이 노드를 스패닝 트리로 이동한다.
        m_ShortestPathTree[NextClosestNode] = m_SearchFrontier[NextClosestNode];
        
        //목표를 찾았으면 종료한다.
        if (NextClosestNode == m_iTarget) return;
        
        //이제 이 노드에 연결된 모든 엣지들을 검사한다.
        SparseGraph::EdgeIterator EdgeItr(*m_pGraph, NextClosestNode);
        
        for (Edge* pE = EdgeItr.begin();
             !EdgeItr.end();
             pE = EdgeItr.next())
        {
            //이 노드에서 목표(H)까지의 휴리스틱 비용을 계산한다.
            float HCost = Calculate(m_pGraph, m_iTarget, pE->GetTo());
            
            //근원 (G)dptj 이 노드까지의 실제 비용을 계산한다.
            float GCost = m_GCosts[NextClosestNode] + pE->GetCost();
            
            //노드가 변방에 추가되지 않았다면, 추가하고 G와 F비용을 갱신한다.
            if (m_SearchFrontier[pE->GetTo()] == NULL)
            {
                m_FCosts[pE->GetTo()] = GCost + HCost;
                m_GCosts[pE->GetTo()] = GCost;
                
                pq.insert(pE->GetTo());
                
                m_SearchFrontier[pE->GetTo()] = pE;
            }
            
            //만약 이 노드가 이미 변방에 있지만
            //이 방법으로 여기에 도달하는 비용이 이전에 찾은 것보다 작다면
            //노드 비용과 변방을 적절하게 갱신한다.
            else if ((GCost < m_GCosts[pE->GetTo()]) && (m_ShortestPathTree[pE->GetTo()] == NULL))
            {
                m_FCosts[pE->GetTo()] = GCost + HCost;
                m_GCosts[pE->GetTo()] = GCost;
                
                pq.ChangePriority(pE->GetTo());
                
                m_SearchFrontier[pE->GetTo()] = pE;
            }
        }
    }
}

//-----------------------------------------------------------------------------
std::list<int> Graph_SearchAStar::GetPathToTarget()const
{
    std::list<int> path;
    
    //just return an empty path if no target or no path found
    if (m_iTarget < 0)  return path;
    
    int nd = m_iTarget;
    
    path.push_front(nd);
    
    while ((nd != m_iSource) && (m_ShortestPathTree[nd] != 0))
    {
        nd = m_ShortestPathTree[nd]->GetFrom();
        
        path.push_front(nd);
    }
    
    return path;
}