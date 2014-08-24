//
//  MonsterTrap.cpp
//  Animal_Squad
//
//  Created by wd on 8/23/14.
//
//scale 0.15, density 1f

#include "MonsterTrap.h"

MonsterTrap* MonsterTrap::create(cocos2d::Layer *gameScene_, b2World *gameWorld_, Point pos, string armatureName, float scale, float density)
{
    MonsterTrap *z = new MonsterTrap();
    if (z && z->init(gameScene_, gameWorld_, pos, armatureName,scale,density)) {
        return z;
    }
    return NULL;
}

/*
bool MonsterTrap::init(cocos2d::Layer *gameScene_, b2World *gameWorld_, Point pos, string armatureName)
{
    gameScene = gameScene_;
    gameWorld = gameWorld_;
    
    //load the armature for the reference first
    active = false;
    armature = Armature::create(armatureName);
    armature->setPosition(pos);
    armature->setScale(0.15);
    armature->setVisible(true);
    armature->setAnchorPoint(Point(0.5,0));
    gameScene->addChild(armature,12);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x/PTM_RATIO,pos.y/PTM_RATIO);
    //beastTrap->setPosition(Point(body_->GetPosition().x*PTM_RATIO,body_->GetPosition().y*PTM_RATIO));
    
    body_ = gameWorld->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((0.075*armature->getContentSize().width*0.5/PTM_RATIO), 0.075*armature->getContentSize().height*0.5/PTM_RATIO);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.2f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = ZOMBIE;
    fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND;
    fixtureDef.fixturetype = f_object;
    body_->CreateFixture(&fixtureDef);
    body_->SetUserData(&armature);
    return true;
}
*/

void MonsterTrap::update(float dt, Bear *bear)
{
    EnemyObject::update(dt, bear);
    //if (activeBeastTrap) {
    armature->setPosition(Point(body_->GetPosition().x*PTM_RATIO,body_->GetPosition().y*PTM_RATIO));
    float angle = body_->GetAngle()/M_PI*180;
    while (angle <= 0){
        angle += 360;
    }
    while (angle > 360){
        angle -= 360;
    }
    armature->setRotation(angle);
    //}
    if ( bear->theBody->getBoundingBox().intersectsRect(armature->getBoundingBox())&&active == false) {
        //body_->SetType(b2_staticBody);
        enemyObjectAction();
    }
}

void MonsterTrap::enemyObjectAction()
{
    EnemyObject::enemyObjectAction();
    body_->ApplyLinearImpulse(b2Vec2(0, 5), b2Vec2(body_->GetPosition().x-3, body_->GetPosition().y-6), true);
    armature->getAnimation()->playWithIndex(0);
    active = true;
}

MonsterTrap::~MonsterTrap()
{
    gameScene->removeChild(armature, true);
    gameWorld->DestroyBody(body_);
}