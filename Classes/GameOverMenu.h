//
//  GameOverMenu.h
//  Animal_Squad
//
//  Created by wd on 10/5/14.
//
//

#ifndef __Animal_Squad__GameOverMenu__
#define __Animal_Squad__GameOverMenu__

#include <iostream>
#import "cocos2d.h"
#include <string.h>
#include "cocos-ext.h"
#include "UniversalAttributes.h"

using namespace std;
using namespace cocos2d;
using namespace extension;

class MenuScene;

class GameOverMenu : public Layer
{
public:
    static GameOverMenu* create(MenuScene *ms);
    virtual bool init(MenuScene *ms);
    ~GameOverMenu();
    
private:
    bool isShownButton = false;
    MenuScene *menuscene;
    MenuItemSprite *restartButton;
    MenuItemSprite *menuButton;
    
    Armature *gameover;
    
    void restartButtonCalled(Ref *sender);
    void menuButtonCalled(Ref *sender);
    
    bool touchesBegan(Touch* touch, Event* event);
    void touchesMoved(Touch* touch, Event* event);
    void touchesEnded(Touch* touch, Event* event);
    
    Sequence *sq;
    Sprite *gameOverSprite;
    void update(float dt);
    string int2str(int &i);
    
};

#endif /* defined(__Animal_Squad__GameOverMenu__) */
