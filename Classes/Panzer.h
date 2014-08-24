//
//  Panzer.h
//  Animal_Squad
//
//  Created by wd on 8/23/14.
//
//Panzer density:10f, scale:0.1

#ifndef __Animal_Squad__Panzer__
#define __Animal_Squad__Panzer__

#include "EnemyObject.h"

class Panzer : public EnemyObject{
    
private:
    
public:
    static Panzer* create(Layer *gameScene_, b2World *gameWorld_,Point pos,string armatureName,float scale,float density);
    virtual bool init(Layer *gameScene_, b2World *gameWorld_, Point pos,string armatureName,float scale,float density);
    ~Panzer();
    
    virtual void update(float dt,Bear *bear);
    
};


#endif /* defined(__Animal_Squad__Panzer__) */
