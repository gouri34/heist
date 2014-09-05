//
//  MonsterTrap.h
//  Animal_Squad
//
//  Created by wd on 8/23/14.
//
//

#ifndef __Animal_Squad__MonsterTrap__
#define __Animal_Squad__MonsterTrap__

#include "EnemyObject.h"

class MonsterTrap : public EnemyObject
{
private:
    ParticleSystemQuad *p;
public:
    static MonsterTrap* create(Layer *gameScene_, b2World *gameWorld_,Point pos,string armatureName,float scale,float density);
    virtual bool init(Layer *gameScene_, b2World *gameWorld_,Point pos,string armatureName,float scale,float density);
    ~MonsterTrap();
    
    virtual void update(float dt,Bear *bear);
    virtual void enemyObjectAction();

};


#endif /* defined(__Animal_Squad__MonsterTrap__) */
