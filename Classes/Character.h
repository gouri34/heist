//
//  Character.h
//  Zombie_Project
//
//  Created by cong ku on 14-3-19.
//
//

#ifndef __Zombie_Project__Character__
#define __Zombie_Project__Character__

#import "cocos2d.h"
#include "Box2D/Box2D.h"
#include "cocostudio/CocoStudio.h"
#include "PhysicsSprite.h"

#include "MyQueryCallback.h"


using namespace std;
using namespace cocostudio;

typedef enum {
    player, normalEnemy
} CharacterType;

typedef enum {
    wp_normal, wp_stair, wp_ladder
} waypoint_t;

struct waypoint {
    int wp_id;
    Point waypointPos;
    waypoint_t waypointType;
};


class Character : public Ref
{
public:
    float life;
    bool dead = false;
    bool afterDeath = false;
    bool readyDeleteBody = false;
    
    Vector<PhysicsSprite*> deadSpriteArray;
    vector<b2Body*>bodies;
    
    Point position;
    
    b2Body *footBody;
    Rect footRect;
    b2Body *groundStandOn;
    b2Vec2 lowestGroundContact;
    
    float analogY = 0;
    bool want_stair_down = false;
    
    CharacterType chType;
    
    
    virtual void setArmatureBody();
    virtual void updateArmatureBody();
    virtual void setB2bodyPosition();
    virtual void setB2bodyPartPosition();
    virtual float setBodySprites();
    virtual void creatfootBody();
    
};

#endif /* defined(__Zombie_Project__Character__) */
