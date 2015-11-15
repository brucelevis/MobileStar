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
, m_bDoubleTouch(false)
, m_iTouchCnt(0)
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
    m_pMap->LoadMap(20,20);
    m_pCameraLayer->addChild(m_pMap);
    
    //저글링 생성
    for(int i=0;i<12;i++){
        //auto pZergling = new Zergling(this,0,(DIVIDE_NODE ? 205+i : 114+i));
        auto pZergling = new Zergling(this,0,(DIVIDE_NODE ? 660+i : 330+i));
        //auto pZergling = new Zergling(this,0,(DIVIDE_NODE ? 20800+i : 114+i));
        pZergling->setColor(Color3B(255,0,0));
        pZergling->setCascadeColorEnabled(true);
        m_pCameraLayer->addChild(pZergling);
        m_Units[pZergling->GetID()] = pZergling;
    }
    
    for(int i=0;i<3;i++){
        auto pZergling = new Zergling(this,1,(DIVIDE_NODE ? 540+i : 270+i));
        m_pCameraLayer->addChild(pZergling);
        m_Units[pZergling->GetID()] = pZergling;
    }
    
    //선택 DrawNode 생성
    m_pSelectDrawNode = DrawNode::create();
    m_pSelectDrawNode->setVisible(false);
    m_pCameraLayer->addChild(m_pSelectDrawNode);
    
    //카메라 설정 : 맵 크기 (64*256, 64*256)
    CameraMgr->SetScreen(TILE_WIDTH_SIZE*TILE_WIDTH_NUM, TILE_HEIGHT_SIZE*TILE_HEIGHT_NUM);
    //CameraMgr->SetScreen(6400,3200 - 32);
    
    //네트워크 매니저 초기화
    NetMgr->SetGameWorld(this);
    NetMgr->SetupWhatPlayerFlag();
    
    LogMgr->Log("This is %d Computer",NetMgr->GetPlayerFlag());

    //디버그용
    m_pDebugLabel = Label::createWithTTF("","fonts/arial.ttf",24);
    m_pDebugLabel->setAnchorPoint(Vec2(0,0));
    m_pDebugLabel->setPosition(0,120);
    addChild(m_pDebugLabel);

    //초기화
    Init();
    
//    for(int i=0;i<12;i++){
//        m_TouchedUnits.push_back(m_Units[i]);
//    }

    //Scene setting
    GameClient::GetInstance().currentScene = GAME_SCENE_NOW;
    
    //타일 스캔
//    {
//        auto pDrawNode = DrawNode::create();
//            for(int i=0;i<=720;i++){
//                for(int j=0;j<1280;j++){
//                    if(m_pMap->GetTileIndexFromPosition(Vec2(j,i)) == 1){
//                        pDrawNode->drawDot(Vec2(j,i), 1, Color4F(1,1,1,1));
//                    }
//                }
//            }
//        pDrawNode->setTag(5);
//        addChild(pDrawNode);
//    }
}

GameWorld::~GameWorld(){
}

void GameWorld::onEnter(){
    Layer::onEnter();
    
    auto listener = EventListenerTouchAllAtOnce::create();
    

    
    listener->onTouchesBegan = CC_CALLBACK_2(GameWorld::TouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(GameWorld::TouchesMoved, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(GameWorld::TouchesCancelled, this);
    listener->onTouchesEnded = CC_CALLBACK_2(GameWorld::TouchesEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    scheduleUpdate();
}

void GameWorld::Init(){
    char buf[128];
    
    //저글링 아이들 애니메이션
    for(int i=0;i<2;i++){
        for(int j=0;j<16;j++){
            Animation* ani = Animation::create();
            for(int k=0;k<1;k++){
                sprintf(buf,"Texture/Zergling/%s/%s/Ani_%02d.png",StrMgr->GetPlayerFlagStr(i),StrMgr->GetUnitDirStr(j),k);
                ani->addSpriteFrameWithFile(buf);
            }
            ani->setDelayPerUnit(0.3f);
            
            sprintf(buf,"Zergling%s%sIdle",StrMgr->GetPlayerFlagStr(i),StrMgr->GetUnitDirStr(j));
            AnimationCache::getInstance()->addAnimation(ani,buf);
        }
    }
    
    //저글링 이동 애니메이션
    for(int i=0;i<2;i++){
        for(int j=0;j<16;j++){
            Animation* ani = Animation::create();
            for(int k=0;k<7;k++){
                sprintf(buf,"Texture/Zergling/%s/%s/Ani_%02d.png",StrMgr->GetPlayerFlagStr(i),StrMgr->GetUnitDirStr(j),k);
                ani->addSpriteFrameWithFile(buf);
            }
            ani->setDelayPerUnit(0.05f);
            
            sprintf(buf,"Zergling%s%sRun",StrMgr->GetPlayerFlagStr(i),StrMgr->GetUnitDirStr(j));
            AnimationCache::getInstance()->addAnimation(ani,buf);
        }
    }
    
    //저글링 공격 애니메이션
    for(int i=0;i<2;i++){
        for(int j=0;j<16;j++){
            Animation* ani = Animation::create();
            for(int k=7;k<12;k++){
                sprintf(buf,"Texture/Zergling/%s/%s/Ani_%02d.png",StrMgr->GetPlayerFlagStr(i),StrMgr->GetUnitDirStr(j),k);
                ani->addSpriteFrameWithFile(buf);
            }
            ani->setDelayPerUnit(0.05f);
            
            sprintf(buf,"Zergling%s%sAttack",StrMgr->GetPlayerFlagStr(i),StrMgr->GetUnitDirStr(j));
            AnimationCache::getInstance()->addAnimation(ani,buf);
        }
    }
}

void GameWorld::update(float eTime){
    //게임 시작 초 계산
    if(NetMgr->GetFirstTaskPacket() < NetMgr->GetSecondTaskPacket() + 3)
        m_fStartFrame += eTime;
    
    //카메라 업데이트
    CameraMgr->Update(eTime);
    //m_pCameraLayer->setPosition(-CameraMgr->GetPos());
    //m_pCameraLayer->setPosition(-CameraMgr->GetPos() + (Vec2(3200,-1600)));
    m_pCameraLayer->setPosition(-CameraMgr->GetPos() + (Vec2(1280,640)));
    
    //네트워크 시간초 동기화 : 4초마다 시간을 동기화 해준다.
//    int CntSynchTime = (int)(m_fStartFrame * 0.25) - NetMgr->GetCntSynchTime();
//    while(CntSynchTime-- > 0){
//        NetMgr->SynchTime();
//    }
    
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
    sprintf(buf,"FirstPacket %d, SecondPacket %d\nFirstTaskSize %d, SecondTaskSize %d\nFirstFront %d, SecondFront %d\nTouchCnt %d",
            NetMgr->GetFirstTaskPacket(),NetMgr->GetSecondTaskPacket(),
            NetMgr->GetFirstTaskSize(),NetMgr->GetSecondTaskSize(),
            NetMgr->GetFirstFront(),NetMgr->GetSecondFront(),
            m_iTouchCnt);
    m_pDebugLabel->setString(buf);
}
bool GameWorld::TouchesBegan(const std::vector<Touch*>& touches, Event* _event){
    m_iTouchCnt += touches.size();
    
    printf("Index : %d\n",m_pMap->GetTileIndexFromPosition(m_pCameraLayer->convertToNodeSpace(touches.front()->getLocation())));
    
    if(m_iTouchCnt == 2){
        m_bDoubleTouch = true;
    }else{
        if(m_iTouchCnt == 1)
            m_vFirstTouchPosition = touches.front()->getLocation();
        
        m_bDoubleTouch = false;
    }
    
    m_vTouchPosition = touches.front()->getLocation();
    
    for(auto pUnit : m_Units){
        for(auto pTouch : touches){
            pUnit.second->TouchBegan(pTouch, _event);
        }
        
    }
    
    return true;
}
void GameWorld::TouchesMoved(const std::vector<Touch*>& touches, Event* _event){
    
    //만약 터치를 한 개만 하였다면
    if(m_iTouchCnt == 1 && !m_bDoubleTouch){
        Vec2 MoveVec = m_vTouchPosition - touches.front()->getLocation();
        
        CameraMgr->AddMovePos(MoveVec);
        m_vTouchPosition = touches.front()->getLocation();
    }else if(m_iTouchCnt == 2){
        Vec2 v1 = m_pCameraLayer->convertToNodeSpace(touches.front()->getLocation());
        Vec2 v2 = m_pCameraLayer->convertToNodeSpace(touches.back()->getLocation());
        
        m_SelectRect = Rect( MathMgr->Min(v1.x,v2.x), MathMgr->Min(v1.y,v2.y),
                            MathMgr->Max(v1.x,v2.x)-MathMgr->Min(v1.x,v2.x),
                            MathMgr->Max(v1.y,v2.y)-MathMgr->Min(v1.y,v2.y) );
        
        m_pSelectDrawNode->setVisible(true);
        m_pSelectDrawNode->clear();
        m_pSelectDrawNode->drawRect(Vec2(m_SelectRect.origin.x,m_SelectRect.origin.y),
                                    Vec2(m_SelectRect.origin.x + m_SelectRect.size.width,
                                         m_SelectRect.origin.y + m_SelectRect.size.height),
                                    Color4F(46/256.0f,169/256.0f,15/256.0f,1));
        
    }
    
    for(auto pUnit : m_Units){
        for(auto pTouch : touches){
            pUnit.second->TouchMoved(pTouch, _event);
        }
    }
}
void GameWorld::TouchesCancelled(const std::vector<Touch*>& touches, Event* _event){
    TouchesEnded(touches, _event);
}
void GameWorld::TouchesEnded(const std::vector<Touch*>& touches, Event* _event){
    //만약 터치를 한 개만 하였다면
    if(m_iTouchCnt == 1){
        Vec2 TouchVec = touches.front()->getLocation();
        if(Vec2DistanceSq(m_vFirstTouchPosition, TouchVec) < 50){
            for(auto pUnit : m_TouchedUnits){
                if(pUnit->GetPlayerFlag() != NetMgr->GetPlayerFlag())
                    continue;
                Vec2 MoveVec = m_pCameraLayer->convertToNodeSpace(touches.front()->getLocation());
                int TileIndex = pUnit->GetGameWorld()->GetMap()->GetTileIndexFromPosition(MoveVec);
                
                auto tel = new TelegramMove(TileIndex);
                tel->PushBackUnitCode(pUnit->GetID());
                NetMgr->PushBackMessage(tel);
            }
            
        }
    }else{
        m_pSelectDrawNode->setVisible(false);
        
        //유닛중 이 범위에 속하는 아이들 고르기
        m_TouchedUnits.clear();
        
        for(auto pUnit : m_Units){
            if(m_SelectRect.containsPoint(pUnit.second->getPosition())){
                if(pUnit.second->GetPlayerFlag() != NetMgr->GetPlayerFlag())
                    continue;
                
                m_TouchedUnits.push_back(pUnit.second);
                
                if(m_TouchedUnits.size() >= 12)
                    break;
            }
        }
    }
    
    m_iTouchCnt -= touches.size();
    
    for(auto pUnit : m_Units){
        for(auto pTouch : touches){
            pUnit.second->TouchEnded(pTouch, _event);
        }
    }
}