#include "Clan.h"
#include "User.h"
#include "Log.h"

Clan::Clan()
{
}


bool Clan::initialize(ClanInfo* _clanInfo)
{
    memcpy(&clanInfo, _clanInfo, sizeof(ClanInfo));
    userList.clear();
	return true;
}


int Clan::addUser(User* user)
{
    userList.push_back(user);
    
    return SUCCESS;
}


int Clan::removeUser(User* user)
{
    std::vector<User*>::iterator itr;
    
    for(itr = userList.begin(); itr != userList.end(); itr++)
    {
        if(user == *itr)
        {
            userList.erase(itr);
            return SUCCESS;
        }
    }
    
    return INTERNAL_ERROR;
}

void Clan::removeAllUser()
{
    std::vector<User*>::iterator itr;
    
    for(itr = userList.begin(); itr != userList.end(); itr++)
    {
        //TODO. remove all user
    }
}


Clan::~Clan()
{

}


ClanManager::ClanManager()
{

}


bool ClanManager::initialize()
{
	clanMap.clear();
	return true;
}


int ClanManager::addClan(ClanInfo *clanInfo)
{
    Clan* clan = new Clan();
    if (clan->initialize(clanInfo) == false)
    {
        ErrorLog("clan Initialize error");
        return INTERNAL_ERROR;
    }
    
    return SUCCESS;
}


Clan* ClanManager::getClanByClanNo(int32_t clanNo)
{
    Clan* clan = NULL;
    
    std::map< std::string, Clan* >::iterator iter;
    
    for(iter = clanMap.begin(); iter != clanMap.end(); iter++)
    {
        clan = iter->second;
        
        if(clan->clanInfo.clanNo == clanNo)
        {
            return clan;
        }
    }
    
    return NULL;
}

Clan* ClanManager::getClanByClanNameInfo(ClanNameInfo *clanNameInfo)
{
    std::map< std::string, Clan* >::const_iterator iter = clanMap.find(std::string(clanNameInfo->clanName, clanNameInfo->clanNameLen));
    
    if (iter == clanMap.end())
    {
        ErrorLog("not exist Clan");
        return NULL;
    }
    
    return iter->second;
}

bool ClanManager::removeClan(Clan* clan)
{
    if (clanMap.erase(std::string(clan->clanInfo.clanName, clan->clanInfo.clanNameLen)) != 1)
    {
        ErrorLog("erase fail");
        return false;
    }
    
    clan->removeAllUser();
    
    delete (clan);
    
    return true;
}

bool ClanManager::removeClanByClanNameInfo(ClanNameInfo *clanNameInfo)
{
    Clan* clan = getClanByClanNameInfo(clanNameInfo);
    
    if (clan == 0)
    {
        ErrorLog("clan not exist");
        return false;
    }
    
    if (clanMap.erase(std::string(clan->clanInfo.clanName, clan->clanInfo.clanNameLen)) != 1)
    {
        ErrorLog("erase fail");
        return false;
    }
    
    clan->removeAllUser();
    
    delete (clan);
    
    return true;
}

