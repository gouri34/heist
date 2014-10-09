//
//  OffGameMenu.cpp
//  Animal_Squad
//
//  Created by wd on 9/29/14.
//
//

#include "OffGameMenu.h"
#include "CocosGUI.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include <string>
#include <sstream>
#include "MainMenuCoordinator.h"

USING_NS_CC;

OffGameMenu* OffGameMenu::create()
{
    OffGameMenu *a = new OffGameMenu();
    if (a&&a->init()) {
        return a;
    }
    return NULL;
}

bool OffGameMenu::init()
{
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->onTouchBegan = CC_CALLBACK_2(OffGameMenu::touchesBegan, this);
    touchEventListener->onTouchMoved = CC_CALLBACK_2(OffGameMenu::touchesMoved, this);
    touchEventListener->onTouchEnded = CC_CALLBACK_2(OffGameMenu::touchesEnded, this);
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
    
    creditSprite = Sprite::create("credit_sprite.png");
    creditSprite->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    creditSprite->setAnchorPoint(Point(0.5,0.5));
    creditSprite->setVisible(false);
    this->addChild(creditSprite,5);
    
    //add soundbutton
    if (UniversalAttributes::GetInstance()->soundOn==true)
        soundButton = MenuItemSprite::create(Sprite::create("soundon_button.png"),nullptr,CC_CALLBACK_1(OffGameMenu::soundButtonCalled, this));
    else
        soundButton = MenuItemSprite::create(Sprite::create("soundoff_button.png"),nullptr,CC_CALLBACK_1(OffGameMenu::soundButtonCalled, this));
    soundBool = UniversalAttributes::GetInstance()->soundOn;
    soundButton->setPosition(Point(visibleSize.width*0.5,visibleSize.height*0.8));
    soundButton->setAnchorPoint(Point(0.5,0.5));
    soundButton->setScale(1.5);
    
    //add creditbutton
    creditButton = MenuItemSprite::create(Sprite::create("credit_button.png"), nullptr, CC_CALLBACK_1(OffGameMenu::creditButtonCalled, this));
    creditButton->setPosition(Point(visibleSize.width*0.5,soundButton->getPositionY()-soundButton->getBoundingBox().size.height-50));
    creditButton->setAnchorPoint(Point(0.5,0.5));
    creditButton->setScale(1.5);
    
    //add close button
    closeButton = MenuItemSprite::create(Sprite::create("optionclose_button.png"), nullptr, CC_CALLBACK_1(OffGameMenu::closeButtonCalled, this));
    closeButton->setPosition(Point(visibleSize.width*0.5,creditButton->getPositionY()-creditButton->getBoundingBox().size.height-50));
    closeButton->setAnchorPoint(Point(0.5,0.5));
    closeButton->setScale(1.5);
    
    auto menu = Menu::create(soundButton,creditButton,closeButton, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu,5);
    
    return true;
}

void OffGameMenu::soundButtonCalled(cocos2d::Ref *sender)
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

void OffGameMenu::creditButtonCalled(cocos2d::Ref *sender)
{
    //need implemented
    if (inCredit==false) {
        allElementsInvisible();
        inCredit = true;
    }
}

void OffGameMenu::closeButtonCalled(cocos2d::Ref *sender)
{
    MainMenuCoordinator::GetInstance()->mm->allElementsVisible();
    this->removeFromParentAndCleanup(true);
}

bool OffGameMenu::touchesBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void OffGameMenu::touchesMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{}

void OffGameMenu::touchesEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (inCredit == true) {
        inCredit = false;
        allElementsVisible();
    }
}

void OffGameMenu::allElementsVisible()
{
    soundButton->setVisible(true);
    closeButton->setVisible(true);
    creditButton->setVisible(true);
    creditSprite->setVisible(false);
}

void OffGameMenu::allElementsInvisible()
{
    soundButton->setVisible(false);
    closeButton->setVisible(false);
    creditButton->setVisible(false);
    creditSprite->setVisible(true);

}


OffGameMenu::~OffGameMenu()
{
    //do nothing
}