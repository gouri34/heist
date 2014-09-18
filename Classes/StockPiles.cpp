//
//  StockPiles.cpp
//  Animal_Squad
//
//  Created by wd on 9/18/14.
//
//

#include "StockPiles.h"

StockPiles* StockPiles::create(std::string fileName, cocos2d::Point pos, float scalex, float scaley)
{
    StockPiles *a = new StockPiles();
    if (a&&a->init(fileName, pos, scalex, scaley)) {
        return a;
    }
    return NULL;
}

void StockPiles::collisionProcess(Monster *monster)
{
    if (monster->isDashing()==true) {
        destroy();
        
    }
}

StockPiles::~StockPiles()
{
    //do nothing
}
