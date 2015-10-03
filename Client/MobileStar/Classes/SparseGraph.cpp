#include "SparseGraph.h"

bool SparseGraph::IsNodePresent(int nd)
{
    if ((nd >= (int)m_Nodes.size() || (m_Nodes[nd].GetIndex() == -1)))
    {
        return false;
    }
    else return true;
}

bool SparseGraph::IsEdgePresent(int from, int to)
{
    if (IsNodePresent(from) && IsNodePresent(to))
    {
        for (EdgeList::iterator curEdge = m_Edges[from].begin();
             curEdge != m_Edges[from].end();
             ++curEdge)
        {
            if (curEdge->GetTo() == to) return true;
        }
        
        return false;
    }
    else return false;
}

//노드의 인덱스를 받아 노드를 받아온다.
NavGraphNode& SparseGraph::GetNode(int idx)
{
    assert((idx < (int)m_Nodes.size()) &&
           (idx >= 0) &&
           "<SparseGraph::GetNode>: invalid index");
    
    return m_Nodes[idx];
}


//엣지를 반환한다.
GraphEdge& SparseGraph::GetEdge(int from, int to)
{
    assert((from < m_Nodes.size()) &&
           (from >= 0) &&
           m_Nodes[from].GetIndex() != -1 &&
           "<SparseGraph::GetEdge>: invalid 'from' index");
    
    assert((to < m_Nodes.size()) &&
           (to >= 0) &&
           m_Nodes[to].GetIndex() != -1 &&
           "<SparseGraph::GetEdge>: invalid 'to' index");
    
    for (EdgeList::iterator curEdge = m_Edges[from].begin();
         curEdge != m_Edges[from].end();
         ++curEdge)
    {
        if (curEdge->GetTo() == to) return *curEdge;
    }
    
    assert(0 && "<SparseGraph::GetEdge>: edge does not exist");
}

//엣지를 추가한다.
void SparseGraph::AddEdge(GraphEdge edge)
{
    //엣지의 양끝이 유효한 노드인지 확인한다.
    assert((edge.GetFrom() < m_iNextNodeIndex) && (edge.GetTo() < m_iNextNodeIndex) &&
           "<SparseGraph::AddEdge>: invalid node index");
    
    //유효한 노드이면
    if ((m_Nodes[edge.GetTo()].GetIndex() != -1) &&
        (m_Nodes[edge.GetFrom()].GetIndex() != -1))
    {
        //유니크한 엣지인지 확인한다.
        if (UniqueEdge(edge.GetFrom(), edge.GetTo()))
        {
            m_Edges[edge.GetFrom()].push_back(edge);
        }
        
        
        //거꾸로 방향도 확인한다.
        if (UniqueEdge(edge.GetTo(), edge.GetFrom()))
        {
            GraphEdge NewEdge = edge;
            
            NewEdge.SetTo(edge.GetFrom());
            NewEdge.SetFrom(edge.GetTo());
            
            m_Edges[edge.GetTo()].push_back(NewEdge);
        }
        
    }
}


//엣지를 제거한다.
void SparseGraph::RemoveEdge(int from, int to)
{
    //유효한 노드 인덱스인지 확인한다.
    assert((from < (int)m_Nodes.size()) && (to < (int)m_Nodes.size()) &&
           "<SparseGraph::RemoveEdge>:invalid node index");
    
    EdgeList::iterator curEdge;
    
    
    for (curEdge = m_Edges[to].begin();
         curEdge != m_Edges[to].end();
         ++curEdge)
    {
        if (curEdge->GetTo() == from){ curEdge = m_Edges[to].erase(curEdge); break; }
    }
    
    
    for (curEdge = m_Edges[from].begin();
         curEdge != m_Edges[from].end();
         ++curEdge)
    {
        if (curEdge->GetTo() == to){ curEdge = m_Edges[from].erase(curEdge); break; }
    }
}

//모든 노드가 존재하는 시점에서, 해당 함수를 실행하면 노드 8방향으로 엣지를 잇는다.
void SparseGraph::AddAllEdgeFromPresentNode(int cellX,int cellY){
    for(int i=0;i<cellY;i++){
        for(int j=0;j<cellX;j++){
            int nodeIndex = i * cellX + j;
            
            //위
            if( nodeIndex >= cellX ){
                int nodeUp = nodeIndex - cellX;
                GraphEdge newEdge(nodeIndex, nodeUp,71.552);
                AddEdge(newEdge);
            }

            //오른쪽
            if( nodeIndex % cellX < cellX - 1){
                int nodeRight = nodeIndex + 1;
                GraphEdge newEdge(nodeIndex, nodeRight,71.552);
                AddEdge(newEdge);
            }
            
            //왼쪽
            if( nodeIndex % cellX > 0 ){
                int nodeLeft = nodeIndex - 1;
                GraphEdge newEdge(nodeIndex,nodeLeft,71.552);
                AddEdge(newEdge);
            }
            
            //아래
            if( nodeIndex / cellX < cellY - 1 ){
                int nodeDown = nodeIndex + cellX;
                GraphEdge newEdge(nodeIndex,nodeDown,71.552);
                AddEdge(newEdge);
            }
            
            //오른쪽 위
            if( nodeIndex >= cellX && nodeIndex % cellX < cellX -1){
                int nodeUpRight = nodeIndex - cellX + 1;
                GraphEdge newEdge(nodeIndex,nodeUpRight, 64);
                AddEdge(newEdge);
            }
            
            //오른쪽 아래
            if(nodeIndex % cellX < cellX -1 && nodeIndex / cellX < cellY -1){
                int nodeDownRight = nodeIndex + cellX + 1;
                GraphEdge newEdge(nodeIndex,nodeDownRight, 128);
                AddEdge(newEdge);
            }
            
            //왼쪽 위
            if( nodeIndex >= cellX && nodeIndex % cellX > 0){
                int nodeUpLeft = nodeIndex - cellX - 1;
                GraphEdge newEdge(nodeIndex,nodeUpLeft, 128);
                AddEdge(newEdge);
            }
            
            //왼쪽 아래
            if( nodeIndex % cellX > 0 && nodeIndex / cellX < cellY - 1){
                int nodeDownLeft = nodeIndex + cellX - 1;
                GraphEdge newEdge(nodeIndex,nodeDownLeft, 64);
                AddEdge(newEdge);
            }
        }
    }
}

//노드를 추가한다.
int SparseGraph::AddNode(NavGraphNode node)
{
    if (node.GetIndex() < (int)m_Nodes.size())
    {
        //같은 인덱스를 가진 노드가 이미 있는지 확인한다.
        assert(m_Nodes[node.GetIndex()].GetIndex() == -1 &&
               "<SparseGraph::AddNode>: Attempting to add a node with a duplicate ID");
        
        m_Nodes[node.GetIndex()] = node;
        
        return m_iNextNodeIndex;
    }
    
    else
    {
        //노드 리스트 맨 뒤에 추가하는지 확인한다.
        assert(node.GetIndex() == m_iNextNodeIndex && "<SparseGraph::AddNode>:invalid index");
        
        m_Nodes.push_back(node);
        m_Edges.push_back(EdgeList());
        
        return m_iNextNodeIndex++;
    }
}

//무효화된 노드를 포함하고 있는 엣지들을 모두 지운다.
void SparseGraph::CullInvalidEdges()
{
    for (EdgeListVector::iterator curEdgeList = m_Edges.begin(); curEdgeList != m_Edges.end(); ++curEdgeList)
    {
        for (EdgeList::iterator curEdge = (*curEdgeList).begin(); curEdge != (*curEdgeList).end(); ++curEdge)
        {
            if (m_Nodes[curEdge->GetTo()].GetIndex() == -1 ||
                m_Nodes[curEdge->GetFrom()].GetIndex() == -1)
            {
                curEdge = (*curEdgeList).erase(curEdge);
            }
        }
    }
}


//노드를 지운다.
void SparseGraph::RemoveNode(int node)
{
    assert(node < (int)m_Nodes.size() && "<SparseGraph::RemoveNode>: invalid node index");
    
    //노드를 무효화한다.
    m_Nodes[node].SetIndex(-1);
    
    //각각의 노드를 순회하며 엣지를 지운다.
    for (EdgeList::iterator curEdge = m_Edges[node].begin();
         curEdge != m_Edges[node].end();
         ++curEdge)
    {
        for (EdgeList::iterator curE = m_Edges[curEdge->GetTo()].begin();
             curE != m_Edges[curEdge->GetTo()].end();
             ++curE)
        {
            if (curE->GetTo() == node)
            {
                curE = m_Edges[curEdge->GetTo()].erase(curE);
                
                break;
            }
        }
    }
    
    //마지막으로 해당 노드에서부터 뻗어있는 엣지를 지운다.
    m_Edges[node].clear();
}

//해당 엣지의 비용을 설정한다.
void SparseGraph::SetEdgeCost(int from, int to, float NewCost)
{
    //유효한 노드인지 확인한다.
    assert((from < (int)m_Nodes.size()) && (to < (int)m_Nodes.size()) &&
           "<SparseGraph::SetEdgeCost>: invalid index");
    
    //visit each neighbour and erase any edges leading to this node
    for (EdgeList::iterator curEdge = m_Edges[from].begin();
         curEdge != m_Edges[from].end();
         ++curEdge)
    {
        if (curEdge->GetTo() == to)
        {
            curEdge->SetCost(NewCost);
            break;
        }
    }
}

//해당 엣지가 유일한 엣지인지 확인한다.
bool SparseGraph::UniqueEdge(int from, int to)const
{
    for (EdgeList::const_iterator curEdge = m_Edges[from].begin();
         curEdge != m_Edges[from].end();
         ++curEdge)
    {
        if (curEdge->GetTo() == to)
        {
            return false;
        }
    }
    
    return true;
}
