//
//  LandMine.h
//  Animal_Squad
//
//  Created by wd on 9/2/14.
//
//

#ifndef __Animal_Squad__LandMine__
#define __Animal_Squad__LandMine__

#include <iostream>
#include "EnemyObject.h"

class LandMine : public EnemyObject{
    
private:
    bool activated = false;
    bool exploded = false;
    
public:
    static LandMine* create(Layer *gameScene_, b2World *gameWorld_,Point pos,string armatureName,float scale,float density);
    ~LandMine();
    
    virtual void update(float dt,Bear *bear);
    virtual void enemyObjectAction();

};

#endif /* defined(__Animal_Squad__LandMine__) */
