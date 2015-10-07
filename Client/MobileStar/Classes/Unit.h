#pragma once
#include "Thing.h"
#include "StateMachine.h"
#include "PathPlanner.h"

class GameWorld;

class Unit : public Thing{
private:
	enum class Status{
		Alive,
		Dead,
	};

protected:
	//유닛의 방향 구조체
	struct Dir{
		enum{
			Front,
			Left,
			Back,
			Right
		};
	};

public:
	//유닛의 방향을 담은 변수
	int m_iDir;
protected:
	Status	m_Status;			//유닛이 죽었나 살았나?
    
	float	m_fSpeed;			//유닛의 이동속도
	float	m_fAttackSpeed;		//유닛의 공격속도
	float	m_fDamage;			//유닛의 공격력
	float	m_fArmor;			//유닛의 방어력
	float	m_fMaxHp;			//유닛의 최대HP
	float	m_fMaxMp;			//유닛의 최대MP
	float	m_fHp;				//유닛의 HP
	float	m_fMp;				//유닛의 MP
	float	m_fHpRegen;			//유닛의 HP 재생력
	float	m_fMpRegen;			//유닛의 MP 재생력

	/*----------------------------------- AI관련 변수 --------------------------------*/

    //유한 상태 기계 머신
    StateMachine<Unit>* m_pFSM;
    
    //길 찾기 플래너
    PathPlanner* m_pPathPlanner;
    
	//유닛이 바라보는 방향
	Vec2 m_vHeading;


	/*-----------------------------------------------------------------------------------*/
    
    /*----------------------------------- Network관련 변수 --------------------------------*/
    
    //현재 등록된 AutoTaskPacket;
    int m_iAutoTaskPacket;
    
    /*-----------------------------------------------------------------------------------*/

public:
	Unit(GameWorld* pGame, int tileIndex, float hp, float mp, float speed, float attackSpeed, float dmg, float armor);
	~Unit();

	void update(float eTime);
    
	/*								터치 관련 함수								*/
    
    bool TouchBegan(Touch* touch, Event* _event);
    void TouchMoved(Touch* touch, Event* _event);
    void TouchEnded(Touch* touch, Event *_event);
    
	/*								유틸리티 함수								*/
    
    //현재 Path.front에 저장되어 있는 타일 인덱스로 이동한다. 만약 Path가 비어있으면 false를 반환한다.
    bool MoveToPathFront(int CurrentPacket);
    
    //AutoTaskMove의 packet 값이 유효한지?
    bool IsValidAutoTask(int packet){ return m_iAutoTaskPacket == packet; }

	/*								게터세터 함수								*/

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

	void	AddDamage(float damage){m_fDamage += damage;}
	void	SetDamage(float damage){m_fDamage = damage;}
	float	GetDamage()const{return m_fDamage;}

	void	AddArmor(float armor){m_fArmor += armor;}
	void	SetArmor(float armor){m_fArmor = armor;}
	float	GetArmor()const{return m_fArmor;}

	void	AddMaxHp(float maxhp){m_fMaxHp += maxhp;}
	void	SetMaxHp(float maxhp){m_fMaxHp = maxhp;}
	float	GetMaxHp()const{return m_fMaxHp;}

	void	AddMaxMp(float maxmp){m_fMaxMp += maxmp;}
	void	SetMaxMp(float maxmp){m_fMaxMp = maxmp;}
	float	GetMaxMp()const{return m_fMaxMp;}

	void	AddHp(float hp){m_fHp += hp;}
	void	SetHp(float hp){m_fHp = hp;}
	float	GetHp()const{return m_fHp;}

	void	AddMp(float mp){m_fMp += mp;}
	void	SetMp(float mp){m_fMp = mp;}
	float	GetMp()const{return m_fMp;}

	void	AddHpRegen(float hpregen){m_fHpRegen += hpregen;}
	void	SetHpRegen(float hpregen){m_fHpRegen = hpregen;}
	float	GetHpRegen()const{return m_fHpRegen;}

	void	AddMpRegen(float mpregen){m_fMpRegen += mpregen;}
	void	SetMpRegen(float mpregen){m_fMpRegen = mpregen;}
	float	GetMpRegen()const{return m_fMpRegen;}

	Vec2	GetHeading()const{ return m_vHeading; }
    void	SetHeading(Vec2 new_heading);
    
    void    SetAutoTaskPacket(int packet){ m_iAutoTaskPacket = packet;}
    int     GetAutoTaskPacket()const{return m_iAutoTaskPacket;}
    
    StateMachine<Unit>* GetFSM()const{return m_pFSM;}
    
    PathPlanner* GetPathPlanner()const{return m_pPathPlanner; }

};

inline bool operator<(const Unit& c1, const Unit& c2){
	return c1.getPosition().y < c1.getPosition().y;
}