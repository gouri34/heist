//
//  MainMenu.cpp
//  Animal_Squad
//
//  Created by wd on 9/28/14.
//
//

#include "MainMenu.h"
#include "CocosGUI.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "MainMenuCoordinator.h"

USING_NS_CC;


MainMenu* MainMenu::create()
{
    MainMenu *a = new MainMenu();
    if (a&&a->init()) {
        return a;
    }
    return NULL;
}

bool MainMenu::init()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //touch enabled
    //enable touching
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->onTouchBegan = CC_CALLBACK_2(MainMenu::touchesBegan, this);
    touchEventListener->onTouchMoved = CC_CALLBACK_2(MainMenu::touchesMoved, this);
    touchEventListener->onTouchEnded = CC_CALLBACK_2(MainMenu::touchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchEventListener, this);
    
    retroactivelogo = Sprite::create("retroactivegameslogo.png");
    retroactivelogo->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    retroactivelogo->setScale(0.5);
    retroactivelogo->setVisible(false);
    this->addChild(retroactivelogo,5);
    
    mhwords = Sprite::create("monsterheisttitle.png");
    mhwords->setPosition(Point(visibleSize.width/2,visibleSize.height*0.74));
    mhwords->setAnchorPoint(Point(0.5,0.5));
    mhwords->setScale(3.0);
    this->addChild(mhwords);
    
    sb1 = ScaleBy::create(2.0, 0.33);
    //sb2 = ScaleBy::create(1.0, 1.25);
    //sq = Sequence::create(sb1,sb2, NULL);
    //rf = RepeatForever::create(sq);
    mhwords->runAction(sb1);
    
    tutorialButton = MenuItemSprite::create(Sprite::create("tutorial_button.png"),nullptr, CC_CALLBACK_1(MainMenu::tutorialButtonCalled, this));
    tutorialButton->setPosition(Point(visibleSize.width*0.17,visibleSize.height*0.2));
    tutorialButton->setAnchorPoint(Point(0.5,0.5));
    //this->addChild(tutorialButton);
    
    aboutUsButton = MenuItemSprite::create(Sprite::create("aboutus_button.png"), nullptr, CC_CALLBACK_1(MainMenu::aboutusButtonCalled, this));
    aboutUsButton->setPosition(Point(tutorialButton->getPositionX()+aboutUsButton->getBoundingBox().size.width,visibleSize.height*0.2));
    aboutUsButton->setAnchorPoint(Point(0.5,0.5));
    //this->addChild(aboutUsButton);
    
    monstersButton = MenuItemSprite::create(Sprite::create("monsters_button.png"), nullptr, CC_CALLBACK_1(MainMenu::monstersButtonCalled, this));
    monstersButton->setPosition(Point(aboutUsButton->getPositionX()+aboutUsButton->getBoundingBox().size.width,visibleSize.height*0.2));
    monstersButton->setAnchorPoint(Point(0.5,0.5));
    //this->addChild(monstersButton);
    
    goButton = MenuItemSprite::create(Sprite::create("go_button.png"), nullptr, CC_CALLBACK_1(MainMenu::goButtonCalled, this));
    goButton->setPosition(Point(monstersButton->getPositionX()+monstersButton->getBoundingBox().size.width,visibleSize.height*0.2));
    goButton->setAnchorPoint(Point(0.5,0.5));
    //this->addChild(goButton);
    
    rb1 = RotateBy::create(2.0, 360);
    rotationrf = RepeatForever::create(rb1);
    
    optionButton = MenuItemSprite::create(Sprite::create("option_button.png"),Sprite::create("option_button.png"),  CC_CALLBACK_1(MainMenu::optionButtonCalled, this));
    optionButton->setAnchorPoint(Point(0.5,0.5));
    optionButton->setPosition(Point(visibleSize.width*0.07,visibleSize.height*0.9));
    optionButton->runAction(rotationrf);
    
    auto menu = Menu::create(tutorialButton,aboutUsButton,monstersButton,goButton,optionButton, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    MainMenuCoordinator::GetInstance()->mm = this;
    
    return true;
}

MainMenu::~MainMenu()
{
    //do nothing
}

void MainMenu::tutorialButtonCalled(cocos2d::Ref *sender)
{
    tutorialButton->setScale(0.7);
    goButton->setScale(1.0);
    monstersButton->setScale(1.0);
    aboutUsButton->setScale(1.0);
    TutorialMenu *tm = TutorialMenu::create();
    this->addChild(tm,55);
    
}

void MainMenu::goButtonCalled(cocos2d::Ref *sender)
{
    goButton->setScale(0.7);
    tutorialButton->setScale(1.0);
    monstersButton->setScale(1.0);
    aboutUsButton->setScale(1.0);

    UniversalAttributes::GetInstance()->inMenuMode = false;
    this->removeFromParentAndCleanup(true);
}

void MainMenu::aboutusButtonCalled(cocos2d::Ref *sender)
{
    if (islogo==false) {
        islogo=true;
        allElementsInvisible();
        retroactivelogo->setVisible(true);
    }
}

void MainMenu::monstersButtonCalled(cocos2d::Ref *sender)
{    
    monstersButton->setScale(0.7);
    aboutUsButton->setScale(1.0);
    tutorialButton->setScale(1.0);
    goButton->setScale(1.0);
    msm = MonsterSelectionMenu::create();
    this->addChild(msm);
    allElementsInvisible();
}

void MainMenu::optionButtonCalled(cocos2d::Ref *sender)
{
    ogm = OffGameMenu::create();
    this->addChild(ogm,30);
    allElementsInvisible();
}

bool MainMenu::touchesBegan(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();
    auto nodePosition = convertToNodeSpace( touchLocation );
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    return true;
}

void MainMenu::touchesMoved(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();
    auto nodePosition = convertToNodeSpace( touchLocation );
    
    //gScene->MouseMove(b2Vec2(nodePosition.x/PTM_RATIO,nodePosition.y/PTM_RATIO));
}

void MainMenu::touchesEnded(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();
    auto nodePosition = convertToNodeSpace( touchLocation );
    
    //gScene->MouseUp(b2Vec2(nodePosition.x/PTM_RATIO,nodePosition.y/PTM_RATIO));
    Size visibleSize = Director::getInstance()->getVisibleSize();
    if (islogo==true) {
        retroactivelogo->setVisible(false);
        allElementsVisible();
        islogo = false;
    }
    
}

void MainMenu::allElementsInvisible()
{
    tutorialButton->setVisible(false);
    aboutUsButton->setVisible(false);
    monstersButton->setVisible(false);
    goButton->setVisible(false);
    mhwords->setVisible(false);
    optionButton->setVisible(false);
}

void MainMenu::allElementsVisible()
{
    tutorialButton->setVisible(true);
    aboutUsButton->setVisible(true);
    monstersButton->setVisible(true);
    goButton->setVisible(true);
    mhwords->setVisible(true);
    optionButton->setVisible(true);
    tutorialButton->setScale(1.0);
    aboutUsButton->setScale(1.0);
    monstersButton->setScale(1.0);
    goButton->setScale(1.0);
}

