//
//  skyBuilding.cpp
//  Animal_Squad
//
//  Created by cong ku on 8/12/14.
//
//

#include "SkyBuilding.h"

#include "MapGenerator.h"

#define CEILING_HIGHT 161
#define BUILDING_TEX_WIDTH 413

SkyBuilding* SkyBuilding::create(Point pos, int groundY)
{
    SkyBuilding *a = new SkyBuilding();
    if (a&&a->init(pos, groundY)) {
        return a;
    }
    return NULL;
}

bool SkyBuilding::init(Point pos, int groundY)
{
    gLayer = MapGenerator::GetInstance()->gameLayer;
    gWorld = MapGenerator::GetInstance()->gameWorld;
    
    
    startPos = pos;
    groundYpos = groundY;
    
    cocos2d::Texture2D::TexParams params = {GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT};
    
    Texture2D* viewTexture = Director::getInstance()->getTextureCache()->addImage("building1.png");
    viewTexture->setTexParameters(params);
    viewTextureSize = Size(viewTexture->getPixelsWide(), viewTexture->getPixelsHigh());
    
    Texture2D* terrainTexture = Director::getInstance()->getTextureCache()->addImage("ground_terrain.png");
    terrainTexture->setTexParameters(params);
    
    Texture2D* surfaceTexture = Director::getInstance()->getTextureCache()->addImage("ground_surface.png");
    surfaceTexture->setTexParameters(params);
    surfaceTexWidth = 256;
    
    
    Vector2dVector empty;
    
    terrain = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, terrainTexture);
    surface = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, surfaceTexture);
    higherFrontView = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, viewTexture);
    lowerFrontView = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, viewTexture);
    gLayer->addChild(terrain,2);
    gLayer->addChild(surface, 3);
    gLayer->addChild(higherFrontView, 60);
    gLayer->addChild(lowerFrontView, 60);
    
    
    setVertices(pos);
    
    lastSetupPos = startPos;
    
    
    return true;
}

void SkyBuilding::setVertices(Point pos)
{
    //set ground texture
    
    Vector2dVector groundVecs;
    Vector2dVector ceilVecs;
    
    float ex_Y_hight = pos.y+1024;
    
    Point ground_lp;
    Point ground_rp;
    Point ceil_lp;
    Point ceil_rp;
    Point base_lp;
    Point base_rp;
    Point base_lp_lower;
    Point base_rp_lower;
    
    if ((pos.x - startPos.x) > MonsterOffset) {
        ground_lp = Point(pos.x - MonsterOffset, startPos.y);
        ground_rp = Point(pos.x + 1024, startPos.y);
    }
    else {
        ground_lp = Point(startPos.x, startPos.y);
        ground_rp = Point(pos.x + 1024, startPos.y);
    }
    
    offScreenPoint = ground_rp;
    
    if (ground_rp.x < lastSetupPos.x) {
        ground_rp = Point(lastSetupPos.x, ground_rp.y);
    }
    Vector2dVector points;
    Vector2dVector texCoords;

    
    
    
    //calc offset
    int x_offset = 0; //offset to make the building complete
    //int minCoordx = (ground_lp.x+5.0)/floorTextureSize.width;
    
   
    
    
    
    //add offset to the points
    ground_lp = Point(ground_lp.x, ground_lp.y);
    ground_rp = Point(ground_rp.x+x_offset, ground_rp.y);
    ceil_lp = Point(ground_lp.x, ground_lp.y+CEILING_HIGHT);
    ceil_rp = Point(ground_rp.x, ground_rp.y+CEILING_HIGHT);
    base_lp = Point(ground_lp.x, groundYpos);
    base_rp = Point(ground_rp.x, groundYpos);
    base_lp_lower = Point(base_lp.x, base_lp.y - SURFACE_THICKNESS);
    base_rp_lower = Point(base_rp.x, base_rp.y - SURFACE_THICKNESS);

    //setup surface
    float ground_l_texX = (base_lp.x - startPos.x)/surfaceTexWidth;
    ground_l_texX = ground_l_texX - (int)ground_l_texX;
    float ground_r_texX = ground_l_texX + (base_rp.x - base_lp.x)/surfaceTexWidth;
    points = makeVector(base_lp_lower, base_lp, base_rp, base_rp_lower);
    texCoords = makeVector(Point(ground_l_texX, 1.0), Point(ground_l_texX, 0.0), Point(ground_r_texX, 0.0), Point(ground_r_texX, 1.0));
    surface->customSetting(points, texCoords);

    //setup terrain
    points = pointsToVector(Point(base_lp.x, base_lp.y - 512),Point(base_lp.x, base_lp.y), Point(base_rp.x, base_rp.y), Point(base_rp.x, base_rp.y - 512));
    terrain->setPoints(points);
    
    //setup higherfrontview
    float side_texCoordX_l = (ground_lp.x - startPos.x)/BUILDING_TEX_WIDTH;
    float side_texCoordX_r = (ground_rp.x - startPos.x)/BUILDING_TEX_WIDTH;
    float side_height = ex_Y_hight - ceil_lp.y;
    float side_texCoordY = side_height/viewTextureSize.height;
    points = makeVector(ceil_lp, Point(ceil_lp.x, ceil_lp.y+side_height), Point(ceil_rp.x, ceil_rp.y+side_height), ceil_rp);
    texCoords = makeVector(Point(side_texCoordX_l, 1.0), Point(side_texCoordX_l, 1 - side_texCoordY), Point(side_texCoordX_r, 1 - side_texCoordY), Point(side_texCoordX_r, 1.0));
    higherFrontView->customSetting(points, texCoords);

    
    //setup lowerfrontview
    side_texCoordY = (ground_lp.y - groundYpos)/viewTextureSize.height;
    points = makeVector( Point(ground_lp.x, groundYpos), ground_lp, ground_rp, Point(ground_rp.x, groundYpos));
    texCoords = makeVector(Point(side_texCoordX_l, side_texCoordY), Point(side_texCoordX_l, 0.0), Point(side_texCoordX_r, 0.0), Point(side_texCoordX_r, side_texCoordY));
    lowerFrontView->customSetting(points, texCoords);
    
    
    
    
    if ((startPos.y - pos.y) > 600) {
        groundVecs =  pointsToVector(Point(ground_lp.x, pos.y - 400), Point(ground_lp.x, pos.y + 600), Point(ground_rp.x, pos.y + 600), Point(ground_rp.x, pos.y-400));
    }
    else {
        groundVecs = pointsToVector(Point(ground_lp.x, pos.y - 400), ground_lp, ground_rp, Point(ground_rp.x, pos.y-400));
    }
    ceilVecs = pointsToVector(Point(ceil_lp.x, ex_Y_hight), ceil_lp, ceil_rp, Point(ceil_rp.x, ex_Y_hight));
    
    setPhysicsTerrain(groundVecs, &groundBody);
    setPhysicsTerrain(ceilVecs, &ceilingBody);
    
    
    
    lastPos = ground_rp;
    
}


void SkyBuilding::terrainSceneArrangement()
{
    while (lastSetupPos.x < lastPos.x) {
        int randNum = rand()%3;
        string setupName;
        switch (randNum) {
            case 0:
                setupName = "apartment_room1";
                break;
            case 1:
                setupName = "apartment_room2";
                break;
            case 2:
                setupName = "apartment_room3";
                break;
            default:
                setupName = "apartment_room1";
                break;
        }
        MapGenerator::GetInstance()->setupSceneWithInfo(setupName, lastSetupPos);
        lastSetupPos = Point(lastSetupPos.x+BUILDING_TEX_WIDTH, lastSetupPos.y);
    }
}


void SkyBuilding::update(float dt, Point pos)
{
    Terrain::update(dt, pos);

    if (!dead) {
        terrainSceneArrangement();
        setVertices(pos);
    }
    else {
        if ((pos.x - lastPos.x) > MonsterOffset) {
            afterDeath = true;
        }
    }
}

void SkyBuilding::setDead()
{
    dead = true;
    //setVertices(pos);
}


SkyBuilding::~SkyBuilding()
{
    gLayer->removeChild(terrain, true);
    gLayer->removeChild(higherFrontView, true);
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
