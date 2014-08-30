//
//  EnemyObject.cpp
//  Animal_Squad
//
//  Created by wd on 8/23/14.
//
//

#include "EnemyObject.h"

EnemyObject* EnemyObject::create(Layer *gameScene_, b2World *gameWorld_,Point pos,string armatureName,float scale,float density)
{
    EnemyObject *z = new EnemyObject();
    if (z && z->init(gameScene_, gameWorld_, pos, armatureName,scale,density)) {
        return z;
    }
    return NULL;
}

bool EnemyObject::init(cocos2d::Layer *gameScene_, b2World *gameWorld_, Point pos, string armatureName, float scale, float density)
{
    gameScene = gameScene_;
    gameWorld = gameWorld_;
    
    //load the armature for the reference first
    active = false;
    armature = Armature::create(armatureName);
    armature->setPosition(pos);
    armature->setScale(scale);
    armature->setVisible(true);
    armature->setAnchorPoint(Point(0.5,0.5));
    gameScene->addChild(armature,23);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x/PTM_RATIO,pos.y/PTM_RATIO);
    //beastTrap->setPosition(Point(body_->GetPosition().x*PTM_RATIO,body_->GetPosition().y*PTM_RATIO));
    
    body_ = gameWorld->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((scale*armature->getContentSize().width*0.5/PTM_RATIO), scale*armature->getContentSize().height*0.5/PTM_RATIO);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = density;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = ZOMBIE;
    fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND;
    fixtureDef.fixturetype = f_object;
    body_->CreateFixture(&fixtureDef);
    body_->SetUserData(&armature);
    return true;
}

void EnemyObject::enemyObjectAction()
{
    
}

EnemyObject::~EnemyObject()
{
//    gameScene->removeChild(armature, true);
//    gameWorld->DestroyBody(body_);
}

void EnemyObject::update(float dt, Bear *bear)
{
    
}