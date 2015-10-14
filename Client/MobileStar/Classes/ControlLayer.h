
#ifndef __CONTROL_LAYER_H__
#define __CONTROL_LAYER_H__

#include "cocos2d.h"
#include "BasicPacket.h"

USING_NS_CC;


class ControlLayer : public cocos2d::Layer
{
public:
    virtual bool init();

    ~ControlLayer();
    
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesCancelled(const std::vector<Touch*>& touches, Event* event);
    
  	CREATE_FUNC(ControlLayer);
    
    
private:
    
    bool isMiniMapTouched;
    Touch* pMiniMapTouch;
    
    bool isButtonesTouched;
    Touch* pButtonesTouch;
    
    bool isInfoViewTouched;
    Touch* pInfoViewTouch;
    
    bool isMenuTouched;
    Touch* pMenuTouch;
    
    Size miniMap;
    Size buttones;
    Size button;
};

#endif //__CONTROL_LAYER_H__