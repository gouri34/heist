//
//  SkyBuildingRoof.cpp
//  Animal_Squad
//
//  Created by cong ku on 8/17/14.
//
//

#include "SkyBuildingRoof.h"


#include "MapGenerator.h"

#define CEILING_HIGHT 300.0

SkyBuildingRoof* SkyBuildingRoof::create(Layer *gameScene_, b2World *gameWorld_, Point pos, int groundY)
{
    SkyBuildingRoof *a = new SkyBuildingRoof();
    if (a&&a->init(gameScene_,gameWorld_, pos, groundY)) {
        return a;
    }
    return NULL;
}

bool SkyBuildingRoof::init(Layer *gameScene_, b2World *gameWorld_, Point pos, int groundY)
{
    gLayer = gameScene_;
    gWorld = gameWorld_;
    
    
    startPos = pos;
    groundYpos = groundY;
    
    cocos2d::Texture2D::TexParams params = {GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT};
    
    
    
    Texture2D* viewTexture = Director::getInstance()->getTextureCache()->addImage("testbuilding_view.png");
    viewTexture->setTexParameters(params);
    viewTextureSize = Size(viewTexture->getPixelsWide(), viewTexture->getPixelsHigh());
    
    Texture2D* terrainTexture = Director::getInstance()->getTextureCache()->addImage("terrain.png");
    terrainTexture->setTexParameters(params);
    
    
    Vector2dVector empty;
    
    terrain = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, terrainTexture);
    lowerFrontView = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, viewTexture);
    gLayer->addChild(terrain,2);
    gLayer->addChild(lowerFrontView, 60);
    
    
    setVertices(pos);
    
    
    return true;
}

void SkyBuildingRoof::setVertices(Point pos)
{
    //set ground texture
    Vector2dVector groundVecs;
    Vector2dVector ceilVecs;
    
    
    Point ground_lp;
    Point ground_rp;
    Point ceil_lp;
    Point ceil_rp;
    
    
    Vector2dVector points;
    Vector2dVector texCoords;
    
    
    
    if ((pos.x - startPos.x) > MonsterOffset) {
        
        ground_lp = Point(pos.x - MonsterOffset, startPos.y);
        ground_rp = Point(pos.x + 1024, startPos.y);
        ceil_lp = Point(pos.x - MonsterOffset, startPos.y + CEILING_HIGHT);
        ceil_rp = Point(pos.x + 1024, startPos.y + CEILING_HIGHT);
        
    }
    else {
        
        ground_lp = Point(startPos.x, startPos.y);
        ground_rp = Point(pos.x + 1024, startPos.y);
        ceil_lp = Point(startPos.x, startPos.y + CEILING_HIGHT);
        ceil_rp = Point(pos.x + 1024, startPos.y + CEILING_HIGHT);
        
        
    }
    
    //calc offset
    int x_offset = 0; //offset to make the building complete
    
    int x_intexture = (int)(ground_rp.x+5 - startPos.x)%1024;
    
    if (x_intexture < 354) {
        x_offset = 354 - x_intexture;
    }
    else if (x_intexture < 697) {
        x_offset = 697 - x_intexture;
    }
    else if (x_intexture < 1024) {
        x_offset = 1024 - x_intexture;
    }
    
    
    //add offset to the points
    ground_lp = Point(ground_lp.x, ground_lp.y);
    ground_rp = Point(ground_rp.x+x_offset, ground_rp.y);
    ceil_lp = Point(ceil_lp.x, ceil_lp.y);
    ceil_rp = Point(ceil_rp.x+x_offset, ceil_rp.y);
    
    
    points = pointsToVector(Point(ground_lp.x, groundYpos - 512),Point(ground_lp.x, groundYpos), Point(ground_rp.x, groundYpos), Point(ground_rp.x, groundYpos - 512));
    terrain->setPoints(points);
    
    
    float side_texCoordX_l = (ground_lp.x - startPos.x)/viewTextureSize.width;
    float side_texCoordX_r = (ground_rp.x - startPos.x)/viewTextureSize.width;
    float side_texCoordY = (ground_lp.y - groundYpos)/viewTextureSize.height;
    points = makeVector(Point(ground_lp.x,groundYpos), ground_lp, ground_rp, Point(ground_rp.x, groundYpos));
    texCoords = makeVector(Point(side_texCoordX_l, 0.0), Point(side_texCoordX_l, side_texCoordY), Point(side_texCoordX_r, side_texCoordY), Point(side_texCoordX_r, 0.0));
    lowerFrontView->customSetting(points, texCoords);
    
    
    
    if ((startPos.y - pos.y) > 600) {
        groundVecs =  pointsToVector(Point(ground_lp.x, pos.y - 400), Point(ground_lp.x, pos.y + 600), Point(ground_rp.x, pos.y + 600), Point(ground_rp.x, pos.y-400));
    }
    else {
        groundVecs = pointsToVector(Point(ground_lp.x, pos.y - 400), ground_lp, ground_rp, Point(ground_rp.x, pos.y-400));
    }
    
    setPhysicsTerrain(groundVecs, &groundBody);
    
    lastPos = ground_rp;
}

void SkyBuildingRoof::update(float dt, Point pos)
{
    if (!dead) {
        setVertices(pos);
        // setGroundBuildings(pos);
    }
    else {
        if ((pos.x - lastPos.x) > MonsterOffset) {
            afterDeath = true;
        }
    }
}

void SkyBuildingRoof::setDead()
{
    dead = true;
    //setVertices(pos);
}


SkyBuildingRoof::~SkyBuildingRoof()
{
    gLayer->removeChild(terrain, true);
    gLayer->removeChild(lowerFrontView, true);
    
    if (groundBody != NULL) {
        gWorld->DestroyBody(groundBody);
        groundBody = NULL;
    }
    if (roofBody != NULL) {
        gWorld->DestroyBody(roofBody);
        roofBody = NULL;
    }
}