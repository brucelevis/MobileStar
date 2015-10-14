#include "Thing.h"
#include "GameWorld.h"
#include "GameMap.h"
#include "SparseGraph.h"

Thing::Thing(GameWorld* pGame, Object_Type type, int playerFlag, int tileIndex)
 :m_pGameWorld(pGame)
 , m_ObjectType(type)
 , m_iID(IDMgr->GetNextThingID())
 , m_iTileIndex(tileIndex)
 , m_iPlayerFlag(playerFlag)
 ,m_bErase(false)
{
    //Thing의 위치를 설정한다.
    setPosition(pGame->GetMap()->GetNavGraph().GetNode(tileIndex).getPosition());
}
Thing::~Thing(){
}

void Thing::update(float eTime){
	Layer::update(eTime);
}

void Thing::SetTileIndex(int _tileindex){
    //그래프를 얻어온다.
    auto& Graph = m_pGameWorld->GetMap()->GetNavGraph();
    
    //이전 타일 인덱스를 비워 놓는다.
    Graph.GetNode(m_iTileIndex).SetEmpty();
    
    //다음 타일 인덱스를 채운다.
    Graph.GetNode(_tileindex).SetDynamic();
    
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