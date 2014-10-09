//
//  MainMenuCoordinator.cpp
//  Animal_Squad
//
//  Created by wd on 9/30/14.
//
//

#include "MainMenuCoordinator.h"

MainMenuCoordinator* MainMenuCoordinator::instance = NULL;

MainMenuCoordinator::MainMenuCoordinator()
{
    //DO NOTHING
}

MainMenuCoordinator* MainMenuCoordinator::GetInstance()
{
    if(instance==NULL){
        instance = new MainMenuCoordinator();
    }
    return instance;
}

MainMenuCoordinator::~MainMenuCoordinator()
{
    printf("singleton destructor");
}