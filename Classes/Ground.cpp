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

#define UPPER_GROUND_WIDTH 20.0
#define LOWER_GROUND_WIDTH 21.0

Ground* Ground::create(Layer *gameScene_, b2World *gameWorld_, Point pos, double _lastTexCoordX)
{
    Ground *a = new Ground();
    if (a&&a->init(gameScene_,gameWorld_, pos, _lastTexCoordX)) {
        return a;
    }
    return NULL;
}

bool Ground::init(Layer *gameScene_, b2World *gameWorld_, Point pos, double _lastTexCoordX)
{
    gLayer = gameScene_;
    gWorld = gameWorld_;
    
    startPos = pos;
    lastTexCoordX = (float)_lastTexCoordX - (int)_lastTexCoordX;
    
    cocos2d::Texture2D::TexParams params = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
    
    
    Texture2D* groundTexture = Director::getInstance()->getTextureCache()->addImage("ground.png");
    groundTexture->setTexParameters(params);
    
    textureSize = groundTexture->getPixelsWide();
    
    Texture2D* terrainTexture = Director::getInstance()->getTextureCache()->addImage("terrain.png");
    terrainTexture->setTexParameters(params);
    
    
    float texCoordStartX = pos.x/textureSize;
    texCoordStartX = texCoordStartX - (int)texCoordStartX;
    texCoordXoffset = lastTexCoordX-texCoordStartX;
    if (texCoordXoffset < 0) {
        texCoordXoffset += 1.0;
    }
    
    
    Vector2dVector empty;
    
    
   
    
    terrain = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, terrainTexture);
    gLayer->addChild(terrain,2);
    
    setVertices(pos);
    
   // setupGroundBuildings(pos);
    
    
    return true;
}


void Ground::setupGroundBuildings(Point pos)
{
    float targetX = pos.x + 1500;
    float curX = pos.x;
    int baseZ = 20;
    
    //add firstbuilding
    Sprite* f_bld = Sprite::create("tall_bld.png");
    f_bld->setAnchorPoint(Point(0,0));
    f_bld->setPosition(Point(pos.x, pos.y+38));
    gLayer->addChild(f_bld, baseZ);
    baseZ -= 1;
    groundBuildings.push_back(f_bld);
    curX += 352;
    
    while (curX < targetX) {
        int randNum = rand()%3;
        
        Sprite* bld;
        
        if (randNum == 0) {
            bld = Sprite::create("short_bld.png");
            bld->setAnchorPoint(Point(0,0));
            bld->setPosition(Point(curX, pos.y+38));
            gLayer->addChild(bld, baseZ);
            baseZ -= 1;
            groundBuildings.push_back(bld);
            curX += 370;

        }
        else if (randNum == 1)
        {
            bld = Sprite::create("mid_bld.png");
            bld->setAnchorPoint(Point(0,0));
            bld->setPosition(Point(curX, pos.y+38));
            gLayer->addChild(bld, baseZ);
            baseZ -= 1;
            groundBuildings.push_back(bld);
            curX += 342;
        }
        else {
            bld = Sprite::create("tall_bld.png");
            bld->setAnchorPoint(Point(0,0));
            bld->setPosition(Point(curX, pos.y+38));
            gLayer->addChild(bld, baseZ);
            baseZ -= 1;
            groundBuildings.push_back(bld);
            curX += 352;

        }
    }
    
    //set ground base Z to get right order
    groundBaseZ = baseZ;
    lastBuildingX = curX;
}




void Ground::setVertices(Point pos)
{
    Point ground_lp;
    Point ground_rp;
    if ((pos.x - startPos.x) > MonsterOffset) {
        ground_lp = Point(pos.x - MonsterOffset, startPos.y);
        ground_rp = Point(pos.x + 1024, startPos.y);
    }
    else {
        ground_lp = Point(startPos.x, startPos.y);
        ground_rp = Point(pos.x + 1024, startPos.y);
    }

    
    
    //Vector2dVector gvecs = makeVector(lp1, lp2, rp1, rp2);
    //ground->setPoints(gvecs);
    Vector2dVector points;
    Vector2dVector texCoords;
   
    Vector2dVector tvecs = pointsToVector(Point(ground_lp.x, ground_lp.y-512), ground_lp, ground_rp, Point(ground_rp.x, ground_rp.y - 512));
    terrain->setPoints(tvecs);
    
    //set physics
    setPhysicsTerrain(tvecs, &groundBody);

    lastTexCoordX = ground_rp.x/textureSize;
    lastPos = ground_rp;
    groundYpos = ground_rp.y;
}


void Ground::setGroundBuildings(Point pos)
{
    
    
    //put buildings if needed
    int baseZ = groundBaseZ;
    float targetX = pos.x+1500;
    float curX = lastBuildingX;
    
    while (curX < targetX) {
        int randNum = rand()%3;
        
        Sprite* bld;
        
        if (randNum == 0) {
            bld = Sprite::create("short_bld.png");
            bld->setAnchorPoint(Point(0,0));
            bld->setPosition(Point(curX, startPos.y+38));
            gLayer->addChild(bld, baseZ);
            baseZ -= 1;
            groundBuildings.push_back(bld);
            curX += 370;
            
        }
        else if (randNum == 1)
        {
            bld = Sprite::create("mid_bld.png");
            bld->setAnchorPoint(Point(0,0));
            bld->setPosition(Point(curX, startPos.y+38));
            gLayer->addChild(bld, baseZ);
            baseZ -= 1;
            groundBuildings.push_back(bld);
            curX += 342;
            
        }
        else {
            bld = Sprite::create("tall_bld.png");
            bld->setAnchorPoint(Point(0,0));
            bld->setPosition(Point(curX, startPos.y+38));
            gLayer->addChild(bld, baseZ);
            baseZ -= 1;
            groundBuildings.push_back(bld);
            curX += 352;
            
        }
    }

    groundBaseZ = baseZ;
    lastBuildingX = curX;
    
    
    //remove used groundBuildings
    std::vector<Sprite*> usedBld;
    for (int i = 0; i < groundBuildings.size(); i++) {
        Sprite* b = groundBuildings.at(i);
        if (b->getPositionX() < pos.x - MonsterOffset-600) {
            usedBld.push_back(b);
        }
    }
    for (int i = 0; i < usedBld.size(); i++) {
        Sprite* b = usedBld.at(i);
        groundBuildings.erase(std::remove(groundBuildings.begin(), groundBuildings.end(), b), groundBuildings.end());
        gLayer->removeChild(b, true);
        groundBaseZ ++;
        
        for (int i = 0; i < groundBuildings.size(); i++) {
            Sprite* b = groundBuildings.at(i);
            b->setZOrder(b->getZOrder()+1);
        }
    }

    
}

void Ground::update(float dt, cocos2d::Point pos)
{
   // printf("updating\n");
    
    if (!dead) {
        // printf("updating\n");
        setVertices(pos);
       // setGroundBuildings(pos);
    }
    else {
        if ((pos.x - lastPos.x) > MonsterOffset) {
            afterDeath = true;
        }
    }
}



Ground::~Ground()
{
    //printf("removed\n");

    gLayer->removeChild(terrain, true);
    
    if (groundBody != NULL) {
        gWorld->DestroyBody(groundBody);
        groundBody = NULL;
    }
}
