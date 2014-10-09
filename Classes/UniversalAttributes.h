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
#include "Monster.h"
#import "cocos2d.h"
#include <string.h>

using namespace std;
using namespace cocos2d;

class UniversalAttributes
{
public:
    
    bool inMenuMode = true;
    bool soundOn = true;
    NodeGrid *menuGrid;
    Monster *monster;
    ProgressTimer *pt;
    //std::vector<Sprite*>healths;
    int healthCount;
    int destructionScore = 0;
    int galaxyMatterScore = 0;
    int comboStreak = 0;
    float enemyTimer = 1.3;
    
    static UniversalAttributes* GetInstance(); //acquire instance
    
    virtual ~UniversalAttributes();
    
    void setScreenRightEdge(Point p);
    
    void setMonsterData(Monster *mon);
    
    void addDestructionScore();
    void addMattersScore();
    
    void resetGame();
    
private:
    UniversalAttributes();
    
    static UniversalAttributes* instance;
    
    //record real time right edge
    
    
};

#endif /* defined(__Animal_Squad__UniversalAttributes__) */
