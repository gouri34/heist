//
//  MonsterSelectionMenu.cpp
//  Animal_Squad
//
//  Created by wd on 9/29/14.
//
//

#include "MonsterSelectionMenu.h"
#include "CocosGUI.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include <string>
#include <sstream>
#include "CoverView.h"
#include "MainMenuCoordinator.h"
USING_NS_CC;

MonsterSelectionMenu* MonsterSelectionMenu::create()
{
    MonsterSelectionMenu *a = new MonsterSelectionMenu();
    if (a&&a->init()) {
        return a;
    }
    return NULL;
}

bool MonsterSelectionMenu::init()
{
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->onTouchBegan = CC_CALLBACK_2(MonsterSelectionMenu::touchesBegan, this);
    touchEventListener->onTouchMoved = CC_CALLBACK_2(MonsterSelectionMenu::touchesMoved, this);
    touchEventListener->onTouchEnded = CC_CALLBACK_2(MonsterSelectionMenu::touchesEnded, this);
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
    bg->setOpacity(230);
    this->addChild(bg,0);
    
    auto carddock = Sprite::create("carddock.png");
    carddock->setAnchorPoint(Point(0.5,0.5));
    carddock->setScale(1.5);
    carddock->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    this->addChild(carddock,2);
    
    auto select_monster = Sprite::create("SELECT_YOUR_MONSTER.png");
    select_monster->setAnchorPoint(Point(0,1));
    select_monster->setScale(2.0);
    select_monster->setPosition(Point(visibleSize.width*0.24,visibleSize.height*0.99));
    this->addChild(select_monster,2);
    
    //add close button
    closebutton = MenuItemSprite::create(Sprite::create("closedock.png"), nullptr, CC_CALLBACK_1(MonsterSelectionMenu::closeButtonCalled, this));
    closebutton->setPosition(Point(visibleSize.width*0.98,visibleSize.height*0.9));
    closebutton->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
    closebutton->setScale(2.0);
    this->addChild(closebutton,5);

    
    //add monster card flow
    Rect swRect = Rect(0.25f*visibleSize.width, 0.2f*visibleSize.height, 0.55f*visibleSize.width, 0.6f*visibleSize.height);
    Size slSize = Size(1.3f*visibleSize.width, 0.33f*visibleSize.height);
    float disDistance = 0.2f*visibleSize.width;
    float disScale = 0.3f;
    CoverView* coverView = CoverView::create(swRect,slSize,disDistance,disScale);
    for(int i = 0 ; i< 6 ; i++)
    {
        Sprite* player = Sprite::create("monstercard.png");
        player->setScale(2);
        if (i==0) {
            Armature *flammerMon = Armature::create("FlammerMonster");
            flammerMon->setScale(0.8);
            flammerMon->setAnchorPoint(Point(0.5,0.5));
            flammerMon->setPosition(Point(player->getPositionX()+player->getContentSize().width/2,player->getPositionY()+player->getContentSize().height/2));
            player->addChild(flammerMon);
            Sprite *flammonword = Sprite::create("flammontitle.png");
            flammonword->setAnchorPoint(Point(0.5,0));
            flammonword->setScale(1.5);
            flammonword->setPosition(Point(player->getPositionX()+player->getContentSize().width/2,player->getPositionY()+player->getContentSize().height));
            player->addChild(flammonword);
        }
        else{
            if(i==1){
                Sprite *golmon = Sprite::create("golmon.png");
                golmon->setScale(0.14);
                golmon->setAnchorPoint(Point(0.5,0.5));
                golmon->setPosition(Point(player->getPositionX()+player->getContentSize().width/2,player->getPositionY()+player->getContentSize().height/2));
                player->addChild(golmon);
            }
            Sprite *lockword = Sprite::create("lockedtitle.png");
            lockword->setAnchorPoint(Point(0.5,0));
            lockword->setScale(1.5);
            lockword->setPosition(Point(player->getPositionX()+player->getContentSize().width/2,player->getPositionY()+player->getContentSize().height));
            player->addChild(lockword);
        }
        coverView->addCard(player);
    }
    auto menu = Menu::create(closebutton, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu,5);
    
    coverView->setPosition(swRect.origin);
    coverView->setAnchorPoint(Point(0.5,0.5));
    this->addChild(coverView,5);
    return true;

}

void MonsterSelectionMenu::closeButtonCalled(cocos2d::Ref *sender)
{
    closebutton->setScale(0.5);
    this->removeFromParentAndCleanup(true);
    MainMenuCoordinator::GetInstance()->mm->allElementsVisible();
    
}

bool MonsterSelectionMenu::touchesBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    isMoveing = false;
    return true;
}

void MonsterSelectionMenu::touchesMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    isMoveing = true;
}

void MonsterSelectionMenu::touchesEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto touchLocation = touch->getLocation();
    auto nodePosition = convertToNodeSpace( touchLocation );
    Size visibleSize = Director::getInstance()->getVisibleSize();
    if ((touchLocation.x>visibleSize.width/2-50&&touchLocation.x<visibleSize.width/2+50
        &&touchLocation.y<visibleSize.height/2+100&&touchLocation.y>visibleSize.height/2-100)&&isMoveing==false) {
        this->removeFromParentAndCleanup(true);
        MainMenuCoordinator::GetInstance()->mm->allElementsVisible();
    }
    
}

MonsterSelectionMenu::~MonsterSelectionMenu()
{
    //do nothing
}