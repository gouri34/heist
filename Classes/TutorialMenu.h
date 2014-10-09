//
//  TutorialMenu.h
//  Animal_Squad
//
//  Created by wd on 10/4/14.
//
//

#ifndef __Animal_Squad__TutorialMenu__
#define __Animal_Squad__TutorialMenu__

#include <iostream>
#import "cocos2d.h"
#include <string.h>
#include "cocos-ext.h"
#include "UniversalAttributes.h"

using namespace std;
using namespace cocos2d;
using namespace extension;

class TutorialMenu : public Layer
{
public:
    static TutorialMenu* create();
    virtual bool init();
    ~TutorialMenu();
private:
    vector<Sprite*>tutorialSprites;
    Armature *word;
    int tCount = 0;
    bool touchesBegan(Touch* touch, Event* event);
    void touchesMoved(Touch* touch, Event* event);
    void touchesEnded(Touch* touch, Event* event);
    
};
#endif /* defined(__Animal_Squad__TutorialMenu__) */
