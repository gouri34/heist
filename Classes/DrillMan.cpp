//
//  DrillMan.cpp
//  Animal_Squad
//
//  Created by wd on 8/25/14.
//
//

#include "DrillMan.h"

DrillMan* DrillMan::create(cocos2d::Scene *parentScene, b2World *world, const char *name, Point pos, float scale)
{
    DrillMan *z = new DrillMan();
    if (z&&z->init(parentScene, world, name, pos, scale)) {
        return z;
    }
    return NULL;
}

bool DrillMan::init(cocos2d::Scene *parentScene, b2World *world, const char *name, Point pos, float scale)
{
    
    chType = drillMan;
    
    gameScene = parentScene;
    gameWorld = world;
    armature = NULL;
    footBody = NULL;
    dead = false;
    life = 100;
    desireVel = -1.2;
    onGround = false;
    
    
    
    armature = Armature::create(name);
    armature->setPosition(pos);
    armature->setScale(scale);
    armature->setAnchorPoint(Point(0.5, 0.5));
    zombie_scale = scale;
    // armature->setColor(Color3B(255, 150, 150));
    
    parentScene->addChild(armature,21);
    // armature->getAnimation()->playByIndex(0);
    setArmatureBody();
    
    DrillMan::creatfootBody();
    
    return true;
}

void DrillMan::creatfootBody()
{
    b2Vec2 pos = b2Vec2(armature->getPositionX()/PTM_RATIO, armature->getPositionY()/PTM_RATIO);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.fixedRotation = true;
    bodyDef.userData = this;
    
    footBody = gameWorld->CreateBody(&bodyDef);
    
    b2CircleShape circleShape;
    circleShape.m_radius = 0.6f;
    
    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.2;
    fixtureDef.friction = 0.5f;
    fixtureDef.fixturetype = f_foot;
    fixtureDef.filter.categoryBits = BULLET;
    fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | PLAYER;
    fixtureDef.shape = &circleShape;
    footBody->CreateFixture(&fixtureDef);
    
    b2PolygonShape polyShape;
    fixtureDef.filter.maskBits = 0;
    polyShape.SetAsBox(0.1, 1.7, b2Vec2(0, 1.7), 0);
    
    
    fixtureDef.shape = &polyShape;
    footBody->CreateFixture(&fixtureDef);
    
    
    footRect = Rect(footBody->GetPosition().x*PTM_RATIO-0.45*PTM_RATIO/2, footBody->GetPosition().y-0.45*PTM_RATIO/2, 0.45*PTM_RATIO, 0.45*PTM_RATIO);
    
    collisionPoint = Rect(armature->getBoundingBox().size.width/2, 0, 3, 3);
}

void DrillMan::update(float dt, Bear *bear)
{
    Enemy::update(dt, bear);
    if (footBody != NULL)
    {
        
        if ( bear->theBody->getBoundingBox().intersectsRect(collisionPoint)) {
            bear->gettingHurt();
        }
        
        //aabb detect collision with the ground, if true then
        //apply footbody a linear impulse to the upper left direction
        MyQueryCallback queryCallback;
        b2AABB aabb;
        b2Vec2 detectionVec = b2Vec2(armature->getPositionX()/PTM_RATIO,armature->getPositionY()/PTM_RATIO);
        aabb.lowerBound = detectionVec - b2Vec2(0.5*armature->getBoundingBox().size.width/PTM_RATIO ,0.5*armature->getBoundingBox().size.height/PTM_RATIO);
        aabb.upperBound = detectionVec + b2Vec2(0.5*armature->getBoundingBox().size.width/PTM_RATIO,0.5*armature->getBoundingBox().size.height/PTM_RATIO);
        gameWorld->QueryAABB(&queryCallback, aabb);
        for (int j = 0; j < queryCallback.foundBodies.size(); j++) {
            b2Body* body = queryCallback.foundBodies[j];
            b2Fixture* f = body->GetFixtureList();
            if (f) {
                FixtureType t = f->GetFixtureType();
                
                //if collision with ground, apply impulse and start animation
                if (t == f_ground) {
                    if (onGround==false) {
                        armature->getAnimation()->playWithIndex(0);
                        footBody->ApplyLinearImpulse(b2Vec2(0, 7), b2Vec2(footBody->GetPosition().x, footBody->GetPosition().y), true);
                        footBody->ApplyLinearImpulse(b2Vec2(-2,0), b2Vec2(footBody->GetPosition().x, footBody->GetPosition().y), true);

                        onGround = true;
                        return;
                    }
                }
            }
        }
        onGround = false;
        return;
    }
}



DrillMan::~DrillMan()
{
    //delete drillman
}