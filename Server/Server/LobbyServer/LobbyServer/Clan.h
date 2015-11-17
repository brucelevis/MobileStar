
#ifndef __CLAN_H__
#define __CLAN_H__

#include <string.h>
#include <stdint.h>
#include <vector>
#include "BasicPacket.h"


class User;

class Clan
{
public:

	Clan();

    bool initialize(ClanInfo* clanInfo);
    ClanInfo* getClanInfo() { return &clanInfo; }
    int addUser(User* user);
    int removeUser(User* user);
    void removeAllUser();

	~Clan();

public:

	friend class ClanManager;

private:
    ClanInfo clanInfo;
    int userCount;
    std::vector<User*> userList;
};


#include <deque>
#include <string>
#include <map>

class ClanManager
{
public:
    ClanManager();
    
    bool initialize();
    int addClan(ClanInfo* clanInfo);
    
    Clan* getClanByClanNo(int32_t clanNo);
    Clan* getClanByClanNameInfo(ClanNameInfo* clanNameInfo);
    
    bool removeClan(Clan* _clan);
    bool removeClanByClanNameInfo(ClanNameInfo* clanNameInfo);
    
    ~ClanManager();
    
private:
    std::map<std::string, Clan*> clanMap;
};

#endif //__USER_H__
