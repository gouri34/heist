//
//  GameOverMenu.cpp
//  Animal_Squad
//
//  Created by wd on 10/5/14.
//
//

#include "GameOverMenu.h"
#include "CocosGUI.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include <string>
#include <sstream>
#include "MenuScene.h"

USING_NS_CC;

GameOverMenu* GameOverMenu::create(MenuScene *ms)
{
    GameOverMenu *a = new GameOverMenu();
    if (a&&a->init(ms)) {
        return a;
    }
    return NULL;
}

bool GameOverMenu::init(MenuScene *ms)
{
    menuscene = ms;
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->onTouchBegan = CC_CALLBACK_2(GameOverMenu::touchesBegan, this);
    touchEventListener->onTouchMoved = CC_CALLBACK_2(GameOverMenu::touchesMoved, this);
    touchEventListener->onTouchEnded = CC_CALLBACK_2(GameOverMenu::touchesEnded, this);
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
    
    restartButton = MenuItemSprite::create(Sprite::create("restart_button.png"), nullptr,CC_CALLBACK_1(GameOverMenu::restartButtonCalled, this));
    restartButton->setPosition(Point(visibleSize.width*0.5-5,visibleSize.height*0.4));
    restartButton->setAnchorPoint(Point(1,0.5));
    restartButton->setScale(1.5);
    restartButton->setVisible(false);
    
    menuButton = MenuItemSprite::create(Sprite::create("menu_button.png"), nullptr,CC_CALLBACK_1(GameOverMenu::menuButtonCalled, this));
    menuButton->setPosition(Point(visibleSize.width*0.5+5,visibleSize.height*0.4));
    menuButton->setAnchorPoint(Point(0,0.5));
    menuButton->setScale(1.5);
    menuButton->setVisible(false);
    
    auto menu = Menu::create(restartButton,menuButton, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu,5);
    
    ScaleTo *st1 = ScaleTo::create(0.35, 1.8);
    ScaleTo *st2 = ScaleTo::create(0.55, 1.2);
    sq = Sequence::create(st1,st2, NULL);
    gameOverSprite = Sprite::create("GAME_OVER.png");
    gameOverSprite->setAnchorPoint(Point(0.5,0.5));
    gameOverSprite->setPosition(Point(visibleSize.width/2,visibleSize.height*0.55));
    this->addChild(gameOverSprite,5);
    gameOverSprite->runAction(sq);
    
    //score
    auto label1 = Sprite::create("SCORE1.png");
    label1->setPosition(Point(visibleSize.width*0.54, visibleSize.height*0.925));
    label1->setScaleY(0.5);
    label1->setScaleX(0.5);
    //label1->setColor(Color3B::RED);
    label1->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
    this->addChild(label1,50);
    
    auto dpCounter = Label::createWithTTF(int2str(UniversalAttributes::GetInstance()->destructionScore), "Vermin-Vibes-Slant.ttf", 25);
    dpCounter->setPosition(Point(visibleSize.width*0.56, visibleSize.height*0.924));
    dpCounter->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    dpCounter->setColor(Color3B::RED);
    dpCounter->enableGlow(Color4B::RED);
    //dpCounter->setScaleY(0.4);
    this->addChild(dpCounter,50);

    
    
    this->scheduleUpdate();
    
    return true;

}

void GameOverMenu::update(float dt)
{
    if (gameOverSprite->getActionManager()->numberOfRunningActionsInTarget(gameOverSprite)==0&&isShownButton==false) {
        menuButton->setVisible(true);
        restartButton->setVisible(true);
        isShownButton = true;
        menuscene->isPausing = true;
    }
}

void GameOverMenu::restartButtonCalled(cocos2d::Ref *sender)
{
    menuscene->isPausing = false;
    menuscene->reloadMenu();
    this->removeFromParentAndCleanup(true);
}

void GameOverMenu::menuButtonCalled(cocos2d::Ref *sender)
{
    UniversalAttributes::GetInstance()->inMenuMode = true;
    menuscene->isPausing = false;
    menuscene->reloadMenu();
    this->removeFromParentAndCleanup(true);

}

bool GameOverMenu::touchesBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void GameOverMenu::touchesEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{}

void GameOverMenu::touchesMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{}

GameOverMenu::~GameOverMenu()
{
    //do nothing
}

string GameOverMenu::int2str(int &i)
{
    string s;
    stringstream ss(s);
    ss << i;
    return ss.str();
}
