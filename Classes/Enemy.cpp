//
//  zombie.cpp
//  fusion
//
//  Created by cong ku on 11/15/13.
//
//


#include "Enemy.h"

Enemy* Enemy::create(Scene *parentScene, b2World *world,const char*name, Point pos, float scale)
{
    Enemy *z = new Enemy();
    if (z && z->init(parentScene, world, name, pos, scale)) {
        return z;
    }
    
    return NULL;
}

bool Enemy::init(Scene *parentScene, b2World *world,const char*name, Point pos, float scale)
{
    startAnimationTimer = (rand()%100)/100.0*1.0;
    
    chType = normalEnemy;
    
    gameScene = parentScene;
    gameWorld = world;
    armature = NULL;
    footBody = NULL;
    dead = false;
    life = 100;
    desireVel = -1.2;
    
   
    
    armature = Armature::create(name);
    armature->setPosition(pos);
    armature->setScale(scale);
    armature->setAnchorPoint(Point(0.5, 0.5));
    zombie_scale = scale;
   // armature->setColor(Color3B(255, 150, 150));
    
    parentScene->addChild(armature,21);
    setArmatureBody();
    
    creatfootBody();
    
    armature->setScaleX(-scale);

    
    return true;
}


void Enemy::initWayPoints(vector<waypoint> wps)
{
    haveWayPoint = true;
    
    waypointIndex = firstPos;
    wpArray = wps;
    
    waypoint fwp = wps[waypointIndex];
    waypointOnRight = false;
    desireVel = -defaultSpeed;
    armature->setScaleX(-zombie_scale);
    if (fwp.waypointPos.x > armature->getPosition().x) {
        waypointOnRight = true;
        desireVel = defaultSpeed;
        armature->setScaleX(zombie_scale);
    }
}


Enemy::~Enemy()
{
    //free(waypointArray);
    
    deleteProperties();
    //gameScene->removeChild(batch, true);
    gameScene->removeChild(armature, true);
    
    if (footBody != NULL) {
        gameWorld->DestroyBody(footBody);
        footBody = NULL;
    }
    
    //gameScene = NULL;
    //gameWorld = NULL;
    
   // delete deadSpriteArray;
}


void Enemy::die(b2Vec2 vec)
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


void Enemy::update(float dt,Bear *bear)
{
    
    if (dead) {
        Sprite* s = deadSpriteArray.at(0);
        position = s->getPosition();
    }
    else {
        position = armature->getPosition();
    }
    
    
    if (!dead) {
        
        if (!playedStartUpAnimation) {
            if (startAnimationTimer>0) {
                startAnimationTimer-=dt;
            }
            else {

                //armature->getAnimation()->playByIndex(1);
                playedStartUpAnimation = true;
            }
        }
        
        
        updateArmatureBody();
        
        //zombie movement
        if (footBody != NULL)
        {
            if (haveWayPoint)
            {
                if (waypointOnRight) {
                    if (armature->getPosition().x >= wpArray[waypointIndex].waypointPos.x) {
                        reachWaypoint = true;
                    }
                }
                else {
                    if (armature->getPosition().x <= wpArray[waypointIndex].waypointPos.x) {
                        reachWaypoint = true;
                    }
                }
                
                if (reachWaypoint) {
                    
                    if (!walkingBack) {
                        waypointIndex++;
                        
                    }
                    else
                    {
                        waypointIndex--;
                    }
                    
                    if (waypointIndex == 0) {
                        walkingBack = false;
                    }
                    if (waypointIndex == wpArray.size()-1) {
                        walkingBack = true;
                    }
                    
                    if (waypointIndex < wpArray.size()) {
                        waypoint wp = wpArray[waypointIndex];
                        waypointOnRight = false;
                        desireVel = -defaultSpeed;
                        armature->setScaleX(-zombie_scale);
                        if (wp.waypointPos.x > armature->getPosition().x) {
                            waypointOnRight = true;
                            desireVel = defaultSpeed;
                            armature->setScaleX(zombie_scale);
                            
                            //armature->
                        }
                        
                        if (wp.waypointPos.y -  footBody->GetPosition().y*PTM_RATIO > 20) {
                            want_stair_down = false;
                        }
                        else if (wp.waypointPos.y - footBody->GetPosition().y*PTM_RATIO < -20)
                        {
                            want_stair_down = true;
                        }
                        
                        
                        
                        reachWaypoint = false;
                        
                    }
                    else {
                        
                    }
                    
                }
                
                b2Vec2 vel = footBody->GetLinearVelocity();
                float velChange = desireVel - vel.x;
                float impulse = velChange*footBody->GetMass()/1.1;
                footBody->ApplyLinearImpulse(b2Vec2(impulse, 0), footBody->GetWorldCenter(), true);
                footRect = Rect(footBody->GetPosition().x*PTM_RATIO, footBody->GetPosition().y*PTM_RATIO, 0.45*PTM_RATIO, 0.45*PTM_RATIO);

            }
            
            
            float offsetX = HEIGHTDIFFX;
            if (waypointOnRight) {
                offsetX = -HEIGHTDIFFX;
            }
            
            armature->setPosition(Point(footBody->GetPosition().x*
                                      PTM_RATIO+offsetX, footBody->GetPosition().y*PTM_RATIO+HEIGHTDIFFY));

            
        }
    }
    else {
        
        if (!afterDeath) {
            
            float maxVelocity = setBodySprites();
            
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



void Enemy::deleteProperties()
{
    for (auto o : deadSpriteArray) {
        if (o->body) {
            b2Body *b = o->body;
           // gameWorld->DestroyJoint(o->body->GetJointList()->joint);
            /*b2Joint *j = (b2Joint*)(o->body->GetJointList());
            if (j) {
                gameWorld->DestroyJoint(j);
            }*/
            gameWorld->DestroyBody(b);
            b = NULL;
            gameScene->removeChild(o);
        }
        
        //o->release();
    }
    
    deadSpriteArray.clear();
   // batch->removeAllChildrenWithCleanup(true);

}



//-----------------------------------------------------------

void Enemy::setArmatureBody()
{
    
    Vector<Node*> bonearr = armature->getChildren();
    //Skin *dump = (Skin*)((Bone*)(bonearr.at(2)))->getDisplayRenderNode();
   // batch = SpriteBatchNode::createWithTexture(dump->getTexture());
    //gameScene->addChild(batch, armature->getZOrder());
 
    
   // int z = bonearr->count();
    for (int i = 0; i < bonearr.size(); i++)
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
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 0.2f;
            fixtureDef.friction = 0.9f;
            fixtureDef.filter.categoryBits = ZOMBIE;
            fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | ARROW | BULLET;
            fixtureDef.fixturetype = f_zbody_body;
            
            //printf("bonename = %s\n", boneName.c_str());

            
            body_->CreateFixture(&fixtureDef);
            body_->SetUserData(this);
            body_->SetTransform(b2Vec2(partpos.x/PTM_RATIO, partpos.y/PTM_RATIO), bodyrotation);
            body_->SetAngularDamping(1.2);
            
            skin->body = body_;
            
            PhysicsSprite *dumpSprite = (PhysicsSprite*)PhysicsSprite::createWithTexture(skin->getTexture(), skin->getTextureRect());
            dumpSprite->setPosition(skin->getWorldPosition());
            dumpSprite->setAnchorPoint(Point(0.5, 0.5));
            dumpSprite->setScale(armature->getScale());
            dumpSprite->setVisible(false);
            dumpSprite->body = body_;
            //dumpSprite->setZOrder(bone->getZOrder()+5);
            gameScene->addChild(dumpSprite,bone->getZOrder()+20);
            deadSpriteArray.pushBack(dumpSprite);

            bodies.push_back(body_);
        }
        
    }

}


void Enemy::updateArmatureBody()
{
    for (int i = 0; i < bodies.size(); i++) {
        b2Body *b = bodies[i];
        b->SetActive(false);
    }
    
    MyQueryCallback queryCallback; //see "World querying topic"
    b2AABB aabb;
    //b2Vec2 explosionCenterVec = b2Vec2(explo->posX/PTM_RATIO, explo->posY/PTM_RATIO);
    b2Vec2 detectionVec = b2Vec2(armature->getPositionX()/PTM_RATIO, armature->getPositionY()/PTM_RATIO);
    //Point detectionCenter = ccp(armature->getPositionX(), armature->getPositionY());
    aabb.lowerBound = detectionVec - b2Vec2( 5, 5);
    aabb.upperBound = detectionVec + b2Vec2( 5, 5);
    gameWorld->QueryAABB( &queryCallback, aabb );
    
    for (int j = 0; j < queryCallback.foundBodies.size(); j++) {
        b2Body* body = queryCallback.foundBodies[j];
        FixtureType t = body->GetFixtureList()->GetFixtureType();
        //if it's a zombie, explosion metters
        if (t == f_bullet) {
            setB2bodyPartPosition();
        }
    }

}




void Enemy::setB2bodyPosition()
{
    Map<std::string, Bone*> dic = armature->getBoneDic();
    
    for (auto o : dic) {
        Bone* bone = o.second;
        Skin* skin = (Skin*)bone->getDisplayRenderNode();
        
        if (skin != NULL) {
            if (skin->isphysicsObject) {
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

void Enemy::setB2bodyPartPosition()
{
    Map<std::string, Bone*> dic = armature->getBoneDic();
    //CCArray *dickeyarr = dic->allKeys();
    
    for (auto o : dic) {
        //Ref *o = dickeyarr->objectAtIndex(i);
        //CCString *key = dynamic_cast<CCString*>(o);
        //CCSkin *skin = (CCSkin*)armature->getBone(key->getCString())->getDisplayRenderNode();
        Bone* bone = o.second;
        Skin* skin = (Skin*)bone->getDisplayRenderNode();
        string key = o.first;
        
        if (skin != NULL) {
            if (skin->isphysicsObject) {
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

float Enemy::setBodySprites()
{
    float maxVelocity;
    
    for (auto o : deadSpriteArray) {
        b2Body *b = o->body;
        if (!readyDeleteBody) {
            o->setPosition(Point(b->GetPosition().x*PTM_RATIO, b->GetPosition().y*PTM_RATIO));
            
            if (o->getScaleX() >= 0) {
                o->setRotation(-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));
                
            }
            else {
                o->setRotation(180-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));
            }
            
            float cur_v = b->GetLinearVelocity().Length();
            if (fabs(cur_v) > maxVelocity) {
                maxVelocity = fabs(cur_v);
            }
            
        }
        else {
            gameWorld->DestroyBody(b);
        }
        
        
    }
    
    return maxVelocity;
}


//zombie movement
void Enemy::creatfootBody()
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
    fixtureDef.filter.categoryBits = ZOMBIE;
    fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND;
    fixtureDef.shape = &circleShape;
    footBody->CreateFixture(&fixtureDef);
    
    b2PolygonShape polyShape;
    fixtureDef.filter.maskBits = 0;
    polyShape.SetAsBox(0.1, 1.7, b2Vec2(0, 1.7), 0);
    
    
    fixtureDef.shape = &polyShape;
    footBody->CreateFixture(&fixtureDef);
    
    
    footRect = Rect(footBody->GetPosition().x*PTM_RATIO-0.45*PTM_RATIO/2, footBody->GetPosition().y-0.45*PTM_RATIO/2, 0.45*PTM_RATIO, 0.45*PTM_RATIO);
}
