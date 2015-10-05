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
    
    //Thing의 고유 아이디
    int m_iID;

    //해당 Thing이 서있는 타일의 인덱스
    int m_iTileIndex;
    
	//해당 유닛을 지워야 하나?
	bool m_bErase;
public:
	Thing(GameWorld* pGame, Object_Type type, int tileIndex);
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

    void SetTileIndex(int _tileindex);
	int GetTileIndex()const{ return m_iTileIndex; }

	void SetErase(bool erase){ m_bErase = erase; }
	bool IsErase()const{ return m_bErase; }
    
    int GetID()const{return m_iID;}

	/*							타입 관련 함수							*/
	void SetObjectType(Object_Type type){ m_ObjectType = type; }

	bool IsTypeUnit()const{ return m_ObjectType == Object_Type::Unit; }
	bool IsTypeStructure()const{ return m_ObjectType == Object_Type::Structure; }
};

