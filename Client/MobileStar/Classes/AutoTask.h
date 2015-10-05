#pragma once
#include <stdio.h>
#include <functional>

struct AutoTaskType{
    enum {
        Null,   //빈 메시지
        Move,
    };
};

class AutoTask{
public:
    int messageType;
    int packet;
    
    AutoTask()
    : messageType(-1)
    {}
    
    AutoTask(int messagetype, int _packet)
    : messageType(messagetype)
    , packet(_packet)
    {}
    
    virtual ~AutoTask(){
        
    }
    
    struct Compare : public std::binary_function<AutoTask*, AutoTask*, bool>{
        bool operator()(AutoTask* left, AutoTask* right)const{
            return left->packet > right->packet;
        }
    };
};

//유닛 이동 메시지
class AutoTaskMove : public AutoTask{
public:
    int unitID;
    
    AutoTaskMove(int _packet, int _unitID)
    : AutoTask(AutoTaskType::Move,_packet)
    , unitID(_unitID)
    {
        
    }
};