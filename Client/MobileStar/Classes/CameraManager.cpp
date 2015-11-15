#include "CameraManager.h"
#include "Thing.h"
CameraManager::CameraManager(){
    m_vCameraPos = Vec2();
    SetMovePos(0,0);
}
CameraManager::~CameraManager(){
}

CameraManager* CameraManager::Instance(){
    static CameraManager instance;
    return &instance;
}
void CameraManager::Update(float eTime){
    
    MathMgr->Clamp(m_vMovePos.x, SCREEN_WIDTH / 2, m_iWidth - SCREEN_WIDTH/2);
    MathMgr->Clamp(m_vMovePos.y, SCREEN_HEIGHT / 2, m_iHeight - SCREEN_HEIGHT/2);
    
    m_vCameraPos.x += (m_vMovePos.x - m_vCameraPos.x - SCREEN_WIDTH/2) * eTime * 5;
    m_vCameraPos.y += (m_vMovePos.y - m_vCameraPos.y - SCREEN_HEIGHT/2) * eTime * 5;
    
    m_vCameraPos.x = (float)static_cast<int>(m_vCameraPos.x);
    m_vCameraPos.y = (float)static_cast<int>(m_vCameraPos.y);
    
    MathMgr->Clamp(m_vCameraPos.x, 0, m_iWidth - SCREEN_WIDTH);
    MathMgr->Clamp(m_vCameraPos.y, 0, m_iHeight - SCREEN_HEIGHT);
}