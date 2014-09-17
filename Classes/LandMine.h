//
//  LandMine.h
//  Animal_Squad
//
//  Created by wd on 9/17/14.
//
//

#ifndef __Animal_Squad__LandMine__
#define __Animal_Squad__LandMine__

#include <iostream>
#include "Enemy.h"
#include "UniversalAttributes.h"

class LandMine : public Enemy{
    
private:
    ParticleSun *explo;
    UniversalAttributes *a;

    void explosionAction();
    
public:
    static LandMine* create(const char*name, Point pos, float scalex, float scaley);
    //virtual bool init(const char*name, Point pos, float scalex, float scaley);
    
    ~LandMine();
    
    virtual void setArmatureBody();
    virtual void creatfootBody();
    
    virtual void update(float dt);
    virtual void collisionProcess(Monster *monster);

    bool activated = false;
    bool exploded = false;
};

#endif /* defined(__Animal_Squad__LandMine__) */
