//
//  glassWindow.h
//  Animal_Squad
//
//  Created by cong ku on 14-7-4.
//
//

#ifndef __Animal_Squad__glassWindow__
#define __Animal_Squad__glassWindow__

#define PTM_RATIO 32.0

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "cocostudio/CocoStudio.h"
#include "MyQueryCallback.h"

using namespace cocos2d;
using namespace cocostudio;

class GlassWindow : public Ref
{
private:
    std::vector<b2Body*>wallBlocks;
    Rect wallRect;
    bool iscontacting;
    Layer* gameScene;
    b2World *gameWorld;
    
public:
    bool destroyed =false;
    Armature* theBody;
    b2Body *staticWall;
    static GlassWindow* create(Layer *gameScene_, b2World *gameWorld_, Point pos);
    virtual bool init(Layer *gameScene_, b2World *gameWorld_, Point pos);
    ~GlassWindow();
    
    void destroyWall();
    
    void update(Point pos, float dt);
    
};


#endif /* defined(__Animal_Squad__glassWindow__) */
