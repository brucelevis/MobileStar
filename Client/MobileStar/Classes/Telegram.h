#pragma once
#include <stdio.h>

struct TelegramType{
    enum {
        Null,   //빈 메시지
        Move,
        Attack,
        Create,
        CreateComplete,
        CreateCancel,
        
    };
};


#pragma pack(push, 1)

struct Telegram{
    int8_t messageType;
    
    Telegram()
    : messageType(-1)
    {}
    
    Telegram(int8_t messagetype)
    : messageType(messagetype)
    {}
};
//유닛 이동 메시지
struct TelegramMove : public Telegram{
    //현재 subject에 저장된 유닛 수
    int8_t currentSize;
    uint16_t subject[12];
    uint16_t tileIndex;
    
    TelegramMove(uint16_t _tileIndex)
    : Telegram(TelegramType::Move)
    , currentSize(0)
    , tileIndex(_tileIndex)
    {
        for(int i=0;i<12;i++){
            subject[i] = 0;
        }
    }
    
    void PushBackUnitCode(uint16_t unit){
        subject[currentSize++] = unit;
    }
};

//주체, 방향이 필요한 메시지 : Move
struct TelegramOD : public Telegram
{
    int32_t subject;
    
    int8_t direction;
    
    TelegramOD()
    : subject(-1)
    , direction(-1)
    {}
    
    TelegramOD(int8_t type, int32_t _subject, int8_t _direction)
    : Telegram(type)
    , subject(_subject)
    , direction(_direction)
    {}
};

//주체, 목적체가 필요한 메시지 : Attack, CreateComplete, CreateCancel
struct TelegramOO : public Telegram
{
    int32_t subject;
    
    int32_t object;
    
    TelegramOO()
    : subject(-1)
    , object(-1)
    {}
    
    TelegramOO(int8_t type, int32_t _subject, int32_t _object)
    : Telegram(type)
    , subject(_subject)
    , object(_object)
    {}
};

//주체, 목적체, 방향이 필요한 메시지 : Create
struct TelegramOOD : public Telegram
{
    int32_t subject;
    
    int32_t object;
    
    int8_t direction;
    
    TelegramOOD()
    : subject(-1)
    , object(-1)
    , direction(-1)
    {}
    
    TelegramOOD(int8_t type, int32_t _subject,int32_t _object, int8_t _direction)
    : Telegram(type)
    , subject(_subject)
    , object(_object)
    , direction(_direction)
    {}
};

#pragma pack(pop)


//패킷 번호가 붙여진 메시지
struct TelegramWithPacket{
    int packet;
    Telegram* tel;
    TelegramWithPacket(int pack,Telegram* t)
    : packet(pack)
    , tel(t)
    {
        
    }
    
    
};