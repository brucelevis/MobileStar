#include "Unit.h"
#include "GameWorld.h"
#include "State_Unit.h"

Unit::Unit(GameWorld* pGame, int playerFlag, int tileIndex, float hp, float mp, float speed, float attackSpeed, float dmg, float armor)
: Thing(pGame, Object_Type::Unit, playerFlag,tileIndex)
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
, m_iPrevDir(10)
, m_iDir(10)
, m_iAutoTaskPacket(-1)
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
    m_iPrevDir = m_iDir;
    float Angle = static_cast<float>( (acos(m_vHeading.dot(Vec2(1,0)))) *
                                     180.0f / MathMgr->Pi *
                                     m_vHeading.sign(Vec2(1, 0))) + 180;
    
    m_iDir = (int)(Angle / 22.5);
    MathMgr->Clamp(m_iDir, 0, 15);
    
    //유닛이 맵 전체에서 나가지 못하게 한다.
    //MathMgr->Clamp(_position.x,30,GAME_SCREEN_WIDTH-30);
    //MathMgr->Clamp(_position.y, 30, GAME_SCREEN_HEIGHT - 30);
    
}

//현재 Path.front에 저장되어 있는 타일 인덱스로 이동한다. 만약 Path가 비어있으면 false를 반환한다.
bool Unit::MoveToPathFront(int CurrentPacket){
    //해당 유닛의 패스를 가져온다.
    auto& Path = GetPathPlanner()->GetPath();
    auto pMap = m_pGameWorld->GetMap();
    auto& Graph = pMap->GetNavGraph();
    
    //LogMgr->Log("Path Size : %d",Path.size());
    
    if(Path.empty()){
        //LogMgr->Log("취소");
        m_pFSM->ChangeState(State_Idle::Instance());
        return false;
    }
    
    //움직일 다음 칸 인덱스를 가져온다.
    int MoveIndex = Path.front();
    Path.pop_front();
    
    //MoveIndex의 타일이 비어있는지 확인한다.
    //비어 있지 않으면 새로 AStar알고리즘을 실행하여 움직인다.
    //TODO : 나중에 최적화 할 것
    if(!Graph.GetNode(MoveIndex).IsEmpty()){
        m_pPathPlanner->CreatePathToPosition(m_pPathPlanner->GetDestination());
        
        if(Path.empty()){
            GetFSM()->ChangeState(State_Idle::Instance());
            return false;
        }
        
        MoveIndex = Path.front();
        Path.pop_front();
    }
    
    //이동시킨다.
    int PrevIndex = GetTileIndex();
    SetTileIndex(MoveIndex);
    Vec2 MyPosition = m_pGameWorld->GetMap()->GetNavGraph().GetNode(PrevIndex).getPosition();
    Vec2 PathFrontPosition = m_pGameWorld->GetMap()->GetNavGraph().GetNode(MoveIndex).getPosition();
    
    //Heading 설정
    SetHeading(PathFrontPosition - getPosition());
    
    //Move 액션 생성 과정 : 타일 간의 거리와 유닛의 이동속도를 고려하여 만든다.
    
    //다음 AutoTask를 먼저 등록한다.
    //본래 속도보다 TweakPacket 정도 더 빠르게 실행시킨다.
    float Distance = Vec2Distance(PathFrontPosition, MyPosition) / (DIVIDE_NODE ? 32.0f : 64.0f);
    float Duration = Distance / GetSpeed();
    
    int TweakPacket = 1;
    int NextPacket = (int)(NETWORK_FPS * Duration) - TweakPacket;
    
    //만일 NextPacket이 0보다 작거나 같다면 다음 타일을 가져온다.
    if(NextPacket <= 0){
        return MoveToPathFront(CurrentPacket);
    }
    
    NetMgr->PushAutoTask(new AutoTaskMove(CurrentPacket + NextPacket, m_iID));
    SetAutoTaskPacket(CurrentPacket+NextPacket);
    
    auto pMove = MoveTo::create(Duration,PathFrontPosition);
    
    //액션을 실행한다.
    stopAllActions();
    runAction(pMove);
    
    return true;
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