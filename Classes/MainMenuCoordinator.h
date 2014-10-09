//
//  MainMenuCoordinator.h
//  Animal_Squad
//
//  Created by wd on 9/30/14.
//
//main menu coordinator is a singleton class that set as a observer class which whill observer all the sub menu commands
// send them back to the main menu

#ifndef __Animal_Squad__MainMenuCoordinator__
#define __Animal_Squad__MainMenuCoordinator__

#include <iostream>
#import "cocos2d.h"
#include <string.h>
#include "MainMenu.h"

using namespace std;
using namespace cocos2d;

class MainMenuCoordinator
{
public:
    static MainMenuCoordinator* GetInstance();
    
    virtual ~MainMenuCoordinator();
    
    MainMenu *mm;
    
private:
    MainMenuCoordinator();
    static MainMenuCoordinator* instance;
};

#endif /* defined(__Animal_Squad__MainMenuCoordinator__) */
