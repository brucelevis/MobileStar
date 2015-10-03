#pragma once
#include "cocos2d.h"

USING_NS_CC;

class TileNode : public Layer{
private:
    //타일의 스프라이트
    Sprite* m_pTileSprite;
public:
    TileNode();
    ~TileNode();
};