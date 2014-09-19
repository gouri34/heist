//
//  BazookaEnemy.cpp
//  Animal_Squad
//
//  Created by wd on 9/15/14.
//
//

#include "BazookaEnemy.h"

BazookaEnemy* BazookaEnemy::create(const char *name, Point pos, float scalex, float scaley)
{
    BazookaEnemy *z = new BazookaEnemy();
    if(z&&z->init(name, pos, scalex, scaley))
    {
        return z;
    }
    return NULL;
}

BazookaEnemy::~BazookaEnemy()
{
    //do nothing
}

void BazookaEnemy::setArmatureBody()
{
    //add rocket shell into the armature
    
    
    Vector<Node*> bonearr = armature->getChildren();
    
    
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
            fixtureDef.density = 0.3f;
            fixtureDef.restitution = 0.8;
            fixtureDef.friction = 0.01f;
            fixtureDef.filter.categoryBits = ZOMBIE;
            fixtureDef.fixturetype = f_enemy_body;
            
            // printf("bonename = %s\n", boneName.c_str());
            
            if (boneName.compare("headbone") == 0||boneName.compare("head")==0) {
                fixtureDef.fixturetype = f_enemy_body;
                fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | ARROW | BULLET;
            }
            else if (boneName.compare("bodybone") == 0||boneName.compare("body")==0) {
                fixtureDef.fixturetype = f_enemy_body;
                fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | ARROW | BULLET;
            }
            else if (boneName.compare("leglbone") == 0 || boneName.compare("right_leg") == 0|| boneName.compare("right_foreleg")|| boneName.compare("foreleg_rbone") ||boneName.compare("left_leg")==0||boneName.compare("left_foreleg")==0) {
                fixtureDef.fixturetype = f_enemy_body;
                fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | ARROW;
            }
            else {
                fixtureDef.fixturetype = f_enemy_body;
                fixtureDef.filter.categoryBits = PLAYER;
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

void BazookaEnemy::createRocketShell()
{
    rocketShellSprite = Sprite::create("bazooka_shell.png");
    rocketShellSprite->setScale(0.3);
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
    fixtureDef.fixturetype = f_missle;
    rocketShellBody_->CreateFixture(&fixtureDef);
    rocketShellBody_->SetUserData(&rocketShellSprite);
    isFiring = true;
    
    //add rocket flame effect
    pf = ParticleMeteor::create();
    pf->setStartRadius(25);
    pf->setEndRadius(10);
    pf->setStartSize(20);
    pf->setEndSize(7);
    pf->setPosition(Point(rocketShellSprite->getPositionX()+(rocketShellSprite->getScale()*rocketShellSprite->getContentSize().width/2),rocketShellSprite->getPositionY()));
    pf->setGravity(Point(4,0));
    pf->setDuration(0.2);
    pf->setPositionType(kCCPositionTypeGrouped);
    pf->setAutoRemoveOnFinish(true);
    gameScene->addChild(pf,30);
    
}

void BazookaEnemy::shellCollisionDetector()
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
        if (f&&shellDeleted==false) {
            FixtureType t = f->GetFixtureType();
            
            //if collision with player without dashing, deal damage to the player, if collision with player with dashing, bouncing back
            if (t == f_monster_body&&isBouncingBack==false) {
                // if not in dash mode
                if (a->GetInstance()->monster->isDashing()==false) {
                    //monster get hurt
                    explod();
                    gameWorld->DestroyBody(rocketShellBody_);
                    rocketShellBody_ = NULL;
                    shellDeleted = true;
                    a->GetInstance()->monster->getHurt();
                    
                }
                // is dashing
                else{
                    shellVelo = 150;
                    isBouncingBack = true;
                    rocketShellSprite->setScaleX(-1*rocketShellSprite->getScale());
                    //rocketShellBody_->SetLinearVelocity(b2Vec2(shellVelo/PTM_RATIO, 0));
                }
            }
            if((t==f_enemy_body)&&isBouncingBack==true)
            {
                if(isExploding==false)
                    explod();
                Enemy *e = (Enemy*)body->GetUserData();
                float XdistanceDiff = body->GetWorldCenter().x-detectionVec.x;
                float randSeed = rand()%100;
                float randForce = randSeed/50.0+2.8;
                float yForce = 1.0+fabs(XdistanceDiff)/8.5*1.2;
                e->die(b2Vec2(2*randForce, 5*yForce));
                rocketShellSprite->setVisible(false);
                rocketShellBody_->SetActive(false);
                gameWorld->DestroyBody(rocketShellBody_);
                rocketShellBody_ = NULL;
                shellDeleted = true;
            }
        }
    }
    
}

void BazookaEnemy::explod()
{
    //add explosion effect
    explo = ParticleSun::createWithTotalParticles(100);
    explo->setAutoRemoveOnFinish(true);
    explo->setStartSize(100.0f);
    explo->setRadialAccel(150.0f);
    explo->setSpeed(0.8f);
    explo->setAnchorPoint(Point(0.5f,0.5f));
    explo->setPositionType(kCCPositionTypeGrouped);
    explo->setPosition(rocketShellSprite->getPosition());
    explo->setDuration(0.2f);
    gameScene->addChild(explo, 20);
    rocketShellSprite->setVisible(false);
    rocketShellBody_->SetActive(false);
    isExploding = true;
    
}

void BazookaEnemy::update(float dt)
{
    if (dead) {
        Sprite* s = deadSpriteArray.at(0);
        position = s->getPosition();
    }
    else {
        position = armature->getPosition();
    }
    

    //if close enough, fire rocket shell
    if(!dead&&footBody)
    {
        updateArmatureBody();
        armature->setPosition(Point(footBody->GetPosition().x*
                                    PTM_RATIO, footBody->GetPosition().y*PTM_RATIO+10));

        if(isFiring==false&&isPlayingAnimation==false&&(armature->getPositionX()<=a->GetInstance()->monster->theBody->getPositionX()+Director::getInstance()->getVisibleSize().width*0.6))
        {
            armature->getAnimation()->playWithIndex(0);
            isPlayingAnimation = true;
            createRocketShell();
        }
        if(isFiring==true&&isExploding==false&&shellDeleted==false)
        {
            rocketShellBody_->SetLinearVelocity(b2Vec2(shellVelo,0));
            
            pf->setPosition(Point(rocketShellSprite->getPositionX()+(rocketShellSprite->getScale()*rocketShellSprite->getContentSize().width/2),rocketShellSprite->getPositionY()));

            rocketShellSprite->setPosition(rocketShellBody_->GetPosition().x*PTM_RATIO,rocketShellBody_->GetPosition().y*PTM_RATIO);
            shellCollisionDetector();
            if ((rocketShellSprite->getPositionX()>a->GetInstance()->monster->theBody->getPositionX()+Director::getInstance()->getVisibleSize().width
                ||a->GetInstance()->monster->theBody->getPositionX()-rocketShellSprite->getPositionX()>300)&&shellDeleted==false) {
                isExploding = true;
                rocketShellSprite->setVisible(false);
                gameWorld->DestroyBody(rocketShellBody_);
                rocketShellBody_ = NULL;
                shellDeleted = true;
            }
        }
    }
    else {
        
        if (!afterDeath) {
            
            setBodySprites();
            
            /* if (readyDeleteBody) {
             afterDeath = true;
             }
             
             if (maxVelocity < 0.5) {
             readyDeleteBody = true;
             bodies.clear();
             }*/
            
        }
        
    }
}

void BazookaEnemy::collisionProcess(Monster *monster)
{
    Enemy::collisionProcess(monster);
    if(monster->isDashing()==false)
    {
        float randSeed = rand()%100;
        float randForce = 1.0+randSeed/100.0;
        float yForce = 2.4+randSeed/100.0;
        //rocketShellBody_->SetActive(false);
        rocketShellSprite->setVisible(false);
        die(b2Vec2(randForce, yForce));
    }
    else
    {
        //is dashing
        float randSeed = rand()%100;
        float randForce = 1.0+randSeed/100.0;
        float yForce = 2.4+randSeed/100.0;
        //rocketShellBody_->SetActive(false);
        rocketShellSprite->setVisible(false);
        die(b2Vec2(3*randForce, 2*yForce));
    }
}


