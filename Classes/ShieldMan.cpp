//
//  ShieldMan.cpp
//  Animal_Squad
//
//  Created by wd on 8/20/14.
//
//

#include "ShieldMan.h"

ShieldMan * ShieldMan::create(Scene *parentScene, b2World *world,const char*name, Point pos, float scale)
{
    ShieldMan *z = new ShieldMan();
    if (z && z->init(parentScene, world, name, pos, scale)) {
        return z;
    }
    
    return NULL;
}

bool ShieldMan::init(cocos2d::Scene *parentScene, b2World *world, const char *name, Point pos, float scale)
{
    isPlayingAnimation = false;
    
    chType = shieldMan;
    
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
    //
    parentScene->addChild(armature,21);
    ShieldMan::setArmatureBody();
    
    Enemy::creatfootBody();
    
    armature->setScaleX(scale);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set((pos.x-(armature->getScale()*armature->getContentSize().width*0.5))/PTM_RATIO,pos.y/PTM_RATIO);
    
    shield = gameWorld->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((armature->getScale()*armature->getContentSize().width*0.1/PTM_RATIO), armature->getScale()*armature->getContentSize().height*0.5/PTM_RATIO);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 10.0;
    fixtureDef.friction = 0.3f;
    //BULLET INDICATES IT IS NOT DASHABLE
    fixtureDef.filter.categoryBits = BULLET;
    fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | PLAYER;
    fixtureDef.fixturetype = f_stiff_object;
    shield->CreateFixture(&fixtureDef);
    //shield->SetUserData(&armature);

    
    return true;
}

ShieldMan::~ShieldMan()
{
    // do nothing
}

void ShieldMan::setArmatureBody()
{
    
    Vector<Node*> bonearr = armature->getChildren();

    for(int i = 0; i< bonearr.size();i++)
    {
        Bone *bone = (Bone*)bonearr.at(i);
        string boneName = bone->getName();
        Skin *skin = (Skin*)bone->getDisplayRenderNode();
        
        
        if (skin !=NULL)
        {
            skin->isphysicsObject = true;
            skin->parentScale = armature->getScale();
            Rect a = skin->getTextureRect();
            
            Point partpos = skin->getWorldPosition();
            float partrotation = skin->getWorldRotation();
            float bodyrotation = partrotation*M_PI/180.0;
            Size partSize = Size((a.getMaxX()-a.getMinX())/PTM_RATIO*armature->getScale(), (a.getMaxY()-a.getMinY())/PTM_RATIO*armature->getScale());
            
            b2BodyDef bodyDef;
            if (boneName.compare("dun")==0) {
                bodyDef.type = b2_dynamicBody;
            }
            else
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

            fixtureDef.restitution = 0.8;
            fixtureDef.friction = 0.01f;
            //fixtureDef.filter.categoryBits = ZOMBIE;
            
            // printf("bonename = %s\n", boneName.c_str());
            
            if(boneName.compare("dun")==0){
                fixtureDef.fixturetype = f_spikes;
                fixtureDef.filter.categoryBits = BULLET;
                fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | PLAYER;
                fixtureDef.density = 10.3f;

            }
            
            else {
            fixtureDef.density = 0.3f;
            fixtureDef.filter.categoryBits = ZOMBIE;
            fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND;
            fixtureDef.fixturetype = f_zbody_body;
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


void ShieldMan::update(float dt,Bear *bear)
{
    Enemy::update(dt, bear);
    if (footBody != NULL)
    {
        armature->setPosition(Point(footBody->GetPosition().x*
                                    PTM_RATIO, footBody->GetPosition().y*PTM_RATIO+20));
        
        if (isPlayingAnimation == false&&(armature->getPositionX()<=bear->theBody->getPositionX()+Director::getInstance()->getVisibleSize().width*0.6)) {
            armature->getAnimation()->playWithIndex(0);
            isPlayingAnimation = true;
        }
        
        //detect shield spikes and the player
        MyQueryCallback queryCallback; //see "World querying topic"
        b2AABB aabb;
        //b2Vec2 explosionCenterVec = b2Vec2(explo->posX/PTM_RATIO, explo->posY/PTM_RATIO);
//        Bone* shieldBone = armature->getBone("dun");
//        Mat4 shieldPosMat = shieldBone->_getNodeToParentTransform();
//        Point shieldPos = Point(shieldPosMat.m[12], shieldPosMat.m[13]);
        b2Vec2 detectionVec = shield->GetPosition();
        aabb.lowerBound = detectionVec - b2Vec2(2,2);
        aabb.upperBound = detectionVec + b2Vec2(2,2);

        gameWorld->QueryAABB( &queryCallback, aabb );
        
        for (int j = 0; j < queryCallback.foundBodies.size(); j++) {
            b2Body* body = queryCallback.foundBodies[j];
            b2Fixture* f = body->GetFixtureList();
            if (f&&(armature->getPositionX()<=bear->theBody->getPositionX()+Director::getInstance()->getVisibleSize().width*0.6)) {
                FixtureType t = f->GetFixtureType();
                
                //if it's the player, player get hurt
                if (t == f_bear_body) {
                    if (bear->isDashing()==false) {
                        printf("Bear hit the spikes!\n");
                        bear->gettingHurt();
                    }
                    else if(bear->isDashing()==true&&dead==false){
                        //gameWorld->DestroyBody(shield);
                        shield->SetActive(false);
                        float XdistanceDiff = body->GetWorldCenter().x-detectionVec.x;
                        float randSeed = rand()%100;
                        float randForce = randSeed/50.0+2.8;
                        float yForce = 1.0+fabs(XdistanceDiff)/8.5*1.2;
                        Enemy::die(b2Vec2(5*randForce, yForce));
                    }
                    
                }
            }
        }
    }

}
