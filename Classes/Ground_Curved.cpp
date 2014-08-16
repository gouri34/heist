//
//  Ground_Curved.cpp
//  Animal_Squad
//
//  Created by cong ku on 8/1/14.
//
//

#include "Ground_Curved.h"

#define SLOPE_DEGREE 24.0
#define SLOPE_DIUS 500
#define SLOPE_SMOOTH 24.0


Ground_Curved* Ground_Curved::create(Layer *gameScene_, b2World *gameWorld_, Point pos, bool curveUp, bool lowerOne,double _lastTexCoordX)
{
    Ground_Curved *a = new Ground_Curved();
    if (a&&a->init(gameScene_,gameWorld_, pos, curveUp, lowerOne,_lastTexCoordX)) {
        return a;
    }
    return NULL;
}


bool Ground_Curved::init(Layer *gameScene_, b2World *gameWorld_, Point pos, bool curveUp, bool lowerOne,double _lastTexCoordX)
{
    gLayer = gameScene_;
    gWorld = gameWorld_;
    
    startPos = pos;
    lastTexCoordX = (float)_lastTexCoordX - (int)_lastTexCoordX;
    
    isCurveUp = curveUp;
    isLowerOne = lowerOne;
    
    cocos2d::Texture2D::TexParams params = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
    
    
    Texture2D* groundTexture = Director::getInstance()->getTextureCache()->addImage("ground.png");
    groundTexture->setTexParameters(params);

    
    textureSize = groundTexture->getPixelsWide();
    
    
    Texture2D* terrainTexture = Director::getInstance()->getTextureCache()->addImage("terrain.png");
    terrainTexture->setTexParameters(params);
    
    
    
    Vector2dVector empty;
    
    terrain = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, terrainTexture);
    gLayer->addChild(terrain,2);
    
    ground = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, groundTexture);
    gLayer->addChild(ground, 2);
    
   
    
    setVertices(pos);
    
    // setupGroundBuildings(pos);
    setDead();
    
    return true;
}


void Ground_Curved::setVertices(Point pos)
{
    //set raw points
    Point centre;
    if (isCurveUp) {
        if (isLowerOne) {
            centre = Point(pos.x, pos.y+SLOPE_DIUS);
        }
        else {
            float x_offset = SLOPE_DIUS*sin(CC_DEGREES_TO_RADIANS(SLOPE_DEGREE));
            float y_offset = SLOPE_DIUS*cos(CC_DEGREES_TO_RADIANS(SLOPE_DEGREE));
            
            centre = Point(pos.x+x_offset, pos.y-y_offset);
        }
    }
    else {
        if (isLowerOne) {
            float x_offset = SLOPE_DIUS*sin(CC_DEGREES_TO_RADIANS(SLOPE_DEGREE));
            float y_offset = SLOPE_DIUS*cos(CC_DEGREES_TO_RADIANS(SLOPE_DEGREE));
            
            centre = Point(pos.x+x_offset, pos.y+y_offset);

        }
        else {
            centre = Point(pos.x, pos.y-SLOPE_DIUS);

        }
    }
    
    float currentDegree = 0;
    
    Vector2dVector vecs;
    vecs.push_back(Vector2d(pos.x, pos.y - 512));

    
    
    while (currentDegree < SLOPE_DEGREE) {
        float x_offset;
        float y_offset;
        
        
        Vector2d vec = Vector2d(0, 0);
        
        //int minCoordX = (int)(roadDistance/textureSize);

        
        
        if (isCurveUp) {
            
            if (isLowerOne) {
                x_offset = SLOPE_DIUS*sin(CC_DEGREES_TO_RADIANS(currentDegree));
                y_offset = SLOPE_DIUS*cos(CC_DEGREES_TO_RADIANS(currentDegree));
                
                vec = Vector2d(centre.x + x_offset, centre.y-y_offset);
                
                
            }
            else {
                float reversedDegree = SLOPE_DEGREE - currentDegree;
                x_offset = SLOPE_DIUS*sin(CC_DEGREES_TO_RADIANS(reversedDegree));
                y_offset = SLOPE_DIUS*cos(CC_DEGREES_TO_RADIANS(reversedDegree));
                
                vec = Vector2d(centre.x - x_offset, centre.y + y_offset);
            }
            
            
        }
        else {
            if (isLowerOne) {
                float reversedDegree = SLOPE_DEGREE - currentDegree;
                x_offset = SLOPE_DIUS*sin(CC_DEGREES_TO_RADIANS(reversedDegree));
                y_offset = SLOPE_DIUS*cos(CC_DEGREES_TO_RADIANS(reversedDegree));
                
                vec = Vector2d(centre.x - x_offset, centre.y - y_offset);
            }
            else {
                x_offset = SLOPE_DIUS*sin(CC_DEGREES_TO_RADIANS(currentDegree));
                y_offset = SLOPE_DIUS*cos(CC_DEGREES_TO_RADIANS(currentDegree));
                
                vec = Vector2d(centre.x+x_offset, centre.y+y_offset);
            }
        }
        
        
        //push vec to vecs
        vecs.push_back(vec);
        
        
        
        //reset pre points
        currentDegree += SLOPE_DEGREE/SLOPE_SMOOTH;
        lastPos = Point(vec.GetX(), vec.GetY());
        
    }
    
    Vector2d lastPoint = vecs.at(vecs.size()-1);
    vecs.push_back(Vector2d(lastPoint.GetX(), lastPoint.GetY()-512));
    
    
    
    terrain->setPoints(vecs);
    
    
    //construct physics
    setPhysicsTerrain(vecs, &groundBody);
    
}

void Ground_Curved::update(float dt, Point pos)
{
    if (!dead) {
        // printf("updating\n");
        //setVertices(pos);
        // setGroundBuildings(pos);
    }
    else {
        if ((pos.x - lastPos.x) > MonsterOffset) {
            afterDeath = true;
        }
    }
}


Ground_Curved::~Ground_Curved()
{
    gLayer->removeChild(ground, true);
    gLayer->removeChild(terrain, true);
    
    if (groundBody != NULL) {
        gWorld->DestroyBody(groundBody);
        groundBody = NULL;
    }
  
}