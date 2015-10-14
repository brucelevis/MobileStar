#include "MiniMapLayer.h"


bool MiniMapLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(MiniMapLayer::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(MiniMapLayer::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(MiniMapLayer::onTouchesEnded, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(MiniMapLayer::onTouchesCancelled, this);
    
    auto dispatcher = this->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    setContentSize(Size(200, 200));
    
    return true;
}

MiniMapLayer::~MiniMapLayer()
{
    
}


void MiniMapLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    CCLOG("onTouchesBegan");
    
}


void MiniMapLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
    //   CCLOG("onTouchesMoved");
    
}


void MiniMapLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    CCLOG("onTouchesEnded");
    
}


void MiniMapLayer::onTouchesCancelled(const std::vector<Touch*>& touches, Event* event)
{
    CCLOG("onTouchesCancelled");
    
}