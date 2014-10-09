//
//  MainMenu.h
//  Animal_Squad
//
//  Created by wd on 9/28/14.
//
//

#ifndef __Animal_Squad__MainMenu__
#define __Animal_Squad__MainMenu__

#include <iostream>
#import "cocos2d.h"
#include <string.h>
#include "cocos-ext.h"
#include "UniversalAttributes.h"
#include "MonsterSelectionMenu.h"
#include "OffGameMenu.h"
#include "TutorialMenu.h"

using namespace std;
using namespace cocos2d;
using namespace extension;

class MainMenuCoordinator;

class MainMenu : public Layer
{
public:
    static MainMenu* create();
    virtual bool init();
    ~MainMenu();
    
    void allElementsInvisible();
    void allElementsVisible();
    
private:
    void tutorialButtonCalled(Ref *sender);
    void goButtonCalled(Ref *sender);
    void aboutusButtonCalled(Ref *sender);
    void monstersButtonCalled(Ref *sender);
    void optionButtonCalled(Ref *sender);

    MenuItemSprite *tutorialButton;
    MenuItemSprite *monstersButton;
    MenuItemSprite *aboutUsButton;
    MenuItemSprite *goButton;
    MenuItemSprite *optionButton;
    Sprite *mhwords;
    Sprite *retroactivelogo;
    ScaleBy *sb1;
    ScaleBy *sb2;
    Sequence *sq;
    RepeatForever *rf;
    
    RotateBy *rb1;
    RepeatForever *rotationrf;
    bool islogo = false;
    
    //touch events
    bool touchesBegan(Touch* touch, Event* event);
    void touchesMoved(Touch* touch, Event* event);
    void touchesEnded(Touch* touch, Event* event);
    
    //menus
    MonsterSelectionMenu *msm;
    OffGameMenu *ogm;
};


#endif /* defined(__Animal_Squad__MainMenu__) */
