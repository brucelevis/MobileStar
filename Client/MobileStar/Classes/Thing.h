#pragma once
#include "StdHeader.h"

USING_NS_CC;

class GameWorld;

enum class Object_Type{
	Null,
	Unit,
	Structure,
	Etc,
};

class Thing : public Layer
{
protected:
	GameWorld* m_pGameWorld;

	Object_Type m_ObjectType;

	//Y축 유닛 정렬 Float
	float m_fYSortFloat;

	//해당 유닛을 지워야 하나?
	bool m_bErase;
public:
	Thing(GameWorld* pGame, Object_Type type);
	virtual ~Thing();

	void update(float eTime);

	/*							터치 관련 함수							*/
	virtual bool TouchBegan(Touch* touch, Event* _event);
	virtual void TouchMoved(Touch* touch, Event* _event);
	virtual void TouchCancelled(Touch* touch, Event* _event);
	virtual void TouchEnded(Touch* touch, Event *_event);

	/*							유틸리티 함수							*/


	/*							게터세터 함수							*/
	GameWorld* GetGameWorld()const{ return m_pGameWorld; }

	float GetYSortFloat()const{ return m_fYSortFloat; }

	void SetErase(bool erase){ m_bErase = erase; }
	bool IsErase()const{ return m_bErase; }

	/*							타입 관련 함수							*/
	void SetObjectType(Object_Type type){ m_ObjectType = type; }

	bool IsTypeUnit()const{ return m_ObjectType == Object_Type::Unit; }
	bool IsTypeStructure()const{ return m_ObjectType == Object_Type::Structure; }
};

