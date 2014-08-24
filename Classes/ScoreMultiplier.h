//
//  ScoreMultiplier.h
//  Animal_Squad
//
//  Created by wd on 8/22/14.
//
//Score mutiplier, handle all the score calculation

#ifndef __Animal_Squad__ScoreMultiplier__
#define __Animal_Squad__ScoreMultiplier__

#include <iostream>
#include <string.h>

using namespace std;

class ScoreMultiplier
{
public:
    static ScoreMultiplier* GetInstance(); //acquire instance
    
    int getVar();
    
    void setVar(int var);
    
    virtual ~ScoreMultiplier();
    
private:
    ScoreMultiplier();
    
    static ScoreMultiplier* instance;
    
    int var;
    
protected:
    
    
};

#endif /* defined(__Animal_Squad__ScoreMultiplier__) */
