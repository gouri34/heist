//
//  archer.h
//  fusion
//
//  Created by cong ku on 11/19/13.
//
//

#ifndef __fusion__archer__
#define __fusion__archer__

#include "Character.h"
#include "Enemy.h"

#include "Wall.h"
#include "glassWindow.h"

#include "LeaserRayCallback.h"
#include "EnemyDetectionRayCallback.h"

#include "PistolBullet.h"


#define  PTM_RATIO 32.0

using namespace cocos2d;

typedef  enum {
    dash, jump,run
} ActionStatus;


class Bear:public Character{
    
public:
    static Bear* create(Layer *gameScene_, b2World *gameWorld_, Point pos);
    virtual bool init(Layer *gameScene_, b2World *gameWorld_, Point pos);
    ~Bear();
    
    void creatfootBody();
    void setArmatureBody();
        
    Armature *theBody;
    
    //change state
    ActionStatus getActionStatus();
    void changeActionStatus(ActionStatus status);
    
    //movement relative
    void verticalAdjust();
    void stop();
    void action();
    
    void jumppy();
    void dashy();
    
    //attack related
    void HeavyAttack();
    void HeavyAttackEffect();
    
    //animation callback
    void animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);

    void die();
    
    void update(float dt);
    
    //ragdoll related
    void setB2bodyPartPosition();
    float setBodySprites();
    
private:
    Layer *gameScene;
    b2World *gameWorld;
    
    ActionStatus actionStatus;
    
    bool inDash;
    float dashTimer;
    
    bool inAttack;
    float attackTimer;
    
    float jumpTimer;
    
    int actionState;
    bool inMovement;
    
    bool inJump;
    void onGroundDetector();
    
    float targetSpeed;
    
};

#endif /* defined(__fusion__archer__) */
