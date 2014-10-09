//
//  MonsterTrap.cpp
//  Animal_Squad
//
//  Created by wd on 9/30/14.
//
//

#include "MonsterTrap.h"
MonsterTrap* MonsterTrap::create(const char*name, Point pos, float scalex, float scaley)
{
    MonsterTrap *a = new MonsterTrap();
    if (a&&a->init(name,pos,scalex,scaley)) {
        return a;
    }
    return NULL;
}

MonsterTrap::~MonsterTrap()
{
    // do nothing
}

void MonsterTrap::setArmatureBody()
{
    //do nothing
}

void MonsterTrap::creatfootBody()
{
    armature->setAnchorPoint(Point(0.5,0.5));
    b2Vec2 pos = b2Vec2(armature->getPositionX()/PTM_RATIO, armature->getPositionY()/PTM_RATIO);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.fixedRotation = true;
    bodyDef.userData = this;
    
    footBody = gameWorld->CreateBody(&bodyDef);
    
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((armature->getScaleX()*armature->getContentSize().width*0.2/PTM_RATIO),armature->getScaleY()*armature->getContentSize().height*0.36/PTM_RATIO);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.2;
    fixtureDef.friction = 0.5f;
    fixtureDef.fixturetype = f_enemy_foot;
    fixtureDef.filter.categoryBits = ZOMBIE;
    fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND;
    fixtureDef.shape = &dynamicBox;
    footBody->CreateFixture(&fixtureDef);
    
    b2PolygonShape polyShape;
    fixtureDef.filter.maskBits = 0;
    polyShape.SetAsBox(0.1, 1.7, b2Vec2(0, 1.7), 0);
    
    
    fixtureDef.shape = &polyShape;
    footBody->CreateFixture(&fixtureDef);
    
    
    footRect = Rect(footBody->GetPosition().x*PTM_RATIO-0.45*PTM_RATIO/2, footBody->GetPosition().y-0.45*PTM_RATIO/2, 0.45*PTM_RATIO, 0.45*PTM_RATIO);
}

void MonsterTrap::update(float dt)
{
    Enemy::update(dt);
    armature->setPosition(Point(footBody->GetPosition().x*PTM_RATIO, footBody->GetPosition().y*PTM_RATIO));
    float angle = footBody->GetAngle()/M_PI*180;
    while (angle <= 0){
        angle += 360;
    }
    while (angle > 360){
        angle -= 360;
    }
    armature->setRotation(angle);
    
    //in proper range,monstertrap plays animation
    if (armature->getPositionX()<=UniversalAttributes::GetInstance()->monster->theBody->getPositionX()+Director::getInstance()->getVisibleSize().width*0.7&&isAnimated==false) {
        armature->getAnimation()->playWithIndex(0);
        isAnimated = true;
    }
}

void MonsterTrap::collisionProcess(Monster *monster)
{
    if (monster->isDashing()==false) {
        monster->getHurt();
    }
}

