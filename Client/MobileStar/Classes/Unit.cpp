#include "Unit.h"
#include "GameWorld.h"
#include "State_Unit.h"

Unit::Unit(GameWorld* pGame, int playerFlag, int tileIndex, int hp, int mp, float speed, float attackSpeed, int dmg, int armor)
: Thing(pGame, Object_Type::Unit, playerFlag,tileIndex,hp,mp,speed,attackSpeed,dmg,armor)
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
        m_pGameWorld->GetMap()->GetNavGraph().GetNode(GetTileIndex()).SetEmpty();
        m_pGameWorld->GetMap()->GetNavGraph().GetNode(GetTileIndex()).SetThing(NULL);
    }
    
    //유한 상태 기계
    m_pFSM->Update(eTime);
    
    updateDir();
    
    //유닛이 맵 전체에서 나가지 못하게 한다.
    //MathMgr->Clamp(_position.x,30,GAME_SCREEN_WIDTH-30);
    //MathMgr->Clamp(_position.y, 30, GAME_SCREEN_HEIGHT - 30);
    
}
void Unit::updateDir(){
    //방향 설정한다.
    //dir : 0~360
    m_iPrevDir = m_iDir;
    float Angle = static_cast<float>( (acos(m_vHeading.dot(Vec2(1,0)))) *
                                     180.0f / MathMgr->Pi *
                                     m_vHeading.sign(Vec2(1, 0))) + 180;
    
    m_iDir = (int)(Angle / 22.5);
    MathMgr->Clamp(m_iDir, 0, 15);
}
//현재 Path.front에 저장되어 있는 타일 인덱스로 이동한다. 만약 Path가 비어있으면 false를 반환한다.
bool Unit::MoveToPathFront(int CurrentPacket){
    //해당 유닛의 패스를 가져온다.
    auto& Path = GetPathPlanner()->GetPath();
    auto pMap = m_pGameWorld->GetMap();
    auto& Graph = pMap->GetNavGraph();
    
    //LogMgr->Log("Path Size : %d",Path.size());
    
    //타겟팅부터 시작
//    FindTarget();
//    
//    //만약 타겟시스템에 유닛이 있고 해당 유닛이 공격 사정거리 안에 들었을 경우 때린다.
//    auto pTarget = GetTargetSystem()->GetTarget();
//    if(pTarget && CanAttackThing(pTarget)){
//        AttackTarget(CurrentPacket);
//        
//        SetHeading(pTarget->getPosition() - getPosition());
//        GetFSM()->ChangeState(State_Attack::Instance());
//        return false;
//    }
    
    
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
    GetFSM()->ChangeState(State_Move::Instance());
    
    return true;
}
void Unit::AttackTarget(int CurrentPacket){
    auto pTarget = m_pTargetSystem->GetTarget();
    
    //타겟이 없다면
    if(!pTarget){
        GetFSM()->ChangeState(State_Idle::Instance());
        return;
    }
    
    pTarget->AddHp( -5 );
    
    //만약 체력이 0보다 작을 시에, 타겟을 지운다.
    if(pTarget->GetHp() <= 0){
        pTarget->SetDead();
        m_pTargetSystem->ClearTarget();
        GetFSM()->ChangeState(State_Idle::Instance());
        //주변에 타겟이 있으면 새로운 공격, 아니면 정지 상태
        MoveToPathFront(CurrentPacket);
        return;
    }
    
    NetMgr->PushAutoTask(new AutoTaskAttack(CurrentPacket + 5, m_iID));
    SetAutoTaskPacket(CurrentPacket+5);
}
void Unit::FindTarget(){
    //만약 타겟이 없다면 주위에서 타겟을 찾는다.
    if(!m_pTargetSystem->GetTarget()){
        auto Targets = m_pGameWorld->GetNearThings(this,6);
        
        Thing* ClosestTarget = NULL;
        float ClosestDist = MathMgr->MaxFloat;
        
        //가장 가까운 타겟을 찾는다.
        auto Graph = m_pGameWorld->GetMap()->GetNavGraph();
        for(auto pTarget : Targets){
            //만약 해당 타겟의 팀이 내 팀이면 넘긴다.
            if(pTarget->GetPlayerFlag() == NetMgr->GetPlayerFlag())
                continue;
            
            //타겟이 죽었으면 넘긴다.
            if(pTarget->IsDead())
                continue;
            
            float Dist = Vec2DistanceSq(Graph.GetNode(GetTileIndex()).getPosition(), Graph.GetNode(pTarget->GetTileIndex()).getPosition());
            
            if(Dist < ClosestDist) {
                ClosestDist = Dist;
                ClosestTarget = pTarget;
            }
        }
        
        //만약 타겟이 있으면 타겟시스템에 잡는다.
        if(ClosestTarget){
            m_pTargetSystem->SetTarget(ClosestTarget);
            
            //해당 타겟으로 새로운 경로를 잡는다.
            m_pPathPlanner->CreatePathToPosition(ClosestTarget->GetTileIndex());
        }
    }
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