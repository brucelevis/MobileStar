#pragma once
#include "StdHeader.h"
#include "TargetSystem.h"

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
private:
    enum class Status{
        Alive,
        Dead,
    };

protected:
    
    Status	m_Status;			//유닛이 죽었나 살았나?
    
	GameWorld* m_pGameWorld;

	Object_Type m_ObjectType;
    
    //Thing의 고유 아이디
    int m_iID;
    
    //Thing의 팀 소속
    int m_iPlayerFlag;
    
    float	m_fSpeed;			//유닛의 이동속도
    float	m_fAttackSpeed;		//유닛의 공격속도
    int     m_iDamage;			//유닛의 공격력
    int     m_iArmor;			//유닛의 방어력
    int     m_iMaxHp;			//유닛의 최대HP
    int     m_iMaxMp;			//유닛의 최대MP
    int     m_iHp;				//유닛의 HP
    int     m_iMp;				//유닛의 MP
    int     m_iAttackDistance;	//물체의 공격 사정거리

    //해당 Thing이 서있는 타일의 인덱스
    int m_iTileIndex;
    
	//해당 유닛을 지워야 하나?
	bool m_bErase;
    
    //물체 스프라이트
    Sprite* m_pSprite;
    
    //타겟 시스템
    TargetSystem* m_pTargetSystem;
    
    
public:
	Thing(GameWorld* pGame, Object_Type type, int playerFlag, int tileIndex, int hp, int mp, float speed, float attackSpeed, int dmg, int armor);
	virtual ~Thing();

	void update(float eTime);

	/*							터치 관련 함수							*/
	virtual bool TouchBegan(Touch* touch, Event* _event);
	virtual void TouchMoved(Touch* touch, Event* _event);
	virtual void TouchCancelled(Touch* touch, Event* _event);
	virtual void TouchEnded(Touch* touch, Event *_event);

	/*							유틸리티 함수							*/
    
    bool CanAttackThing(Thing* pThing);
    
	/*							게터세터 함수							*/
    
    bool	IsAlive()const{return m_Status == Status::Alive;}
    bool	IsDead()const{return m_Status == Status::Dead;}
    void	SetAlive(){m_Status = Status::Alive;}
    void	SetDead(){m_Status = Status::Dead;}
    
    void	AddSpeed(float speed){m_fSpeed += speed;}
    void	SetSpeed(float speed){m_fSpeed = speed;}
    float	GetSpeed()const{return m_fSpeed;}
    
    void	AddAttackSpeed(float attackspeed){m_fAttackSpeed += attackspeed;}
    void	SetAttackSpeed(float attackspeed){m_fAttackSpeed = attackspeed;}
    float	GetAttackSpeed()const{return m_fAttackSpeed;}
    
    void	AddDamage(int damage){m_iDamage += damage;}
    void	SetDamage(int damage){m_iDamage = damage;}
    int     GetDamage()const{return m_iDamage;}
    
    void	AddArmor(int armor){m_iArmor += armor;}
    void	SetArmor(int armor){m_iArmor = armor;}
    int     GetArmor()const{return m_iArmor;}
    
    void	AddMaxHp(int maxhp){m_iMaxHp += maxhp;}
    void	SetMaxHp(int maxhp){m_iMaxHp = maxhp;}
    int     GetMaxHp()const{return m_iMaxHp;}
    
    void	AddMaxMp(int maxmp){m_iMaxMp += maxmp;}
    void	SetMaxMp(int maxmp){m_iMaxMp = maxmp;}
    int     GetMaxMp()const{return m_iMaxMp;}
    
    void	AddHp(int hp){m_iHp += hp;}
    void	SetHp(int hp){m_iHp = hp;}
    int     GetHp()const{return m_iHp;}
    
    void	AddMp(int mp){m_iMp += mp;}
    void	SetMp(int mp){m_iMp = mp;}
    int     GetMp()const{return m_iMp;}
    
    GameWorld* GetGameWorld()const{ return m_pGameWorld; }

    void SetTileIndex(int _tileindex);
	int GetTileIndex()const{ return m_iTileIndex; }

	void SetErase(bool erase){ m_bErase = erase; }
	bool IsErase()const{ return m_bErase; }
    
    int GetID()const{return m_iID;}
    
    int GetAttackDistance()const{return m_iAttackDistance;}
    
    int GetPlayerFlag()const{return m_iPlayerFlag;}
    
    TargetSystem* GetTargetSystem()const{return m_pTargetSystem;}

	/*							타입 관련 함수							*/
	void SetObjectType(Object_Type type){ m_ObjectType = type; }

	bool IsTypeUnit()const{ return m_ObjectType == Object_Type::Unit; }
	bool IsTypeStructure()const{ return m_ObjectType == Object_Type::Structure; }
};

