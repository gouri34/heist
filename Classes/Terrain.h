//
//  Terrain.h
//  Animal_Squad
//
//  Created by cong ku on 7/31/14.
//
//

#ifndef __Animal_Squad__Terrain__
#define __Animal_Squad__Terrain__

#include "EasyPolygon.h"
#include "PRFilledPolygon.h"
#include "PRSubTerrainPolygon.h"
#include "Box2D/Box2D.h"

#define MonsterOffset 300
#define PTM_RATIO 32.0


using namespace cocos2d;

class Terrain : public Ref {
public:
    b2World *gWorld;
    Layer* gLayer;
    b2Body *groundBody = NULL;


    bool dead = false;
    bool afterDeath = false;
    int height;
    float textureSize;
    
    int groundYpos;//
    Point startPos;
    Point lastPos;
    double startTexCoordX;
    double lastTexCoordX;

    Vector2dVector pointsToVector(Point p1, Point p2, Point p3, Point p4);
    Vector2dVector makeVector(Point v1, Point v2,Point v3,Point v4);
    virtual void setVertices(Point pos);
    virtual void setPhysicsTerrain(Vector2dVector v, b2Body **b);
    
    virtual void update(float dt, Point pos);
    
    virtual void setDead();
    
    virtual ~Terrain();



};

#endif /* defined(__Animal_Squad__Terrain__) */
