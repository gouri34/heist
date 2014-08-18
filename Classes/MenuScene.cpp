//
//  MenuScene.cpp
//  Bear
//
//  Created by cong ku on 14-5-27.
//
//

#include "MenuScene.h"

USING_NS_CC;


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
    auto background = Sprite::create("test_bg.png");
    background->setAnchorPoint(Point(0,0));
    background->setPosition(Point(0,0));
    this->addChild(background, -3);
    

    //init background layer 3
    Sprite *bg = Sprite::create("layer3.png");
    bg->setAnchorPoint(Point(0,0));
    bg->setPosition(0,0);
    bg->setScaleX(0.4);
    bg->setScaleY(0.7);
    this->addChild(bg,-2);
    
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(reloatItem, BearDIEButton,NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    //add game layer
    gScene = GameScene::create();
    this->addChild(gScene);
    
    //update
    this->scheduleUpdate();
    
    return true;
}

void MenuScene::update(float dt)
{
    //update gameloop
    gScene->update(dt);
}


void MenuScene::leftPushCallback()
{
//    if (gScene->bear->getActionStatus() == jump){
//        gScene->bear->changeActionStatus(dash);
//    }
//    else if (gScene->bear->getActionStatus() == dash) {
//        gScene->bear->changeActionStatus(jump);
//    }
}

void MenuScene::leftReleaseCallback()
{
    bearMovement = 0;

}


void MenuScene::attacPushkCallback()
{
    //bearMovement = 3;
//    gScene->bear->action();
}

void MenuScene::attacReleasekCallback()
{
    bearMovement = -1;
}

void MenuScene::menuReloadCallback(Ref* pSender)
{
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MenuScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    Director::getInstance()->replaceScene(scene);
}

void MenuScene::bearDie(Ref* pSender)
{
    gScene->bear->die();
}



bool MenuScene::touchesBegan(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();
    auto nodePosition = convertToNodeSpace( touchLocation );
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

    if (nodePosition.x < visibleSize.width/2) {
        //leftPushCallback();
        // dash
        gScene->bear->dashy();
    }
    else {
        //attacPushkCallback();
        //jump
        gScene->bear->jumppy();
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



void MenuScene::cleanup()
{
    this->removeChild(gScene, true);
    removeAllChildrenWithCleanup(true);
    CCNode::cleanup();
}
