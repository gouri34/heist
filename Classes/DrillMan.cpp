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
    isDead = false;
    
    
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
    fixtureDef.restitution = 0;
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
    
    collisionPoint = Rect(armature->getPositionX(), armature->getPositionY()-(armature->getScale()*armature->getContentSize().height*0.5),7, 3);
}

void DrillMan::update(float dt, Bear *bear)
{
    Enemy::update(dt, bear);
    collisionPoint = Rect(armature->getPositionX(), armature->getPositionY()-(armature->getScale()*armature->getContentSize().height*0.5), 7, 3);
    if (isDead == false)
    {
        
        if ( bear->theBody->getBoundingBox().intersectsRect(collisionPoint)) {
            bear->gettingHurt();
            footBody->ApplyLinearImpulse(b2Vec2(0, 7), b2Vec2(footBody->GetPosition().x, footBody->GetPosition().y), true);
            return;
        }
        
        //aabb detect collision with the ground, if true then
        //apply footbody a linear impulse to the upper left direction
        MyQueryCallback queryCallback;
        b2AABB aabb;
        b2Vec2 detectionVec = b2Vec2(armature->getPositionX()/PTM_RATIO,armature->getPositionY()/PTM_RATIO);
        aabb.lowerBound = detectionVec - b2Vec2(0.5*armature->getScale()*armature->getContentSize().width/PTM_RATIO ,0.5*armature->getScale()*armature->getContentSize().height/PTM_RATIO);
        aabb.upperBound = detectionVec + b2Vec2(0.5*armature->getScale()*armature->getContentSize().width/PTM_RATIO,0.5*armature->getScale()*armature->getContentSize().height/PTM_RATIO);
        gameWorld->QueryAABB(&queryCallback, aabb);
        for (int j = 0; j < queryCallback.foundBodies.size(); j++) {
            b2Body* body = queryCallback.foundBodies[j];
            b2Fixture* f = body->GetFixtureList();
            if (f) {
                FixtureType t = f->GetFixtureType();
                
                if(t == f_bear_body)
                {
                    // if not in dash mode
                    if (bear->isDashing()==false) {
                        float XdistanceDiff = body->GetWorldCenter().x-detectionVec.x;
                        float randSeed = rand()%100;
                        float randForce = randSeed/50.0+2.8;
                        float yForce = 1.0+fabs(XdistanceDiff)/8.5*1.2;
                        gottaDie(0);
                        isDead = true;
                        return;
                    }
                    // is dashing
                    else{
                        float XdistanceDiff = body->GetWorldCenter().x-detectionVec.x;
                        float randSeed = rand()%100;
                        float randForce = randSeed/50.0+2.8;
                        float yForce = 1.0+fabs(XdistanceDiff)/8.5*1.2;
                        gottaDie(1);
                        isDead = true;
                        return;
                        
                    }
                }
                //if collision with ground, apply impulse and start animation
                if (t == f_ground&&isDead==false&&(armature->getPositionX()<=bear->theBody->getPositionX()+Director::getInstance()->getVisibleSize().width*0.6))
                {
                    printf("onGround = %d\n",onGround);
                    if (onGround==false) {
                        armature->getAnimation()->playWithIndex(0);
                        footBody->ApplyLinearImpulse(b2Vec2(0, 17), b2Vec2(footBody->GetPosition().x, footBody->GetPosition().y), true);
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

void DrillMan::gottaDie(int dashingFlag)
{
//    if (footBody != NULL) {
//        gameWorld->DestroyBody(footBody);
//        footBody = NULL;
//    }
    if (dashingFlag == 0) {
        footBody->ApplyLinearImpulse(b2Vec2(0, 150), b2Vec2(footBody->GetPosition().x, footBody->GetPosition().y-5), true);
    }
    else
        footBody->ApplyLinearImpulse(b2Vec2(0, 150), b2Vec2(footBody->GetPosition().x, footBody->GetPosition().y-5), true);
    RotateBy *rb = RotateBy::create(3, 3000);
    armature->runAction(rb);
    
}


void DrillMan::die(b2Vec2 vec)
{
    if (footBody != NULL) {
        gameWorld->DestroyBody(footBody);
        footBody = NULL;
    }
    
    setB2bodyPosition();
    
    if (!dead) {
        
        for (auto o : deadSpriteArray) {
            if (armature->getScaleX() < 0) {
                o->setScaleX(-o->getScaleX());
            }
        }
        
        
        armature->setVisible(false);
        
        
        b2Filter filter;
        filter.categoryBits = DEAD_ZOMBIE;
        filter.maskBits = UPPER_GROUND | BASE_GROUND;
        
        const Map<std::string, Bone*>& dic = armature->getBoneDic();
        for(auto& element : dic)
        {
            
            Bone *bone = element.second;
            Skin *skin = (Skin*)element.second->getDisplayRenderNode();
            string name = element.first;
            
            int torqRandWay = rand()%2;
            float torgRand = (rand()%100)/100.0*2.7+2.7;
            if (torqRandWay == 0)
            {
                torgRand = -torgRand;
            }
            
            if (skin) {
                b2Body *body = skin->body;
                
                body->GetFixtureList()->SetFixtureType(f_bodydead);  //dead bodies fixturetype
                body->GetFixtureList()->SetFilterData(filter);
                body->SetType(b2_dynamicBody);
                printf("name: %s\n", name.c_str());
                if (name.compare("tou") == 0) {
                    float rNum = (rand()%100)/100*1.1+1.0;
                    vec = b2Vec2(vec.x*rNum, vec.y);
                }
                body->ApplyLinearImpulse(vec, body->GetWorldCenter(), true);
                body->ApplyAngularImpulse(torgRand, true);
                
                Bone *parentBone = bone->getParentBone();
                if (parentBone) {
                    Skin *parentSkin = (Skin*)parentBone->getDisplayRenderNode();
                    b2Body *parentBody = parentSkin->body;
                    b2RevoluteJointDef jdef;
                    //CCAffineTransform tran = bone->nodeToParentTransform();
                    Mat4 tran = bone->_getNodeToParentTransform();
                    Point p = Point(tran.m[12], tran.m[13]);
                    // Point p = skin->getWorldPosition();
                    //printf("p x = %f y = %f\n", p.x, p.y);
                    jdef.Initialize(parentBody, body, b2Vec2(p.x/PTM_RATIO, p.y/PTM_RATIO));
                    jdef.lowerAngle = -0.35f * b2_pi;
                    jdef.upperAngle = 0.35f * b2_pi;
                    jdef.enableLimit = true;
                    gameWorld->CreateJoint(&jdef);
                }
                
                
                
            }
        }
        
        //update(0);
        
        dead = true;
        
        setBodySprites();
        
        for (auto o : deadSpriteArray) {
            o->setVisible(true);
        }
        
        
    }
}



DrillMan::~DrillMan()
{
    //delete drillman
}