#pragma once
#include "StdHeader.h"
#include "State.h"


class Unit;

class State_Idle : public State<Unit>{
private:
    State_Idle(){}
    
    //복사 생성자 막기
    State_Idle(const State_Idle&);
    State_Idle& operator=(const State_Idle&);
public:
    ~State_Idle(){}
    
    static State_Idle* Instance();
    
    void Begin(Unit* unit);
    void Update(Unit* unit,float eTime);
    void End(Unit* unit);
    
    bool TouchBegan(Unit* unit, Touch* touch, Event* _event);
    void TouchMoved(Unit* unit, Touch* touch, Event* _event);
    void TouchCancelled(Unit* unit, Touch* touch, Event* _event);
    void TouchEnded(Unit* unit, Touch* touch, Event *_event);
};

class State_Move : public State<Unit>{
private:
    State_Move(){}
    
    //복사 생성자 막기
    State_Move(const State_Move&);
    State_Move& operator=(const State_Move&);
    
public:
    ~State_Move(){}
    
    static State_Move* Instance();
    
    void Begin(Unit* unit);
    void Update(Unit* unit,float eTime);
    void End(Unit* unit);
    
    bool TouchBegan(Unit* unit, Touch* touch, Event* _event);
    void TouchMoved(Unit* unit, Touch* touch, Event* _event);
    void TouchCancelled(Unit* unit, Touch* touch, Event* _event);
    void TouchEnded(Unit* unit, Touch* touch, Event *_event);
};


class State_Attack : public State<Unit>{
private:
    State_Attack(){}
    
    //복사 생성자 막기
    State_Attack(const State_Attack&);
    State_Attack& operator=(const State_Attack&);
    
public:
    ~State_Attack(){}
    
    static State_Attack* Instance();
    
    void Begin(Unit* unit);
    void Update(Unit* unit,float eTime);
    void End(Unit* unit);
    
    bool TouchBegan(Unit* unit, Touch* touch, Event* _event);
    void TouchMoved(Unit* unit, Touch* touch, Event* _event);
    void TouchCancelled(Unit* unit, Touch* touch, Event* _event);
    void TouchEnded(Unit* unit, Touch* touch, Event *_event);
};


