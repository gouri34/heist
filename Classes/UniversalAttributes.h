//
//  UniversalAttributes.h
//  Animal_Squad
//
//  Created by wd on 9/15/14.
//
//

#ifndef __Animal_Squad__UniversalAttributes__
#define __Animal_Squad__UniversalAttributes__

#include <iostream>
#include <string.h>
#include "Monster.h"
#import "cocos2d.h"

using namespace std;
using namespace cocos2d;

class UniversalAttributes
{
public:
    
    NodeGrid *menuGrid;
    Monster *monster;
    ProgressTimer *pt;
    
    static UniversalAttributes* GetInstance(); //acquire instance
    
    virtual ~UniversalAttributes();
    
    void setScreenRightEdge(Point p);
    
    void setMonsterData(Monster *mon);
    
    //void storeMenuScenePointer(MenuScene *ms_);
    
private:
    UniversalAttributes();
    
    static UniversalAttributes* instance;
    
    //record real time right edge
    
    
};
#endif /* defined(__Animal_Squad__UniversalAttributes__) */
