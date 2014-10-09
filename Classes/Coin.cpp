//
//  Coin.cpp
//  Animal_Squad
//
//  Created by wd on 9/28/14.
//
//

#include "Coin.h"

Coin* Coin::create(Layer* gameLayer, b2World *gameWorld, Point pos){
    Coin *a = new Coin();
    if (a&&a->init(gameLayer,gameWorld,pos)) {
        return a;
    }
    return NULL;
}

bool Coin::init(cocos2d::Layer *gameLayer, b2World *gameWorld, Point pos){
    gameLayer_ = gameLayer;
    gameWorld_ = gameWorld;
    armature = Armature::create("yuzhouwuzhi");
    armature->setAnchorPoint(Point(0.5,0.5));
    armature->setScale(0.5);
    armature->setPosition(pos);
    armature->getAnimation()->playWithIndex(0);
    gameLayer_->addChild(armature,50);
    
    return true;
}

void Coin::update(float dt)
{
    if(armature->getBoundingBox().intersectsRect(UniversalAttributes::GetInstance()->monster->theBody->getBoundingBox())&&collected==false){
        //coin collected
        armature->setVisible(false);
        UniversalAttributes::GetInstance()->destructionScore += score;
        if(UniversalAttributes::GetInstance()->inMenuMode==false)
            UniversalAttributes::GetInstance()->galaxyMatterScore++;
        collected = true;
    }
}

Coin::~Coin()
{
    //do nothing
}

