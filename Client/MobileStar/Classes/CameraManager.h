#pragma once
#include "cocos2d.h"
#define CameraMgr CameraManager::Instance()
USING_NS_CC;
class Thing;

class CameraManager{
private:
    //카메라의 현재 위치
    Vec2 m_vCameraPos;
    
    //카메라가 이동해야 할 위치
    Vec2 m_vMovePos;
    
    //화면의 크기
    int m_iWidth;
    int m_iHeight;
    
public:
    CameraManager();
    ~CameraManager();
    
    static CameraManager* Instance();
    
    void Update(float eTime);
public:
    Vec2 GetPos()const{ return m_vCameraPos; }
//    void SetPos(Vec2 pos){ m_vCameraPos = pos; }
//    template<typename T>
//    void SetPos(T x,T y){m_vCameraPos.x = static_cast<float>(x); m_vCameraPos.y = static_cast<float>(y);}
    
    
    Vec2 GetMovePos()const{return m_vMovePos;}
    void SetMovePos(Vec2 pos){ m_vMovePos = pos; }
    template<typename T>
    void SetMovePos(T x,T y){m_vMovePos.x = static_cast<float>(x); m_vMovePos.y = static_cast<float>(y);}
    
    void AddMovePos(Vec2 pos){ m_vMovePos += pos; }
    template<typename T>
    void AddMovePos(T x,T y){m_vMovePos.x += static_cast<float>(x); m_vMovePos.y += static_cast<float>(y);}

    
    int GetWidth()const{return m_iWidth;}
    template<typename T>
    void SetWidth(T width){m_iWidth = static_cast<int>(width);}
    
    int GetHeight()const{return m_iHeight;}
    template<typename T>
    void SetHeight(T height){m_iHeight = static_cast<int>(height);}
    
    template<typename T>
    void SetScreen(T width,T height){m_iWidth = static_cast<int>(width); m_iHeight = static_cast<int>(height);}
};