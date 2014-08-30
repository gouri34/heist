//
//  zombie.h
//  fusion
//
//  Created by cong ku on 11/15/13.
//
//

#ifndef __Animal_Squad__Enemy__
#define __Animal_Squad__Enemy__

#include "Character.h"
#include "Bear.h"

#define  PTM_RATIO 32.0
#define  HEIGHTDIFFX -10
//#define  HEIGHTDIFFY 37.0
#define  HEIGHTDIFFY 60.0

#define defaultSpeed 5.0


using namespace cocos2d;
using namespace cocostudio;

class Bear;

class Enemy : public Character
{
    
public:
    Scene *gameScene;
    b2World *gameWorld;

    //zombie();
    //zombie(Scene *parentScene, b2World *world,const char*name, float scale);
    static Enemy* create(Scene *parentScene, b2World *world,const char*name, Point pos, float scale);
    ~Enemy();
    
    //waypoints
    //waypoint *waypointArray;
    //int wArraySize;
    
    Point position;
    
    bool haveWayPoint = false;
    vector<waypoint> wpArray;
    int firstPos;
    int waypointIndex = 0;
    bool walkingBack = false;
    bool reachWaypoint;
    bool waypointOnRight;
    

    //properties
    Armature *armature;
    float zombie_scale;
    float desireVel;
        //vector<PhysicsSprite*>deadSpriteArray;
    
    
    //function delaretions
   // static zombie *create(Scene *parentScene, b2World *world,const char*name, float scale);
    virtual bool init(Scene *parentScene, b2World *world,const char*name, Point pos, float scale);
    virtual void initWayPoints(vector<waypoint> wps);
    //virtual void update(float dt);
    virtual void update(float dt,Bear *bear);
    virtual void die(b2Vec2 vec);
    virtual void deleteProperties();
    
    virtual void setArmatureBody();
    virtual void updateArmatureBody();
    virtual void setB2bodyPosition();
    virtual void setB2bodyPartPosition();
    virtual float setBodySprites();
    virtual void creatfootBody();

private:
    bool playedStartUpAnimation = false;
    float startAnimationTimer;


};


#endif /* defined(__fusion__zombie__) */


