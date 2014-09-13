//
//  Ground_Curved.cpp
//  Animal_Squad
//
//  Created by cong ku on 8/1/14.
//
//

#include "Ground_Curved.h"
#include "MapGenerator.h"

#define SLOPE_DIUS 500
#define SLOPE_SMOOTH 20.0


Ground_Curved* Ground_Curved::create( Point pos, bool curveUp, bool lowerOne,double _lastTexCoordX)
{
    Ground_Curved *a = new Ground_Curved();
    if (a&&a->init(pos, curveUp, lowerOne,_lastTexCoordX)) {
        return a;
    }
    return NULL;
}


bool Ground_Curved::init(Point pos, bool curveUp, bool lowerOne,double _lastTexCoordX)
{
    gLayer = MapGenerator::GetInstance()->gameLayer;
    gWorld = MapGenerator::GetInstance()->gameWorld;
    
    startPos = Point(pos.x, pos.y);
    texCoordXoffset = (float)_lastTexCoordX - (int)_lastTexCoordX;
    
    isCurveUp = curveUp;
    isLowerOne = lowerOne;
    
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
    Vector2dVector lowerVecs;
    
    
    Vector2d pre_vec = Vector2d(-100, -100);
    Vector2d pre_lowerVec = Vector2d(-100, -100);
    
    float pre_textureCoordX;
    float textureCoordX = texCoordXoffset;

    Vector2dVector surfacePoints;
    Vector2dVector surfaceTexCoords;
    
    while (currentDegree < SLOPE_DEGREE) {
        float x_offset;
        float y_offset;
        float ground_x_offset;
        float ground_y_offset;
        
        Vector2d vec = Vector2d(0, 0);
        Vector2d lower_vec = Vector2d(0, 0);
        
        //int minCoordX = (int)(roadDistance/textureSize);
        
        if (isCurveUp) {
            
            if (isLowerOne) {
                x_offset = SLOPE_DIUS*sin(CC_DEGREES_TO_RADIANS(currentDegree));
                y_offset = SLOPE_DIUS*cos(CC_DEGREES_TO_RADIANS(currentDegree));
                vec = Vector2d(centre.x + x_offset, centre.y-y_offset);
                
                ground_x_offset = SURFACE_THICKNESS*sin(CC_DEGREES_TO_RADIANS(currentDegree));
                ground_y_offset = SURFACE_THICKNESS*cos(CC_DEGREES_TO_RADIANS(currentDegree));
                lower_vec = Vector2d(vec.GetX()+ground_x_offset, vec.GetY()-ground_y_offset);
                
            }
            else {
                float reversedDegree = SLOPE_DEGREE - currentDegree;
                x_offset = SLOPE_DIUS*sin(CC_DEGREES_TO_RADIANS(reversedDegree));
                y_offset = SLOPE_DIUS*cos(CC_DEGREES_TO_RADIANS(reversedDegree));
                vec = Vector2d(centre.x - x_offset, centre.y + y_offset);
                
                ground_x_offset = SURFACE_THICKNESS*sin(CC_DEGREES_TO_RADIANS(reversedDegree));
                ground_y_offset = SURFACE_THICKNESS*cos(CC_DEGREES_TO_RADIANS(reversedDegree));
                lower_vec = Vector2d(vec.GetX()+ground_x_offset, vec.GetY()-ground_y_offset);

            }
            
            
        }
        else {
            if (isLowerOne) {
                float reversedDegree = SLOPE_DEGREE - currentDegree;
                x_offset = SLOPE_DIUS*sin(CC_DEGREES_TO_RADIANS(reversedDegree));
                y_offset = SLOPE_DIUS*cos(CC_DEGREES_TO_RADIANS(reversedDegree));
                vec = Vector2d(centre.x - x_offset, centre.y - y_offset);
                
                ground_x_offset = SURFACE_THICKNESS*sin(CC_DEGREES_TO_RADIANS(reversedDegree));
                ground_y_offset = SURFACE_THICKNESS*cos(CC_DEGREES_TO_RADIANS(reversedDegree));
                lower_vec = Vector2d(vec.GetX()-ground_x_offset, vec.GetY()-ground_y_offset);
            }
            else {
                x_offset = SLOPE_DIUS*sin(CC_DEGREES_TO_RADIANS(currentDegree));
                y_offset = SLOPE_DIUS*cos(CC_DEGREES_TO_RADIANS(currentDegree));
                vec = Vector2d(centre.x+x_offset, centre.y+y_offset);
                
                ground_x_offset = SURFACE_THICKNESS*sin(CC_DEGREES_TO_RADIANS(currentDegree));
                ground_y_offset = SURFACE_THICKNESS*cos(CC_DEGREES_TO_RADIANS(currentDegree));
                lower_vec = Vector2d(vec.GetX()-ground_x_offset, vec.GetY()-ground_y_offset);
            }
        }
        
        if (currentDegree == 0) {
            //add the starting lower vec point
            lowerVecs.push_back(Vector2d(lower_vec.GetX(), lower_vec.GetY()-800));
        }
        if (currentDegree != 0) {
            textureCoordX += Point(vec.GetX()-pre_vec.GetX(), vec.GetY()-pre_vec.GetY()).getLength()/surfaceTexWidth;
        }
        
        //push vec to vecs
        lowerVecs.push_back(lower_vec);
        vecs.push_back(vec);
        
        if (currentDegree != 0) {
            surfacePoints.push_back(pre_vec);
            surfacePoints.push_back(vec);
            surfacePoints.push_back(pre_lowerVec);
            surfacePoints.push_back(pre_lowerVec);
            surfacePoints.push_back(lower_vec);
            surfacePoints.push_back(vec);
            
            Vector2d pre_texcoord = Vector2d(pre_textureCoordX, 0.0);
            Vector2d pre_lower_texcoord = Vector2d(pre_textureCoordX, 1.0);
            Vector2d texcoord = Vector2d(textureCoordX, 0.0);
            Vector2d lower_texcoord = Vector2d(textureCoordX, 1.0);
            surfaceTexCoords.push_back(pre_texcoord);
            surfaceTexCoords.push_back(texcoord);
            surfaceTexCoords.push_back(pre_lower_texcoord);
            surfaceTexCoords.push_back(pre_lower_texcoord);
            surfaceTexCoords.push_back(lower_texcoord);
            surfaceTexCoords.push_back(texcoord);
        }
        
        
        
        
        //reset pre points
        currentDegree += SLOPE_DEGREE/SLOPE_SMOOTH;
        
        pre_vec = vec;
        pre_lowerVec = lower_vec;
        pre_textureCoordX = textureCoordX;
        
    }
    
    
    Vector2d lastPoint = vecs.at(vecs.size()-1);
    groundYpos = lastPoint.GetY();
    surface->customSetting(surfacePoints, surfaceTexCoords);
    
    Vector2d lastLowerPoint = lowerVecs.at(lowerVecs.size()-1);
    lowerVecs.push_back(Vector2d(lastLowerPoint.GetX(), lastLowerPoint.GetY()-800));
    terrain->setPoints(lowerVecs);
    
    
    //construct physics
    setPhysicsTerrain(vecs, &groundBody);
    
    
    lastPos = Point(lastPoint.GetX(), lastPoint.GetY());
    lastTexCoordX = textureCoordX;

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
    gLayer->removeChild(terrain, true);
    
    if (groundBody != NULL) {
        gWorld->DestroyBody(groundBody);
        groundBody = NULL;
    }
  
}