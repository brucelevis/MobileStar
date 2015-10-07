#include "CustomPopUp.h"
#include "GameDefines.h"

USING_NS_CC;

bool CustomPopUp::Initialize()
{
	textLabel = LabelTTF::create("", "fonts/koreaFont.ttf", 32);
	textLabel->setColor(Color3B::BLACK);
	textLabel->retain();

	textLabel->setPosition(300, 300);

	return true;
}

void CustomPopUp::PopUp(std::string text)
{
	textLabel->stopAllActions();
	textLabel->removeFromParentAndCleanup(false);

	textLabel->setString(text);
	Director::getInstance()->getRunningScene()->addChild(textLabel, 10000000, TAG_POP_UP);

	auto cb1 = CallFunc::create(CC_CALLBACK_0(CustomPopUp::FinishPopUp, this));
	auto seq = Sequence::create(DelayTime::create(3), cb1, NULL);
	textLabel->runAction(seq);
}

void CustomPopUp::FinishPopUp()
{
	textLabel->removeFromParentAndCleanup(false);
}
