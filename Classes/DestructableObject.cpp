//
//  DestructableObject.cpp
//  Animal_Squad
//
//  Created by wd on 8/22/14.
//
//

#include "DestructableObject.h"

DestructableObject* DestructableObject::create(cocos2d::Layer *gameScene_, b2World *gameWorld_, Point pos, string armatureName, float scale, float density)
{
    DestructableObject *z = new DestructableObject();
    if (z&&z->init(gameScene_, gameWorld_, pos, armatureName, scale, density)) {
        return z;
    }
    return NULL;
}

bool DestructableObject::init(cocos2d::Layer *gameScene_, b2World *gameWorld_, Point pos, string armatureName, float scale, float density)
{
    gameScene = gameScene_;
    gameWorld = gameWorld_;
    density_ = density;
    //load the armature
    armature = Armature::create(armatureName);
    armature->setPosition(pos);
    armature->setScale(scale);
    armature->setVisible(true);
    armature->setAnchorPoint(Point(0.5,0.5));
    gameScene->addChild(armature,12);
    
    //create body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x/PTM_RATIO,pos.y/PTM_RATIO);
    body_ = gameWorld->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((0.5*armature->getScale()*armature->getContentSize().width/PTM_RATIO), 0.5*armature->getScale()*armature->getContentSize().height/PTM_RATIO);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = density;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = ZOMBIE;
    fixtureDef.filter.maskBits =  BASE_GROUND | UPPER_GROUND | PLAYER;
    fixtureDef.fixturetype = f_object;
    body_->CreateFixture(&fixtureDef);
    body_->SetUserData(&armature);
    active = false;
    
    // set the damaged part invisible
    Vector<Node*> bonearr = armature->getChildren();
    for(int i = 0; i< bonearr.size();i++)
    {
        Bone *bone = (Bone*)bonearr.at(i);
        std::string boneName = bone->getName();
        if (strstr(boneName.c_str(), "damaged")) {
            Skin *skin = (Skin*)bone->getDisplayRenderNode();
            if (skin !=NULL) {
                skin->setVisible(false);
            }
        }
    }
    

    
    return true;
}

void DestructableObject::update(float dt)
{
    if (active == false)
    {
        armature->setPosition(Point(body_->GetPosition().x*PTM_RATIO,body_->GetPosition().y*PTM_RATIO));
        float angle = body_->GetAngle()/M_PI*180;
        while (angle <= 0){
            angle += 360;
        }
        while (angle > 360){
            angle -= 360;
        }
        armature->setRotation(angle);
        //if collide
        MyQueryCallback queryCallback;
        b2AABB aabb;
        b2Vec2 detectionVec = b2Vec2(body_->GetPosition().x,body_->GetPosition().y);
        aabb.lowerBound = detectionVec - b2Vec2(armature->getContentSize().width/2*armature->getScale()/PTM_RATIO,armature->getContentSize().height/2*armature->getScale()/PTM_RATIO);
        aabb.upperBound = detectionVec + b2Vec2(armature->getContentSize().width/2*armature->getScale()/PTM_RATIO,armature->getContentSize().height/2*armature->getScale()/PTM_RATIO);
        gameWorld->QueryAABB(&queryCallback, aabb);
        for (int j=0; j<queryCallback.foundBodies.size(); j++)
        {
            b2Body* body = queryCallback.foundBodies[j];
            b2Fixture *f = body->GetFixtureList();
            if (f) {
                FixtureType t = f->GetFixtureType();
                
                //if its player shatters.
                if (t==f_bear_body||t==f_bear_foot)
                {
                    Vector<Node*> bonearr = armature->getChildren();
                    
                    for(int i = 0; i< bonearr.size();i++)
                    {
                        Bone *bone = (Bone*)bonearr.at(i);
                        std::string boneName = bone->getName();
                        if (strstr(boneName.c_str(), "damaged"))
                        {
                            Skin *skin = (Skin*)bone->getDisplayRenderNode();
                            if (skin !=NULL)
                            {
                                skin->setVisible(true);
                                skin->isphysicsObject = true;
                                skin->parentScale = armature->getScale();
                                Rect a = skin->getTextureRect();
                                
                                //create sprite
                                Sprite *dumpSprite = (Sprite*)Sprite::createWithTexture(skin->getTexture(), skin->getTextureRect());
                                dumpSprite->setPosition(skin->getParentRelatePosition());
                                dumpSprite->setScale(armature->getScale());
                                dumpSprite->setVisible(true);
                                dumpSprite->setZOrder(bone->getZOrder());
                                gameScene->addChild(dumpSprite,13);
                                
                                //create b2body
                                Point partpos = skin->getParentRelatePosition();
                                float partrotation = skin->getWorldRotation();
                                float bodyrotation = partrotation*M_PI/180.0;
                                Size partSize = Size((a.getMaxX()-a.getMinX())/PTM_RATIO*armature->getScale(), (a.getMaxY()-a.getMinY())/PTM_RATIO*armature->getScale());
                                
                                b2BodyDef bodyDef;
                                bodyDef.type = b2_dynamicBody;
                                
                                bodyDef.position.Set(partpos.x/PTM_RATIO, partpos.y/PTM_RATIO);
                                b2Body *bodyfrag_ = gameWorld->CreateBody(&bodyDef);
                                
                                b2PolygonShape dynamicBox;
                                dynamicBox.SetAsBox(partSize.width/2.0, partSize.height/2.0);//These are mid points for our 1m box
                                
                                b2FixtureDef fixtureDef;
                                fixtureDef.shape = &dynamicBox;
                                fixtureDef.density = density_;
                                fixtureDef.friction = 0.3f;
                                fixtureDef.filter.categoryBits = ZOMBIE;
                                
                                // printf("bonename = %s\n", boneName.c_str());
                                
                                fixtureDef.fixturetype = f_wallblock;
                                fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND;
                                
                                
                                bodyfrag_->CreateFixture(&fixtureDef);
                                bodyfrag_->SetUserData(dumpSprite);
                                bodyfrag_->SetTransform(b2Vec2(partpos.x/PTM_RATIO, partpos.y/PTM_RATIO), bodyrotation);
                                bodyfrag_->SetAngularDamping(2.0);
                                
                                skin->body = bodyfrag_;
                                
                                
                                objectfrags.push_back(dumpSprite);
                                objectbodylist.push_back(bodyfrag_);
                                
                                active = true;
                            }
                            
                        }
                        // set invisible for these undamaged parts
                        else
                        {
                            Skin *skin = (Skin*)bone->getDisplayRenderNode();
                            if (skin !=NULL) {
                                skin->setVisible(false);
                            }
                        }
                        
                    }
                    //remove the original body and armatures
                    //gameWorld->DestroyBody(body_);
                    armature->setVisible(false);
                    
                    destoryObject();
                    active = true;
                }
            }
        }
    }
    else
    {
        armature->setPosition(Point(body_->GetPosition().x*PTM_RATIO,body_->GetPosition().y*PTM_RATIO));
        
        float angle = body_->GetAngle()/M_PI*180;
        while (angle <= 0){
            angle += 360;
        }
        while (angle > 360){
            angle -= 360;
        }
        armature->setRotation(angle);
    }
}

void DestructableObject::destoryObject()
{
    for (int i = 0; i < objectbodylist.size(); i++) {
        b2Body *b = objectbodylist.at(i);
        
        float xforce = (0.018+(float)(rand()%100)/6500.0)*1400;
        float yforce = ((50.0 - rand()%100)/3000.0)*1400;
        float torque = ((float)(50.0-rand()%100)/320000.0)*16000;
        
        b->ApplyLinearImpulse(b2Vec2(-4*xforce, 3*yforce), b->GetWorldCenter(), true);
        b->ApplyAngularImpulse(8*torque, true);
    }
}

DestructableObject::~DestructableObject()
{
    gameScene->removeChild(armature, true);
    gameWorld->DestroyBody(body_);
    for (auto o : objectbodylist) {
        gameWorld->DestroyBody(o);
    }
    for (auto o : objectfrags) {
        gameScene->removeChild(o);
    }
    objectfrags.clear();
    objectbodylist.clear();

}



