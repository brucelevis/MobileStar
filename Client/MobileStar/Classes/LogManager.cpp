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

void LogManager::Log(std::string str, ...){
    if(!m_bDebug) return;
    
    char szOutStr[ 1024 ];
    va_list argptr;
    va_start( argptr, str.c_str() );
    vsprintf( szOutStr , str.c_str(), argptr );
    va_end( argptr );
    printf("%s\n",szOutStr);
}