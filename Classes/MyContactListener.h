//
//  MyContactListener.h
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#import "cocos2d.h"
#import "Box2D/Box2D.h"

#include "Character.h"


struct MyContact {
    b2Fixture *fixtureA;
    b2Fixture *fixtureB;
    b2WorldManifold worldnamifold;
    float normalImpulse;
    bool operator==(const MyContact& other) const
    {
        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
    }
};

struct bulletContact {
    b2Body *hitBody;
    b2Body *bulletBody;
    //float base_damage;
    b2WorldManifold worldnamifold;
    
    bool operator<(bulletContact other) const { return bulletBody < other.bulletBody; }
    bool operator==(bulletContact other) const { return bulletBody == other.bulletBody; }
};

struct StickyInfo {
    b2Body* arrowBody;
    b2Body* targetBody;
    float normalImpulse;
    b2WorldManifold worldnamifold;

    bool operator<(StickyInfo other) const { return arrowBody < other.arrowBody; }
    bool operator==(StickyInfo other) const { return arrowBody == other.arrowBody; }
};

using namespace cocos2d;

class MyContactListener : public b2ContactListener {

public:
   // std::vector<MyContact>_contacts;
    std::vector<StickyInfo>collisionsMakeStiky;
    std::vector<bulletContact>bulletContacts;
    
    MyContactListener();
    ~MyContactListener();
    
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
};
