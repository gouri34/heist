//
//  LandMine.cpp
//  Animal_Squad
//
//  Created by wd on 9/2/14.
//
//

#include "LandMine.h"

LandMine* LandMine::create(cocos2d::Layer *gameScene_, b2World *gameWorld_, Point pos, string armatureName, float scale, float density)
{
    LandMine *s = new LandMine();
    if (s&&s->init(gameScene_, gameWorld_, pos, armatureName, scale, density)) {
        return s;
    }
    return NULL;
}

bool LandMine::init(cocos2d::Layer *gameScene_, b2World *gameWorld_, Point pos, string armatureName, float scale, float density)
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

void LandMine::update(float dt, Bear* bear)
{
    // if monster is closing, run animation
    if ((armature->getPositionX()<=bear->theBody->getPositionX()+Director::getInstance()->getVisibleSize().width*0.7)&&activated==false) {
        armature->getAnimation()->playWithIndex(0);
        activated=true;
    }
    
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
    
    //AABB range, if colliding with player, give a impulse
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
            if ((t == f_bear_body||t==f_bear_foot)&&bear->isDashing()==true) {
                body_->ApplyLinearImpulse(b2Vec2(800, 75), body_->GetWorldCenter(), true);
                body_->ApplyTorque(350, true);
            }
        }
    }
    
    //-----------------------
    
    if (activated==true&&exploded==false) {
        if (armature->getAnimation()->isComplete()==true) {
            //explod with fire
            enemyObjectAction();
        }
    }
    
    if (exploded==true) {
        //aabb testing
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
                if (t == f_bear_body)
                {
                    bear->gettingHurt();
                }
                else if (t==f_foot||t==f_bodydead)
                {
//                    Enemy *e = (Enemy*)body->GetUserData();
//                    if (e->chType!=drillMan) {
//                        float XdistanceDiff = body->GetWorldCenter().x-detectionVec.x;
//                        float randSeed = rand()%100;
//                        float randForce = randSeed/50.0+2.8;
//                        float yForce = 1.0+fabs(XdistanceDiff)/8.5*1.2;
//                        int randtmp = rand()%2;
//                        if (randtmp==1) {
//                            e->die(b2Vec2(-2*randForce, 5*yForce));
//                        }
//                        else
//                            e->die(b2Vec2(2*randForce, 5*yForce));
//                    }
                }

            }
        }
    }

}

void LandMine::enemyObjectAction()
{
    //set armatuer invisible
    armature->setVisible(false);
    
    //add explosion effect
    explo = ParticleSun::createWithTotalParticles(100);
    explo->retain();
    explo->setStartSize(100.0f);
    explo->setRadialAccel(250.0f);
    explo->setSpeed(0.5f);
    explo->setAnchorPoint(Point(0.5f,0.5f));
    explo->setPositionType(kCCPositionTypeGrouped);
    explo->setPosition(Point(armature->getPositionX(),armature->getPositionY()-15));
    explo->setDuration(0.5f);
    gameScene->addChild(explo, 20);
    
    exploded = true;

}

LandMine::~LandMine()
{
    gameScene->removeChild(armature, true);
    gameWorld->DestroyBody(body_);
}
