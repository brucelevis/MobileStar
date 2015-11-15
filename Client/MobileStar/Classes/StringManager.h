#pragma once
#include <iostream>
#define StrMgr StringManager::Instance()

class StringManager{
private:
public:
    StringManager();
    ~StringManager();
    
    static StringManager* Instance();
    
    const char* GetPlayerFlagStr(int idx);
    
    const char* GetUnitDirStr(int idx);
};