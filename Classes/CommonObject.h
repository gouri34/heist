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
#include "Monster.h"
#include "ScreenShaker.h"

using namespace cocos2d;
using namespace cocostudio;

class CommonObject : public Ref
{
private:
    std::vector<b2Body*>wallBlocks;
    Rect wallRect;
    bool iscontacting;

public:
    Layer* gameScene;
    b2World *gameWorld;
    bool destroyed =false;
    Armature* theBody;
    Armature* bam;
    int score = 40;
    b2Body *staticWall;
    static CommonObject* create(std::string fileName,cocos2d::Point pos, float scalex, float scaley);
    virtual bool init(std::string fileName,cocos2d::Point pos, float scalex, float scaley);
    ~CommonObject();
    
    virtual void collisionProcess(Monster* monster);
    virtual void destroy();
    
    
    virtual void update(Point pos, float dt);
    
};


#endif /* defined(__Animal_Squad__glassWindow__) */
