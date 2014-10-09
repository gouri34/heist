//
//  GuardTower.cpp
//  Animal_Squad
//
//  Created by wd on 9/16/14.
//
//

#include "GuardTower.h"
#include "MapGenerator.h"

GuardTower* GuardTower::create(std::string fileName, cocos2d::Point pos, float scalex, float scaley)
{
    GuardTower *a = new GuardTower();
    if (a&&a->init(fileName, pos, scalex, scaley)) {
        return a;
    }
    return NULL;
}


GuardTower::~GuardTower()
{
    //do nothing
}

void GuardTower::collisionProcess(Monster *monster)
{
    //CommonObject::collisionProcess(monster);
    //if (monster->isDashing()==true) {
        destroy();
        
    //}
}

void GuardTower::destroy()
{
    CommonObject::destroy();
    ScreenShaker *ss = ScreenShaker::create(0.5, 6);
    gameScene->getParent()->runAction(ss);
}

