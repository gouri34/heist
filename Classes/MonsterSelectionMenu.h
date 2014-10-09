//
//  MonsterSelectionMenu.h
//  Animal_Squad
//
//  Created by wd on 9/29/14.
//
//

#ifndef __Animal_Squad__MonsterSelectionMenu__
#define __Animal_Squad__MonsterSelectionMenu__

#include <iostream>
#include "cocos-ext.h"
#include "UniversalAttributes.h"

using namespace std;
using namespace cocos2d;
using namespace extension;

class MonsterSelectionMenu : public Layer
{

private:
    
    bool isMoveing;
    //touch events
    bool touchesBegan(Touch* touch, Event* event);
    void touchesMoved(Touch* touch, Event* event);
    void touchesEnded(Touch* touch, Event* event);
    
public:
    static MonsterSelectionMenu* create();
    virtual bool init();
    ~MonsterSelectionMenu();
    void closeButtonCalled(Ref *sender);
    MenuItemSprite *closebutton;
};

#endif /* defined(__Animal_Squad__MonsterSelectionMenu__) */
