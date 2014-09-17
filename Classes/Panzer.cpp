//
//  Panzer.cpp
//  Animal_Squad
//
//  Created by wd on 9/15/14.
//
//

#include "Panzer.h"

Panzer* Panzer::create(const char *name, Point pos, float scalex, float scaley)
{
    Panzer *z = new Panzer();
    if (z&&z->init(name, pos, scalex, scaley)) {
        return z;
    }
    return NULL;
}

Panzer::~Panzer()
{
    //DO NOTHING
}

void Panzer::setArmatureBody()
{
    //do nothing
}

void Panzer::creatfootBody()
{
    b2Vec2 pos = b2Vec2(armature->getPositionX()/PTM_RATIO, armature->getPositionY()/PTM_RATIO);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.fixedRotation = true;
    bodyDef.userData = this;
    
    footBody = gameWorld->CreateBody(&bodyDef);
    
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((armature->getContentSize().width*0.5/PTM_RATIO), armature->getContentSize().height*0.5/PTM_RATIO);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.2;
    fixtureDef.friction = 0.5f;
    fixtureDef.fixturetype = f_enemy_foot;
    fixtureDef.filter.categoryBits = BULLET;
    fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | PLAYER;
    fixtureDef.shape = &dynamicBox;
    footBody->CreateFixture(&fixtureDef);
    
    b2PolygonShape polyShape;
    fixtureDef.filter.maskBits = 0;
    polyShape.SetAsBox(0.1, 1.7, b2Vec2(0, 1.7), 0);
    
    
    fixtureDef.shape = &polyShape;
    footBody->CreateFixture(&fixtureDef);
    
    
    footRect = Rect(footBody->GetPosition().x*PTM_RATIO-0.45*PTM_RATIO/2, footBody->GetPosition().y-0.45*PTM_RATIO/2, 0.45*PTM_RATIO, 0.45*PTM_RATIO);
    
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
    armature->getAnimation()->playWithIndex(0);

}

void Panzer::update(float dt)
{
    Enemy::update(dt);

    armature->setPosition(Point(footBody->GetPosition().x*PTM_RATIO, footBody->GetPosition().y*PTM_RATIO));
    b2Vec2 vel =footBody->GetLinearVelocity();
    float velChange = -(vel.x+4);
    float impulse = velChange*footBody->GetMass()/1.1;
    footBody->ApplyLinearImpulse(b2Vec2(impulse/2, 0), footBody->GetWorldCenter(), true);
    float angle = footBody->GetAngle()/M_PI*180;
    while (angle <= 0){
        angle += 360;
    }
    while (angle > 360){
        angle -= 360;
    }
    armature->setRotation(angle);
}

void Panzer::collisionProcess(Monster *monster)
{
    if (monster->isDashing()==true) {
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
    if(monster->inSprint==true){
        footBody->ApplyLinearImpulse(b2Vec2(30,100), footBody->GetWorldCenter(), true);
    }
}