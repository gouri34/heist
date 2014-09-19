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

#define  PTM_RATIO 32.0

using namespace cocos2d;

typedef  enum {
    dash, jump,run
} ActionStatus;

class UniversalAttributes;

class Monster:public Character{
    
public:
    static Monster* create(Layer *gameScene_, b2World *gameWorld_, Point pos);
    virtual bool init(Layer *gameScene_, b2World *gameWorld_, Point pos);
    ~Monster();
    
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
    
    bool isDashing();
    
    //item related
    void goSprint(float timer);
    bool inSprint;
    
    //damage related
    bool Unhurtable = false;
    float hurtTimer = 0;
    void getHurt();

    
private:
    Layer *gameScene;
    b2World *gameWorld;
    int monsterHealth;
    
    ActionStatus actionStatus;
    
    //dash related
    bool inDash;
    float dashTimer;
    float dashCountDown;        //Note: This is the parameter that controll the cool down of the monster dash
    void dashProgressBarHandler();
    
    bool inAttack;
    float attackTimer;
    
    //jump related
    float jumpTimer;
    
    int actionState;
    bool inMovement;
    
    bool inJump;
    bool inDoubleJump;
    void onGroundDetector();
    
    float targetSpeed;
    float prevSpeed;
    
    void collisionDetector();
    
    //item related
    ParticleMeteor *pe;
    float sprintTimer;
    void itemUpdate(float dt);
    
    Sequence *sq;
    bool barFull = true;
    
};

#endif /* defined(__fusion__archer__) */
