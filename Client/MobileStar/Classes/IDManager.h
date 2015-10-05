#pragma once
#define IDMgr IDManager::Instance()

class IDManager{
private:
    //다음에 생성될 Node Index
    int m_iNextNodeID;
    
    //다음에 생성될 Thing 고유 ID
    int m_iNextThingID;
public:
    IDManager();
    ~IDManager();
    
    static IDManager* Instance();
    
    int GetNextNodeID(){return m_iNextNodeID++;}
    
    int GetNextThingID(){return m_iNextThingID++;}
};