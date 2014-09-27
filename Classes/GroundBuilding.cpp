//
//  GroundBuilding.cpp
//  Animal_Squad
//
//  Created by cong ku on 8/5/14.
//
//

#include "GroundBuilding.h"
#include "MapGenerator.h"


#define CEILING_HIGHT 161
#define TEMP_HIGHT 8000

#define BUILDING_TEX_WIDTH 413

GroundBuilding* GroundBuilding::create(Point pos)
{
    GroundBuilding *a = new GroundBuilding();
    if (a&&a->init(pos)) {
        return a;
    }
    return NULL;
}

bool GroundBuilding::init(Point pos)
{
    gLayer = MapGenerator::GetInstance()->gameLayer;
    gWorld = MapGenerator::GetInstance()->gameWorld;
    
    
    startPos = pos;
    
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
    
    higherFrontView = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, viewTexture);
    terrain = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, terrainTexture);
    surface = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, surfaceTexture);
    gLayer->addChild(terrain,40);
    gLayer->addChild(surface,41);
    gLayer->addChild(higherFrontView, 60);
    
    setVertices(pos);
    //terrainSceneArrangement();
    
    lastSetupPos = startPos;
    
    return true;
}

void GroundBuilding::setVertices(Point pos)
{
    //printf("updating %f\n", pos.x);
    
    //set ground texture
    Vector2dVector groundVecs;
    Vector2dVector ceilVecs;
    
    float ex_Y_hight = pos.y+2048;
    
    Point ground_lp;
    Point ground_rp;
    Point ceil_lp;
    Point ceil_rp;
    Point ground_lp_lower;
    Point ground_rp_lower;
    
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
    
    /*int x_intexture = (int)(ground_rp.x+5 - startPos.x)%1024;
    
    if (x_intexture < 354) {
        x_offset = 354 - x_intexture;
    }
    else if (x_intexture < 697) {
        x_offset = 697 - x_intexture;
    }
    else if (x_intexture < 1024) {
        x_offset = 1024 - x_intexture;
    }*/
    
    
    //add offset to the points
    ground_lp = Point(ground_lp.x, ground_lp.y);
    ground_rp = Point(ground_rp.x+x_offset, ground_rp.y);
    ceil_lp = Point(ground_lp.x, ground_lp.y+CEILING_HIGHT);
    ceil_rp = Point(ground_rp.x, ground_rp.y+CEILING_HIGHT);
    ground_lp_lower = Point(ground_lp.x, ground_lp.y - SURFACE_THICKNESS);
    ground_rp_lower = Point(ground_rp.x, ground_rp.y - SURFACE_THICKNESS);
    
    
    
    float ground_l_texX = (ground_lp.x - startPos.x)/surfaceTexWidth;
    ground_l_texX = ground_l_texX - (int)ground_l_texX;
    float ground_r_texX = ground_l_texX + (ground_rp.x - ground_lp.x)/surfaceTexWidth;
    points = makeVector(ground_lp_lower, ground_lp, ground_rp, ground_rp_lower);
    texCoords = makeVector(Point(ground_l_texX, 1.0), Point(ground_l_texX, 0.0), Point(ground_r_texX, 0.0), Point(ground_r_texX, 1.0));
    surface->customSetting(points, texCoords);

    
    points = pointsToVector(Point(ground_lp.x, ground_lp.y-512), ground_lp, ground_rp, Point(ground_rp.x, ground_rp.y - 512));
    terrain->setPoints(points);
    
    
    float side_texCoordX_l = (ground_lp.x - startPos.x)/BUILDING_TEX_WIDTH;
    float side_texCoordX_r = (ground_rp.x - startPos.x)/BUILDING_TEX_WIDTH;
    float side_height = ex_Y_hight - ceil_lp.y;
    float side_texCoordY = side_height/viewTextureSize.height;
    points = makeVector(ceil_lp, Point(ceil_lp.x, ceil_lp.y+side_height), Point(ceil_rp.x, ceil_rp.y+side_height), ceil_rp);
    texCoords = makeVector(Point(side_texCoordX_l, 1.0), Point(side_texCoordX_l, 1 - side_texCoordY), Point(side_texCoordX_r, 1 - side_texCoordY), Point(side_texCoordX_r, 1.0));
    higherFrontView->customSetting(points, texCoords);
    
    groundVecs = pointsToVector(Point(ground_lp.x, ground_lp.y-512), ground_lp, ground_rp, Point(ground_rp.x, ground_rp.y-512));
    ceilVecs = pointsToVector(Point(ceil_lp.x, ex_Y_hight), ceil_lp, ceil_rp, Point(ceil_rp.x, ex_Y_hight));
    
    setPhysicsTerrain(groundVecs, &groundBody);
    setPhysicsTerrain(ceilVecs, &ceilingBody);
    
    
    lastPos = ground_rp;
}



void GroundBuilding::terrainSceneArrangement()
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



void GroundBuilding::update(float dt, Point pos)
{
    Terrain::update(dt, pos);

    if (!dead) {
        // setGroundBuildings(pos);
        terrainSceneArrangement();
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