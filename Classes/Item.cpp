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
    itemType= rand()%2;
//    itemType= 1;
    if (itemType==0) {
        // spawn sprint item
        armature = Armature::create("Meteo");
    }
    else if(itemType==1){
        //spawn one health item
        armature = Armature::create("healthItem");
        
    }
    else {
        // spawn shield item
        armature = Armature::create("shieldItem");
    }
    armature->setPosition(pos);
    armature->setAnchorPoint(Point(0.5,0.5));
    armature->setVisible(true);
    armature->setScale(0.35);
    gameLayer_->addChild(armature,30);
    armature->getAnimation()->playWithIndex(0);
    
    return true;

}

void Item::setAsSprint()
{
    Point ap = armature->getPosition();
    gameLayer_->removeChild(armature);
    armature = NULL;
    armature = Armature::create("Meteo");
    armature->setPosition(ap);
    armature->setAnchorPoint(Point(0.5,0.5));
    armature->setVisible(true);
    armature->setScale(0.35);
    gameLayer_->addChild(armature,30);
    armature->getAnimation()->playWithIndex(0);
    itemType = 0;
}

void Item::setAsHealth()
{
    Point ap = armature->getPosition();
    gameLayer_->removeChild(armature);
    armature = NULL;
    armature = Armature::create("healthItem");
    armature->setPosition(ap);
    armature->setAnchorPoint(Point(0.5,0.5));
    armature->setVisible(true);
    armature->setScale(0.35);
    gameLayer_->addChild(armature,30);
    armature->getAnimation()->playWithIndex(0);
    itemType = 1;
}

void Item::update(float dt){
    if (armature->getBoundingBox().intersectsRect(a->GetInstance()->monster->theBody->getBoundingBox())&&triggered==false) {
        //trigger item effect
        if (itemType==0) {
            UniversalAttributes::GetInstance()->destructionScore += score;
            a->GetInstance()->monster->goSprint(6.0);
        }
        else if (itemType==1){
            if(UniversalAttributes::GetInstance()->healthCount<5){
                UniversalAttributes::GetInstance()->healthCount++;
            }
        }
        else if (itemType==2){
            
        }
        armature->setVisible(false);
        triggered = true;
    }

}

Item::~Item()
{
    gameLayer_->removeChild(armature,true);
}