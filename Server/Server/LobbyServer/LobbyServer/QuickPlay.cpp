
#include "QuickPlay.h"
#include "Room.h"
#include "User.h"
#include "Log.h"
#include "LobbyServer.h"
#include "LobbyDefine.h"
bool QuickPlayManager::initialize()
{
    isPossibleAlone = true;
    return true;
}


bool QuickPlayManager::addUser(User* user, int8_t tribe)
{
    QuickUserInfo* qui = new QuickUserInfo(user, tribe);
    
    for(quickPlayUserItr = quickPlayUserQueue.begin(); quickPlayUserItr < quickPlayUserQueue.end(); quickPlayUserItr++)
    {
        if(user == ((QuickUserInfo*)*quickPlayUserItr)->user)
        {
            return false;
        }
    }
    
    quickPlayUserQueue.push_back(qui);
    
    return true;
}

bool QuickPlayManager::removeUser(User* user)
{
    for(quickPlayUserItr = quickPlayUserQueue.begin(); quickPlayUserItr < quickPlayUserQueue.end(); quickPlayUserItr++)
    {
        if(user == ((QuickUserInfo*)*quickPlayUserItr)->user)
        {
            delete ((QuickUserInfo*)*quickPlayUserItr);
            quickPlayUserQueue.erase(quickPlayUserItr);
            return true;
        }
    }
    
    return false;
}


void QuickPlayManager::matchUser()
{
    QuickUserInfo* user1 = NULL;
    QuickUserInfo* user2 = NULL;
    
    RoomInfo roomInfo;
    roomInfo.roomNameLen = 0;
    
    if(isPossibleAlone == true)    ///////////////test alone
    {
        while(quickPlayUserQueue.empty() == false)
        {
            user1 = quickPlayUserQueue.front();
            quickPlayUserQueue.pop_front();
                
                Room* room = LobbyServer::getInstance()->roomMgr->createRoom(&roomInfo, user1->user, 0);
                
                if(room == NULL)
                {
                    ErrorLog("??");
                    continue;
                }
            
                room->startGame();
                
                
                delete user1;
        }
        
    }
    else
    {
        while(quickPlayUserQueue.empty() == false)
        {
            user1 = quickPlayUserQueue.front();
            quickPlayUserQueue.pop_front();
            
            if(quickPlayUserQueue.empty() == false)
            {
                user2 = quickPlayUserQueue.front();
                quickPlayUserQueue.pop_front();
                
                Room* room = LobbyServer::getInstance()->roomMgr->createRoom(&roomInfo, user1->user, 0);
                
                if(room == NULL)
                {
                    ErrorLog("??");
                    continue;
                }
                
                if(room->joinUserInRoom(user2->user) != SUCCESS)
                {
                    ErrorLog("??");
                    continue;
                }
                
                room->startGame();
                
                
                delete user1;
                delete user2;
            }
            else
            {
                quickPlayUserQueue.push_front(user1);
                return ;
            }
        }
    }
}
