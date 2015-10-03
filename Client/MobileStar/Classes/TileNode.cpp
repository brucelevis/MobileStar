#include "TileNode.h"

TileNode::TileNode(){
    m_pTileSprite = Sprite::create("Texture/Tile.png");
    addChild(m_pTileSprite);
}
TileNode::~TileNode(){
    
}