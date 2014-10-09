//
//  TouchContinueScene.h
//  Animal_Squad
//
//  Created by wd on 9/24/14.
//
//

#ifndef __Animal_Squad__TouchContinueScene__
#define __Animal_Squad__TouchContinueScene__

#include <iostream>
#include "cocos-ext.h"
#include "UniversalAttributes.h"
#include "MenuScene.h"

using namespace extension;

class TouchContinueScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    //touch events
    bool touchesBegan(Touch* touch, Event* event);
    void touchesMoved(Touch* touch, Event* event);
    void touchesEnded(Touch* touch, Event* event);
    
    CREATE_FUNC(TouchContinueScene);
    //virtual void cleanup();
    
    //void update(float dt);

private:
    MenuScene* mScene;
    bool goMenuScene = false;
    Node *loginscene;
};

#endif /* defined(__Animal_Squad__TouchContinueScene__) */
