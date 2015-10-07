#ifndef __CUSTOM_POP_UP_H__
#define __CUSTOM_POP_UP_H__

#include "cocos2d.h"

class CustomPopUp
{
public:
	bool Initialize();
	void PopUp(std::string text);

private:
	void FinishPopUp();

	cocos2d::LabelTTF* textLabel;
};

#endif //__CUSTOM_POP_UP_H__