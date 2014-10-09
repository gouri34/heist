//
//  TutorialMenu.cpp
//  Animal_Squad
//
//  Created by wd on 10/4/14.
//
//

#include "TutorialMenu.h"
#include "CocosGUI.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include <string>
#include <sstream>
#include "MainMenuCoordinator.h"


USING_NS_CC;

TutorialMenu* TutorialMenu::create()
{
    TutorialMenu *a = new TutorialMenu();
    if (a&&a->init()) {
        return a;
    }
    return NULL;
}

bool TutorialMenu::init()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->onTouchBegan = CC_CALLBACK_2(TutorialMenu::touchesBegan, this);
    touchEventListener->onTouchMoved = CC_CALLBACK_2(TutorialMenu::touchesMoved, this);
    touchEventListener->onTouchEnded = CC_CALLBACK_2(TutorialMenu::touchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchEventListener, this);
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("word.ExportJson");
    word = Armature::create("word");
    word->setPosition(Point(visibleSize.width/2,-72));
    word->setAnchorPoint(Point(0.5,0));
    word->setScale(1.5);
    word->getAnimation()->playWithIndex(0);
    this->addChild(word,7);

    Sprite *tmp = Sprite::create("t1.png");
    tmp->setPosition(visibleSize.width/2,visibleSize.height/2);
    tmp->setAnchorPoint(Point(0.5,0.5));
    float scalex = visibleSize.width/tmp->getContentSize().width;
    float scaley = visibleSize.height/tmp->getContentSize().height;
    tmp->setScaleX(scalex);
    tmp->setScaleY(scaley);
    this->addChild(tmp,5);
    tutorialSprites.push_back(tmp);
    tmp->setVisible(true);
    
    tmp = Sprite::create("t2.png");
    tmp->setPosition(visibleSize.width/2,visibleSize.height/2);
    tmp->setAnchorPoint(Point(0.5,0.5));
    tmp->setScaleX(scalex);
    tmp->setScaleY(scaley);
    this->addChild(tmp,5);
    tutorialSprites.push_back(tmp);
    tmp->setVisible(false);
    
    tmp = Sprite::create("t3.png");
    tmp->setPosition(visibleSize.width/2,visibleSize.height/2);
    tmp->setAnchorPoint(Point(0.5,0.5));
    tmp->setScaleX(scalex);
    tmp->setScaleY(scaley);
    this->addChild(tmp,5);
    tutorialSprites.push_back(tmp);
    tmp->setVisible(false);

    tmp = Sprite::create("t4.png");
    tmp->setPosition(visibleSize.width/2,visibleSize.height/2);
    tmp->setAnchorPoint(Point(0.5,0.5));
    tmp->setScaleX(scalex);
    tmp->setScaleY(scaley);
    this->addChild(tmp,5);
    tutorialSprites.push_back(tmp);
    tmp->setVisible(false);
    return true;
}

bool TutorialMenu::touchesBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void TutorialMenu::touchesEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto touchLocation = touch->getLocation();
    auto nodePosition = convertToNodeSpace( touchLocation );
    if (tCount==3) {
        this->removeFromParentAndCleanup(true);
        MainMenuCoordinator::GetInstance()->mm->allElementsVisible();
    }
    else{
        tutorialSprites[tCount]->setVisible(false);
        tCount++;
        tutorialSprites[tCount]->setVisible(true);
    }
}

void TutorialMenu::touchesMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

TutorialMenu::~TutorialMenu()
{
    //do nothing
}
