#include "RandomManager.h"

RandomManager* RandomManager::Instance(){
    static RandomManager instance;
    return &instance;
}

RandomManager::RandomManager()
{
    NextRand = 0;
    
    for(int i=0;i<100;i++){
        RandomTable[i] = 1000 + i*777 + (i%23)*1234;
    }
}
RandomManager::~RandomManager(){
}

int RandomManager::Rand(){
    int rand = RandomTable[NextRand++];
    NextRand %= 100;
    return rand;
}