//
//  PhysicsSprite.h
//  fusion
//
//  Created by cong ku on 11/16/13.
//
//

#ifndef __fusion__PhysicsSprite__
#define __fusion__PhysicsSprite__

#import "cocos2d.h"
#import "Box2D/Box2D.h"

using namespace cocos2d;

class PhysicsSprite : public Sprite {
public:
    b2Body *body;
};

#endif /* defined(__fusion__PhysicsSprite__) */