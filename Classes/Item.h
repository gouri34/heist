//
//  Item.h
//  Animal_Squad
//
//  Created by wd on 9/17/14.
//
//

#ifndef __Animal_Squad__Item__
#define __Animal_Squad__Item__

#include <iostream>
#include "UniversalAttributes.h"
#define PTM_RATIO 32.0

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "cocostudio/CocoStudio.h"
//#include "Monster.h"

using namespace cocos2d;
using namespace cocostudio;
class Item : public Ref{
private:
    Layer *gameLayer_;
    b2World *gameWorld_;
    UniversalAttributes *a;
    int itemType;
    bool triggered = false;
public:
    Armature *armature;
    b2Body *body_;
    static Item* create(Layer* gameLayer, b2World *gameWorld, Point pos);
    virtual bool init(Layer* gameLayer, b2World *gameWorld, Point pos);
    ~Item();
    void update(float dt);
};
#endif /* defined(__Animal_Squad__Item__) */
