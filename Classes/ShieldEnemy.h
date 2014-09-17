//
//  ShieldEnemy.h
//  Animal_Squad
//
//  Created by wd on 9/17/14.
//
//

#ifndef __Animal_Squad__ShieldEnemy__
#define __Animal_Squad__ShieldEnemy__

#include <iostream>
#include "Enemy.h"
#include "UniversalAttributes.h"

class ShieldEnemy : public Enemy
{
private:
    bool isPlayingAnimation = false;
    UniversalAttributes *a;
    bool shieldIsDestoryed=false;
    
    b2Body *shield;
    Point shieldPos;
    Size shieldRect;
    
public:
    static ShieldEnemy* create(const char *name, Point pos, float scalex, float scaley);
    virtual ~ShieldEnemy();
    virtual void setArmatureBody();
    
    //virtual void setB2odyPartPosition();
    
    //collision
    virtual void update(float dt);
    virtual void collisionProcess(Monster *monster);
    virtual void spikeProcess(Monster *monster);
};

#endif /* defined(__Animal_Squad__ShieldEnemy__) */
