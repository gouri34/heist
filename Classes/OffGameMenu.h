//
//  OffGameMenu.h
//  Animal_Squad
//
//  Created by wd on 9/29/14.
//
//

#ifndef __Animal_Squad__OffGameMenu__
#define __Animal_Squad__OffGameMenu__

#include <iostream>
#import "cocos2d.h"
#include <string.h>
#include "cocos-ext.h"
#include "UniversalAttributes.h"

using namespace std;
using namespace cocos2d;
using namespace extension;

class MainMenuCoordinator;

class OffGameMenu : public Layer
{
public:
    static OffGameMenu* create();
    virtual bool init();
    ~OffGameMenu();
    void allElementsVisible();
    void allElementsInvisible();
    
private:
    bool inCredit = false;
    Sprite *creditSprite;
    bool soundBool;
    MenuItemSprite *soundButton;
    MenuItemSprite *creditButton;
    MenuItemSprite *closeButton;
    
    void soundButtonCalled(Ref *sender);
    void creditButtonCalled(Ref *sender);
    void closeButtonCalled(Ref *sender);
    
    bool touchesBegan(Touch* touch, Event* event);
    void touchesMoved(Touch* touch, Event* event);
    void touchesEnded(Touch* touch, Event* event);
};
#endif /* defined(__Animal_Squad__OffGameMenu__) */
