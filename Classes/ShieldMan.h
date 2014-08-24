//
//  ShieldMan.h
//  Animal_Squad
//
//  Created by wd on 8/20/14.
//
//Shield with big spikes in the front of it, if monster interact with the spikes, it will get hit. If monster interact with the enemies, enemy will die

#ifndef __Animal_Squad__ShieldMan__
#define __Animal_Squad__ShieldMan__

#include <iostream>
#include "Enemy.h"

class ShieldMan : public Enemy
{
public:
    static ShieldMan* create(Scene *parentScene, b2World *world,const char*name, Point pos, float scale);
    virtual ~ShieldMan();
    virtual void setArmatureBody();
    virtual void setB2bodyPartPosition();
    virtual void update(float dt,Bear *bear);
    //actions
    void getHit();
    
private:
    
    
};

#endif /* defined(__Animal_Squad__ShieldMan__) */
