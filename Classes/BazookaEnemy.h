//
//  BazookaEnemy.h
//  Animal_Squad
//
//  Created by wd on 9/15/14.
//
//

#ifndef __Animal_Squad__BazookaEnemy__
#define __Animal_Squad__BazookaEnemy__

#include <iostream>
#include "UniversalAttributes.h"
#include "Enemy.h"

class BazookaEnemy : public Enemy
{
    
private:
    void createRocketShell();
    void shellCollisionDetector();
    void explod();
    Armature *rocketShellSprite;
    b2Body *rocketShellBody_;
    bool isFiring = false;
    bool isPlayingAnimation = false;
    bool isExploding = false;
    bool isBouncingBack = false;
    bool shellDeleted = false;
    ParticleSun *explo;
    float shellVelo = -10.0;
    
    UniversalAttributes *a;
    
    ParticleMeteor *pf;
    
public:
    static BazookaEnemy* create(const char *name, Point pos, float scalex, float scaley);
    virtual ~BazookaEnemy();
    virtual void setArmatureBody();
    
    //virtual void setB2odyPartPosition();
    
    //collision
    virtual void update(float dt);
    virtual void collisionProcess(Monster *monster);
};

#endif /* defined(__Animal_Squad__BazookaEnemy__) */
