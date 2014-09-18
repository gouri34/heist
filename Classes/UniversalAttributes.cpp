//
//  UniversalAttributes.cpp
//  Animal_Squad
//
//  Created by wd on 9/15/14.
//
//Universal Attributes can be accessed throughout the program, you can add any want-to-access parameter into the universal attributes.

#include "UniversalAttributes.h"

UniversalAttributes* UniversalAttributes::instance = NULL;

// universal attributes constructor
UniversalAttributes::UniversalAttributes()
{
    //do nothing
}


UniversalAttributes* UniversalAttributes::GetInstance()
{
    if (instance==NULL) {
        instance = new UniversalAttributes();
    }
    return instance;
}

void UniversalAttributes::setScreenRightEdge(Point p)
{
    //
}

void UniversalAttributes::setMonsterData(Monster *mon)
{
    monster = mon;
}



UniversalAttributes::~UniversalAttributes()
{
    //delete instance
    printf("Singleton Destructor\n");
}