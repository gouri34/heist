//
//  GuardTower.h
//  Animal_Squad
//
//  Created by wd on 9/16/14.
//
//

#ifndef __Animal_Squad__GuardTower__
#define __Animal_Squad__GuardTower__

#define PTM_RATIO 32.0
#include <iostream>
#include "CommonObject.h"

using namespace cocos2d;
using namespace cocostudio;

class GuardTower : public CommonObject
{
private:
    
public:
    bool destroyed =false;
    
    static GuardTower* create(std::string fileName,cocos2d::Point pos, float scalex, float scaley);
    virtual ~GuardTower();
    
    //virtual void destroy();
    
    
    virtual void collisionProcess(Monster* monster);
    
};

#endif /* defined(__Animal_Squad__GuardTower__) */
