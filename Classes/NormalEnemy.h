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
    static NormalEnemy* create(const char*name, Point pos, float scalex, float scaley);
    virtual ~NormalEnemy();
    virtual void setArmatureBody();
    
    virtual void setB2bodyPartPosition();
    
    //actions
    void animationEvent(Armature *armature, MovementEventType movementType, const char *movementID);
    void getHit();
    void headDropDie();
    void dieToExplosion(float damage, Point exploPosition);
    
    //collison
    virtual void update(float dt);
    virtual void collisionProcess(Monster *monster);

private:
    
};

#endif /* defined(__fusion__normalZombie__) */
