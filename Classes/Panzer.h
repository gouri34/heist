//
//  Panzer.h
//  Animal_Squad
//
//  Created by wd on 9/15/14.
//
//

#ifndef __Animal_Squad__Panzer__
#define __Animal_Squad__Panzer__

#include <iostream>

#include "Enemy.h"

class Panzer : public Enemy
{
public:
    static Panzer* create(const char*name, Point pos, float scalex, float scaley);
    virtual ~Panzer();
    virtual void setArmatureBody();
    virtual void creatfootBody();
    virtual void update(float dt);
    virtual void collisionProcess(Monster *monster);
    
private:
    Armature *chopper;
    int randChopper;
};

#endif /* defined(__Animal_Squad__Panzer__) */
