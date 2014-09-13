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

#include "CommonObject.h"

using namespace cocos2d;
using namespace cocostudio;

class Wall : public CommonObject
{
private:
    std::vector<b2Body*>wallBlocks;
    b2Body *staticWall;
    
    Layer* gameScene;
    b2World *gameWorld;
    
public:
    bool destroyed =false;
    
    static Wall* create(Point pos);
    virtual bool init(Point pos);
    ~Wall();
    
    virtual void destroy();
    
    void update(Point pos, float dt);
    
};

#endif /* defined(__Bear__Wall__) */
