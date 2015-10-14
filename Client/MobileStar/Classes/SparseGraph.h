#pragma once

#include <vector>
#include <list>
#include <cassert>
#include <string>
#include <iostream>

#include "NavGraphNode.h"
#include "GraphEdge.h"

class SparseGraph
{
public:
    typedef std::vector<NavGraphNode>   NodeVector;
    typedef std::list<GraphEdge>     EdgeList;
    typedef std::vector<EdgeList>    EdgeListVector;
private:
    
    //이 그래프를 구성하는 노드들
    NodeVector      m_Nodes;
    
    //인접 엣지 리스트의 벡터(각 노드 인덱스는 그 노드와 관련된 엣지의 리스트로 키화된다.
    EdgeListVector  m_Edges;
    
    //추가될 다음 노드 인덱스
    int             m_iNextNodeIndex;
    
    
    //추가하려는 엣지가 이미 있는 엣지인가?
    bool  UniqueEdge(int from, int to)const;
    
    //모든 엣지를 순환한 후 무효화되어있는 노드를 포함하고 있는 엣지를 제거한다.
    void  CullInvalidEdges();
    
    //타일의 갯수
    int m_iTileX;
    int m_iTileY;

    
public:
    
    SparseGraph(int tileX, int tileY) : m_iNextNodeIndex(0), m_iTileX(tileX), m_iTileY(tileY) {}
    
    //노드를 반환한다.
    NavGraphNode&  GetNode(int idx);
    
    //엣지를 반환한다.
    GraphEdge& GetEdge(int from, int to);
    
    //모든 노드가 존재하는 시점에서, 해당 함수를 실행하면 노드 8방향으로 엣지를 잇는다.
    void AddAllEdgeFromPresentNode();
    
    //해당 노드로부터 8방향으로 엣지를 업데이트한다.
    void UpdateEdgeFromNode(int nodeIndex);
    
    //다음으로 비어있는 노드 인덱스를 반환한다.
    int   GetNextFreeNodeIndex()const{ return m_iNextNodeIndex; }
    
    //노드를 추가하고 그 인덱스를 반환한다.
    int   AddNode(NavGraphNode node);
    
    //해당 인덱스의 노드를 제거한다
    void  RemoveNode(int node);
    
    //엣지를 추가한다.
    void  AddEdge(GraphEdge edge);
    
    //엣지를 제거한다.
    void  RemoveEdge(int from, int to);
    
    //해당 엣지의 비용을 설정한다.
    void  SetEdgeCost(int from, int to, float cost);
    
    //활성화된 노드, 비활성화된 노드 모두의 갯수를 반환한다.
    int   NumNodes()const{ return (int)m_Nodes.size(); }
    
    //활성화된 노드의 갯수를 반환한다.
    int   NumActiveNodes()const
    {
        int count = 0;
        
        for (unsigned int n = 0; n<m_Nodes.size(); ++n) if (m_Nodes[n].GetIndex() != -1) ++count;
        
        return count;
    }
    
    //엣지의 갯수를 반환한다.
    int   NumEdges()const
    {
        int tot = 0;
        
        for (EdgeListVector::const_iterator curEdge = m_Edges.begin();
             curEdge != m_Edges.end();
             ++curEdge)
        {
            tot += curEdge->size();
        }
        
        return tot;
    }
    
    //노드가 비었나?
    bool	IsEmpty()const{ return m_Nodes.empty(); }
    
    //이 인덱스의 노드가 존재하나?
    bool IsNodePresent(int nd);
    
    //해당 엣지가 존재하나?
    bool IsEdgePresent(int from, int to);
    
    //그래프를 비운다.
    void Clear(){ m_iNextNodeIndex = 0; m_Nodes.clear(); m_Edges.clear(); }
    
    void RemoveEdges()
    {
        for (EdgeListVector::iterator it = m_Edges.begin(); it != m_Edges.end(); ++it)
        {
            it->clear();
        }
    }
    
    //Edge Iterator 클래스
    class EdgeIterator
    {
    private:
        
        EdgeList::iterator curEdge;
        
        SparseGraph&  G;
        
        const int NodeIndex;
        
    public:
        
        EdgeIterator(SparseGraph& graph,int node): G(graph), NodeIndex(node)
        {
            curEdge = G.m_Edges[NodeIndex].begin();
        }
        
        GraphEdge*  begin()
        {
            curEdge = G.m_Edges[NodeIndex].begin();
            
            return &(*curEdge);
        }
        
        GraphEdge*  next()
        {
            ++curEdge;
            
            if (end()) return NULL;
            
            return &(*curEdge);
            
        }
        
        bool end()
        {
            return (curEdge == G.m_Edges[NodeIndex].end());
        }
    };
    
    friend class EdgeIterator;
    
    class NodeIterator
    {
    private:
        
        NodeVector::iterator         curNode;
        
        SparseGraph&    G;

        void GetNextValidNode(NodeVector::iterator& it)
        {
            if ( curNode == G.m_Nodes.end() || it->GetIndex() != -1) return;
            
            while ( (it->GetIndex() == -1) )
            {
                ++it;
                
                if (curNode == G.m_Nodes.end()) break;
            }
        }
        
    public:
        
        NodeIterator(SparseGraph &graph):G(graph)
        {
            curNode = G.m_Nodes.begin();
        }
        
        
        NavGraphNode* begin()
        {
            curNode = G.m_Nodes.begin();
            
            GetNextValidNode(curNode);
            
            return &(*curNode);
        }
        
        NavGraphNode* next()
        {
            ++curNode;
            
            if (end()) return NULL;
            
            GetNextValidNode(curNode);
            
            return &(*curNode);
        }
        
        bool end()
        {
            return (curNode == G.m_Nodes.end());
        }
    };
    
    friend class NodeIterator;
};
