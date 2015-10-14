
#ifndef __MINI_MAP_LAYER_H__
#define __MINI_MAP_LAYER_H__

#include "cocos2d.h"
#include "BasicPacket.h"

USING_NS_CC;


class MiniMapLayer : public cocos2d::Layer
{
public:
    virtual bool init();

    ~MiniMapLayer();
  
	CREATE_FUNC(MiniMapLayer);
    
    
    
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesCancelled(const std::vector<Touch*>& touches, Event* event);
};

#endif //__CONTROL_LAYER_H__