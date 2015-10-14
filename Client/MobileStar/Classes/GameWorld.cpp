#include "GameWorld.h"
#include "AStarAlgorithm.h"
#include "GameClient.h"

Scene* GameWorld::createScene()
{
    auto scene = Scene::create();
    
    auto layer = GameWorld::create();
    
    scene->addChild(layer);
    
    return scene;
}

GameWorld::GameWorld()
: m_fStartFrame(0)
{
    //로그 찍을 것인가?
    LogMgr->SetLog(true);
    
    //카메라 레이어 초기화
    m_pCameraLayer = Layer::create();
    addChild(m_pCameraLayer, 0);
    
    //UI 레이어 초기화
    m_pUILayer = Layer::create();
    addChild(m_pUILayer, 1);
    
    //맵 생성
    m_pMap = new GameMap();
    //m_pMap->LoadMap(TILE_WIDTH_NUM * TILE_SIZE / NODE_SIZE, TILE_HEIGHT_NUM * TILE_SIZE / NODE_SIZE);
    m_pMap->LoadMap(30,20);
    m_pCameraLayer->addChild(m_pMap);
    
    //마린 생성
    {
        auto pMarine = new Marine(this,0,134);
        pMarine->setColor(Color3B(255,0,0));
        pMarine->setCascadeColorEnabled(pMarine);
        m_pCameraLayer->addChild(pMarine);
        m_Units[pMarine->GetID()] = pMarine;
    }
    
    
    {
        auto pMarine = new Marine(this,1,105);
        m_pCameraLayer->addChild(pMarine);
        m_Units[pMarine->GetID()] = pMarine;
    }
    {
        auto pMarine = new Marine(this,1,135);
        m_pCameraLayer->addChild(pMarine);
        m_Units[pMarine->GetID()] = pMarine;
    }
    {
        auto pMarine = new Marine(this,1,164);
        m_pCameraLayer->addChild(pMarine);
        m_Units[pMarine->GetID()] = pMarine;
    }
    {
        auto pMarine = new Marine(this,1,165);
        m_pCameraLayer->addChild(pMarine);
        m_Units[pMarine->GetID()] = pMarine;
    }
    {
        auto pMarine = new Marine(this,1,163);
        m_pCameraLayer->addChild(pMarine);
        m_Units[pMarine->GetID()] = pMarine;
    }
    {
        auto pMarine = new Marine(this,1,133);
        m_pCameraLayer->addChild(pMarine);
        m_Units[pMarine->GetID()] = pMarine;
    }
    {
        auto pMarine = new Marine(this,1,103);
        m_pCameraLayer->addChild(pMarine);
        m_Units[pMarine->GetID()] = pMarine;
    }
    {
        auto pMarine = new Marine(this,1,104);
        m_pCameraLayer->addChild(pMarine);
        m_Units[pMarine->GetID()] = pMarine;
    }
    
    
    //카메라 설정 : 맵 크기 (64*256, 64*256)
    CameraMgr->SetScreen(TILE_WIDTH_SIZE*TILE_WIDTH_NUM, TILE_HEIGHT_SIZE*TILE_HEIGHT_NUM);
    CameraMgr->SetMovePos(0,100);
    
    //네트워크 매니저 초기화
    NetMgr->SetGameWorld(this);
    NetMgr->SetupWhatPlayerFlag();
    
    LogMgr->Log("This is %d Computer",NetMgr->GetPlayerFlag());

    CameraMgr->SetMovePos(Vec2(0,500));
    

    //디버그용
    m_pDebugLabel = Label::createWithTTF("","fonts/arial.ttf",24);
    m_pDebugLabel->setAnchorPoint(Vec2(0,0));
    m_pDebugLabel->setPosition(0,120);
    addChild(m_pDebugLabel);

    
    //Scene setting
    
    GameClient::GetInstance().currentScene = GAME_SCENE_NOW;

}

GameWorld::~GameWorld(){
}

void GameWorld::onEnter(){
    Layer::onEnter();
    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(GameWorld::TouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameWorld::TouchMoved, this);
    listener->onTouchCancelled = CC_CALLBACK_2(GameWorld::TouchCancelled, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameWorld::TouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    scheduleUpdate();
    
    //schedule(schedule_selector(GameWorld::updateNetwork),1 / (float)NETWORK_FPS);
}

void GameWorld::update(float eTime){
    //게임 시작 초 계산
    m_fStartFrame += eTime;
    
    //카메라 업데이트
    CameraMgr->Update(eTime);
    m_pCameraLayer->setPosition(-CameraMgr->GetPos());
    
    //업데이트 네트워크
    int CntUpdateNetwork = (int)(m_fStartFrame * NETWORK_FPS) - NetMgr->GetCntCarryOutMessages();
    while(CntUpdateNetwork-- > 0){
        updateNetwork(eTime);
    }
    
    //유닛 업데이트
    for(auto pUnit : m_Units){
        pUnit.second->update(eTime);
    }
}

//1초에 4번 실행된다.
void GameWorld::updateNetwork(float eTime){
    //DispatchTask에 쌓인 메시지를 서버로 보낸다.
    NetMgr->DispatchToServer();
    
    //Task에 쌓인 메시지를 처리한다.
    NetMgr->CarryOutMessages();
    
    //디버그용
    char buf[256];
    sprintf(buf,"FirstPacket %d, SecondPacket %d\nFirstTaskSize %d, SecondTaskSize %d",
            NetMgr->GetFirstTaskPacket(),NetMgr->GetSecondTaskPacket(),
            NetMgr->GetFirstTaskSize(),NetMgr->GetSecondTaskSize());
    m_pDebugLabel->setString(buf);
}
bool GameWorld::TouchBegan(Touch* touch, Event* _event){
    for(auto pUnit : m_Units){
        pUnit.second->TouchBegan(touch, _event);
    }
    
    return true;
}
void GameWorld::TouchMoved(Touch* touch, Event* _event){
    for(auto pUnit : m_Units){
        pUnit.second->TouchMoved(touch, _event);
    }
}
void GameWorld::TouchCancelled(Touch* touch, Event* _event){
    for(auto pUnit : m_Units){
        pUnit.second->TouchCancelled(touch, _event);
    }
    
    TouchEnded(touch, _event);
}
void GameWorld::TouchEnded(Touch* touch, Event* _event){
    for(auto pUnit : m_Units){
        pUnit.second->TouchEnded(touch, _event);
    }
}