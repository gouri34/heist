//
//  StockPiles.h
//  Animal_Squad
//
//  Created by wd on 9/8/14.
//
//StockPiles are objects that are stock together, for exmaple wall made of bricks, and sandbags etc.

#ifndef __Animal_Squad__StockPiles__
#define __Animal_Squad__StockPiles__

#define PTM_RATIO 32.0

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "cocostudio/CocoStudio.h"
#include "UniversalAttributes.h"

using namespace cocos2d;
using namespace cocostudio;

class StockPiles : public Ref
{
private:
    std::vector<b2Body*>fragBlocks;
    b2Body *staticBody_;
    
    Layer* gameScene;
    b2World *gameWorld;
    
    UniversalAttributes *a;
    
    Shaky3D *e1;
    
    bool colliding;
    
public:
    bool destroyed =false;
    
    static StockPiles* create(Layer *gameScene_, b2World *gameWorld_, Point pos,string armatureName,float scale);
    virtual bool init(Layer *gameScene_, b2World *gameWorld_, Point pos,string armatureName,float scale);
    ~StockPiles();
    
    void destroyIt();
    
    void update(Point pos, float dt);
    
};
#endif /* defined(__Animal_Squad__StockPiles__) */
