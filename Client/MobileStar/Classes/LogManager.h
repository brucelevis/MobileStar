#pragma once
#include <stdio.h>
#include <stdarg.h>
#define LogMgr LogManager::Instance()

class LogManager{
private:
    //디버그 로그를 찍을지 말지
    bool m_bDebug;
public:
    LogManager();
    ~LogManager();
    
    void Log(char *str, ...);
    
    static LogManager* Instance();
};