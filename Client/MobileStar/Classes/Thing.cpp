#include "Thing.h"
#include "GameWorld.h"
#include "GameMap.h"
#include "SparseGraph.h"

Thing::Thing(GameWorld* pGame, Object_Type type, int playerFlag, int tileIndex, int hp, int mp, float speed, float attackSpeed, int dmg, int armor)
 : m_pGameWorld(pGame)
 , m_Status(Status::Alive)
 , m_ObjectType(type)
 , m_iID(IDMgr->GetNextThingID())
 , m_iTileIndex(tileIndex)
 , m_iPlayerFlag(playerFlag)
 , m_bErase(false)
 , m_iAttackDistance(1)
 , m_fSpeed(speed)
 , m_fAttackSpeed(attackSpeed)
 , m_iDamage(dmg)
 , m_iArmor(armor)
 , m_iMaxHp(hp)
 , m_iMaxMp(mp)
 , m_iHp(hp)
 , m_iMp(mp)
{
    //Thing의 위치를 설정한다.
    setPosition(pGame->GetMap()->GetNavGraph().GetNode(tileIndex).getPosition());
    
    m_pTargetSystem = new TargetSystem(this);
}
Thing::~Thing(){
    delete m_pTargetSystem;
}

void Thing::update(float eTime){
	Layer::update(eTime);
}

void Thing::SetTileIndex(int _tileindex){
    //그래프를 얻어온다.
    auto& Graph = m_pGameWorld->GetMap()->GetNavGraph();
    
    //이전 타일 인덱스를 비워 놓는다.
    Graph.GetNode(m_iTileIndex).SetEmpty();
    Graph.GetNode(m_iTileIndex).SetThing(NULL);
    
    //다음 타일 인덱스를 채운다.
    Graph.GetNode(_tileindex).SetDynamic();
    Graph.GetNode(_tileindex).SetThing(this);
    
    //두 타일 인덱스의 엣지를 업데이트 시켜준다.
    Graph.UpdateEdgeFromNode(m_iTileIndex);
    Graph.UpdateEdgeFromNode(_tileindex);

    m_iTileIndex = _tileindex;
}

bool Thing::TouchBegan(Touch* touch, Event* _event){
	
	return true;
}
void Thing::TouchMoved(Touch* touch, Event* _event){

}
void Thing::TouchCancelled(Touch* touch, Event* _event){
	TouchEnded(touch, _event);
}
void Thing::TouchEnded(Touch* touch, Event *_event){

}
bool Thing::CanAttackThing(Thing* pThing){
    bool bAttack = false;
    
    auto Targets = m_pGameWorld->GetNearThings(this,m_iAttackDistance);
    
    for(auto pTarget : Targets){
        if(pTarget == pThing){
            bAttack = true;
            break;
        }
    }
    return bAttack;
}