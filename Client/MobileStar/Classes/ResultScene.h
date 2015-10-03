#ifndef __RESULT_SCENE_H__
#define __RESULT_SCENE_H__

#include "cocos2d.h"

class ResultScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);
	void clickLoginBtn(cocos2d::Ref* pSender);

	CREATE_FUNC(ResultScene);
};

#endif //__FRONT_SCENE_H__
