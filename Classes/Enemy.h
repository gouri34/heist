//
//  zombie.h
//  fusion
//
//  Created by cong ku on 11/15/13.
//
//

#ifndef __fusion__zombie__
#define __fusion__zombie__

#include "Character.h"
#include "Monster.h"
#include "UniversalAttributes.h"

#define  PTM_RATIO 32.0
#define  HEIGHTDIFFX -10
#define  HEIGHTDIFFY 40.0

#define defaultSpeed 5.0

typedef  enum {
    moving, climbing, jumping,attacking,
} actionType;

struct zombieAction {
    actionType atype;
    bool actionDone = false;
};



using namespace cocos2d;
using namespace cocostudio;

class Enemy : public Character
{
    
public:
    Layer *gameScene;
    b2World *gameWorld;

    //zombie();
    //zombie(Scene *parentScene, b2World *world,const char*name, float scale);
    static Enemy* create(const char*name, Point pos, float scalex, float scaley);
    virtual bool init(const char*name, Point pos, float scalex, float scaley);
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
    Armature *bam;
    float zombie_scale;
    float desireVel;
        //vector<PhysicsSprite*>deadSpriteArray;
    
    int score = 20;
    
    
    //function delaretions
   // static zombie *create(Scene *parentScene, b2World *world,const char*name, float scale);
    virtual void initWayPoints(vector<waypoint> wps);
    virtual void update(float dt);
    virtual void die(b2Vec2 vec);
    virtual void deleteProperties();
    
    virtual void setArmatureBody();
    virtual void updateArmatureBody();
    virtual void setB2bodyPosition();
    virtual void setB2bodyPartPosition();
    virtual float setBodySprites();
    virtual void creatfootBody();
    
    virtual void collisionProcess(Monster *monster);

private:
    bool playedStartUpAnimation = false;
    float startAnimationTimer;


};


#endif /* defined(__fusion__zombie__) */


