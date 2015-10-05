#pragma once
#include "IDManager.h"
class GraphNode{
protected:
    //모든 노드는 인덱스를 가진다. 유효한 인덱스는 0보다 크거나 같다.
    int m_iIndex;
public:
    GraphNode():m_iIndex(IDMgr->GetNextNodeID())
    {
    }
    
    virtual ~GraphNode(){}
    
    int GetIndex()const{return m_iIndex;}
    void SetIndex(int idx){m_iIndex = idx;}
    
};
