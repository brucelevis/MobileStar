
// create by yong chan kim
// since 2014.7.29


#ifndef __QUICK_PLAY_INFO_H__
#define __QUICK_PLAY_INFO_H__

#include <stdint.h>
#include <deque>

class User;
class ConnectInfo;

struct QuickUserInfo
{
    QuickUserInfo(User* _user, int8_t _tribe) { user = _user; tribe = _tribe; }
    User* user;
    int8_t tribe;
};

class QuickPlayManager
{
public:
    QuickPlayManager() {}
	bool initialize();
    bool addUser(User* user, int8_t tribe);
    bool removeUser(User* user);
    
    void matchUser();

    ~QuickPlayManager() {}

private:
    std::deque<QuickUserInfo*> quickPlayUserQueue;
    std::deque<QuickUserInfo*>::iterator quickPlayUserItr;
    
    bool isPossibleAlone;
};

#endif //__QUICK_PLAY_INFO_H__
