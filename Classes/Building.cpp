//
//  Building.cpp
//  Animal_Squad
//
//  Created by cong ku on 14-7-4.
//
//

#include "Building.h"

#define PTM_RATIO 32.0

#define MonsterOffset 300
#define WallHeight 300


Building* Building::create(Layer *gameScene_, b2World *gameWorld_, Point pos, int btype)
{
    Building *a = new Building();
    if (a&&a->init(gameScene_,gameWorld_, pos, btype)) {
        return a;
    }
    return NULL;
}

bool Building::init(Layer *gameScene_, b2World *gameWorld_, Point pos, int btype)
{
    gLayer = gameScene_;
    gWorld = gameWorld_;
    
    startPos = pos;
    
    cocos2d::Texture2D::TexParams params = { GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT };

    
    Texture2D* groundTexture = Director::getInstance()->getTextureCache()->addImage("GlassBuilding.png");
    groundTexture->setTexParameters(params);
    

    Texture2D* wallTexture = Director::getInstance()->getTextureCache()->addImage("GlassBuilding_Wall.png");
    wallTexture->setTexParameters(params);
    
    
    
    Vector2dVector empty;
    
    buildingType = btype;
    
       setVertices(pos);

   
    
    
    return true;
}



Building::~Building()
{

}
