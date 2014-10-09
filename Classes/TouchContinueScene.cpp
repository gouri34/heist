//
//  TouchContinueScene.cpp
//  Animal_Squad
//
//  Created by wd on 9/24/14.
//
//

#include "TouchContinueScene.h"
#include "cocos-ext.h"
#include "CocosGUI.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include <string>
#include <sstream>

USING_NS_CC;

using namespace ui;
using namespace cocostudio;

Scene* TouchContinueScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TouchContinueScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TouchContinueScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    //enable touching
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->onTouchBegan = CC_CALLBACK_2(TouchContinueScene::touchesBegan, this);
    touchEventListener->onTouchMoved = CC_CALLBACK_2(TouchContinueScene::touchesMoved, this);
    touchEventListener->onTouchEnded = CC_CALLBACK_2(TouchContinueScene::touchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchEventListener, this);

    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    loginscene = SceneReader::getInstance()->createNodeWithSceneFile("loginScene.json");
    
    float scalex = visibleSize.width/960;
    float scaley = visibleSize.height/640;
    printf("scalx = %f\n",scalex);
    printf("scaly = %f\n",scaley);
    loginscene->setScaleX(scalex);
    loginscene->setScaleY(scaley);
    //node->setAnchorPoint(Point(0.5,0.5));
    
    if (loginscene) {
        this->addChild(loginscene);
    }
    //update
    //this->scheduleUpdate();
    return true;
}

bool TouchContinueScene::touchesBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto touchLocation = touch->getLocation();
    auto nodePosition = convertToNodeSpace(touchLocation);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //switch to the game
    if(!goMenuScene){
        goMenuScene = true;
        auto director = Director::getInstance();
        auto scene = MenuScene::createScene();
        // run
        director->runWithScene(scene);
    }
    return true;
}

void TouchContinueScene::touchesMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{}

void TouchContinueScene::touchesEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{}