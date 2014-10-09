//
//  InGameMenu.h
//  Animal_Squad
//
//  Created by wd on 10/1/14.
//
//

#ifndef __Animal_Squad__InGameMenu__
#define __Animal_Squad__InGameMenu__

#include <iostream>
#import "cocos2d.h"
#include <string.h>
#include "cocos-ext.h"
#include "UniversalAttributes.h"

using namespace std;
using namespace cocos2d;
using namespace extension;

class MenuScene;

class InGameMenu : public Layer
{
public:
    static InGameMenu* create(MenuScene *ms);
    virtual bool init(MenuScene *ms);
    ~InGameMenu();
    
private:
    MenuScene *menuscene;
    bool soundBool = true;
    MenuItemSprite *soundButton;
    MenuItemSprite *restartButton;
    MenuItemSprite *menuButton;
    MenuItemSprite *closeButton;
    
    void soundButtonCalled(Ref *sender);
    void restartButtonCalled(Ref *sender);
    void menuButtonCalled(Ref *sender);
    void closeButtonCalled(Ref *sender);
    
    bool touchesBegan(Touch* touch, Event* event);
    void touchesMoved(Touch* touch, Event* event);
    void touchesEnded(Touch* touch, Event* event);
};

#endif /* defined(__Animal_Squad__InGameMenu__) */
