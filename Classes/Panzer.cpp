//
//  Panzer.cpp
//  Animal_Squad
//
//  Created by wd on 8/23/14.
//
//

#include "Panzer.h"

Panzer* Panzer::create(Layer *gameScene_, b2World *gameWorld_,Point pos,string armatureName,float scale,float density)
{
    Panzer *z = new Panzer();
    if (z&&z->init(gameScene_,gameWorld_,pos,armatureName,scale,density)) {
        return z;
    }
    return NULL;
}

bool Panzer::init(Layer *gameScene_, b2World *gameWorld_,Point pos,string armatureName,float scale,float density)
{
    gameScene = gameScene_;
    gameWorld = gameWorld_;
    
    //load the armature for the reference first
    active = false;
    armature = Armature::create(armatureName);
    armature->setPosition(pos);
    armature->setScale(scale);
    armature->setVisible(true);
    armature->setAnchorPoint(Point(0.5,0.5));
    gameScene->addChild(armature,12);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x/PTM_RATIO,pos.y/PTM_RATIO);
    //beastTrap->setPosition(Point(body_->GetPosition().x*PTM_RATIO,body_->GetPosition().y*PTM_RATIO));
    
    body_ = gameWorld->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((scale*armature->getContentSize().width*0.5/PTM_RATIO), scale*armature->getContentSize().height*0.5/PTM_RATIO);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = density;
    fixtureDef.friction = 0.3f;
    //BULLET INDICATES IT IS NOT DASHABLE
    fixtureDef.filter.categoryBits = BULLET;
    fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | PLAYER;
    fixtureDef.fixturetype = f_stiff_object;
    body_->CreateFixture(&fixtureDef);
    body_->SetUserData(&armature);
    armature->getAnimation()->playWithIndex(0);
    
    //hide the damaged panzer
    Vector<Node*> bonearr = armature->getChildren();
    for (int i=0; i<bonearr.size(); i++) {
        Bone *bone = (Bone*)bonearr.at(i);
        std::string boneName = bone->getName();
        if (strstr(boneName.c_str(), "damaged_panzer")) {
            Skin *skin = (Skin*)bone->getDisplayRenderNode();
            if (skin!=NULL) {
                skin->setVisible(false);
            }
        }
    }
    
    return true;
}


void Panzer::update(float dt, Bear* bear)
{
    EnemyObject::update(dt, bear);
    //if (activeBeastTrap) {
    armature->setPosition(Point(body_->GetPosition().x*PTM_RATIO,body_->GetPosition().y*PTM_RATIO));
    float angle = body_->GetAngle()/M_PI*180;
    while (angle <= 0){
        angle += 360;
    }
    while (angle > 360){
        angle -= 360;
    }
    armature->setRotation(angle);
    
    // give linear impulse
    b2Vec2 vel =body_->GetLinearVelocity();
    float velChange = -(vel.x+4);
    float impulse = velChange*body_->GetMass()/1.1;
    body_->ApplyLinearImpulse(b2Vec2(impulse/2, 0), body_->GetWorldCenter(), true);
    
    //aabb checking for collison with the player
    MyQueryCallback queryCallback;
    b2AABB aabb;
    b2Vec2 detectionVec = b2Vec2(body_->GetPosition().x, body_->GetPosition().y);
    aabb.lowerBound = detectionVec - b2Vec2(0.5*armature->getContentSize().width*armature->getScale()/PTM_RATIO ,0.5*armature->getContentSize().height*armature->getScale()/PTM_RATIO);
    aabb.upperBound = detectionVec + b2Vec2(0.5*armature->getContentSize().width*armature->getScale()/PTM_RATIO,0.5*armature->getContentSize().height*armature->getScale()/PTM_RATIO);
    gameWorld->QueryAABB(&queryCallback, aabb);
    for (int j = 0; j<queryCallback.foundBodies.size(); j++) {
        b2Body *body = queryCallback.foundBodies[j];
        b2Fixture *f = body->GetFixtureList();
        if (f) {
            FixtureType t = f->GetFixtureType();
            //if collape with player, will hide the panzer bone and show damaged_panzer bone
            if (t==f_bear_body&&bear->isDashing()==true) {
                Vector<Node*> bonearr = armature->getChildren();
                for (int i=0; i<bonearr.size(); i++) {
                    Bone *bone = (Bone*)bonearr.at(i);
                    std::string boneName = bone->getName();
                    if (strstr(boneName.c_str(), "damaged_panzer")) {
                        Skin *skin = (Skin*)bone->getDisplayRenderNode();
                        if (skin!=NULL) {
                            skin->setVisible(true);
                        }
                    }
                    else if(strstr(boneName.c_str(), "panzer")){
                        Skin *skin = (Skin*)bone->getDisplayRenderNode();
                        if(skin!=NULL){
                            skin->setVisible(false);
                        }
                    }
                }
            }
        }
    }

}

Panzer::~Panzer()
{
    gameScene->removeChild(armature, true);
    gameWorld->DestroyBody(body_);
}
