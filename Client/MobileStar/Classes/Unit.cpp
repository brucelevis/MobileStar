#include "Unit.h"
#include "GameWorld.h"
#include "State_Unit.h"

Unit::Unit(GameWorld* pGame, float hp, float mp, float speed, float attackSpeed, float dmg, float armor, int16_t unitType, int zPosition)
: Thing(pGame, Object_Type::Unit, zPosition)
, m_Status(Status::Alive)
, m_fSpeed(speed)
, m_fAttackSpeed(attackSpeed)
, m_fDamage(dmg)
, m_fArmor(armor)
, m_fMaxHp(hp)
, m_fMaxMp(mp)
, m_fHp(hp)
, m_fMp(mp)
, m_fHpRegen(0.0f)
, m_fMpRegen(0.0f)
, m_vHeading(Vec2(0.0f,-1.0f))
, m_iDir(Dir::Front)
, m_unitType(unitType)
{
    //유한 상태 기계 초기화
    m_pFSM = new StateMachine<Unit>(this);
    m_pFSM->SetCurrentState(State_Idle::Instance());
    
    //길 찾기 플래너 초기화
    m_pPathPlanner = new PathPlanner(this);
}
Unit::~Unit(){
    delete m_pPathPlanner;
    delete m_pFSM;
}
void Unit::update(float eTime){
	Thing::update(eTime);

	//Status가 살아있고, HP가 0보다 작을 때, 죽은 상태로 전환한다.
	if( GetHp() <= 0 && IsAlive() ){
		SetDead();
	}
    
    //유한 상태 기계
    m_pFSM->Update(eTime);

	//체력, 마나 리젠
	m_fHp += m_fHpRegen * eTime;
	m_fMp += m_fMpRegen * eTime;
	MathMgr->Clamp(m_fHp,0,m_fMaxHp);
	MathMgr->Clamp(m_fMp, 0, m_fMaxMp);

	//방향 설정한다.
	//dir : 0~360
	int dir = static_cast<int>( (acos(m_vHeading.dot(Vec2(1,0)))) * 
		180.0f / MathMgr->Pi *
		m_vHeading.sign(Vec2(1, 0))) + 180;
	
	if( (dir > 0 && dir <= 45)  || dir > 315) m_iDir = Dir::Left;
	if( dir >  45 && dir <  135) m_iDir = Dir::Back;
	if( dir >=135 && dir <  225) m_iDir = Dir::Right;
	if( dir >=225 && dir <= 315) m_iDir = Dir::Front;
	
	//유닛이 맵 전체에서 나가지 못하게 한다.
	//MathMgr->Clamp(_position.x,30,GAME_SCREEN_WIDTH-30);
	//MathMgr->Clamp(_position.y, 30, GAME_SCREEN_HEIGHT - 30);

}

void Unit::SetHeading(Vec2 new_heading){
	m_vHeading = Vec2Normalize(new_heading);
}
bool Unit::TouchBegan(Touch* touch, Event* _event){
    m_pFSM->TouchBegan(touch, _event);
    return true;
}
void Unit::TouchMoved(Touch* touch, Event* _event){
    m_pFSM->TouchMoved(touch, _event);
}
void Unit::TouchEnded(Touch* touch, Event *_event){
    m_pFSM->TouchEnded(touch, _event);
}