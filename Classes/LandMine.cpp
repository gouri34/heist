//
//  LandMine.cpp
//  Animal_Squad
//
//  Created by wd on 9/17/14.
//
//

#include "LandMine.h"

LandMine* LandMine::create(const char *name, Point pos, float scalex, float scaley)
{
    LandMine *s = new LandMine();
    if (s&&s->init(name, pos, scalex, scaley)) {
        return s;
    }
    return NULL;
}

LandMine::~LandMine()
{
    //DO NOTHING
}

void LandMine::setArmatureBody()
{
    //Do nothing
}

void LandMine::creatfootBody()
{
    //create footbody for the armature
    b2Vec2 pos = b2Vec2(armature->getPositionX()/PTM_RATIO, armature->getPositionY()/PTM_RATIO);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.fixedRotation = true;
    bodyDef.userData = this;
    
    footBody = gameWorld->CreateBody(&bodyDef);
    
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((armature->getScaleX()*armature->getContentSize().width*0.5/PTM_RATIO), armature->getScaleY()*armature->getContentSize().height*0.5/PTM_RATIO);
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

void LandMine::update(float dt)
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
    
    //if monster is closing, run animation
    if (armature->getPositionX()<=a->GetInstance()->monster->theBody->getPositionX()+Director::getInstance()->getVisibleSize().width*0.7&&activated==false) {
        armature->getAnimation()->playWithIndex(0);
        activated = true;
    }
    
    //if landmine is activated and the animation is complete, explod it.
    if (activated==true&&exploded==false) {
        if (Enemy::armature->getAnimation()->isComplete()==true) {
            LandMine::explosionAction();
        }
    }
    // aabb testing for explosion
    if (exploded==true) {
        MyQueryCallback queryCallback;
        b2AABB aabb;
        b2Vec2 detectionVec = b2Vec2(armature->getPositionX()/PTM_RATIO,armature->getPositionY()/PTM_RATIO);
        aabb.lowerBound = detectionVec - b2Vec2(10 ,10);
        aabb.upperBound = detectionVec + b2Vec2(10,10);
        gameWorld->QueryAABB(&queryCallback, aabb);
        for (int j = 0; j < queryCallback.foundBodies.size(); j++) {
            b2Body* body = queryCallback.foundBodies[j];
            b2Fixture* f = body->GetFixtureList();
            if (f) {
                FixtureType t = f->GetFixtureType();
                
                //if collision with ground, apply impulse and start animation
                if (t == f_monster_body)
                {
                    printf("MONSTER GET HIT!\n");
                }
            }
        }
    }
}

void LandMine::explosionAction()
{
    //set armatuer invisible
    armature->setVisible(false);
    
    //add explosion effect
    explo = ParticleSun::createWithTotalParticles(100);
    explo->setAutoRemoveOnFinish(true);
    explo->setStartSize(100.0f);
    explo->setRadialAccel(250.0f);
    explo->setSpeed(0.7f);
    explo->setAnchorPoint(Point(0.5f,0.5f));
    explo->setPositionType(kCCPositionTypeGrouped);
    explo->setPosition(Point(armature->getPositionX(),armature->getPositionY()-15));
    explo->setDuration(0.25f);
    gameScene->addChild(explo, 30);
    
    exploded = true;

}

void LandMine::collisionProcess(Monster *monster)
{
    if (monster->isDashing()==true) {
        footBody->ApplyLinearImpulse(b2Vec2(800, 75), footBody->GetWorldCenter(), true);
        footBody->ApplyTorque(350, true);
    }
}
