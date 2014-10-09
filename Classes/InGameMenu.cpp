//
//  InGameMenu.cpp
//  Animal_Squad
//
//  Created by wd on 10/1/14.
//
//

#include "InGameMenu.h"
#include "CocosGUI.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include <string>
#include <sstream>
#include "MenuScene.h"

USING_NS_CC;

InGameMenu* InGameMenu::create(MenuScene *ms)
{
    InGameMenu *a = new InGameMenu();
    if (a&&a->init(ms)) {
        return a;
    }
    return NULL;
}

bool InGameMenu::init(MenuScene *ms)
{
    menuscene = ms;
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->onTouchBegan = CC_CALLBACK_2(InGameMenu::touchesBegan, this);
    touchEventListener->onTouchMoved = CC_CALLBACK_2(InGameMenu::touchesMoved, this);
    touchEventListener->onTouchEnded = CC_CALLBACK_2(InGameMenu::touchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchEventListener, this);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //add transparent background
    auto bg = Sprite::create("transparent.png");
    bg->setAnchorPoint(Point(0.5,0.5));
    float scalex = visibleSize.width/bg->getContentSize().width;
    float scaley = visibleSize.height/bg->getContentSize().height;
    bg->setScaleX(scalex+0.5);
    bg->setScaleY(scaley);
    bg->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    //bg->setOpacity(1);
    this->addChild(bg,0);

    if (UniversalAttributes::GetInstance()->soundOn==true)
        soundButton = MenuItemSprite::create(Sprite::create("soundon_button.png"),nullptr,CC_CALLBACK_1(InGameMenu::soundButtonCalled, this));
    else
        soundButton = MenuItemSprite::create(Sprite::create("soundoff_button.png"),nullptr,CC_CALLBACK_1(InGameMenu::soundButtonCalled, this));
    soundBool = UniversalAttributes::GetInstance()->soundOn;
    soundButton->setPosition(Point(visibleSize.width*0.5,visibleSize.height*0.77));
    soundButton->setAnchorPoint(Point(0.5,0.5));
    soundButton->setScale(1.5);

    restartButton = MenuItemSprite::create(Sprite::create("restart_button.png"), nullptr,CC_CALLBACK_1(InGameMenu::restartButtonCalled, this));
    restartButton->setPosition(Point(visibleSize.width*0.5,soundButton->getPositionY()-soundButton->getBoundingBox().size.height));
    restartButton->setAnchorPoint(Point(0.5,0.5));
    restartButton->setScale(1.5);
    
    menuButton = MenuItemSprite::create(Sprite::create("menu_button.png"), nullptr,CC_CALLBACK_1(InGameMenu::menuButtonCalled, this));
    menuButton->setPosition(Point(visibleSize.width*0.5,restartButton->getPositionY()-restartButton->getBoundingBox().size.height));
    menuButton->setAnchorPoint(Point(0.5,0.5));
    menuButton->setScale(1.5);
    
    closeButton = MenuItemSprite::create(Sprite::create("optionclose_button.png"), nullptr,CC_CALLBACK_1(InGameMenu::closeButtonCalled, this));
    closeButton->setPosition(Point(visibleSize.width*0.5,menuButton->getPositionY()-menuButton->getBoundingBox().size.height));
    closeButton->setAnchorPoint(Point(0.5,0.5));
    closeButton->setScale(1.5);
    
    auto menu = Menu::create(soundButton,restartButton,menuButton,closeButton, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu,5);
    
    return true;
    
}

void InGameMenu::soundButtonCalled(cocos2d::Ref *sender)
{
    if (soundBool==true) {
        soundBool = false;
        UniversalAttributes::GetInstance()->soundOn = soundBool;
        soundButton->setNormalImage(Sprite::create("soundoff_button.png"));
        CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);

    }
    else if(soundBool == false){
        soundBool = true;
        UniversalAttributes::GetInstance()->soundOn = soundBool;
        soundButton->setNormalImage(Sprite::create("soundon_button.png"));
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(5);


    }
}

void InGameMenu::restartButtonCalled(cocos2d::Ref *sender)
{
    menuscene->isPausing = false;
    menuscene->reloadMenu();
    this->removeFromParentAndCleanup(true);
}

void InGameMenu::menuButtonCalled(cocos2d::Ref *sender)
{
    UniversalAttributes::GetInstance()->inMenuMode = true;
    menuscene->isPausing = false;
    menuscene->reloadMenu();
    this->removeFromParentAndCleanup(true);
}

void InGameMenu::closeButtonCalled(cocos2d::Ref *sender)
{
    menuscene->isPausing = false;
    this->removeFromParentAndCleanup(true);
}

bool InGameMenu::touchesBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void InGameMenu::touchesMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{}

void InGameMenu::touchesEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{}

InGameMenu::~InGameMenu()
{
    //do nothing
}

