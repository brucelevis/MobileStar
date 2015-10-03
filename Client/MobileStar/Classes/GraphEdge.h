#pragma once

class GraphEdge{
protected:
    int m_iFrom;
    int m_iTo;
    
    float m_fCost;
    
public:
    GraphEdge(int from,int to, float cost)
    : m_iFrom(from)
    , m_iTo(to)
    , m_fCost(cost)
    {}
    
    GraphEdge(int from,int to)
    : m_iFrom(from)
    , m_iTo(to)
    , m_fCost(1.0f)
    {}
    
    virtual ~GraphEdge(){}
    
    int GetFrom()const{return m_iFrom;}
    void SetFrom(int from){m_iFrom = from;}
    
    int GetTo()const{return m_iTo;}
    void SetTo(int to){m_iTo = to;}
    
    float GetCost()const{return m_fCost;}
    void SetCost(float cost){m_fCost = cost;}
};