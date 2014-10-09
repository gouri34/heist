//
//  MonsterTrap.h
//  Animal_Squad
//
//  Created by wd on 9/30/14.
//
//

#ifndef __Animal_Squad__MonsterTrap__
#define __Animal_Squad__MonsterTrap__

#include <iostream>
#include "Enemy.h"
#include "UniversalAttributes.h"

class MonsterTrap : public Enemy
{
    
private:
    bool isAnimated = false;
public:
    static MonsterTrap* create(const char*name, Point pos, float scalex, float scaley);
    virtual ~MonsterTrap();
    virtual void setArmatureBody();
    virtual void creatfootBody();
    virtual void update(float dt);
    virtual void collisionProcess(Monster* monster);
    
};

#endif /* defined(__Animal_Squad__MonsterTrap__) */
