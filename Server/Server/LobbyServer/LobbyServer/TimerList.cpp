#include "TimerList.h"
#include "QuickPlay.h"
#include "LobbyServer.h"
#include "Log.h"

MatchUserTimer::MatchUserTimer(int _timerNo, int _interval) : Timer(_timerNo, _interval, 0)
{
    
}


void MatchUserTimer::ProcessTimerInterval()
{
    LobbyServer::getInstance()->quickPlayMgr->matchUser();
}