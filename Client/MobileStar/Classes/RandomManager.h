#pragma once
#include <iostream>
#define RandMgr RandomManager::Instance()

class RandomManager{
private:
    //랜덤 테이블
    int RandomTable[100];
    
    //다음 난수 인덱스
    int NextRand;
public:
    RandomManager();
    ~RandomManager();
    
    static RandomManager* Instance();
    
    //시드를 넣는다.
    void SetSeed(int seed){ NextRand = seed%100; }
    
    //랜덤 값을 얻어온다.
    int Rand();
};