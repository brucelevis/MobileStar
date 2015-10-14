#include "ControlLayer.h"
#include "DefineHeader.h"
#include "GameDefines.h"

bool ControlLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(ControlLayer::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(ControlLayer::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(ControlLayer::onTouchesEnded, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(ControlLayer::onTouchesCancelled, this);
    
    auto dispatcher = this->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    isMiniMapTouched = false;
    pMiniMapTouch = NULL;
    
    isButtonesTouched = false;
    pButtonesTouch = NULL;

    isInfoViewTouched = false;
    pInfoViewTouch = NULL;
    
    isMenuTouched = false;
    pMenuTouch = NULL;
    
    return true;
}

void ControlLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    for (Touch* touch : touches)
    {
        float x = touch->getLocation().x;
        float y = touch->getLocation().y;
        
        if(x < 240 && y < 240) // miniMap touch
        {
            CCLOG("onTouchesBegan : miniMap");
            
            if(!isMiniMapTouched)
            {
                isMiniMapTouched = true;
                pMiniMapTouch = touch;
            }
            else
            {
                CCLOG("onTouchesBegan : miniMap - Muiti");
            }
        }
        else if(x > SCREEN_WIDTH - 240 && y < 240) // buttones touch
        {
            CCLOG("onTouchesBegan : buttones");
            if(!isButtonesTouched)
            {
                isButtonesTouched = true;
                pButtonesTouch = touch;
            }
            else
            {
                CCLOG("onTouchesBegan : buttones - Muiti");
            }
        }
        else if(x > 240 && x < SCREEN_WIDTH - 360 && y < 120) // InfoView touch
        {
            CCLOG("onTouchesBegan : InfoView");
            if(!isInfoViewTouched)
            {
                isInfoViewTouched = true;
                pInfoViewTouch = touch;
            }
            else
            {
                CCLOG("onTouchesBegan : InfoView - Muiti");
            }
            
        }
        else if(x > SCREEN_WIDTH - 360 && x < SCREEN_WIDTH - 240 && y < 120) // Menu touch
        {
            CCLOG("onTouchesBegan : Menu");
            if(!isMenuTouched)
            {
                isMenuTouched = true;
                pMenuTouch = touch;
            }
            else
            {
                CCLOG("onTouchesBegan : Menu - Muiti");
            }
        }
        else
        {
             // send to under layer
        }
    
    }
}


void ControlLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
    for (Touch* touch : touches)
    {
        if(touch == pMiniMapTouch)
        {
            CCLOG("onTouchesMoved : miniMap");
        }
        else if(touch == pButtonesTouch || touch == pInfoViewTouch || touch == pMiniMapTouch)
        {
            
        }
        else
        {
            // send to under layer
        }
    }
}


void ControlLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    for (Touch* touch : touches)
    {
        float x = touch->getLocation().x;
        float y = touch->getLocation().y;
        
        if(touch == pMiniMapTouch)
        {
            CCLOG("onTouchesEnded : miniMap");
            isMiniMapTouched = false;
            pMiniMapTouch = NULL;
        }
        else if(touch == pButtonesTouch)
        {
            isButtonesTouched = false;
            pButtonesTouch = NULL;
            
            if(x > SCREEN_WIDTH - 240 && y < 240) // buttones touch end
            {
                CCLOG("onTouchesEnded : buttones");
                
                //
            }
            else
            {
                CCLOG("onTouchesEnded : buttones - not operate");
            }
        }
        else if(touch == pInfoViewTouch)
        {
            isInfoViewTouched = false;
            pInfoViewTouch = NULL;
            
            if(x > 240 && x < SCREEN_WIDTH - 360 && y < 120) // InfoView touch end
            {
                CCLOG("onTouchesEnded : InfoView");
                
                //
            }
            else
            {
                CCLOG("onTouchesEnded : InfoView - not operate");
            }
        }
        else if(touch == pMenuTouch)
        {
            isMenuTouched = false;
            pMenuTouch = NULL;
            
            if(x > SCREEN_WIDTH - 360 && x < SCREEN_WIDTH - 240 && y < 120) // Menu touch end
            {
                CCLOG("onTouchesEnded : Menu");
                
                //
            }
            else
            {
                CCLOG("onTouchesEnded : Menu - not operate");
            }
        }
    }
}


void ControlLayer::onTouchesCancelled(const std::vector<Touch*>& touches, Event* event)
{
    CCLOG("onTouchesCancelled");
    isMiniMapTouched = false;
    pMiniMapTouch = NULL;
    
    isButtonesTouched = false;
    pButtonesTouch = NULL;
    
    isInfoViewTouched = false;
    pInfoViewTouch = NULL;
    
    isMenuTouched = false;
    pMenuTouch = NULL;
}




ControlLayer::~ControlLayer()
{
    
}

