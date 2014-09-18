//
//  Item.cpp
//  Animal_Squad
//
//  Created by wd on 9/17/14.
//
//

#include "Item.h"

Item* Item::create(Layer* gameLayer, b2World *gameWorld, Point pos)
{
    Item *i = new Item();
    if (i&&i->init(gameLayer,gameWorld,pos)) {
        return i;
    }
    return NULL;
}

bool Item::init(Layer* gameLayer, b2World *gameWorld, Point pos){
    gameLayer_ = gameLayer;
    gameWorld_ = gameWorld;
    //itemType= rand()%3;
    itemType= 0;
    if (itemType==0) {
        // spawn sprint item
        armature = Armature::create("Meteo");
    }
    else if(itemType==1){
        // spawn shield item
    }
    else {
        //spawn one health item
    }
    armature->setPosition(pos);
    armature->setAnchorPoint(Point(0.5,0.5));
    armature->setVisible(true);
    armature->setScale(0.2);
    gameLayer_->addChild(armature,30);
    armature->getAnimation()->playWithIndex(0);
    
    return true;
}

void Item::update(float dt){
    if (armature->getBoundingBox().intersectsRect(a->GetInstance()->monster->theBody->getBoundingBox())&&triggered==false) {
        //trigger item effect
        if (itemType==0) {
            a->GetInstance()->monster->goSprint(2.0);
        }
        else if (itemType==1){
            
        }
        else if (itemType==2){
            
        }
        armature->setVisible(false);
        triggered = true;
    }
}

Item::~Item()
{
    gameLayer_->removeChild(armature);
}