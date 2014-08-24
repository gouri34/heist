//
//  DestructableObject.h
//  Animal_Squad
//
//  Created by wd on 8/22/14.
//
//

#ifndef __Animal_Squad__DestructableObject__
#define __Animal_Squad__DestructableObject__

#include <iostream>
#import "cocos2d.h"
#include "Box2D/Box2D.h"
#include "cocostudio/CocoStudio.h"
#include "PhysicsSprite.h"

#include "MyQueryCallback.h"

#define  PTM_RATIO 32.0


using namespace std;
using namespace cocostudio;

class DestructableObject : public Ref
{

private:
    b2Body *body_;
    Layer *gameScene;
    b2World *gameWorld;
    bool active;
    float density_;
    std::vector<Sprite*> objectfrags;
    std::vector<b2Body*> objectbodylist;
public:
    Armature *armature;
    static DestructableObject* create(Layer *gameScene_, b2World *gameWorld_,Point pos,string armatureName,float scale,float density);
    virtual bool init(Layer *gameScene_, b2World *gameWorld_, Point pos,string armatureName,float scale,float density);
    ~DestructableObject();
    
    void update(float dt);
    void destructableObjectAction();
    void destoryObject();
  
    
};

#endif /* defined(__Animal_Squad__DestructableObject__) */
