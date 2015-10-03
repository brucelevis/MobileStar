#pragma once

#include "cocos2d.h"
USING_NS_CC;

class InvertedAABBox2D{
private:
    Vec2 m_vBottomLeft;
    Vec2 m_vTopRight;
    
    Vec2 m_vCenter;
public:
    InvertedAABBox2D(Vec2 bl, Vec2 tr)
    : m_vBottomLeft(bl)
    , m_vTopRight(tr)
    , m_vCenter((bl+tr)/2.0f)
    {}
    
//    bool IsOverlappedWith(const InvertedAABBox2D& other)const{
//        return !((other.Top() < this->Bottom()) ||
//                 (other.Bottom() > this->Top()) ||
//                 (other.Left() > this->Right()) ||
//                 (other.Right() < this->Left()));
//    }

    bool IsOverlappedWith(const InvertedAABBox2D& other)const
    {
        return !((other.Top() > this->Bottom()) ||
                 (other.Bottom() < this->Top()) ||
                 (other.Left() > this->Right()) ||
                 (other.Right() < this->Left()));
    }
    
    Vec2 BottomLeft()const{ return m_vBottomLeft; }
    Vec2 TopRight()const{ return m_vTopRight; }
    
    float	Top()const{return m_vBottomLeft.y;}
    float   Left()const{return m_vBottomLeft.x;}
    float   Bottom()const{return m_vTopRight.y;}
    float   Right()const{return m_vTopRight.x;}
    Vec2 Center()const{ return m_vCenter; }
};