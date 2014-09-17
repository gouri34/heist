//
//  ShieldEnemy.cpp
//  Animal_Squad
//
//  Created by wd on 9/17/14.
//
//

#include "ShieldEnemy.h"

ShieldEnemy* ShieldEnemy::create(const char*name, Point pos, float scalex, float scaley)
{
    ShieldEnemy *z = new ShieldEnemy();
    if(z && z->init(name, pos, scalex, scaley)){
        return z;
    }
    return NULL;
}

ShieldEnemy::~ShieldEnemy()
{
    //do nothing
}

void ShieldEnemy::setArmatureBody()
{
    
    //----
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set((armature->getPositionX()-(armature->getScale()*armature->getContentSize().width*0.5))/PTM_RATIO,armature->getPositionY()/PTM_RATIO);  //set to the front of enemy
    
    shield = gameWorld->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((armature->getScale()*armature->getContentSize().width*0.1/PTM_RATIO), armature->getScale()*armature->getContentSize().height*0.3/PTM_RATIO);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 10.0;
    fixtureDef.friction = 0.3f;
    //BULLET INDICATES IT IS NOT DASHABLE
    fixtureDef.filter.categoryBits = BULLET;
    fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | PLAYER;
    fixtureDef.fixturetype = f_enemy_body;
    shield->CreateFixture(&fixtureDef);

    //----
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
                bodyDef.type = b2_staticBody;
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
            
            
            fixtureDef.density = 0.3f;
            fixtureDef.filter.categoryBits = ZOMBIE;
            fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND;
            fixtureDef.fixturetype = f_enemy_body;
            
            
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

void ShieldEnemy::update(float dt)
{
    //Enemy::update(dt);
    if (dead) {
        Sprite* s = deadSpriteArray.at(0);
        position = s->getPosition();
    }
    else {
        position = armature->getPosition();
    }
    
    if(!dead&&footBody)
    {
        updateArmatureBody();
        armature->setPosition(Point(footBody->GetPosition().x*
                                    PTM_RATIO, footBody->GetPosition().y*PTM_RATIO+10));

        
        if(isPlayingAnimation==false&&(armature->getPositionX()<=a->GetInstance()->monster->theBody
                                       ->getPositionX()+Director::getInstance()->getVisibleSize().width*0.6))
        {
            armature->getAnimation()->playWithIndex(0);
            isPlayingAnimation = true;
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

    if ((armature->getPositionX()<a->GetInstance()->monster->theBody->getPositionX())&&shield!=NULL&&shieldIsDestoryed==false) {
        shield->SetActive(false);
        gameWorld->DestroyBody(shield);
        shield=NULL;
        shieldIsDestoryed=true;
    }
}

void ShieldEnemy::collisionProcess(Monster *monster)
{
    if(shieldIsDestoryed==true){
        if(monster->isDashing()==false)
        {
            float randSeed = rand()%100;
            float randForce = 1.0+randSeed/100.0;
            float yForce = 2.4+randSeed/100.0;
            if (shield!=NULL) {
                shield->SetActive(false);
            }
            die(b2Vec2(randForce, yForce));
        }
        else
        {
            //is dashing
            float randSeed = rand()%100;
            float randForce = 1.0+randSeed/100.0;
            float yForce = 2.4+randSeed/100.0;
            if (shield!=NULL) {
                shield->SetActive(false);
            }
            die(b2Vec2(3*randForce, 2*yForce));
        }
    }
    else{
        spikeProcess(monster);
    }
}

void ShieldEnemy::spikeProcess(Monster *monster)
{
    if(monster->isDashing()==false)
    {
        //monster get hurt
        printf("Monster get hurt!\n");
    }
    else
    {
        shield->SetActive(false);
        gameWorld->DestroyBody(shield);
        shield=NULL;
        shieldIsDestoryed=true;
    }
}