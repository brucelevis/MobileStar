#pragma once

template <class Entity>
class State{
public:
    virtual ~State(){}
public:
    virtual void Begin(Entity* entity) = 0;
    virtual void Update(Entity* entity, float eTime) = 0;
    virtual void End(Entity* entity) = 0;
    
    virtual bool TouchBegan(Entity* entity, Touch* touch, Event* _event) = 0;
    virtual void TouchMoved(Entity* entity, Touch* touch, Event* _event) = 0;
    virtual void TouchCancelled(Entity* entity, Touch* touch, Event* _event) = 0;
    virtual void TouchEnded(Entity* entity, Touch* touch, Event* _event) = 0;
};