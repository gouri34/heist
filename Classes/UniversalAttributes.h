//
//  UniversalAttributes.h
//  Animal_Squad
//
//  Created by wd on 8/26/14.
//
//Universal Attributes is a singleton class that can be accessed from all classes in the game


#ifndef __Animal_Squad__UniversalAttributes__
#define __Animal_Squad__UniversalAttributes__

#include <iostream>
#include <string.h>
#import "cocos2d.h"

using namespace std;
using namespace cocos2d;

class UniversalAttributes
{
public:
    static UniversalAttributes* GetInstance(); //acquire instance
    
    virtual ~UniversalAttributes();
    
    void setScreenRightEdge(Point p);
    
private:
    UniversalAttributes();
    
    static UniversalAttributes* instance;
    
    Point screenRightEdge;   //record real time right edge
    
};

#endif /* defined(__Animal_Squad__UniversalAttributes__) */
