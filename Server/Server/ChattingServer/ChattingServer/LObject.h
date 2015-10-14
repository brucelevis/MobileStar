
// create by yong chan kim
// since 2014.7.29


#ifndef __L_OBJECT_H__
#define __L_OBJECT_H__

#include <stdint.h>
#include <vector>

class User;

class LObject
{
public:

    LObject();
    
    bool initialize(int32_t _loNo);
    
    int joinUser(User* user);
    bool leaveUser(User* user);
    
    int16_t getLObjectNo() { return loNo; }
    
    int8_t getUserCount() { return userCount; }
    
    
    void chattingNotify(User* user, char* chatting, uint8_t chattingLen);
    
    ~LObject();

private:
	
    friend class LObjectManager;
    
    int32_t loNo;
    
    int8_t userCount;
    
    std::vector<User*>::iterator itr;
    std::vector<User*> userList;
    
    char notifyBuffer[500];
};


class LObjectManager
{
public:
    LObjectManager();
    
    bool initialize();
    LObject* createLObject(int32_t loNo);
    int destroyLObject(int32_t loNo);
    
    bool addUser(int32_t loNo, User* user);
    
    
    LObject* getLObjectByLoNo(int32_t loNo);
    
    ~LObjectManager();
    
private:
    std::vector<LObject*>::iterator itr;
    std::vector<LObject*> lObjectList;
};


#endif //__L_OBJECT_H__
