#pragma once
#include <vector>
#include "SparseGraph.h"
#include "PriorityQueue.h"

class Graph_SearchAStar
{
private:
    typedef GraphEdge Edge;

private:

	SparseGraph*              m_pGraph;

	//노드로 색인화된다. 그 노드까지의 실제 누적 비용을 포함한다.
	std::vector<float>             m_GCosts;

	//노드로 색인화된다. n에서 목표노드까지의 휴리스틱 비용에
    //m_GCosts[n]을 더하는 비용을 포함한다.
    //이것은 iPQ가 색인화하는 벡터이다.
	std::vector<float>             m_FCosts;

	std::vector<const Edge*>       m_ShortestPathTree;
	std::vector<const Edge*>       m_SearchFrontier;

	int                            m_iSource;
	int                            m_iTarget;

	//A* 알고리즘
	void Search();
    
    //휴리스틱 알고리즘
    float Calculate(SparseGraph* G,int nd1, int nd2){
        NavGraphNode& n1 = G->GetNode(nd1);
        NavGraphNode& n2 = G->GetNode(nd2);
        float Dist = pow(n1.getPosition().x - n2.getPosition().x,2) + pow(n1.getPosition().y - n2.getPosition().y,2);
        return Dist;
    }

public:

	Graph_SearchAStar(SparseGraph *graph,
		int   source,
		int   target) :m_pGraph(graph),
		m_ShortestPathTree(m_pGraph->NumNodes()),
		m_SearchFrontier(m_pGraph->NumNodes()),
		m_GCosts(m_pGraph->NumNodes(), 0.0),
		m_FCosts(m_pGraph->NumNodes(), 0.0),
		m_iSource(source),
		m_iTarget(target)
	{
		Search();
	}

	//알고리즘이 검사한 엣지들의 벡터를 반환한다.
	std::vector<const Edge*> GetSPT()const{ return m_ShortestPathTree; }

	//근원에서 목표까지의 최단 경로를 구성하는
    //노드 인덱스들의 벡터를 반환한다.
	std::list<int> GetPathToTarget()const;

	//목표까지의 전체 비용을 반환한다.
	float GetCostToTarget()const{ return m_GCosts[m_iTarget]; }
};

