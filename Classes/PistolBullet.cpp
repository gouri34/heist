//
//  pistolBullet.cpp
//  Zombie_Project
//
//  Created by cong ku on 14-3-22.
//
//

#include "PistolBullet.h"

PistolBullet* PistolBullet::create(string filename, b2Vec2 firePoint)
{
    /*PistolBullet *pb = new PistolBullet();
    if (pb && pb->init(filename, firePoint)) {
        return pb;
    }
    return NULL;*/
    
    PistolBullet *pSprite = new PistolBullet();
    if (pSprite && pSprite->init(filename, firePoint))
    {
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    
    return NULL;
    
}

bool PistolBullet::init(string filename, b2Vec2 firePoint)
{
    //this->Sprite::initWithFile(filename);
    firepos = firePoint;
    
    normalDamage = 30;
    headShootDamage = 80;
    weakDamage = 15;
    pushForce = 40;
    
    damageDone = false;
    
    return initWithFile(filename.c_str());
}

PistolBullet::~PistolBullet()
{
    
}
