#pragma once
#include <list>
#include "Telegram.h"
#define NetMgr NetworkManager::Instance()


class NetworkManager{
private:
    //내 컴퓨터의 일들
    std::list<TelegramWithPacket> FirstTask;
    int FirstTaskPacket;

    //상대방 컴퓨터의 일들
    std::list<TelegramWithPacket> SecondTask;
    int SecondTaskPacket;
    
    //서버로 전송해야 할 일들
    std::list<Telegram*> DispatchTask;
    
    //내가 무슨 컴퓨터인지 ? A?B? [ 0 : A, 1 : B ]
    int m_iKindOfComputer;
    
    //해당 패킷의 내 컴퓨터의 일을 처리한다.
    void CarryOutFirstTask(int _packet);
    
    //해당 패킷의 상대방 컴퓨터의 일을 처리한다.
    void CarryOutSecondTask(int _packet);
public:
    NetworkManager();
    ~NetworkManager();
    
    static NetworkManager* Instance();
public:
    //A 컴퓨터 B 컴퓨터를 정해주기.
    void SetupWhatComputer();
    
    //해당 Message를 DispatchTask에 보낸다.
    void PushBackMessage(Telegram* telegram);
    
    //DispatchTask에 쌓인 Message를 서버로 보낸 후, Task에 전송한다.
    void DispatchToServer();
    
    //서버에서 상대방 클라이언트의 Message를 받은 후, Task에 전송한다.
    void FetchFromServer(int length, const char* str);
    
    //Task에 쌓인 메시지들을 모두 수행한다.
    void CarryOutMessages();
};