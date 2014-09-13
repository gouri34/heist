#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Monster.h"

#include "MyContactListener.h"


class GameScene : public cocos2d::Layer
{
public:
    
    
    Monster *monster;

    
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
    
private:
    b2World * world;
    MyContactListener *_contactListener;
};

#endif // __HELLOWORLD_SCENE_H__
