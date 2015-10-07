#include "LogManager.h"

LogManager* LogManager::Instance(){
    static LogManager instance;
    return &instance;
}

LogManager::LogManager()
: m_bDebug(false)
{
    
}
LogManager::~LogManager(){
}

void LogManager::Log(char *str, ...){
    if(!m_bDebug) return;
    
    char szOutStr[ 1024 ];
    va_list argptr;
    va_start( argptr, str );
    vsprintf( szOutStr , str, argptr );
    va_end( argptr );
    printf("%s",szOutStr);
}