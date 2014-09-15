//
//  Ground_Slope.cpp
//  Animal_Squad
//
//  Created by cong ku on 8/1/14.
//
//

#include "Ground_Slope.h"
#include "MapGenerator.h"



Ground_Slope* Ground_Slope:: create(Point pos, bool curveUp,double _lastTexCoordX)
{
    Ground_Slope *a = new Ground_Slope();
    if (a&&a->init(pos, curveUp, _lastTexCoordX)) {
        return a;
    }
    return NULL;
}

bool Ground_Slope::init(Point pos, bool curveUp,double _lastTexCoordX)
{
    gLayer = MapGenerator::GetInstance()->gameLayer;
    gWorld = MapGenerator::GetInstance()->gameWorld;
    
    startPos = Point(pos.x-1, pos.y);
    texCoordXoffset = (float)_lastTexCoordX - (int)_lastTexCoordX;
    
    isCurveUp = curveUp;

    
    cocos2d::Texture2D::TexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
    
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


void Ground_Slope::setVertices(Point pos)
{
    Point leftPos;
    Point rightPos;
    Point lower_leftPos;
    Point lower_rightPos;
    
   
    
    //set physics
    if ((pos.x - startPos.x) > MonsterOffset) {
        float leftPosX = pos.x - MonsterOffset;
        float xDiff = leftPosX- startPos.x;
        float yDiff = tanf(CC_DEGREES_TO_RADIANS(SLOPE_DEGREE))*xDiff;
        
        float xRightDiff = 1024;
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
    
    float xoffset = SURFACE_THICKNESS*sin(CC_DEGREES_TO_RADIANS(SLOPE_DEGREE));
    float yoffset = SURFACE_THICKNESS*cos(CC_DEGREES_TO_RADIANS(SLOPE_DEGREE));
    if (isCurveUp) {
        lower_leftPos = Point(leftPos.x+xoffset, leftPos.y-yoffset);
        lower_rightPos = Point(rightPos.x+xoffset, rightPos.y-yoffset);
    }
    else {
        lower_leftPos = Point(leftPos.x-xoffset, leftPos.y-yoffset);
        lower_rightPos = Point(rightPos.x-xoffset, rightPos.y-yoffset);
    }
    float ground_l_texX = (leftPos - startPos).length()/surfaceTexWidth + texCoordXoffset;
    ground_l_texX = ground_l_texX - (int)ground_l_texX;
    float ground_r_texX = (rightPos - leftPos).length()/surfaceTexWidth + ground_l_texX;
    Vector2dVector surfacePoints = makeVector(lower_leftPos, leftPos, rightPos, lower_rightPos);
    Vector2dVector surfaceTexCoords = makeVector(Point(ground_l_texX, 1.0), Point(ground_l_texX, 0.0), Point(ground_r_texX, 0.0), Point(ground_r_texX, 1.0));
    surface->customSetting(surfacePoints, surfaceTexCoords);
    
    
    
    
    //render terrain
    Vector2dVector terrainVecs;
    terrainVecs.push_back(Vector2d(lower_leftPos.x, lower_leftPos.y-800));
    terrainVecs.push_back(Vector2d(lower_leftPos.x, lower_leftPos.y));
    terrainVecs.push_back(Vector2d(lower_rightPos.x, lower_rightPos.y));
    terrainVecs.push_back(Vector2d(lower_rightPos.x, lower_rightPos.y-800));
    terrain->setPoints(terrainVecs);
    
    
    //set physics
    Vector2dVector physicsVec;
    physicsVec.push_back(Vector2d(leftPos.x, leftPos.y));
    physicsVec.push_back(Vector2d(rightPos.x, rightPos.y));
    setPhysicsTerrain(physicsVec, &groundBody);
    
    lastPos = rightPos;
    groundYpos = rightPos.y;
    
    
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
    
    gLayer->removeChild(terrain, true);
    gLayer->removeChild(surface, true);
    
    if (groundBody != NULL) {
        gWorld->DestroyBody(groundBody);
        groundBody = NULL;
    }
}