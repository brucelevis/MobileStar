

#ifndef __TIMER_LIST_H__
#define __TIMER_LIST_H__

#include <stdint.h>
#include "Network.h"

class MatchUserTimer : public Timer
{
public:
	MatchUserTimer(int timerNo, int interval);

public:
    virtual void ProcessTimerStart() {};
	virtual void ProcessTimerInterval();
    virtual void ProcessTimerFinish() {};

};


class UserListInChannelTimer : public Timer
{
public:
    UserListInChannelTimer(int timerNo, int interval);
    
public:
    virtual void ProcessTimerStart() {};
    virtual void ProcessTimerInterval();
    virtual void ProcessTimerFinish() {};
    
};

#endif //__TIMER_H__


