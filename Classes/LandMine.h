//
//  LandMine.h
//  Animal_Squad
//
//  Created by wd on 9/2/14.
//
//LaneMine is an enemyObject, it will explod when the timer goes to 1, the explosion will damage both player and enemies. LandMine do not collide with players, however if
//the player dash through it, this will apply a impulse to the landmine and it will fly away.

#ifndef __Animal_Squad__LandMine__
#define __Animal_Squad__LandMine__

#include <iostream>
#include "EnemyObject.h"

class LandMine : public EnemyObject{
    
private:
    bool activated = false;
    bool exploded = false;
    ParticleSun *explo;
    
public:
    static LandMine* create(Layer *gameScene_, b2World *gameWorld_,Point pos,string armatureName,float scale,float density);
    virtual bool init(Layer *gameScene_,b2World *gameWorld_,Point pos,string armatureName,float scale,float density);
    ~LandMine();
    
    virtual void update(float dt,Bear *bear);
    virtual void enemyObjectAction();

};

#endif /* defined(__Animal_Squad__LandMine__) */
