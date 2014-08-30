//
//  normalZombie.cpp
//  fusion
//
//  Created by cong ku on 14-1-25.
//
//

#include "NormalEnemy.h"

/*normalZombie::normalZombie(Scene *parentScene, b2World *world,const char*name, float scale)
{
    gameScene = parentScene;
    gameWorld = world;
    armature = NULL;
    //footBody = NULL;
    deadSpriteArray = NULL;
    dead = false;
    life = 100;
    desireVel = -1.2;
    
    deadSpriteArray = CCArray::create();
    CC_SAFE_RETAIN(deadSpriteArray);
    
    armature = Armature::create(name);
    armature->setScale(scale);
    parentScene->addChild(armature,5);
    setArmatureBody();
    
}*/



NormalEnemy* NormalEnemy::create(Scene *parentScene, b2World *world,const char*name, Point pos, float scale)
{
    NormalEnemy *z = new NormalEnemy();
    if (z && z->init(parentScene, world, name, pos, scale)) {
        return z;
    }
    
    return NULL;
}

NormalEnemy::~NormalEnemy()
{

}



void NormalEnemy::setArmatureBody()
{
    //armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(NormalEnemy::animationEvent));
    
    armature->getAnimation()->playWithIndex(0);
    
    Vector<Node*> bonearr = armature->getChildren();
    printf("bonearr size = %zd\n",bonearr.size());
    //Skin *dump = (Skin*)((Bone*)(bonearr.at(2)))->getDisplayRenderNode();
    //batch = SpriteBatchNode::createWithTexture(dump->getTexture());
    //gameScene->addChild(batch, armature->getZOrder());
    

    
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


void NormalEnemy::setB2bodyPartPosition()
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


//////////////actions/////////////////////
void NormalEnemy::animationEvent(Armature *armature, MovementEventType movementType, const char *movementID)
{
  //  printf("id = %s\n", movementID);
}



void NormalEnemy::getHit()
{
    //armature->getAnimation()->playByIndex()
}


void NormalEnemy::headDropDie()
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
            //Ref *o = dickeyarr->objectAtIndex(i);
            // CCString *key = dynamic_cast<CCString*>(o);
            Bone *bone = element.second;
            Skin *skin = (Skin*)element.second->getDisplayRenderNode();
            string name = element.first;
            
            if (skin) {
                b2Body *body = skin->body;
                
                body->GetFixtureList()->SetFixtureType(f_bodydead);
                body->GetFixtureList()->SetFilterData(filter);
                body->SetType(b2_dynamicBody);
                
                Bone *parentBone = bone->getParentBone();
                string selfName = bone->getName();
                string parentName;
                if (parentBone) {
                    parentName = parentBone->getName();
                }
                

                if (parentBone && selfName.compare("headbone") != 0 && parentName.compare("headbone") != 0) {
                    
                    Skin *parentSkin = (Skin*)parentBone->getDisplayRenderNode();
                    b2Body *parentBody = parentSkin->body;
                    b2RevoluteJointDef jdef;
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
        
       // update(0);

        setBodySprites();
        dead = true;
        
        for (auto o : deadSpriteArray) {
             o->setVisible(true);
        }
        
        
    }
}

void NormalEnemy::dieToExplosion(float damage, Point exploPosition)
{
    if (footBody != NULL) {
        gameWorld->DestroyBody(footBody);
        footBody = NULL;
    }
    
    setB2bodyPosition();

    
    for (auto o : deadSpriteArray)
    {
        if (armature->getScaleX() < 0) {
            o->setScaleX(-o->getScaleX());
        }
    }

    
    if (!dead) {
        armature->setVisible(false);
        
        b2Filter filter;
        filter.categoryBits = DEAD_ZOMBIE;
        filter.maskBits = UPPER_GROUND | BASE_GROUND;
        
        const Map<std::string, Bone*>& dic = armature->getBoneDic();
        for(auto& element : dic)
        {
            //Bone *bone = element.second;
            Skin *skin = (Skin*)element.second->getDisplayRenderNode();
            string name = element.first;

            
            if (skin) {
                b2Body *body = skin->body;
                body->GetFixtureList()->SetFixtureType(f_bodydead);
                body->GetFixtureList()->SetFilterData(filter);
                body->SetType(b2_dynamicBody);
                b2Vec2 selfPosVec = body->GetWorldCenter();
                Point selfPos = Point(selfPosVec.x*PTM_RATIO, selfPosVec.y*PTM_RATIO);
                float angle = (selfPos-exploPosition).getAngle();
                float massFactor = body->GetMass()/0.37;
                b2Vec2 impulse = b2Vec2(cos(angle)*damage*0.02*massFactor , sin(angle)*damage*0.044*massFactor);
                body->ApplyLinearImpulse(impulse, selfPosVec, true);
                float torqueRand = (float)(rand()%100-50)/50*20;
                float torque = (torqueRand+damage)*0.36*massFactor;
                body->ApplyTorque(torque, true);
            }
        }
        
        //update(0);

        
        dead = true;
        
        for (auto o : deadSpriteArray)
        {
            o->setVisible(true);

        }
        
        
    }
}

// normal enemy is the one that only towards you but cannot hurt you or what so ever
void NormalEnemy::update(float dt, Bear *bear)
{
    Enemy::update(dt, bear);
    if (footBody != NULL)
    {
        desireVel = -3.2;
        b2Vec2 vel = Enemy::footBody->GetLinearVelocity();
        float velChange = desireVel - vel.x;
        float impulse = velChange*Enemy::footBody->GetMass()/1.1;
        Enemy::footBody->ApplyLinearImpulse(b2Vec2(impulse, 0), Enemy::footBody->GetWorldCenter(), true);
        Enemy::footRect = Rect(Enemy::footBody->GetPosition().x*PTM_RATIO, Enemy::footBody->GetPosition().y*PTM_RATIO, 0.45*PTM_RATIO, 0.45*PTM_RATIO);
        
        //---
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
                if (t == f_bear_body) {
                    // if not in dash mode
                    if (bear->isDashing()==false) {
                       float XdistanceDiff = body->GetWorldCenter().x-detectionVec.x;
                        float randSeed = rand()%100;
                        float randForce = randSeed/50.0+2.8;
                        float yForce = 1.0+fabs(XdistanceDiff)/8.5*1.2;
                        Enemy::die(b2Vec2(0.7*randForce, yForce));
                    }
                    // is dashing
                    else{
                        float XdistanceDiff = body->GetWorldCenter().x-detectionVec.x;
                        float randSeed = rand()%100;
                        float randForce = randSeed/50.0+2.8;
                        float yForce = 1.0+fabs(XdistanceDiff)/8.5*1.2;
                        Enemy::die(b2Vec2(3*randForce, yForce));
                    }

                }
            }
        }
    }
}




