#include "IDManager.h"

IDManager* IDManager::Instance(){
    static IDManager instance;
    return &instance;
}

IDManager::IDManager()
: m_iNextNodeID(0)
, m_iNextThingID(0)
{
    
}
IDManager::~IDManager(){
}