//
//  ShieldMan.cpp
//  Animal_Squad
//
//  Created by wd on 8/20/14.
//
//

#include "ShieldMan.h"

ShieldMan * ShieldMan::create(Scene *parentScene, b2World *world,const char*name, Point pos, float scale)
{
    ShieldMan *z = new ShieldMan();
    if (z && z->init(parentScene, world, name, pos, scale)) {
        return z;
    }
    
    return NULL;
}

ShieldMan::~ShieldMan()
{
    // do nothing
}

void ShieldMan::setArmatureBody()
{
    
}

void ShieldMan::setB2bodyPartPosition()
{
    
}

