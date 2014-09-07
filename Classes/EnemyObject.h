//
//  EnemyObject.h
//  Animal_Squad
//
//  Created by wd on 8/23/14.
//
//

#ifndef __Animal_Squad__EnemyObject__
#define __Animal_Squad__EnemyObject__

#include <iostream>
#import "cocos2d.h"
#include "Box2D/Box2D.h"
#include "cocostudio/CocoStudio.h"
#include "PhysicsSprite.h"
#include "MyQueryCallback.h"
#include "Bear.h"
#include "Enemy.h"
#include "DestructableObject.h"

#define  PTM_RATIO 32.0


using namespace std;
using namespace cocostudio;

class EnemyObject : public Ref{
public:
    Armature *armature;
    b2Body *body_;
    Layer *gameScene;
    b2World *gameWorld;
    bool active;
    
    virtual void update(float dt, Bear *bear);
    virtual void enemyObjectAction();
    virtual bool init(Layer *gameScene_, b2World *gameWorld_,Point pos,string armatureName,float scale,float density);
    static EnemyObject* create(Layer *gameScene_, b2World *gameWorld_,Point pos,string armatureName,float scale,float density);
    ~EnemyObject();
    //void destoryObject();
    
private:
};
#endif /* defined(__Animal_Squad__EnemyObject__) */
