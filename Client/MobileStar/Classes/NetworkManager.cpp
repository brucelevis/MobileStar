#include "NetworkManager.h"
#include "cocos2d.h"
#include "NetworkLayer.h"
#include "GameDefines.h"
#include "GameWorld.h"
#include "State_Unit.h"
#include "PathPlanner.h"
#include "GameClient.h"

USING_NS_CC;

NetworkManager::NetworkManager()
{
    FirstTaskPacket = 3;
    SecondTaskPacket = 3;
    m_iPlayerFlag = -1;
    m_iCntCarryOutMessages = 0;
    
    //초기화시 각 Task에 0번, 1번, 2번 더미 패킷을 한개씩 넣어놓는다.
    for(int i=0;i<3;i++){
        FirstTask.push_back(TelegramWithPacket(i,NULL));
        SecondTask.push_back(TelegramWithPacket(i,NULL));
    }
    
}
NetworkManager::~NetworkManager(){
}

NetworkManager* NetworkManager::Instance(){
    static NetworkManager instance;
    return &instance;
}
void NetworkManager::SetupWhatPlayerFlag(){

    if(PLAY_ALONE){
        m_iPlayerFlag = 0;
    }else{
        int UserNo = GameClient::GetInstance().userInfo->userNo;
        
        //플레이어 A인가?
        if(GameClient::GetInstance().myGameIndex == 0){
            m_iPlayerFlag = 0;
        }else{
            m_iPlayerFlag = 1;
        }
        
        LogMgr->Log("[0] : %lld, [1] : %lld, userNo : %d",GameClient::GetInstance().gameUserInfo[0].userNo,GameClient::GetInstance().gameUserInfo[1].userNo,UserNo);
    }
}
//DispatchTask에 쌓인 Message를 서버로 보낸 후, Task에 전송한다.
void NetworkManager::DispatchToServer(){
    //메시지의 갯수
    int CntTelegram = (int)DispatchTask.size();
    
    char buffer[5000];
    memset(buffer,0,5000);
    
    int nextBuffer = 0;
    if(CntTelegram > 0){
        for(int i=0;i<CntTelegram;i++){
            Telegram* tel;
            //메시지 타입별로 저장한다.
            switch(DispatchTask.front()->messageType){
                case TelegramType::Move:
                {
                    auto t = (TelegramMove*)DispatchTask.front();
                    

                    //메시지 타입 저장
                    memcpy(&buffer[nextBuffer],(char*)t,sizeof(int8_t));
                    nextBuffer += sizeof(int8_t);
                    
                    //Unit의 수
                    memcpy(&buffer[nextBuffer],(char*)(&t->currentSize),sizeof(int8_t));
                    nextBuffer += sizeof(int8_t);
                    
                    //Unit 코드 갯수
                    memcpy(&buffer[nextBuffer],(char*)t->subject,t->currentSize * sizeof(int16_t));
                    nextBuffer += t->currentSize * sizeof(int16_t);
                    
                    //타일 인덱스
                    memcpy(&buffer[nextBuffer],(char*)(&t->tileIndex), sizeof(int32_t));
                    nextBuffer += sizeof(int32_t);
                    
                    tel = t;
                }
                    break;
                case TelegramType::Attack:
                case TelegramType::CreateCancel:
                case TelegramType::CreateComplete:
                    
                case TelegramType::Create:
                    ;
            }
            
            FirstTask.push_back(TelegramWithPacket(FirstTaskPacket,tel));
            DispatchTask.pop_front();
        }
        
        //서버로 buffer를 보낸다.
        if(PLAY_ALONE) {
            FetchFromServer(nextBuffer, buffer);
        }else{
            NetworkLayer* layer = (NetworkLayer*)cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER);
            layer->handler->gameSendClientNotify(nextBuffer,buffer);
        }
    }else{
        if(PLAY_ALONE) {
            FetchFromServer(0, buffer);
        }else{
            NetworkLayer* layer = (NetworkLayer*)cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER);
            layer->handler->gameSendClientNotify(0,buffer);
        }

        
        //빈 메시지를 보낸다.
        FirstTask.push_back(TelegramWithPacket(FirstTaskPacket,NULL));
        
    }
    
    FirstTaskPacket++;
}

//서버에서 상대방 클라이언트의 Message를 받은 후, Task에 전송한다.
void NetworkManager::FetchFromServer(int length, const char* str){
    int nextBuffer = 0;
    if(length > 0){
        while(nextBuffer < length){
            //메시지 종류를 확인한다.
            int8_t messageType;
            memcpy(&messageType,str+nextBuffer,sizeof(int8_t));
            nextBuffer += sizeof(int8_t);
            
            //시간초 동기화 처리
            if(messageType == TelegramType::Time){
                float time;
                memcpy(&time,str+nextBuffer,sizeof(float));
                nextBuffer += sizeof(float);
                m_pGameWorld->SetStartFrame(time);
                continue;
            }
            
            Telegram* tel;
            switch(messageType){
                case TelegramType::Move:
                {
                    auto t = new TelegramMove(-1);
                    
                    //유닛 수
                    memcpy(&(t->currentSize),str+nextBuffer,sizeof(int8_t));
                    nextBuffer += sizeof(int8_t);
                    
                    //유닛 코드
                    memcpy(t->subject,str+nextBuffer,sizeof(uint16_t) * t->currentSize);
                    nextBuffer += sizeof(uint16_t) * t->currentSize;
                    
                    //타일 인덱스
                    memcpy(&(t->tileIndex),str+nextBuffer,sizeof(int32_t));
                    nextBuffer += sizeof(int32_t);
                    
                    tel = t;
                }
                    break;
                case TelegramType::Attack:
                case TelegramType::CreateCancel:
                case TelegramType::CreateComplete:
                {
                    
                }
                    break;
                case TelegramType::Create:
                {
                    
                }
                    break;
            }
            
            TelegramWithPacket telP(SecondTaskPacket,tel);
            SecondTask.push_back(telP);
        }
    }else{
        //패킷이 비었지만 통신이 왔다.
        SecondTask.push_back(TelegramWithPacket(SecondTaskPacket,NULL));
    }
    SecondTaskPacket++;
    
}

//해당 Message를 DispatchTask에 보낸다.
void NetworkManager::PushBackMessage(Telegram* telegram){
    DispatchTask.push_back(telegram);
}

//해당 AutoTask를 큐에 쌓는다.
void NetworkManager::PushAutoTask(AutoTask* autotask){
    AutoTaskQueue.push(autotask);
}

//Task에 쌓인 메시지들을 모두 수행한다.
void NetworkManager::CarryOutMessages(){
    m_iCntCarryOutMessages++;
    
    //상대방 컴퓨터와 통신이 두절되었는지 확인한다.
    if(!IsCommunicate()){
        //통신이 두절
        LogMgr->Log("통신 두절");
        return ;
    }
    //현재 처리하는 패킷이 통신하는 패킷 번호보다 3보다 작거나 같을 때까지 모두 처리한다.
    while(FirstTask.front().packet <= FirstTaskPacket - 3)
    {
        int CurrentPacket = FirstTask.front().packet;
        
        //먼저 AutoTask의 일을 처리한다.
        CarryOutAutoTask(CurrentPacket);
        
        //내가 A 컴퓨터이면 내 일을 먼저 처리한다.
        if(m_iPlayerFlag == 0){
            CarryOutFirstTask(CurrentPacket);
            CarryOutSecondTask(CurrentPacket);
        }else{  //B 컴퓨터이면 상대방 일을 먼저 처리한다.
            CarryOutSecondTask(CurrentPacket);
            CarryOutFirstTask(CurrentPacket);

        }
    }

}
//메인 일 처리 함수
void NetworkManager::CarryOutTask(Telegram* pTel,int iPacket){
    
    if(pTel == NULL)
        return;
    
    switch(pTel->messageType){
        case TelegramType::Move:
        {
            auto pTelMove = (TelegramMove*)pTel;
            
            //유닛을 이동시킨다.
            auto Units = m_pGameWorld->GetUnits();
            for(int i=0;i<pTelMove->currentSize; i++){
                
                //객체의 포인터들을 얻어온다.
                auto pUnit = Units[pTelMove->subject[i]];
                auto pPlanner = pUnit->GetPathPlanner();
                auto& Path = pPlanner->GetPath();
                
                //타겟 시스템을 비운다.
                pUnit->GetTargetSystem()->ClearTarget();
                
                //만일 현재 위치와 TileIndex의 위치가 같으면 종료한다.
                if(Path.empty() &&
                   pUnit->GetTileIndex() == pTelMove->tileIndex){
                    pUnit->GetFSM()->ChangeState(State_Idle::Instance());
                    continue;
                }
                
                //만일 유닛이 현재 Path가 남아 있으며 그 목적지와 pTelegramMove->tileIndex가 같으면 종료한다.
                if(!Path.empty() &&
                   pPlanner->GetDestination() == pTelMove->tileIndex)
                    continue;
                
                int MoveIndex = pTelMove->tileIndex;
                
                //길을 생성한다.
                pPlanner->CreatePathToPosition(MoveIndex);
                
                //현재 이동중이 아니라면 유닛을 이동시킨다. 만약 이동 중일 경우 아무 짓을 하지 않아도 자동으로 다음 작업을 수행한다.
                if(pUnit->GetFSM()->CurrentState() != State_Move::Instance()){
                    //이동 상태로 전환시킨다.
                    pUnit->GetFSM()->ChangeState(State_Move::Instance());
                    
                    //유닛을 이동시킨다.
                    pUnit->MoveToPathFront(iPacket);
                }
                
            }
            
            delete pTelMove;
        }
            break;
    }
}
//내 컴퓨터의 일을 처리한다.
void NetworkManager::CarryOutFirstTask(int _packet){
    while(!FirstTask.empty() && FirstTask.front().packet == _packet)
    {
        CarryOutTask(FirstTask.front().tel,FirstTask.front().packet);
        FirstTask.pop_front();
    }
}

//상대방 컴퓨터의 일을 처리한다.
void NetworkManager::CarryOutSecondTask(int _packet){
    while(!SecondTask.empty() && SecondTask.front().packet == _packet)
    {
        //혼자 실행용이면 다음 구문을 실행하지 않는다.
        if(!PLAY_ALONE)
            CarryOutTask(SecondTask.front().tel,SecondTask.front().packet);
        SecondTask.pop_front();
    }
    
}

//AutoTask의 일들을 처리한다.
void NetworkManager::CarryOutAutoTask(int _packet){
    if(AutoTaskQueue.empty())
        return;
    
    //만약 AutoTaskQueue의 top 패킷이 현재 패킷보다 느리다면 그냥 지워버린다. (그런 현상은 원래 발생해선 안된다!)
    while(AutoTaskQueue.top()->packet < _packet){
        auto pTask = AutoTaskQueue.top();
        AutoTaskQueue.pop();
        delete pTask;
        LogMgr->Log("AutoTaskQueue.pop() 치명적 오류!!");
    }
    
    while(!AutoTaskQueue.empty() &&
          AutoTaskQueue.top()->packet == _packet){
        
        //일을 처리면서 Queue에서 빼낸다.
        AutoTask* pTask = AutoTaskQueue.top();
        AutoTaskQueue.pop();
        
        switch(pTask->messageType){
            case AutoTaskType::Move:
            {
                auto pMove = (AutoTaskMove*)pTask;
                //유닛 리스트를 얻어온다.
                auto Units = m_pGameWorld->GetUnits();
                
                //해당 유닛의 포인터를 가져온다.
                auto pUnit = Units[pMove->unitID];
                
                //만약 유닛의 AutoTaskPacket과 동기화 되어 있지 않다면 실행시키지 않는다.
                if(!pUnit->IsValidAutoTask(pMove->packet))
                    break;
                
                //해당 유닛을 이동시킨다.
                pUnit->MoveToPathFront(pMove->packet);
            }
                break;
            case AutoTaskType::Attack:
            {
                auto pAttack = (AutoTaskAttack*)pTask;
                //유닛 리스트를 얻어온다.
                auto Units = m_pGameWorld->GetUnits();
                
                //해당 유닛의 포인터를 가져온다.
                auto pUnit = Units[pAttack->unitID];
                
                //만약 유닛의 AutoTaskPacket과 동기화 되어 있지 않다면 실행시키지 않는다.
                if(!pUnit->IsValidAutoTask(pAttack->packet))
                    break;
                
                //해당 유닛을 이동시킨다.
                pUnit->AttackTarget(pAttack->packet);
            }
                break;
        }
        delete pTask;
    }
}
//통신이 두절되었나 확인한다.
bool NetworkManager::IsCommunicate()const{
    //상대방에게 날아온 메시지가 비어있다면, 또는 두 컴퓨터의 패킷 차이가 3이상 차이가 난다면
    return !(SecondTask.empty() || abs(FirstTaskPacket - SecondTaskPacket) >= 3);
}