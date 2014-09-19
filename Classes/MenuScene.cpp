//
//  MenuScene.cpp
//  Bear
//
//  Created by cong ku on 14-5-27.
//
//

#include "MenuScene.h"
#include "cocos-ext.h"
#include "CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;

using namespace ui;
using namespace cocostudio;

Scene* MenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MenuScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    
    //enable touching
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->onTouchBegan = CC_CALLBACK_2(MenuScene::touchesBegan, this);
    touchEventListener->onTouchMoved = CC_CALLBACK_2(MenuScene::touchesMoved, this);
    touchEventListener->onTouchEnded = CC_CALLBACK_2(MenuScene::touchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchEventListener, this);
    
    
    /////////////////////////////
    
    auto reloatItem = MenuItemFont::create("Reload", CC_CALLBACK_1(MenuScene::menuReloadCallback, this));
    reloatItem->setPosition(Point(visibleSize.width*0.85, visibleSize.height*0.9));
    
    auto BearDIEButton = MenuItemFont::create("BEAR DIE!!!", CC_CALLBACK_1(MenuScene::bearDie, this));
    BearDIEButton->setPosition(Point(visibleSize.width*0.15, visibleSize.height*0.9));

    
    /*Scale9Sprite *ls = Scale9Sprite::create("dbutton.png");
    auto leftTitle = LabelTTF::create("Change", "Arial", 44);
    auto leftButton = ControlButton::create(leftTitle, ls);
    leftButton->setPosition(visibleSize.width*0.12, visibleSize.height*0.1);
    this->addChild(leftButton,10);
    leftButton->addTargetWithActionForControlEvents(this, cccontrol_selector(MenuScene::leftPushCallback), Control::EventType::TOUCH_DOWN);
    leftButton->addTargetWithActionForControlEvents(this, cccontrol_selector(MenuScene::leftReleaseCallback), Control::EventType::TOUCH_UP_INSIDE);
    leftButton->addTargetWithActionForControlEvents(this, cccontrol_selector(MenuScene::leftReleaseCallback), Control::EventType::TOUCH_UP_OUTSIDE);*/

    
    /*Scale9Sprite *rs = Scale9Sprite::create("dbutton.png");
    auto rightTitle = LabelTTF::create("Rhino", "Arial", 44);
    auto rightButton = ControlButton::create(rightTitle,(Scale9Sprite*)rs);
    this->addChild(rightButton,10);
    rightButton->setPosition(visibleSize.width*0.3, visibleSize.height*0.1);
    rightButton->addTargetWithActionForControlEvents(this, cccontrol_selector(MenuScene::rightPushCallback), Control::EventType::TOUCH_DOWN);
    rightButton->addTargetWithActionForControlEvents(this, cccontrol_selector(MenuScene::rightReleaseCallback), Control::EventType::TOUCH_UP_INSIDE);
     rightButton->addTargetWithActionForControlEvents(this, cccontrol_selector(MenuScene::rightReleaseCallback), Control::EventType::TOUCH_UP_OUTSIDE);*/
    
   /* Scale9Sprite *as = Scale9Sprite::create("dbutton.png");
    auto attackTitle = LabelTTF::create("Jump", "Arial", 44);
    auto attackButotn = ControlButton::create(attackTitle,(Scale9Sprite*)as);
    attackButotn->setPosition(visibleSize.width*0.86, visibleSize.height*0.1);
    this->addChild(attackButotn,10);
    attackButotn->addTargetWithActionForControlEvents(this, cccontrol_selector(MenuScene::attacPushkCallback), Control::EventType::TOUCH_DOWN);
    attackButotn->addTargetWithActionForControlEvents(this, cccontrol_selector(MenuScene::attacReleasekCallback), Control::EventType::TOUCH_UP_INSIDE);
    attackButotn->addTargetWithActionForControlEvents(this, cccontrol_selector(MenuScene::attacReleasekCallback), Control::EventType::TOUCH_UP_OUTSIDE);*/
   
    
    //add background
    auto background = Sprite::create("test_ground_background.png");
    background->setAnchorPoint(Point(0,0));
    background->setPosition(Point(0,0));
    this->addChild(background, 0);
    

    
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(reloatItem, BearDIEButton,NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    createPerkProgressBar();
    
    //add game layer
    gScene = GameScene::create();
    this->addChild(gScene);
    //UniversalAttributes::GetInstance()->storeMenuScenePointer(this);
    
    
    //update
    this->scheduleUpdate();
    
    
    return true;
}

void MenuScene::update(float dt)
{
    //update gameloop
    gScene->update(dt);
    healthMonitor();
}


void MenuScene::leftPushCallback()
{
    
}

void MenuScene::leftReleaseCallback()
{
    bearMovement = 0;

}


void MenuScene::attacPushkCallback()
{
    //bearMovement = 3;
}

void MenuScene::attacReleasekCallback()
{
    bearMovement = -1;
}

void MenuScene::menuReloadCallback(Ref* pSender)
{
    unscheduleUpdate();
    
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = MenuScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    Director::getInstance()->replaceScene(scene);
}

void MenuScene::bearDie(Ref* pSender)
{
    gScene->monster->die();
}



bool MenuScene::touchesBegan(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();
    auto nodePosition = convertToNodeSpace( touchLocation );
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

    if (nodePosition.x < visibleSize.width/2) {
        //leftPushCallback();
        // dash
        //gScene->monster->dashy();
        gScene->monster->changeActionStatus(dash);
        gScene->monster->action();
    }
    else {
        //attacPushkCallback();
        //jump
        //gScene->monster->jumppy();
        gScene->monster->changeActionStatus(jump);
        gScene->monster->action();
    }
    
    //return gScene->MouseDown(b2Vec2(nodePosition.x/PTM_RATIO,nodePosition.y/PTM_RATIO));
    return true;
}

void MenuScene::touchesMoved(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();
    auto nodePosition = convertToNodeSpace( touchLocation );
    
    //gScene->MouseMove(b2Vec2(nodePosition.x/PTM_RATIO,nodePosition.y/PTM_RATIO));
}

void MenuScene::touchesEnded(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();
    auto nodePosition = convertToNodeSpace( touchLocation );
    
    //gScene->MouseUp(b2Vec2(nodePosition.x/PTM_RATIO,nodePosition.y/PTM_RATIO));
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    if (nodePosition.x < visibleSize.width/2) {
        leftReleaseCallback();
    }
    else {
        attacReleasekCallback();
    }
}

void MenuScene::createPerkProgressBar()
{
    left = ProgressTimer::create(Sprite::create("progress.png"));
    left->setType(kCCProgressTimerTypeBar);
    left->setMidpoint(Point(0,0));
    left->setScaleX(1.857);
    left->setScaleY(1.1);
    left->setBarChangeRate(Point(1,0));
    left->setPosition(Point(0,1));
    left->setAnchorPoint(Point(0,-0.4));
    left->setPercentage(100);
    
    auto *leftBg = Sprite::create("progressBG.png");
    leftBg->addChild(left);
    this->addChild(leftBg,50);
    leftBg->setScale(0.2);
    leftBg->setPosition(Point(Director::getInstance()->getVisibleSize().width*0.12, Director::getInstance()->getVisibleSize().height*0.8));
    UniversalAttributes::GetInstance()->pt = left;
    
    //create header UI
    auto headSprite = Sprite::create("tou.png");
    headSprite->setScale(0.44);
    headSprite->setAnchorPoint(Point(0,1));
    headSprite->setPosition(Point(Director::getInstance()->getVisibleSize().width*0.02, Director::getInstance()->getVisibleSize().height*0.95));
    this->addChild(headSprite,50);
    
    //give 4 lives
    Point initHP = Point(Director::getInstance()->getVisibleSize().width*0.1, Director::getInstance()->getVisibleSize().height*0.9);
    healthCount = 4;
    UniversalAttributes::GetInstance()->healthCount = 4;
    for (int i=0; i<4; i++) {
        Sprite *health = Sprite::create("heart.jpg");
        health->setAnchorPoint(Point(0,1));
        health->setScale(0.42);
        health->setPosition(initHP);
        this->addChild(health,50);
        initHP = Point(initHP.x+health->getScale()*health->getContentSize().width,initHP.y);
        healths.push_back(health);
    }
    
}

void MenuScene::healthMonitor()
{
    if (healthCount>UniversalAttributes::GetInstance()->healthCount) {
        if(UniversalAttributes::GetInstance()->healthCount>=0)
        {
            healths[healthCount-1]->setVisible(false);
            healthCount = UniversalAttributes::GetInstance()->healthCount;
        }
        else{
            //die maybe?
        }
    }
    else if (healthCount<UniversalAttributes::GetInstance()->healthCount){
        healths[healthCount]->setVisible(true);
        healthCount = UniversalAttributes::GetInstance()->healthCount;

    }
}


void MenuScene::cleanup()
{
    this->removeChild(gScene, true);
    removeAllChildrenWithCleanup(true);
    CCNode::cleanup();
}
