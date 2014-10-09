#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Monster.h"

#include "MyContactListener.h"

#include "UniversalAttributes.h"

class GameScene : public cocos2d::Layer
{
public:
    
    
    Monster *monster = NULL;
    b2World * world;

    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    
    //update function
    void update(float dt);
    void physicsUpdate(float dt);
    void beforeStep();
    void afterStep();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    ~GameScene();
    
    //game mechanism
    //void speedUpAnimation();
    
private:
    MyContactListener *_contactListener;
    UniversalAttributes *a;
    Size visibleSize;
    Armature *teleporter;
};

#endif // __HELLOWORLD_SCENE_H__
