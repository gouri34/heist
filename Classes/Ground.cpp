//
//  gound.cpp
//  Bear
//
//  Created by cong ku on 14-6-25.
//
//

//test building lengths
//tall_bld : 352
//mid_bld : 342
//short_bld : 370

//zscales:
//buildings : 10 - 20
//characters : 20 ++




#include "Ground.h"
#include "MapGenerator.h"

Ground* Ground::create(Point pos, double _lastTexCoordX)
{
    Ground *a = new Ground();
    if (a&&a->init(pos, _lastTexCoordX)) {
        return a;
    }
    return NULL;
}

bool Ground::init(Point pos, double _lastTexCoordX)
{
    gLayer = MapGenerator::GetInstance()->gameLayer;
    gWorld = MapGenerator::GetInstance()->gameWorld;
    
    startPos = Point(pos.x -1, pos.y);
    backgroundSetupPos = startPos;
    texCoordXoffset = (float)_lastTexCoordX - (int)_lastTexCoordX;
    
    cocos2d::Texture2D::TexParams params = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
    
    Texture2D* terrainTexture = Director::getInstance()->getTextureCache()->addImage("ground_terrain.png");
    terrainTexture->setTexParameters(params);
   
    Texture2D* surfaceTexture = Director::getInstance()->getTextureCache()->addImage("ground_surface.png");
    surfaceTexture->setTexParameters(params);
    surfaceTexWidth = 256;
    
    
    Vector2dVector empty;
    
    
    terrain = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, terrainTexture);
    surface = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, surfaceTexture);
    gLayer->addChild(terrain,10);
    gLayer->addChild(surface, 11);
    
    setVertices(pos);
    
    
    
    return true;
}


void Ground::setVertices(Point pos)
{
    Point ground_lp;
    Point ground_rp;
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

    if (ground_rp.x < backgroundSetupPos.x) {
        ground_rp = Point(backgroundSetupPos.x, ground_rp.y);
    }
    
    ground_lp_lower = Point(ground_lp.x, ground_lp.y - SURFACE_THICKNESS);
    ground_rp_lower = Point(ground_rp.x, ground_rp.y - SURFACE_THICKNESS);
    
    //Vector2dVector gvecs = makeVector(lp1, lp2, rp1, rp2);
    //ground->setPoints(gvecs);
    Vector2dVector points;
    Vector2dVector texCoords;
    float ground_l_texX = (ground_lp.x - startPos.x)/surfaceTexWidth + texCoordXoffset;
    ground_l_texX = ground_l_texX - (int)ground_l_texX;
    float ground_r_texX = ground_l_texX + (ground_rp.x - ground_lp.x)/surfaceTexWidth;
    points = makeVector(ground_lp_lower, ground_lp, ground_rp, ground_rp_lower);
    texCoords = makeVector(Point(ground_l_texX, 1.0), Point(ground_l_texX, 0.0), Point(ground_r_texX, 0.0), Point(ground_r_texX, 1.0));
    surface->customSetting(points, texCoords);
    
    Vector2dVector tvecs = pointsToVector(Point(ground_lp_lower.x, ground_lp_lower.y-800), ground_lp_lower, ground_rp_lower, Point(ground_rp_lower.x, ground_rp_lower.y - 800));
    terrain->setPoints(tvecs);
    
    //set physics
    Vector2dVector physicsVec;
    physicsVec.push_back(Vector2d(ground_lp.x, ground_lp.y));
    physicsVec.push_back(Vector2d(ground_rp.x, ground_rp.y));
    setPhysicsTerrain(physicsVec, &groundBody);

    lastTexCoordX = ground_r_texX;;
    lastPos = Point(ground_rp.x-1, ground_rp.y);
    groundYpos = ground_rp.y;
}

void Ground::terrainSceneArrangement()
{
    //background arrangement
    while (backgroundSetupPos < lastPos) {
        int seed = rand()%5;
        while (seed == lastSeed) {
            seed = rand()%5;
        }
        lastSeed = seed;
        
        string sceneName;
        switch (seed) {
            case 0:
                sceneName = "groundBack0";
                break;
            case 1:
                sceneName = "groundBack1";
                break;
            case 2:
                sceneName = "groundBack2";
                break;
            case 3:
                sceneName = "groundBack3";
                break;
            case 4:
                sceneName = "groundBack4";
                break;
            default:
                sceneName = "groundBack0";
                break;
        }
        
        int length = MapGenerator::GetInstance()->setupSceneWithInfo(sceneName, backgroundSetupPos);
        backgroundSetupPos = Point(backgroundSetupPos.x+length, backgroundSetupPos.y);
    }
    
    //objects arrangement
    if (MapGenerator::GetInstance()->enemyTimer <= 0) {
        MapGenerator::GetInstance()->enemyTimer = 2.4;
        string arrName = "enemySetup1";
        int length = MapGenerator::GetInstance()->setupSceneWithInfo(arrName, Point(offScreenPoint.x, offScreenPoint.y+100));
        int lastX = offScreenPoint.x+length;
        if (lastX > backgroundSetupPos.x) {
            backgroundSetupPos = Point(lastX, backgroundSetupPos.y);
        }
    }

}


void Ground::update(float dt, cocos2d::Point pos)
{
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



Ground::~Ground()
{
    gLayer->removeChild(terrain, true);
    gLayer->removeChild(surface, true);
    
    if (groundBody != NULL) {
        gWorld->DestroyBody(groundBody);
        groundBody = NULL;
    }
}