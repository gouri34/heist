//
//  ScoreMultiplier.cpp
//  Animal_Squad
//
//  Created by wd on 8/22/14.
//
//

#include "ScoreMultiplier.h"

ScoreMultiplier* ScoreMultiplier::instance = NULL;

ScoreMultiplier::ScoreMultiplier()
{
    this->var = 0;
    
    printf("Score Multiplier constructor\n");
    
}

ScoreMultiplier::~ScoreMultiplier()
{
    printf("Singleton Destructor\n");
    //delete instance
}

//initialize static members

ScoreMultiplier* ScoreMultiplier::GetInstance()
{
    if (instance==NULL) {
        instance = new ScoreMultiplier();
    }
    return instance;
}
