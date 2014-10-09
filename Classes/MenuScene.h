//
//  MenuScene.h
//  Bear
//
//  Created by cong ku on 14-5-27.
//
//

#ifndef __Bear__MenuScene__
#define __Bear__MenuScene__

#include "cocos2d.h"
#include "GameScene.h"
#include "cocos-ext.h"
#include "UniversalAttributes.h"
#include "MainMenu.h"
#include "InGameMenu.h"
#include "GameOverMenu.h"

using namespace extension;


class MenuScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuReloadCallback(Ref *sender) ;
    void bearDie(Ref* pSender);
    void leftPushCallback() ;
    void leftReleaseCallback() ;
    void attacPushkCallback();
    void attacReleasekCallback();
    
    //touch events
    bool touchesBegan(Touch* touch, Event* event);
    void touchesMoved(Touch* touch, Event* event);
    void touchesEnded(Touch* touch, Event* event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MenuScene);
    virtual void cleanup();

    
    void update(float dt);
    
    void reloadMenu();
    
    bool isPausing = false;
    
private:
    GameScene* gScene;
    int bearMovement;
    float gameTimer;
    float speedUpCheckPoint;
    ProgressTimer *left;
    void createPerkProgressBar();
    void healthMonitor();
    int healthCount;
    std::vector<Sprite*>healths;
    //destruction point related
    Label *dpCounter; //damage point counter
    Label *gmCounter; //galaxy matters counter
    Label *comboCounter; // combo counter
    Label *cheerSubTitle;
    int dpScoreCount;
    int gmScoreCount;
    int comboScoreCount;
    int comboMultiplier;
    string int2str(int &i);
    Sequence *sq;
    ScaleTo *sb1;
    ScaleTo *sb2;
    ScaleTo *st1;
    ScaleTo *st2;
    ScaleTo *st3;
    ScaleTo *st4;
    void dpMonitor();
    bool inDummyMode;
    Armature *speeduparmature;
    
    Sequence *speedupSq;
    Sprite *speedupSprite;
    
    void dummyController();
    
    void optionCalled(Ref *sender);
};

#endif /* defined(__Bear__MenuScene__) */
