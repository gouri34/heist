//
//  DrillMan.h
//  Animal_Squad
//
//  Created by wd on 8/25/14.
//
//BoundingBox collision detect mechanism

#ifndef __Animal_Squad__DrillMan__
#define __Animal_Squad__DrillMan__

#include "Enemy.h"

class Bear;

class DrillMan : public Enemy
{
public:
    static DrillMan* create(Scene *parentScene, b2World *world,const char*name, Point pos, float scale);
    virtual bool init(Scene *parentScene, b2World *world,const char*name, Point pos, float scale);
    virtual ~DrillMan();
    
    virtual void update(float dt, Bear *bear);
    virtual void creatfootBody();
    virtual void die(b2Vec2 vec);
    
private:
    Rect collisionPoint;
    bool onGround;
    void gottaDie(int dashingFlag);  //0 is not dashing, 1 is dashing
    bool isDead;
    b2Body* myOwnBody;
};

#endif /* defined(__Animal_Squad__DrillMan__) */
