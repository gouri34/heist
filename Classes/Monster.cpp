//
//  archer.cpp
//  fusion
//
//  Created by cong ku on 11/19/13.
//
//

#include "Monster.h"

#include "Enemy.h"
#include "CommonObject.h"

#include "UniversalAttributes.h"

#define CHARGETIME 1.8
#define CONTROLLENGHT 200
#define LENGTH_POWER_RATIO 5.4
#define MINIMUMPOWER 0

#define  HEIGHTDIFFX_ 0
#define  HEIGHTDIFFY_ 30.0

#define TWEEN_EASING_MAX 10000

/////////
#define bScale 0.45
//#define bScale 0.11


Monster* Monster::create(Layer *gameScene_, b2World *gameWorld_, Point pos)
{
    Monster *a = new Monster();
    if (a&&a->init(gameScene_,gameWorld_, pos)) {
        return a;
    }
    return NULL;
}

bool Monster::init(Layer *gameScene_, b2World *gameWorld_, Point pos)
{
    chType = player;
    
    gameScene = gameScene_;
    gameWorld = gameWorld_;
    position = pos;
    
    life = 100.0;
    dead = false;
    actionStatus = jump;
    inJump = false;
    inDoubleJump = false;
    dashCountDown = 1.0;
    targetSpeed = 18.0;
    prevSpeed = targetSpeed;
    sq = NULL;
    
    //load the archer sprite below.
    theBody = Armature::create("FlammerMonster");
    theBody->setPosition(Point(pos.x, pos.y));
    theBody->setAnchorPoint(Point(0.5,0.5));
    theBody->setScaleX(bScale);
    theBody->setScaleY(bScale);
    gameScene->addChild(theBody, 22);
    
    theBody->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Monster::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    theBody->getAnimation()->playWithIndex(0);
    
    setArmatureBody();
    
    return true;
}

void Monster::creatfootBody()
{
    b2Vec2 pos = b2Vec2(theBody->getPositionX()/PTM_RATIO, theBody->getPositionY()/PTM_RATIO);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.fixedRotation = true;
    bodyDef.userData = this;
    
    footBody = gameWorld->CreateBody(&bodyDef);
    
    b2CircleShape circleShape;
    circleShape.m_radius = 0.75f;

    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.6;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0;
    fixtureDef.fixturetype = f_monster_foot;
    fixtureDef.filter.categoryBits = PLAYER;
    fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | BULLET;
    fixtureDef.shape = &circleShape;
    footBody->CreateFixture(&fixtureDef);
    
    
    footRect = Rect(footBody->GetPosition().x*PTM_RATIO-0.4*PTM_RATIO/2, footBody->GetPosition().y-0.4*PTM_RATIO/2, 0.4*PTM_RATIO, 0.4*PTM_RATIO);
}

void Monster::setArmatureBody()
{
    Vector<Node*> bonearr = theBody->getChildren();
    //Skin *dump = (Skin*)((Bone*)(bonearr.at(2)))->getDisplayRenderNode();
    //batch = SpriteBatchNode::createWithTexture(dump->getTexture());
    //gameScene->addChild(batch, armature->getZOrder());
    
    
    
    // int z = bonearr->count();
    for(int i = 0; i< bonearr.size();i++)
    {
        Bone *bone = (Bone*)bonearr.at(i);
        string boneName = bone->getName();
        Skin *skin = (Skin*)bone->getDisplayRenderNode();
        
        //printf("%s\n", boneName.c_str());
        
        if (skin !=NULL) {
            skin->isphysicsObject = true;
            skin->parentScale = theBody->getScale();
            Rect a = skin->getTextureRect();
            
            Point partpos = skin->getWorldPosition();
            float partrotation = skin->getWorldRotation();
            float bodyrotation = partrotation*M_PI/180.0;
            Size partSize = Size((a.getMaxX()-a.getMinX())/PTM_RATIO*theBody->getScale(), (a.getMaxY()-a.getMinY())/PTM_RATIO*theBody->getScale());
            
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
            else {
                fixtureDef.shape = &dynamicBox;
                
            }
            fixtureDef.density = 0.2f;
            fixtureDef.friction = 0.3f;
            fixtureDef.filter.categoryBits = ZOMBIE;
            
            // printf("bonename = %s\n", boneName.c_str());
            
            fixtureDef.fixturetype = f_monster_body;
            fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND;
            
            
            
            
            body_->CreateFixture(&fixtureDef);
            body_->SetUserData(this);
            body_->SetTransform(b2Vec2(partpos.x/PTM_RATIO, partpos.y/PTM_RATIO), bodyrotation);
            body_->SetAngularDamping(1.2);
            
            skin->body = body_;
            
            
            PhysicsSprite *dumpSprite = (PhysicsSprite*)PhysicsSprite::createWithTexture(skin->getTexture(), skin->getTextureRect());
            dumpSprite->setPosition(skin->getWorldPosition());
            dumpSprite->setScale(theBody->getScale());
            dumpSprite->setVisible(false);
            dumpSprite->body = body_;
            dumpSprite->setZOrder(bone->getZOrder());
            gameScene->addChild(dumpSprite);
            deadSpriteArray.pushBack(dumpSprite);
            
            
            bodies.push_back(body_);
        }
        /*Bone *bone = armature->getBone(key->getCString());
         printf("boneX = %f\n", bone->getPositionX());
         printf("boneY = %f\n", bone->getPositionY());*/
    }

}

Monster::~Monster()
{
    if (footBody != NULL) {
        gameWorld->DestroyBody(footBody);
        footBody = NULL;
    }
    
    gameScene = NULL;
    gameWorld = NULL;
}



/////////////////////MOVEMENT  RELATED///////////////////////////////////////////////////////////////////////////////////////

void Monster::verticalAdjust()
{
    b2Vec2 vel = footBody->GetLinearVelocity();
    float Yimpulse = -vel.y*footBody->GetMass()/1.0;
    footBody->ApplyLinearImpulse(b2Vec2(0, Yimpulse), footBody->GetWorldCenter(), true);
}

void Monster::stop() {
    
    /*if (!dead) {
        float targetSpeed = 0;
        
        b2Vec2 vel = footBody->GetLinearVelocity();
        float XvelChange = targetSpeed - vel.x;
        float Ximpulse = XvelChange*footBody->GetMass()/3.0;
        footBody->ApplyLinearImpulse(b2Vec2(Ximpulse, 0), footBody->GetWorldCenter(), true);

    }
    */
    
}



ActionStatus Monster::getActionStatus()
{
    return actionStatus;
}

void Monster::changeActionStatus(ActionStatus status)
{
    actionStatus = status;
    if (status != dash) {
        theBody->setColor(Color3B(255, 255, 255));
    }
    else if (status == dash) {
        theBody->setColor(Color3B(255, 180,180));
    }
}



void Monster::action()
{
    if (actionStatus == jump) {
        if (inJump==true&&inDoubleJump==false) {
            footBody->ApplyLinearImpulse(b2Vec2(0, 10*1.5), footBody->GetWorldCenter(), true);
            theBody->getAnimation()->playWithIndex(1);
            inDoubleJump = true;
            jumpTimer = 0;
        }
        if (jumpTimer > 0.8) {
            footBody->ApplyLinearImpulse(b2Vec2(0, 10*3), footBody->GetWorldCenter(), true);
            theBody->getAnimation()->playWithIndex(2);
            inJump = true;
            jumpTimer = 0;

        }
    }
    else if (actionStatus == dash&&UniversalAttributes::GetInstance()->pt->getPercentage()==100){
        if (!inDash) {
            inDash = true;
            dashTimer = 0.24;
            prevSpeed = targetSpeed;
            targetSpeed = targetSpeed+40;
            theBody->getAnimation()->playWithIndex(3);
            
            UniversalAttributes::GetInstance()->pt->setPercentage(0);
            barFull=false;
        }
    }
   
}

void Monster::dashProgressBarHandler()
{
        ProgressFromTo *to2 = ProgressFromTo::create(dashCountDown, UniversalAttributes::GetInstance()->pt->getPercentage(), 100);
        UniversalAttributes::GetInstance()->pt->runAction(to2);
}


//////////////////////////////ATTACK RELATED///////////////////////////
void Monster::HeavyAttack()
{
    if (inAttack == false){
        
        theBody->getAnimation()->playByIndex(1);
        inAttack = true;
        attackTimer = 0.65;
    }
}


void Monster::HeavyAttackEffect()
{
    MyQueryCallback queryCallback; //see "World querying topic"
    b2AABB aabb;
    //b2Vec2 explosionCenterVec = b2Vec2(explo->posX/PTM_RATIO, explo->posY/PTM_RATIO);
    Bone* attackBone = theBody->getBone("Layer43");
    Mat4 attackPosMat = attackBone->_getNodeToParentTransform();
    Point attackPos = Point(attackPosMat.m[12], attackPosMat.m[13]);
    b2Vec2 detectionVec = b2Vec2(attackPos.x/PTM_RATIO, attackPos.y/PTM_RATIO);
    aabb.lowerBound = detectionVec - b2Vec2( 2.5, 2.5);
    aabb.upperBound = detectionVec + b2Vec2( 2.5, 2.5);
    gameWorld->QueryAABB( &queryCallback, aabb );
    
    for (int j = 0; j < queryCallback.foundBodies.size(); j++) {
        b2Body* body = queryCallback.foundBodies[j];
        b2Fixture* f = body->GetFixtureList();
        if (f) {
            FixtureType t = f->GetFixtureType();
            
            //if it's a zombie, explosion metters
            if (t == f_foot) {
                Enemy* e = (Enemy*)body->GetUserData();
                
                float XdistanceDiff = body->GetWorldCenter().x-detectionVec.x;
                float randSeed = rand()%100;
                float randForce = randSeed/100.0;
                float yForce = 3.4+fabs(XdistanceDiff)/2.5*1.2;
                if (XdistanceDiff<0) {
                    randForce = -randForce;
                }
                e->die(b2Vec2(randForce, yForce));
            }
        }
    }
    
    //map shake effect

}


void Monster::die()
{
    if (footBody != NULL) {
        gameWorld->DestroyBody(footBody);
        footBody = NULL;
    }
    
    setB2bodyPosition();
    
    if (!dead) {
        for (auto o : deadSpriteArray) {
            if (theBody->getScaleX() < 0) {
                o->setScaleX(-o->getScaleX());
            }
        }
        
        
        theBody->setVisible(false);
        
        
        b2Filter filter;
        filter.categoryBits = DEAD_ZOMBIE;
        filter.maskBits = UPPER_GROUND | BASE_GROUND;
        
        const Map<std::string, Bone*>& dic = theBody->getBoneDic();
        for(auto& element : dic)
        {
            
            Bone *bone = element.second;
            Skin *skin = (Skin*)element.second->getDisplayRenderNode();
            string name = element.first;
            
            if (skin) {
                b2Body *body = skin->body;
                
                body->GetFixtureList()->SetFixtureType(f_bodydead);
                body->GetFixtureList()->SetFilterData(filter);
                body->SetType(b2_dynamicBody);
                
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



void Monster::update(float dt)
{
    //movement
    if (!dead) {
        
        
        setB2bodyPartPosition();
        
        //verticalAdjust();
        if (footBody != NULL) {
            float offsetX = HEIGHTDIFFX_;
            if (theBody->getScaleX()<0) {
                offsetX = -HEIGHTDIFFX_;
            }
            
            //move bear
            theBody->setScaleX(bScale);
            b2Vec2 vel = footBody->GetLinearVelocity();
            float velChange = targetSpeed - vel.x;
            float impulse = velChange*footBody->GetMass()/1.1;
            footBody->ApplyLinearImpulse(b2Vec2(impulse, 0), footBody->GetWorldCenter(), true);
            
            theBody->setPosition(Point(footBody->GetPosition().x*PTM_RATIO+offsetX, footBody->GetPosition().y*PTM_RATIO+HEIGHTDIFFY_));
        }
        
        footRect = Rect(footBody->GetPosition().x*PTM_RATIO-0.4*PTM_RATIO/2, footBody->GetPosition().y*PTM_RATIO-0.4*PTM_RATIO/2, 0.4*PTM_RATIO, 0.4*PTM_RATIO);
        
        if (inAttack) {
            if (attackTimer >0) {
                attackTimer -= dt;
                if (attackTimer <= 0) {
                    HeavyAttackEffect();
                }
            }
        }

        //
        jumpTimer += dt;
        
        //detect jump status
        if (inJump==true&&jumpTimer>0.5) {
            onGroundDetector();
        }
        
        //dash update
        if (inDash&&!inSprint) {
            dashTimer -= dt;
            if (dashTimer<=0) {
                inDash = false;
                targetSpeed = prevSpeed;
                theBody->getAnimation()->playWithIndex(0);
                dashProgressBarHandler();
            }
        }
        
        if (barFull==false) {
            if (UniversalAttributes::GetInstance()->pt->getPercentage()==100) {
                Sprite *tmp = (Sprite*)UniversalAttributes::GetInstance()->pt->getParent();
                //set action
                if (sq!=NULL) {
                    tmp->stopAllActions();
                    sq = NULL;
                }
                ScaleTo *sb1 = ScaleTo::create(0.1, 0.35);
                ScaleTo *sb2 = ScaleTo::create(0.1, 0.2);
                sq = Sequence::create(sb1,sb2,sb1,sb2 ,NULL);
                tmp->runAction(sq);
                barFull=true;
            }
        }

        //item update
        itemUpdate(dt);
        //health update
        if (Unhurtable==true) {
            hurtTimer -= dt;
            if (hurtTimer<=0) {
                Unhurtable = false;
            }
        }
        
        //collision update
        collisionDetector();
    }
    else {
        setBodySprites();
    }
    
}


void Monster::collisionDetector()
{
    // crash the enemies
    MyQueryCallback queryCallback; //see "World querying topic"
    b2AABB aabb;
    //b2Vec2 explosionCenterVec = b2Vec2(explo->posX/PTM_RATIO, explo->posY/PTM_RATIO);
    Point attackPos = Point(theBody->getPositionX(), theBody->getPositionY());
    b2Vec2 detectionVec = b2Vec2(attackPos.x/PTM_RATIO, attackPos.y/PTM_RATIO);
    aabb.lowerBound = detectionVec - b2Vec2( 2.5, 1.0);
    aabb.upperBound = detectionVec + b2Vec2( 2.5, 1.0);
    gameWorld->QueryAABB( &queryCallback, aabb );
    
    for (int j = 0; j < queryCallback.foundBodies.size(); j++) {
        b2Body* body = queryCallback.foundBodies[j];
        b2Fixture* f = body->GetFixtureList();
        if (f) {
            FixtureType t = f->GetFixtureType();
            
            if (t == f_enemy_foot) {
                Enemy* e = (Enemy*)body->GetUserData();
                //e->die(b2Vec2(2.0, 2.0));
                e->collisionProcess(this);
            }
            else if(t == f_commonObj) {
                CommonObject *o = (CommonObject*)body->GetUserData();
                o->collisionProcess(this);
            }
            else{
                //shit
            }
        }
    }
}

void Monster::animationEvent(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
    if (movementType == LOOP_COMPLETE)
    {
        if (movementID == "walk_Copy1")
        {
            theBody->getAnimation()->playByIndex(0);
            inAttack = false;
        }
    }
}

///////////////update ragdoll///////////////
void Monster::setB2bodyPartPosition()
{
    Map<std::string, Bone*> dic = theBody->getBoneDic();
    //CCArray *dickeyarr = dic->allKeys();
    
    for (auto o : dic) {
        //Ref *o = dickeyarr->objectAtIndex(i);
        //CCString *key = dynamic_cast<CCString*>(o);
        //CCSkin *skin = (CCSkin*)armature->getBone(key->getCString())->getDisplayRenderNode();
        Bone* bone = o.second;
        Skin* skin = (Skin*)bone->getDisplayRenderNode();
        string key = o.first;
        
        if (skin != NULL) {
            if (skin->isphysicsObject&&(bone->getName()!="Gshockwave")) {
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


float Monster::setBodySprites()
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

void Monster::onGroundDetector()
{
    MyQueryCallback queryCallback; //see "World querying topic"
    b2AABB aabb;
    //b2Vec2 explosionCenterVec = b2Vec2(explo->posX/PTM_RATIO, explo->posY/PTM_RATIO);

    b2Vec2 detectionVec = footBody->GetPosition();
    aabb.lowerBound = detectionVec - b2Vec2(0.75, 0.75);
    aabb.upperBound = detectionVec + b2Vec2( 0.75, 0.75);
    gameWorld->QueryAABB( &queryCallback, aabb );
    
    for (int j = 0; j < queryCallback.foundBodies.size(); j++) {
        b2Body* body = queryCallback.foundBodies[j];
        b2Fixture* f = body->GetFixtureList();
        if (f) {
            FixtureType t = f->GetFixtureType();
            
            //if collision with ground, switch back to running animation
            if (t == f_ground) {
                theBody->getAnimation()->playWithIndex(0);
                inJump = false;
                inDoubleJump = false;
            }

        }
    }
}

bool Monster::isDashing()
{
    return inDash;
}

//-------item related functions-------

void Monster::goSprint(float timer)
{
    if (inSprint==false) {
        inSprint=true;
        inDash=true;
        targetSpeed = targetSpeed+45;
        sprintTimer = timer;
        theBody->getAnimation()->setSpeedScale(4.0);
        pe = ParticleMeteor::create();
        pe->setGravity(Point(-200,0));
        pe->setStartRadius(550);
        pe->setTexture(Director::getInstance()->getTextureCache()->addImage("meteor.png"));
        pe->setPosition(theBody->getPosition());
        gameScene->addChild(pe,30);
    }
}

void Monster::itemUpdate(float dt)
{
    if (inSprint) {
        sprintTimer -= dt;
        if (sprintTimer<=0) {
            inSprint=false;
            inDash=false;
            targetSpeed=prevSpeed;
            theBody->getAnimation()->setSpeedScale(1.0);
            gameScene->removeChild(pe);
        }
        pe->setPosition(theBody->getPosition());

    }
}

//------------hurt related function (health)---------
void Monster::getHurt(){
    if (Unhurtable==false&&hurtTimer<=0) {
        hurtTimer = 1.0;
        Unhurtable = true;
        //theBody->setColor(Color3B(255, 120,120));
        Blink *blink = Blink::create(0.5, 6);
        theBody->runAction(blink);
        if(UniversalAttributes::GetInstance()->healthCount>=1)
            UniversalAttributes::GetInstance()->healthCount--;

    }
}
