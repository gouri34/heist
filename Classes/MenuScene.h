//
//  MenuScene.h
//  Bear
//
//  Created by cong ku on 14-5-27.
//
//

#ifndef __Bear__MenuScene__
#define __Bear__MenuScene__

#include "cocos2d.h"
#include "GameScene.h"
#include "cocos-ext.h"
#include "ScoreMultiplier.h"

using namespace extension;

class MenuScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuReloadCallback(Ref *sender) ;
    void bearDie(Ref* pSender);
    void leftPushCallback() ;
    void leftReleaseCallback() ;
    void attacPushkCallback();
    void attacReleasekCallback();
    
    //touch events
    bool touchesBegan(Touch* touch, Event* event);
    void touchesMoved(Touch* touch, Event* event);
    void touchesEnded(Touch* touch, Event* event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MenuScene);
    virtual void cleanup();

    
    void update(float dt);

private:
    GameScene* gScene;
    int bearMovement;
    ScoreMultiplier *s;
};

#endif /* defined(__Bear__MenuScene__) */
