#include "StringManager.h"

StringManager* StringManager::Instance(){
    static StringManager instance;
    return &instance;
}

StringManager::StringManager()
{
    
}
StringManager::~StringManager(){
}
const char* StringManager::GetPlayerFlagStr(int idx){
    
    switch (idx){
        case 0:
            return "Red";
        case 1:
            return "Blue";
            
    }
    return "Error";
}
const char* StringManager::GetUnitDirStr(int idx){
    switch (idx){
        case 0:
            return "Left";
        case 1:
            return "LeftLeftUp";
        case 2:
            return "LeftUp";
        case 3:
            return "LeftUpUp";
        case 4:
            return "Up";
        case 5:
            return "RightUpUp";
        case 6:
            return "RightUp";
        case 7:
            return "RightRightUp";
        case 8:
            return "Right";
        case 9:
            return "RightRightDown";
        case 10:
            return "RightDown";
        case 11:
            return "RightDownDown";
        case 12:
            return "Down";
        case 13:
            return "LeftDownDown";
        case 14:
            return "LeftDown";
        case 15:
            return "LeftLeftDown";
    }
    return "Error";
}