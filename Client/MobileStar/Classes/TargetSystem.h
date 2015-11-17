#pragma once
#include <map>
#include "cocos2d.h"
USING_NS_CC;

class Thing;

class TargetSystem{
private:
    Thing* m_pOwner;
    
    Thing* m_pTarget;
    
    //어그로 Map
    typedef std::map<Thing*,int> AggroMap;
    AggroMap m_AggroMap;
public:
    TargetSystem(Thing* pOwner);
    ~TargetSystem();
    
    void Update(float eTime);
    
    void AddAggro(Thing* pThing,int aggro);
    void RemoveAggro(Thing* pThing);
    
    bool IsTargetPresent()const{return m_pTarget != NULL;}
    bool IsAggroEmpty()const{return m_AggroMap.empty();}

    Thing* GetOwner()const{return m_pOwner;}
    
    void SetTarget(Thing* pTarget){m_pTarget = pTarget;}
    Thing* GetTarget()const{return m_pTarget;}
    void ClearTarget(){m_pTarget = NULL;}
};