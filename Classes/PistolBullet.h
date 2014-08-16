//
//  pistolBullet.h
//  Zombie_Project
//
//  Created by cong ku on 14-3-22.
//
//

#ifndef __Zombie_Project__pistolBullet__
#define __Zombie_Project__pistolBullet__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

using namespace cocos2d;
using namespace std;

class PistolBullet : public Sprite
{
public:
    static PistolBullet* create(string filename, b2Vec2 firePoint);
    bool init(string filename, b2Vec2 firePoint);
    ~PistolBullet();
    
    b2Vec2 firepos;
    //damages
    float normalDamage;
    float headShootDamage;
    float weakDamage;
    float pushForce;
    
    bool damageDone = false;

private:
    
    
};

#endif /* defined(__Zombie_Project__pistolBullet__) */
