#include "NetworkManager.h"
#include "cocos2d.h"
#include "NetworkLayer.h"
#include "GameDefines.h"
#include "GameWorld.h"
#include "State_Unit.h"
#include "PathPlanner.h"

#define COCOS2D_DEBUG 1

NetworkManager::NetworkManager()
{
    FirstTaskPacket = 3;
    SecondTaskPacket = 3;
    m_iKindOfComputer = -1;
    
    //초기화시 각 Task에 0번, 1번, 2번 더미 패킷을 한개씩 넣어놓는다.
    for(int i=0;i<3;i++){
        FirstTask.push_back(TelegramWithPacket(i,NULL));
        SecondTask.push_back(TelegramWithPacket(i,NULL));
    }
    
    
    //Temp
    m_iKindOfComputer = 0;
    
}
NetworkManager::~NetworkManager(){
}

NetworkManager* NetworkManager::Instance(){
    static NetworkManager instance;
    return &instance;
}
void NetworkManager::SetupWhatComputer(){
    
    
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
                    memcpy(&buffer[nextBuffer],(char*)(&t->tileIndex), sizeof(int16_t));
                    nextBuffer += sizeof(int16_t);
                    
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
//        NetworkLayer* layer = (NetworkLayer*)cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER);
//        layer->handler->gameSendClientNotify(nextBuffer,buffer);
        
        FetchFromServer(nextBuffer, buffer);
    }else{
        //NetworkLayer* layer = (NetworkLayer*)cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(TAG_NETWORK_LAYER);
        //layer->handler->gameSendClientNotify(0,buffer);
        FetchFromServer(0, buffer);
        
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
                    memcpy(&(t->tileIndex),str+nextBuffer,sizeof(uint16_t));
                    nextBuffer += sizeof(uint16_t);
                    
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
    //상대방에게 날아온 메시지가 비어있다면
    if(SecondTask.empty()){
        //통신이 두절
        return ;
    }
    //현재 처리하는 패킷이 통신하는 패킷 번호보다 3보다 작거나 같을 때까지 모두 처리한다.
    while(FirstTask.front().packet <= FirstTaskPacket - 3)
    {
        int CurrentPacket = FirstTask.front().packet;
        
        //먼저 AutoTask의 일을 처리한다.
        CarryOutAutoTask(CurrentPacket);
        
        //내가 A 컴퓨터이면 내 일을 먼저 처리한다.
        if(m_iKindOfComputer == 0){
            CarryOutFirstTask(CurrentPacket);
            CarryOutSecondTask(CurrentPacket);
        }else{  //B 컴퓨터이면 상대방 일을 먼저 처리한다.
            CarryOutSecondTask(CurrentPacket);
            CarryOutFirstTask(CurrentPacket);

        }
    }

}
//내 컴퓨터의 일을 처리한다.
void NetworkManager::CarryOutFirstTask(int _packet){
    while(FirstTask.empty() == false)
    {
        if(_packet == FirstTask.front().packet){
            Telegram* pTelegram = FirstTask.front().tel;
            int iPacket = FirstTask.front().packet;
            
            if(pTelegram != NULL){
                
                switch(pTelegram->messageType){
                    case TelegramType::Move:
                    {
                        auto pTelegramMove = (TelegramMove*)pTelegram;
                        
                        //유닛을 이동시킨다.
                        auto Units = m_pGameWorld->GetUnits();
                        for(int i=0;i<pTelegramMove->currentSize; i++){
                            auto pUnit = Units[pTelegramMove->subject[i]];
                            
                            //만일 현재 위치와 TileIndex의 위치가 같으면 종료한다.
                            if(pUnit->GetTileIndex() == pTelegramMove->tileIndex){
                                pUnit->GetFSM()->ChangeState(State_Idle::Instance());
                                break;
                            }
                            
                            //Unit의 길을 찾는다.
                            pUnit->GetPathPlanner()->CreatePathToPosition(pTelegramMove->tileIndex);
                            
                            //플래너의 Path를 가져온다.
                            std::list<int>& Path = pUnit->GetPathPlanner()->GetPath();
                            
                            //만약 Path가 비었다면 Idle 상태로 전환한다.
                            if(Path.empty()){
                                pUnit->GetFSM()->ChangeState(State_Idle::Instance());
                                break;
                            }
                            
                            //Path의 front로 이동한다.
                            int MoveIndex = Path.front();
                            Path.pop_front();
                            
                            //AutoTask에 다음 Path.front를 넣는다.
                            PushAutoTask(new AutoTaskMove(iPacket+11,pTelegramMove->subject[i]));
                            pUnit->SetAutoTaskPacket(iPacket+11);
                            
                            //이동시킨다.
                            pUnit->SetTileIndex(MoveIndex);
                            Vec2 PathFrontPosition = pUnit->GetGameWorld()->GetMap()->GetNavGraph().GetNode(MoveIndex).getPosition();
                            
                            auto move = MoveTo::create(1.1f,PathFrontPosition);
                            
                            //액션을 실행한다.
                            pUnit->stopAllActions();
                            pUnit->runAction(move);
                            
                            //이동 상태로 전환시킨다.
                            pUnit->GetFSM()->ChangeState(State_Move::Instance());

                        }
                        
                        delete pTelegramMove;
                    }
                        break;
                }
                
                FirstTask.pop_front();
            }else{
                FirstTask.pop_front();
            }
        }
        else
        {
            break;
        }
    }
}

//상대방 컴퓨터의 일을 처리한다.
void NetworkManager::CarryOutSecondTask(int _packet){
    while(SecondTask.empty() == false)
    {
        if(_packet == SecondTask.front().packet){
            Telegram* pTelegram = SecondTask.front().tel;
            if(pTelegram != NULL){
                
                switch(pTelegram->messageType){
                    case TelegramType::Move:
                    {
                        auto pTelegramMove = (TelegramMove*)pTelegram;
                        
                        //유닛을 이동시킨다.
//                        auto Units = m_pGameWorld->GetUnits();
//                        auto pUnit = Units.begin()->second;
//                        pUnit->GetPathPlanner()->CreatePathToPosition(pTelegramMove->tileIndex);
//                        pUnit->GetFSM()->ChangeState(State_Move::Instance());
                        
                        delete pTelegramMove;
                    }
                        break;
                }
                
                SecondTask.pop_front();
            }else{
                SecondTask.pop_front();
            }
        }
        else
        {
            break;
        }
    }
}

//AutoTask의 일들을 처리한다.
void NetworkManager::CarryOutAutoTask(int _packet){

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
                
                //만약 해당 유닛의 AutoTaskPacket과 동기화 되어 있지 않다면 실행시키지 않는다.
                if(!pUnit->IsValidAutoTask(pMove->packet))
                    break;
                
                //해당 유닛의 패스를 가져온다.
                auto& Path = pUnit->GetPathPlanner()->GetPath();
                
                if(Path.empty()){
                    pUnit->GetFSM()->ChangeState(State_Idle::Instance());
                    break;
                }
                
                //움직일 다음 칸 인덱스를 가져온다.
                int MoveIndex = Path.front();
                Path.pop_front();
                
                //다음 AutoTask를 등록한다.
                PushAutoTask(new AutoTaskMove(pMove->packet+11,pMove->unitID));
                pUnit->SetAutoTaskPacket(pMove->packet+11);
                
                //이동시킨다.
                pUnit->SetTileIndex(MoveIndex);
                Vec2 PathFrontPosition = pUnit->GetGameWorld()->GetMap()->GetNavGraph().GetNode(MoveIndex).getPosition();
                
                auto move = MoveTo::create(1.1f,PathFrontPosition);
                
                //액션을 실행한다.
                pUnit->stopAllActions();
                pUnit->runAction(move);
                
            }
                break;
        }
        delete pTask;
    }
}