//
//  normalZombie.h
//  fusion
//
//  Created by cong ku on 14-1-25.
//
//

#ifndef __fusion__normalZombie__
#define __fusion__normalZombie__

#include "Enemy.h"

class NormalEnemy : public Enemy
{
public:
    //normalZombie(Scene *parentScene, b2World *world,const char*name, float scale);
    static NormalEnemy* create(Scene *parentScene, b2World *world,const char*name, Point pos, float scale);
    virtual ~NormalEnemy();
    virtual void setArmatureBody();
    
    virtual void setB2bodyPartPosition();
    
    //actions
    void animationEvent(Armature *armature, MovementEventType movementType, const char *movementID);
    void getHit();
    void headDropDie();
    void dieToExplosion(float damage, Point exploPosition);

private:
    
};

#endif /* defined(__fusion__normalZombie__) */
