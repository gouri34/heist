#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Bear.h"

#include "Ground.h"
#include "Ground_Curved.h"
#include "Ground_Slope.h"

#include "Building.h"
#include "GroundBuilding.h"

#include "Wall.h"
#include "glassWindow.h"
#include "MyContactListener.h"
#include "NormalEnemy.h"


class GameScene : public cocos2d::Layer
{
public:
    
    
    Bear *bear;

    
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
