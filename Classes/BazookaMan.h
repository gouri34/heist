//
//  BazookaMan.h
//  Animal_Squad
//
//  Created by wd on 9/7/14.
//
//Bazooka Soldier is categorized as enemy, when in range bazookaman will shoot out one rocket shell to the monster, player can use dash to bounce back the shell, once the shell is colliding with the enemies or the monster it will explode and deal damage.

#ifndef __Animal_Squad__BazookaMan__
#define __Animal_Squad__BazookaMan__

#include <iostream>
#include "Enemy.h"

class BazookaMan : public Enemy
{

private:
    bool isPlayingAnimation = false;
    
    void collideAction(Bear *bear);
    void createRocketShell();
    void shellCollisionDetector(Bear *bear);
    void explod();
    Sprite *rocketShellSprite;
    b2Body *rocketShellBody_;
    bool isFiring = false;
    bool isExploding = false;
    bool isBouncingBack = false;
    ParticleSun *explo;
    float shellVelo = -19.0;
    
public:
    static BazookaMan* create(Scene *parentScene, b2World *world,const char*name, Point pos, float scale);
    virtual ~BazookaMan();
    virtual void update(float dt, Bear *bear);
    virtual void setArmatureBody();
    virtual void setB2bodyPartPosition();


};
#endif /* defined(__Animal_Squad__BazookaMan__) */
