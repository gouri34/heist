//
//  Coin.h
//  Animal_Squad
//
//  Created by wd on 9/28/14.
//
//

#ifndef __Animal_Squad__Coin__
#define __Animal_Squad__Coin__

#include <iostream>
#include "UniversalAttributes.h"
#define PTM_RATIO 32.0
#include "Item.h"
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "cocostudio/CocoStudio.h"

using namespace cocos2d;
using namespace cocostudio;

class Coin : public Item
{
private:
    bool collected = false;
    
public:
    
    int score = 20;
    b2Body *body_;
    static Coin* create(Layer* gameLayer, b2World *gameWorld, Point pos);
    virtual bool init(Layer* gameLayer, b2World *gameWorld, Point pos);
    virtual void update(float dt);
    ~Coin();
    void coinGet();
    
    
};

#endif /* defined(__Animal_Squad__Coin__) */
