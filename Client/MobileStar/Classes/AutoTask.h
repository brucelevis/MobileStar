#pragma once
#include <stdio.h>

struct AutoTaskType{
    enum {
        Null,   //빈 메시지
        Move,
    };
};

struct AutoTask{
    int messageType;
    int packet;
    
    AutoTask()
    : messageType(-1)
    {}
    
    AutoTask(int messagetype, int _packet)
    : messageType(messagetype)
    , packet(_packet)
    {}
};

//유닛 이동 메시지
struct AutoTaskMove : public AutoTask{
    //현재 subject에 저장된 유닛 수
    int unitID;
    int tileIndex;
    
    AutoTaskMove(int _packet, int _unitID, int _tileIndex)
    : AutoTask(AutoTaskType::Move,_packet)
    , unitID(_unitID)
    , tileIndex(_tileIndex)
    {
        
    }
};