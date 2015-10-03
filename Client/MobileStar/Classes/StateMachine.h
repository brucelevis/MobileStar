#pragma once
#include "State.h"

template <class Entity>
class StateMachine{
private:
    //상태기계의 주인
    Entity* m_pOwner;
    
    //현재 상태
    State<Entity>* m_pCurrentState;
    //이전 상태
    State<Entity>* m_pPreviousState;
    //글로벌 상태 (항상 진행하고 있는 상태)
    State<Entity>* m_pGlobalState;
    
    StateMachine(const StateMachine&);
    StateMachine& operator=(const StateMachine&);
public:
    StateMachine(Entity* owner)
    : m_pOwner(owner)
    , m_pCurrentState(NULL)
    , m_pPreviousState(NULL)
    , m_pGlobalState(NULL)
    {
    }
    virtual ~StateMachine(){}
    
    //상태 설정
    void SetCurrentState(State<Entity>* s){ m_pCurrentState = s; }
    void SetGlobalState(State<Entity>* s){ m_pGlobalState = s; }
    void SetPreviousState(State<Entity>* s){ m_pPreviousState = s; }
    
    void Update(float eTime)const{
        if (m_pGlobalState)	m_pGlobalState->Update(m_pOwner, eTime);
        
        if (m_pCurrentState) m_pCurrentState->Update(m_pOwner, eTime);
    }
    
    bool TouchBegan(Touch* touch, Event* _event){
        if (m_pGlobalState)	m_pGlobalState->TouchBegan(m_pOwner, touch, _event);
        
        if (m_pCurrentState) m_pCurrentState->TouchBegan(m_pOwner, touch, _event);
        
        return true;
    }
    void TouchMoved(Touch* touch, Event* _event){
        if (m_pGlobalState)	m_pGlobalState->TouchMoved(m_pOwner, touch, _event);
        
        if (m_pCurrentState) m_pCurrentState->TouchMoved(m_pOwner, touch, _event);
    }
    void TouchCancelled(Touch* touch, Event* _event){
        if (m_pGlobalState)	m_pGlobalState->TouchCancelled(m_pOwner, touch, _event);
        
        if (m_pCurrentState) m_pCurrentState->TouchCancelled(m_pOwner, touch, _event);
    }
    void TouchEnded(Touch* touch, Event* _event){
        if (m_pGlobalState)	m_pGlobalState->TouchEnded(m_pOwner, touch, _event);
        
        if (m_pCurrentState) m_pCurrentState->TouchEnded(m_pOwner, touch, _event);
    }
    
    void ChangeState(State<Entity>* pNewState){
        m_pPreviousState = m_pCurrentState;
        
        if (m_pCurrentState)
            m_pCurrentState->End(m_pOwner);
        
        m_pCurrentState = pNewState;
        
        m_pCurrentState->Begin(m_pOwner);
    }
    
    void RevertToPreviousState(){
        ChangeState(m_pPreviousState);
    }
    
    State<Entity>*  CurrentState()  const{ return m_pCurrentState; }
    State<Entity>*  GlobalState()   const{ return m_pGlobalState; }
    State<Entity>*  PreviousState() const{ return m_pPreviousState; }
    
};