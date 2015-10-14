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
    int unitID;
    
    AutoTask()
    : messageType(-1)
    {}
    
    AutoTask(int messagetype, int _packet, int _unitID)
    : messageType(messagetype)
    , packet(_packet)
    , unitID(_unitID)
    {}
    
    virtual ~AutoTask(){
        
    }
    
    struct Compare : public std::binary_function<AutoTask*, AutoTask*, bool>{
        bool operator()(AutoTask* left, AutoTask* right)const{
            if(left->packet != right->packet)
                return left->packet > right->packet;
            else
                return left->unitID > right->unitID;
        }
    };
};

//유닛 이동 메시지
class AutoTaskMove : public AutoTask{
public:
    
    AutoTaskMove(int _packet, int _unitID)
    : AutoTask(AutoTaskType::Move,_packet, _unitID)
    {
        
    }
};