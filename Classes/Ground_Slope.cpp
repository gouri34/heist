//
//  Ground_Slope.cpp
//  Animal_Squad
//
//  Created by cong ku on 8/1/14.
//
//

#include "Ground_Slope.h"

#define SLOPE_DEGREE 24.0


Ground_Slope* Ground_Slope:: create(Layer *gameScene_, b2World *gameWorld_, Point pos, bool curveUp,double _lastTexCoordX)
{
    Ground_Slope *a = new Ground_Slope();
    if (a&&a->init(gameScene_,gameWorld_, pos, curveUp, _lastTexCoordX)) {
        return a;
    }
    return NULL;
}

bool Ground_Slope::init(Layer *gameScene_, b2World *gameWorld_, Point pos, bool curveUp,double _lastTexCoordX)
{
    gLayer = gameScene_;
    gWorld = gameWorld_;
    
    startPos = pos;
    lastTexCoordX = (float)_lastTexCoordX - (int)_lastTexCoordX;
    startTexCoordX = lastTexCoordX;
    
    isCurveUp = curveUp;

    
    cocos2d::Texture2D::TexParams params = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
    
    
    Texture2D* groundTexture = Director::getInstance()->getTextureCache()->addImage("ground.png");
    groundTexture->setTexParameters(params);

    
    textureSize = groundTexture->getPixelsWide();
    
    
    Texture2D* terrainTexture = Director::getInstance()->getTextureCache()->addImage("terrain.png");
    terrainTexture->setTexParameters(params);
    
    
    Vector2dVector empty;
    
    
    ground = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, groundTexture);
    gLayer->addChild(ground, 2);
    
    terrain = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, terrainTexture);
    gLayer->addChild(terrain,2);
    
    setVertices(pos);
    
    
    return true;
}


void Ground_Slope::setVertices(Point pos)
{
    Point leftPos;
    Point rightPos;
    
    
   
    
    //set physics
    if ((pos.x - startPos.x) > MonsterOffset) {
        float leftPosX = pos.x - MonsterOffset;
        float xDiff = leftPosX- startPos.x;
        float yDiff = tanf(CC_DEGREES_TO_RADIANS(SLOPE_DEGREE))*xDiff;
        
        float xRightDiff = 1524;
        float yRightDiff = tanf(CC_DEGREES_TO_RADIANS(SLOPE_DEGREE))*xRightDiff;
        
        if (isCurveUp) {
            leftPos = Point(leftPosX, startPos.y+yDiff);
            rightPos = Point(leftPos.x+xRightDiff, leftPos.y+yRightDiff);
        }
        else {
            leftPos = Point(leftPosX, startPos.y-yDiff);
            rightPos = Point(leftPos.x+xRightDiff, leftPos.y-yRightDiff);
        }
    }
    else {
        leftPos = startPos;
        float xRightDiff = 1024;
        float yRightDiff = tanf(CC_DEGREES_TO_RADIANS(SLOPE_DEGREE))*xRightDiff;
        if (isCurveUp) {
            rightPos = Point(leftPos.x+xRightDiff, leftPos.y+yRightDiff);
        }
        else {
            rightPos = Point(leftPos.x+xRightDiff, leftPos.y-yRightDiff);
        }
        
    }
    
    lastPos = rightPos;
    
    
    
    b2BodyDef abodyDef;
    
	abodyDef.bullet = false;
	abodyDef.type = b2_staticBody;
    b2EdgeShape astaticBox;
    
    b2FixtureDef fixtureDef;
	fixtureDef.shape = &astaticBox;
	fixtureDef.density = 5.0;
	fixtureDef.friction = 0.5;
	fixtureDef.restitution = 0.3;
    fixtureDef.isSensor = false;
    fixtureDef.fixturetype = f_ground;
    
    
    if (groundBody) {
        gWorld->DestroyBody(groundBody);
        groundBody = NULL;
    }
    
    
    groundBody = gWorld->CreateBody(&abodyDef);
    b2Vec2 vv1 = b2Vec2(leftPos.x/PTM_RATIO, leftPos.y/PTM_RATIO);
    b2Vec2 vv2 = b2Vec2(rightPos.x/PTM_RATIO, rightPos.y/PTM_RATIO);
        
    astaticBox.Set(vv1, vv2);
    groundBody->CreateFixture(&fixtureDef);
    
    
    
    
    //render terrain
    Vector2dVector terrainVecs;
    terrainVecs.push_back(Vector2d(leftPos.x, leftPos.y-512));
    terrainVecs.push_back(Vector2d(leftPos.x, leftPos.y));
    terrainVecs.push_back(Vector2d(rightPos.x, rightPos.y));
    terrainVecs.push_back(Vector2d(rightPos.x, rightPos.y-512));
    
    terrain->setPoints(terrainVecs);
    
}



void Ground_Slope::update(float dt, cocos2d::Point pos)
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



Ground_Slope::~Ground_Slope()
{
    //printf("removed\n");
    
    gLayer->removeChild(ground, true);
    gLayer->removeChild(terrain, true);
    
    if (groundBody != NULL) {
        gWorld->DestroyBody(groundBody);
        groundBody = NULL;
    }
}