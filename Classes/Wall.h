//
//  Wall.h
//  Bear
//
//  Created by cong ku on 14-6-27.
//
//

#ifndef __Bear__Wall__
#define __Bear__Wall__

#define PTM_RATIO 32.0

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "cocostudio/CocoStudio.h"
#include "UniversalAttributes.h"

using namespace cocos2d;
using namespace cocostudio;

class Wall : public Ref
{
private:
    std::vector<b2Body*>wallBlocks;
    b2Body *staticWall;
    
    Layer* gameScene;
    b2World *gameWorld;
    
    UniversalAttributes *a;
    
    Shaky3D *e1;
    
    bool colliding;
    
public:
    bool destroyed =false;
    
    static Wall* create(Layer *gameScene_, b2World *gameWorld_, Point pos);
    virtual bool init(Layer *gameScene_, b2World *gameWorld_, Point pos);
    ~Wall();
    
    void destroyWall();
    
    void update(Point pos, float dt);
    
};

#endif /* defined(__Bear__Wall__) */
