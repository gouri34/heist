//
//  StockPiles.h
//  Animal_Squad
//
//  Created by wd on 9/18/14.
//
//

#ifndef __Animal_Squad__StockPiles__
#define __Animal_Squad__StockPiles__

#define PTM_RATIO 32.0
#include <iostream>
#include "CommonObject.h"

using namespace cocos2d;
using namespace cocostudio;

class StockPiles : public CommonObject
{
private:
    
public:
    bool destroyed =false;
    
    static StockPiles* create(std::string fileName,cocos2d::Point pos, float scalex, float scaley);
    virtual ~StockPiles();
    
    //virtual void destroy();
    
    
    virtual void collisionProcess(Monster* monster);
    
};
#endif /* defined(__Animal_Squad__StockPiles__) */
