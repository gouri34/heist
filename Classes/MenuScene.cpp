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
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include <string>
#include <sstream>

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
    
    Director::getInstance()->getTextureCache()->addImage("t1.png");
    Director::getInstance()->getTextureCache()->addImage("t2.png");
    Director::getInstance()->getTextureCache()->addImage("t3.png");
    Director::getInstance()->getTextureCache()->addImage("t4.png");

    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    
    //enable touching
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->onTouchBegan = CC_CALLBACK_2(MenuScene::touchesBegan, this);
    touchEventListener->onTouchMoved = CC_CALLBACK_2(MenuScene::touchesMoved, this);
    touchEventListener->onTouchEnded = CC_CALLBACK_2(MenuScene::touchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchEventListener, this);
    
    
    /////////////////////////////
    ArmatureDataManager::getInstance()->addArmatureFileInfo("speeduparmature.ExportJson");

    speeduparmature = Armature::create("speeduparmature");
    //speeduparmature->setPosition(Point(visibleSize.width*0.98,visibleSize.height*0.8));
    speeduparmature->setPosition(Point(visibleSize.width*0.5,visibleSize.height*0.8));
    speeduparmature->setAnchorPoint(Point(0.5,0.5));
    speeduparmature->setScale(1.0);
    this->addChild(speeduparmature,50);
    
    auto reloatItem = MenuItemFont::create("Reload", CC_CALLBACK_1(MenuScene::menuReloadCallback, this));
    reloatItem->setPosition(Point(visibleSize.width*0.85, visibleSize.height*0.9));
    //reloatItem->setVisible(false);
    auto BearDIEButton = MenuItemFont::create("BEAR DIE!!!", CC_CALLBACK_1(MenuScene::bearDie, this));
    BearDIEButton->setPosition(Point(visibleSize.width*0.15, visibleSize.height*0.9));
    BearDIEButton->setVisible(false);
    
    if (UniversalAttributes::GetInstance()->inMenuMode==false) {
        
        inDummyMode = false;
        
        comboMultiplier = 1;
        
        auto label1 = Sprite::create("SCORE1.png");
        label1->setPosition(Point(visibleSize.width*0.54, visibleSize.height*0.925));
        label1->setScaleY(0.5);
        label1->setScaleX(0.5);
        //label1->setColor(Color3B::RED);
        label1->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
        this->addChild(label1,50);
        
        dpCounter = Label::createWithTTF(int2str(UniversalAttributes::GetInstance()->destructionScore), "Vermin-Vibes-Slant.ttf", 25);
        dpCounter->setPosition(Point(visibleSize.width*0.56, visibleSize.height*0.924));
        dpCounter->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        dpCounter->setColor(Color3B::RED);
        //dpCounter->enableGlow(Color4B::BLUE);
        dpCounter->enableShadow();
        //dpCounter->setScaleY(0.4);
        this->addChild(dpCounter,50);
        dpScoreCount =UniversalAttributes::GetInstance()->destructionScore;
        
        auto optionButton = MenuItemSprite::create(Sprite::create("option_button.png"), nullptr, CC_CALLBACK_1(MenuScene::optionCalled, this));
        optionButton->setPosition(Point(visibleSize.width*0.94, visibleSize.height*0.9));
        optionButton->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
        
        // create menu, it's an autorelease object
        auto menu = Menu::create(optionButton,NULL);
        menu->setPosition(Point::ZERO);
        this->addChild(menu, 5);
        
        //set combo label--------------------------------------------------------------------------------------------------------------------------------
        //comboCounter = Label::createWithSystemFont(int2str(UniversalAttributes::GetInstance()->comboStreak), "Brush Script MT Italic", 80);
        comboCounter = Label::createWithTTF(int2str(UniversalAttributes::GetInstance()->comboStreak), "Vermin-Vibes-Slant.ttf", 50);
        comboCounter->setPosition(Point(visibleSize.width*0.98,visibleSize.height*0.76));
        comboCounter->setAnchorPoint(Point(1,0.5));
        comboCounter->setScale(0);
        comboCounter->setColor(Color3B::RED);
        comboCounter->enableGlow(Color4B::BLUE);
        comboCounter->enableShadow();
        this->addChild(comboCounter,50);
        comboScoreCount = UniversalAttributes::GetInstance()->comboStreak;
        
        cheerSubTitle = Label::createWithTTF("GOOD", "Vermin-Vibes-Slant.ttf", 50);
        cheerSubTitle->setAnchorPoint(Point(1,1));
        cheerSubTitle->setPosition(Point(comboCounter->getPositionX(),comboCounter->getPositionY()-comboCounter->getContentSize().height/2-10));
        cheerSubTitle->setScale(0);
        cheerSubTitle->setColor(Color3B::RED);
        cheerSubTitle->enableGlow(Color4B::BLUE);
        cheerSubTitle->enableShadow();
        this->addChild(cheerSubTitle,50);
        
        //----------------------------------------------------------------------------------------------------------------------------------
        
        //set effects n sound volume
        if(UniversalAttributes::GetInstance()->soundOn==true)
            CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(5);


    }
    else{
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
        inDummyMode = true;
        MainMenu *mm = MainMenu::create();
        this->addChild(mm,50);
    }
    
    //add background
    auto background = Sprite::create("test_ground_background.png");
    background->setScale(visibleSize.width/background->getContentSize().width, visibleSize.height/background->getContentSize().height);
    background->setAnchorPoint(Point(0,0));
    background->setPosition(Point(0,0));
    this->addChild(background, 0);


    
    createPerkProgressBar();
    
    //add game layer
    gScene = GameScene::create();
    this->addChild(gScene);
    gameTimer = 0;
    speedUpCheckPoint = 20;
    
    //update
    this->scheduleUpdate();
    
    
    return true;
}

void MenuScene::update(float dt)
{
    //update gameloop
    if (isPausing==false) {
        gameTimer += dt;
        gScene->update(dt);
        if (inDummyMode==false) {
            healthMonitor();
            dpMonitor();
            if (gameTimer >= speedUpCheckPoint) {
                //speed up boys!
                speedUpCheckPoint = speedUpCheckPoint + 65;
                gScene->monster->targetSpeed = gScene->monster->targetSpeed+1;
                if (UniversalAttributes::GetInstance()->enemyTimer>=0.4) {
                    UniversalAttributes::GetInstance()->enemyTimer = UniversalAttributes::GetInstance()->enemyTimer - 0.1;
                }
                printf("SPEED UP!!!\n");
                speeduparmature->getAnimation()->playWithIndex(0);
            }
        }
        else{
            dummyController();
            if (UniversalAttributes::GetInstance()->inMenuMode==false) {
                reloadMenu();
            }
        }

    }
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
    
    UniversalAttributes::GetInstance()->inMenuMode = false;

    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = MenuScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    Director::getInstance()->replaceScene(scene);
}

void MenuScene::optionCalled(cocos2d::Ref *sender)
{
    if (isPausing==false) {
        InGameMenu *igm = InGameMenu::create(this);
        this->addChild(igm,55);
        isPausing = true;
    }
}

void MenuScene::reloadMenu()
{
    unscheduleUpdate();
    
    UniversalAttributes::GetInstance()->resetGame();
    
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
    if(isPausing==false&&healthCount!=0&&inDummyMode==false){
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
    leftBg->setPosition(Point(Director::getInstance()->getVisibleSize().width*0.12, Director::getInstance()->getVisibleSize().height*0.82));
    UniversalAttributes::GetInstance()->pt = left;
    
    //create header UI
    auto headSprite = Sprite::create("tou.png");
    headSprite->setScale(0.44);
    headSprite->setAnchorPoint(Point(0,1));
    headSprite->setPosition(Point(Director::getInstance()->getVisibleSize().width*0.02, Director::getInstance()->getVisibleSize().height*0.95));
    this->addChild(headSprite,50);
    
    auto galaxymatterUI = Sprite::create("galaxymatters.png");
    galaxymatterUI->setAnchorPoint(Point(0,0.5));
    galaxymatterUI->setScale(2.0);
    galaxymatterUI->setPosition(Point(headSprite->getPositionX()+headSprite->getContentSize().width*0.6,headSprite->getPositionY()));
    this->addChild(galaxymatterUI,50);
    
    //add galaxy matters counter
    gmCounter = Label::createWithTTF(int2str(UniversalAttributes::GetInstance()->galaxyMatterScore), "Vermin-Vibes-Slant.ttf", 15);
    gmCounter->setPosition(Point(galaxymatterUI->getPositionX()+galaxymatterUI->getContentSize().width*2.5,galaxymatterUI->getPositionY()));
    gmCounter->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    gmCounter->setColor(Color3B::RED);
    gmCounter->enableGlow(Color4B::BLUE);
    this->addChild(gmCounter,50);
    gmScoreCount = UniversalAttributes::GetInstance()->galaxyMatterScore;
    
    //give 4 lives
    Point initHP = Point(Director::getInstance()->getVisibleSize().width*0.1, Director::getInstance()->getVisibleSize().height*0.9);
    healthCount = 5;
    UniversalAttributes::GetInstance()->healthCount = 5;
    for (int i=0; i<5; i++) {
        Sprite *health = Sprite::create("heart.jpg");
        health->setAnchorPoint(Point(0,1));
        health->setScale(0.42);
        health->setPosition(initHP);
        this->addChild(health,50);
        initHP = Point(initHP.x+health->getScale()*health->getContentSize().width,initHP.y);
        healths.push_back(health);
        if (inDummyMode==true) {
            health->setVisible(false);
        }
    }
    if (inDummyMode==true) {
        leftBg->setVisible(false);
        headSprite->setVisible(false);
        galaxymatterUI->setVisible(false);
        gmCounter->setVisible(false);
    }
}

void MenuScene::healthMonitor()
{
    if (healthCount>UniversalAttributes::GetInstance()->healthCount) {
        if(UniversalAttributes::GetInstance()->healthCount>0)
        {
            healths[UniversalAttributes::GetInstance()->healthCount]->setVisible(false);
            healthCount = UniversalAttributes::GetInstance()->healthCount;
            comboMultiplier = 1;
        }
        else{
            //die maybe?
            bearDie(nullptr);
            healths[0]->setVisible(false);
            GameOverMenu *gom = GameOverMenu::create(this);
            this->addChild(gom,55);
            healthCount = 0;
            UniversalAttributes::GetInstance()->healthCount = 0;
        }
    }
    else if (healthCount<UniversalAttributes::GetInstance()->healthCount){
        healths[healthCount]->setVisible(true);
        sb1 = ScaleTo::create(0.1, 1);
        sb2 = ScaleTo::create(0.1, 0.42);
        sq = Sequence::create(sb1,sb2,NULL);
        healths[healthCount]->runAction(sq);
        healthCount = UniversalAttributes::GetInstance()->healthCount;

    }
}


void MenuScene::cleanup()
{
    this->removeChild(gScene, true);
    removeAllChildrenWithCleanup(true);
    CCNode::cleanup();
}

//throw it into the update function
void MenuScene::dpMonitor()
{
    if (dpScoreCount!=UniversalAttributes::GetInstance()->destructionScore) {
        dpCounter->setString(int2str(UniversalAttributes::GetInstance()->destructionScore));
        dpScoreCount = UniversalAttributes::GetInstance()->destructionScore;
        if (dpCounter->getActionManager()->getNumberOfRunningActionsInTarget(dpCounter)==0) {
        }
    }
    //monitor galaxy matters
    if (gmScoreCount!=UniversalAttributes::GetInstance()->galaxyMatterScore) {
        gmCounter->setString(int2str(UniversalAttributes::GetInstance()->galaxyMatterScore));
        gmScoreCount = UniversalAttributes::GetInstance()->galaxyMatterScore;
        if (gmCounter->getActionManager()->getNumberOfRunningActionsInTarget(gmCounter)==0) {
            sb1 = ScaleTo::create(0.1, 2.0);
            sb2 = ScaleTo::create(0.1, 1);
            sq = Sequence::create(sb1,sb2,NULL);
            gmCounter->runAction(sq);
        }
    }
    
    //monitor combo streaks
    if (comboScoreCount!=UniversalAttributes::GetInstance()->comboStreak&&UniversalAttributes::GetInstance()->comboStreak!=0) {
        string combo = "X";
        combo.append(int2str(UniversalAttributes::GetInstance()->comboStreak));
        comboCounter->setString(combo);
        comboScoreCount = UniversalAttributes::GetInstance()->comboStreak;
    
        
        if (comboCounter->getActionManager()->getNumberOfRunningActionsInTarget(comboCounter)==0) {
            //--create action
            st1 = ScaleTo::create(0.1, 1.7);
            st2 = ScaleTo::create(0.1, 1);
            st3 = ScaleTo::create(0.4, 1);
            st4 = ScaleTo::create(0.01, 0);
            sq = Sequence::create(st1,st2,st3,st4, NULL);
            //--create action
            comboCounter->runAction(sq);
        }
        string cst;
        if(UniversalAttributes::GetInstance()->comboStreak==50*comboMultiplier)
        {
            if(comboMultiplier==1)
                cst = "GOOD";
            else if(comboMultiplier==2)
                cst = "COOL";
            else if(comboMultiplier==3)
                cst = "AWESOME";
            else
                cst = "EXCELLENT";
    
            comboMultiplier = comboMultiplier+1;
            cheerSubTitle->setString(cst);
            //--create action
            st1 = ScaleTo::create(0.1, 1.5);
            st2 = ScaleTo::create(0.1, 0.8);
            st3 = ScaleTo::create(0.5, 0.8);
            st4 = ScaleTo::create(0.01, 0);
            sq = Sequence::create(st1,st2,st3,st4, NULL);
            //--create action
            cheerSubTitle->runAction(sq);
        }
        
    }
}

string MenuScene::int2str(int &i)
{
    string s;
    stringstream ss(s);
    ss << i;
    return ss.str();
}

void MenuScene::dummyController()
{
    Monster *dummyMon = UniversalAttributes::GetInstance()->monster;
    // crash the enemies
    MyQueryCallback queryCallback; //see "World querying topic"
    b2AABB aabb;
    //b2Vec2 explosionCenterVec = b2Vec2(explo->posX/PTM_RATIO, explo->posY/PTM_RATIO);
    Point attackPos = Point(dummyMon->theBody->getPositionX(), dummyMon->theBody->getPositionY());
    b2Vec2 detectionVec = b2Vec2(attackPos.x/PTM_RATIO, attackPos.y/PTM_RATIO);
    aabb.lowerBound = detectionVec - b2Vec2( 2.5, 1.0);
    aabb.upperBound = detectionVec + b2Vec2( 5, 1.0);
    gScene->world->QueryAABB( &queryCallback, aabb );
    
    for (int j = 0; j < queryCallback.foundBodies.size(); j++) {
        b2Body* body = queryCallback.foundBodies[j];
        b2Fixture* f = body->GetFixtureList();
        if (f) {
            FixtureType t = f->GetFixtureType();
            
            if (t == f_enemy_foot) {
                if (left->getPercentage()==100&&f->GetFilterData().categoryBits!=BULLET) {
                    gScene->monster->changeActionStatus(dash);
                    gScene->monster->action();
                }
                else if(f->GetFilterData().categoryBits==BULLET){
                    gScene->monster->changeActionStatus(jump);
                    gScene->monster->action();
                }
            }
            else if(t == f_commonObj) {
                if (left->getPercentage()==100) {
                    gScene->monster->changeActionStatus(dash);
                    gScene->monster->action();
                }
                
            }
            else{
            }
        }
    }
}

