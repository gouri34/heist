//
//  UniversalAttributes.cpp
//  Animal_Squad
//
//  Created by wd on 8/26/14.
//
//

#include "UniversalAttributes.h"

UniversalAttributes* UniversalAttributes::instance = NULL;

// universal attributes constructor
UniversalAttributes::UniversalAttributes()
{
    screenRightEdge = Point(0,0);
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

UniversalAttributes::~UniversalAttributes()
{
    //delete instance
    printf("Singleton Destructor\n");
}

