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


bool MonsterTrap::init(cocos2d::Layer *gameScene_, b2World *gameWorld_, Point pos, string armatureName,float scale,float density)
{
    gameScene = gameScene_;
    gameWorld = gameWorld_;
    
    //load the armature for the reference first
    active = false;
    armature = Armature::create(armatureName);
    armature->setPosition(pos);
    armature->setScale(0.15);
    armature->setVisible(false);
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
    
    //testing particle effect
    p = ParticleSystemQuad::create("particle.plist");
    p->retain();
    //p->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    p->setScaleY(0.4);
    p->setScaleX(0.6);
    p->setPosition(armature->getPosition());
    p->setPositionType(kCCPositionTypeGrouped);
    gameScene->addChild(p,20);
    
    return true;
}


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
    //aabb
    MyQueryCallback queryCallback;
    b2AABB aabb;
    b2Vec2 detectionVec = b2Vec2(armature->getPositionX()/PTM_RATIO,armature->getPositionY()/PTM_RATIO);
    aabb.lowerBound = detectionVec - b2Vec2(0.5*armature->getContentSize().width*armature->getScale()/PTM_RATIO ,0.5*armature->getContentSize().height*armature->getScale()/PTM_RATIO);
    aabb.upperBound = detectionVec + b2Vec2(0.5*armature->getContentSize().width*armature->getScale()/PTM_RATIO,0.5*armature->getContentSize().height*armature->getScale()/PTM_RATIO);
    gameWorld->QueryAABB(&queryCallback, aabb);
    for (int j = 0; j < queryCallback.foundBodies.size(); j++) {
        b2Body* body = queryCallback.foundBodies[j];
        b2Fixture* f = body->GetFixtureList();
        if (f) {
            FixtureType t = f->GetFixtureType();
            
            //if collision with ground, apply impulse and start animation
            if ((t == f_bear_body||t == f_bodydead)&&active==false&&(armature->getPositionX()<=bear->theBody->getPositionX()+Director::getInstance()->getVisibleSize().width*0.7)) {
                enemyObjectAction();
                active = true;
            }
        }
    }
    //
//    if ( bear->theBody->getBoundingBox().intersectsRect(armature->getBoundingBox())&&active == false) {
//        //body_->SetType(b2_staticBody);
//        enemyObjectAction();
//    }
    
}

void MonsterTrap::enemyObjectAction()
{
    EnemyObject::enemyObjectAction();
    body_->ApplyLinearImpulse(b2Vec2(0, 10), b2Vec2(body_->GetPosition().x-3, body_->GetPosition().y-6), true);
    armature->getAnimation()->playWithIndex(0);
    active = true;
}

MonsterTrap::~MonsterTrap()
{
    gameScene->removeChild(armature, true);
    gameWorld->DestroyBody(body_);
}