//
//  BazookaMan.cpp
//  Animal_Squad
//
//  Created by wd on 9/7/14.
//
//

#include "BazookaMan.h"

BazookaMan* BazookaMan::create(cocos2d::Scene *parentScene, b2World *world, const char *name, Point pos, float scale)
{
    BazookaMan *a = new BazookaMan();
    if (a&&a->init(parentScene, world, name, pos, scale)) {
        return a;
    }
    return NULL;
}



BazookaMan::~BazookaMan()
{
    gameScene->removeChild(rocketShellSprite);
    gameWorld->DestroyBody(rocketShellBody_);
}

void BazookaMan::update(float dt, Bear *bear)
{
    if (dead) {
        Sprite* s = deadSpriteArray.at(0);
        position = s->getPosition();
    }
    else {
        position = armature->getPosition();
    }
    
    
    if (!dead) {
        
        updateArmatureBody();
        
        //zombie movement
        if (footBody != NULL)
        {
            
            float offsetX = HEIGHTDIFFX;
            if (waypointOnRight) {
                offsetX = -HEIGHTDIFFX;
            }
            
            armature->setPosition(Point(footBody->GetPosition().x*
                                        PTM_RATIO+offsetX, footBody->GetPosition().y*PTM_RATIO+heightDiffY));
            
            
        }
    }
    else {
        
        if (!afterDeath) {
            
            float maxVelocity = setBodySprites();
            
        }
        
    }


    // if monster is close to bazookaman, shoot
    if (footBody != NULL)
    {
        if (isFiring==false&&isPlayingAnimation == false&&(armature->getPositionX()<=bear->theBody->getPositionX()+Director::getInstance()->getVisibleSize().width*0.85)) {
            armature->getAnimation()->playWithIndex(0);
            isPlayingAnimation = true;
            createRocketShell();
        }
        

        if (isFiring==true&&isExploding==false) {
            //detect collision
            rocketShellBody_->SetLinearVelocity(b2Vec2(shellVelo, 0));
            shellCollisionDetector(bear);
            rocketShellSprite->setPosition(rocketShellBody_->GetPosition().x*PTM_RATIO,rocketShellBody_->GetPosition().y*PTM_RATIO);
            if (rocketShellSprite->getPositionX()>bear->theBody->getPositionX()+Director::getInstance()->getVisibleSize().width) {
                isExploding=true;
                rocketShellBody_->SetActive(false);
                rocketShellSprite->setVisible(false);
            }
            
        }
        
    
        //detect collision with the monster
        collideAction(bear);
    }
}

void BazookaMan::collideAction(Bear *bear)
{
    //aabb detect collision with the player, if true then dead
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
            
            //if collision with player, die!
            if (t == f_bear_body&&dead==false) {
                // if not in dash mode
                if (bear->isDashing()==false) {
                    float XdistanceDiff = body->GetWorldCenter().x-detectionVec.x;
                    float randSeed = rand()%100;
                    float randForce = randSeed/50.0+2.8;
                    float yForce = 1.0+fabs(XdistanceDiff)/8.5*1.2;
                    Enemy::die(b2Vec2(randForce, yForce));
                }
                // is dashing
                else{
                    float XdistanceDiff = body->GetWorldCenter().x-detectionVec.x;
                    float randSeed = rand()%100;
                    float randForce = randSeed/50.0+2.8;
                    float yForce = 1.0+fabs(XdistanceDiff)/8.5*1.2;
                    Enemy::die(b2Vec2(2*randForce, yForce));
                }
                
            }
            else if (t==f_mutual_hurtful_object)
            {
                float XdistanceDiff = body->GetWorldCenter().x-detectionVec.x;
                float randSeed = rand()%100;
                float randForce = randSeed/50.0+2.8;
                float yForce = 1.0+fabs(XdistanceDiff)/8.5*1.2;
                Enemy::die(b2Vec2(2*randForce, 8*yForce));

            }
        }
    }
}

void BazookaMan::setArmatureBody()
{
    
    
    Vector<Node*> bonearr = armature->getChildren();
    
    heightDiffY = 25;
    
    // int z = bonearr->count();
    for(int i = 0; i< bonearr.size();i++)
    {
        Bone *bone = (Bone*)bonearr.at(i);
        string boneName = bone->getName();
        Skin *skin = (Skin*)bone->getDisplayRenderNode();
        
        
        if (skin !=NULL) {
            skin->isphysicsObject = true;
            skin->parentScale = armature->getScale();
            Rect a = skin->getTextureRect();
            
            Point partpos = skin->getWorldPosition();
            float partrotation = skin->getWorldRotation();
            float bodyrotation = partrotation*M_PI/180.0;
            Size partSize = Size((a.getMaxX()-a.getMinX())/PTM_RATIO*armature->getScale(), (a.getMaxY()-a.getMinY())/PTM_RATIO*armature->getScale());
            
            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            
            bodyDef.position.Set(partpos.x/PTM_RATIO, partpos.y/PTM_RATIO);
            b2Body *body_ = gameWorld->CreateBody(&bodyDef);
            
            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(partSize.width/2.0, partSize.height/2.0);//These are mid points for our 1m box
            b2CircleShape circleShape;
            circleShape.m_radius = 0.45;
            
            b2FixtureDef fixtureDef;
            if (boneName.compare("headbone") == 0||boneName.compare("head")==0) {
                fixtureDef.shape = &circleShape;
                
            }
            else if (boneName.compare("left_feet") == 0||boneName.compare("foot_right")==0)
            {
                circleShape.m_radius = 0.2;
                fixtureDef.shape = &circleShape;
                
            }
            else {
                fixtureDef.shape = &dynamicBox;
            }
            fixtureDef.density = 0.2f;
            fixtureDef.restitution = 0.8;
            fixtureDef.friction = 0.01f;
            fixtureDef.filter.categoryBits = ZOMBIE;
            
            // printf("bonename = %s\n", boneName.c_str());
            
            if (boneName.compare("headbone") == 0||boneName.compare("head")==0) {
                fixtureDef.fixturetype = f_zbody_body;
                fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | ARROW | BULLET;
            }
            else if (boneName.compare("bodybone") == 0||boneName.compare("body")==0) {
                fixtureDef.fixturetype = f_zbody_body;
                fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | ARROW | BULLET;
            }
            else if (boneName.compare("leglbone") == 0 || boneName.compare("right_leg") == 0|| boneName.compare("right_foreleg")|| boneName.compare("foreleg_rbone") ||boneName.compare("left_leg")==0||boneName.compare("left_foreleg")==0) {
                fixtureDef.fixturetype = f_zbody_body;
                fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | ARROW | BULLET;
            }
            else {
                fixtureDef.fixturetype = f_zbody_body;
                fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND;
            }
            
            
            
            body_->CreateFixture(&fixtureDef);
            body_->SetUserData(this);
            body_->SetTransform(b2Vec2(partpos.x/PTM_RATIO, partpos.y/PTM_RATIO), bodyrotation);
            body_->SetAngularDamping(1.2);
            
            skin->body = body_;
            
            
            PhysicsSprite *dumpSprite = (PhysicsSprite*)PhysicsSprite::createWithTexture(skin->getTexture(), skin->getTextureRect());
            dumpSprite->setPosition(skin->getWorldPosition());
            dumpSprite->setScale(armature->getScale());
            dumpSprite->setVisible(false);
            dumpSprite->body = body_;
            // printf("zorder = %i\n", bone->getZOrder());
            // dumpSprite->setZOrder(bone->getZOrder()+3);
            gameScene->addChild(dumpSprite,bone->getZOrder()+20);
            //deadSpriteArray->addObject((Ref*)dumpSprite);
            deadSpriteArray.pushBack(dumpSprite);
            
            
            bodies.push_back(body_);
        }
        /*Bone *bone = armature->getBone(key->getCString());
         printf("boneX = %f\n", bone->getPositionX());
         printf("boneY = %f\n", bone->getPositionY());*/
    }
    
}

void BazookaMan::setB2bodyPartPosition()
{
    const Map<std::string, Bone*>& dic = armature->getBoneDic();
    // CCArray *dickeyarr = dic->allKeys();
    
    for(auto& element : dic)
    {
        //Ref *o = dickeyarr->objectAtIndex(i);
        // CCString *key = dynamic_cast<CCString*>(o);
        Skin *skin = (Skin*)element.second->getDisplayRenderNode();
        string name = element.first;
        
        if (skin != NULL) {
            if (skin->isphysicsObject) {
                if (1) {
                    
                    b2Body *body = skin->body;
                    body->SetActive(true);
                    Point partpos = skin->getParentRelatePosition();
                    float partrotation = skin->getWorldRotation();
                    //printf("%f\n", partrotation);
                    body->SetTransform(b2Vec2(partpos.x/PTM_RATIO, partpos.y/PTM_RATIO), CC_DEGREES_TO_RADIANS(-partrotation));
                }
                
            }
        }
        
    }
    
}

void BazookaMan::createRocketShell()
{
    rocketShellSprite = Sprite::create("bazooka_shell.png");
    rocketShellSprite->setScale(0.4);
    rocketShellSprite->setAnchorPoint(Point(0.5,0.5));
    rocketShellSprite->setPosition(Point(armature->getPositionX()-armature->getScale()*armature->getContentSize().width/2-rocketShellSprite->getBoundingBox().size.width/2-5,
                                        armature->getPositionY()));
    gameScene->addChild(rocketShellSprite,30);
    //create rocket shell body
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(rocketShellSprite->getPositionX()/PTM_RATIO, rocketShellSprite->getPositionY()/PTM_RATIO);
    rocketShellBody_ = gameWorld->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((0.5*rocketShellSprite->getScale()*rocketShellSprite->getContentSize().width/PTM_RATIO),
                        0.5*rocketShellSprite->getScale()*rocketShellSprite->getContentSize().height/PTM_RATIO);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.1f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = ZOMBIE;
    fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND;
    fixtureDef.fixturetype = f_bullet;
    rocketShellBody_->CreateFixture(&fixtureDef);
    rocketShellBody_->SetUserData(&rocketShellSprite);
    isFiring = true;
}

void BazookaMan::shellCollisionDetector(Bear *bear)
{
    //aabb
    //aabb detect collision with the player, if true then dead
    MyQueryCallback queryCallback;
    b2AABB aabb;
    b2Vec2 detectionVec = b2Vec2(rocketShellSprite->getPositionX()/PTM_RATIO,rocketShellSprite->getPositionY()/PTM_RATIO);
    aabb.lowerBound = detectionVec - b2Vec2((rocketShellSprite->getScale()*0.5*rocketShellSprite->getContentSize().width)/PTM_RATIO ,rocketShellSprite->getScale()*0.5*rocketShellSprite->getContentSize().height/PTM_RATIO);
    aabb.upperBound = detectionVec + b2Vec2((rocketShellSprite->getScale()*0.5*rocketShellSprite->getContentSize().width)/PTM_RATIO,rocketShellSprite->getScale()*0.5*rocketShellSprite->getContentSize().height/PTM_RATIO);
    gameWorld->QueryAABB(&queryCallback, aabb);
    for (int j = 0; j < queryCallback.foundBodies.size(); j++) {
        b2Body* body = queryCallback.foundBodies[j];
        b2Fixture* f = body->GetFixtureList();
        if (f) {
            FixtureType t = f->GetFixtureType();
            
            //if collision with player without dashing, deal damage to the player, if collision with player with dashing, bouncing back
            if (t == f_bear_body&&isBouncingBack==false) {
                // if not in dash mode
                if (bear->isDashing()==false) {
                    bear->gettingHurt();
                    explod();
                    
                }
                // is dashing
                else{
                    shellVelo = 150;
                    isBouncingBack = true;
                    rocketShellSprite->setScaleX(-1*rocketShellSprite->getScale());
                    //rocketShellBody_->SetLinearVelocity(b2Vec2(shellVelo/PTM_RATIO, 0));
                }
            }
            if((t==f_zbody_body||t==f_bodydead||t==f_foot)&&isBouncingBack==true)
            {
                explod();
                Enemy *e = (Enemy*)body->GetUserData();
                if (e->chType!=drillMan) {
                    float XdistanceDiff = body->GetWorldCenter().x-detectionVec.x;
                    float randSeed = rand()%100;
                    float randForce = randSeed/50.0+2.8;
                    float yForce = 1.0+fabs(XdistanceDiff)/8.5*1.2;
                    e->die(b2Vec2(2*randForce, 5*yForce));
                }
            }
        }
    }
    
}

void BazookaMan::explod()
{
    //add explosion effect

    explo = ParticleSun::createWithTotalParticles(100);
    explo->retain();
    explo->setStartSize(100.0f);
    explo->setRadialAccel(150.0f);
    explo->setSpeed(0.5f);
    explo->setAnchorPoint(Point(0.5f,0.5f));
    explo->setPositionType(kCCPositionTypeGrouped);
    explo->setPosition(rocketShellSprite->getPosition());
    explo->setDuration(0.4f);
    gameScene->addChild(explo, 20);
    rocketShellSprite->setVisible(false);
    isExploding = true;

}