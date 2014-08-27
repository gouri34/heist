//
//  GroundBuilding.cpp
//  Animal_Squad
//
//  Created by cong ku on 8/5/14.
//
//

#include "GroundBuilding.h"
#include "MapGenerator.h"


#define CEILING_HIGHT 300.0
#define TEMP_HIGHT 8000

GroundBuilding* GroundBuilding::create(Layer *gameScene_, b2World *gameWorld_, Point pos)
{
    GroundBuilding *a = new GroundBuilding();
    if (a&&a->init(gameScene_,gameWorld_, pos)) {
        return a;
    }
    return NULL;
}

bool GroundBuilding::init(Layer *gameScene_, b2World *gameWorld_, Point pos)
{
    gLayer = gameScene_;
    gWorld = gameWorld_;
    
    
    startPos = pos;
    
    cocos2d::Texture2D::TexParams params = {GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT};
    
    Texture2D* wallTexture = Director::getInstance()->getTextureCache()->addImage("testbuilding_wall.png");
    wallTexture->setTexParameters(params);
    wallTextureSize = Size(wallTexture->getPixelsWide(), wallTexture->getPixelsHigh());
    
    Texture2D* viewTexture = Director::getInstance()->getTextureCache()->addImage("testbuilding_view.png");
    viewTexture->setTexParameters(params);
    viewTextureSize = Size(viewTexture->getPixelsWide(), viewTexture->getPixelsHigh());
    
    Texture2D* terrainTexture = Director::getInstance()->getTextureCache()->addImage("terrain.png");
    terrainTexture->setTexParameters(params);

    
    Vector2dVector empty;
    
    
    wall = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, wallTexture);
    higherFrontView = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, viewTexture);
    terrain = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, terrainTexture);
    gLayer->addChild(wall, 2);
    gLayer->addChild(terrain,2);
    gLayer->addChild(higherFrontView, 60);
    
    setVertices(pos);

    
    return true;
}

void GroundBuilding::setVertices(Point pos)
{
    //printf("updating %f\n", pos.x);
    
    //set ground texture
    Vector2dVector groundVecs;
    Vector2dVector ceilVecs;
    
    float ex_Y_hight = pos.y+1024;

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
    //int minCoordx = (ground_lp.x+5.0)/floorTextureSize.width;
    
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

    
    points = pointsToVector(Point(ground_lp.x, ground_lp.y-512), ground_lp, ground_rp, Point(ground_rp.x, ground_rp.y - 512));
    terrain->setPoints(points);
    
    points = pointsToVector(Point(ground_lp.x+5, ground_lp.y), Point(ground_lp.x+5, ground_lp.y+CEILING_HIGHT), Point(ground_rp.x-5, ground_rp.y+CEILING_HIGHT), Point(ground_rp.x-5, ground_rp.y));
    wall->setPoints(points);
    
    
    float side_texCoordX_l = (ground_lp.x - startPos.x)/viewTextureSize.width;
    float side_texCoordX_r = (ground_rp.x - startPos.x)/viewTextureSize.width;
    float side_texCoordY = TEMP_HIGHT/viewTextureSize.height;
    points = makeVector(ceil_lp, Point(ceil_lp.x, ceil_lp.y+TEMP_HIGHT), Point(ceil_rp.x, ceil_rp.y+TEMP_HIGHT), ceil_rp);
    texCoords = makeVector(Point(side_texCoordX_l, 1.0), Point(side_texCoordX_l, 1 - side_texCoordY), Point(side_texCoordX_r, 1 - side_texCoordY), Point(side_texCoordX_r, 1.0));
    higherFrontView->customSetting(points, texCoords);
    
    groundVecs = pointsToVector(Point(ground_lp.x, ground_lp.y-512), ground_lp, ground_rp, Point(ground_rp.x, ground_rp.y-512));
    ceilVecs = pointsToVector(Point(ceil_lp.x, ex_Y_hight), ceil_lp, ceil_rp, Point(ceil_rp.x, ex_Y_hight));
    
    setPhysicsTerrain(groundVecs, &groundBody);
    setPhysicsTerrain(ceilVecs, &ceilingBody);
    
    
    
    
    lastPos = ground_rp;
}




void GroundBuilding::update(float dt, Point pos)
{

    if (!dead) {
        // setGroundBuildings(pos);
        setVertices(pos);

    }
    else {
        if ((pos.x - lastPos.x) > MonsterOffset) {
            afterDeath = true;
        }
    }
}

void GroundBuilding::setDead()
{
    dead = true;
    //setVertices(pos);
}


GroundBuilding::~GroundBuilding()
{
    gLayer->removeChild(wall, true);
    gLayer->removeChild(terrain, true);
    gLayer->removeChild(higherFrontView, true);

    if (groundBody != NULL) {
        gWorld->DestroyBody(groundBody);
        groundBody = NULL;
    }
    if (roofBody != NULL) {
        gWorld->DestroyBody(roofBody);
        roofBody = NULL;
    }
}
