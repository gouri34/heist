//
//  LandMine.cpp
//  Animal_Squad
//
//  Created by wd on 9/2/14.
//
//

#include "LandMine.h"

LandMine* LandMine::create(cocos2d::Layer *gameScene_, b2World *gameWorld_, Point pos, string armatureName, float scale, float density)
{
    LandMine *s = new LandMine();
    if (s&&s->init(gameScene_, gameWorld_, pos, armatureName, scale, density)) {
        return s;
    }
    return NULL;
}

void LandMine::update(float dt, Bear* bear)
{
    // if monster is closing, run animation
    if ((armature->getPositionX()<=bear->theBody->getPositionX()+Director::getInstance()->getVisibleSize().width*0.7)&&activated==false) {
        armature->getAnimation()->playWithIndex(0);
        activated=true;
    }
    
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
    
    if (activated==true) {
        if (armature->getAnimation()->isComplete()==true) {
            //explod with fire
            enemyObjectAction();
        }
    }

}

void LandMine::enemyObjectAction()
{
    //set armatuer invisible
    armature->setVisible(false);
    
    //add explosion effect
//    ParticleExplosion *pe = ParticleExplosion::createWithTotalParticles(200);
//    ParticleSystemQuad *p = ParticleSystemQuad::create(<#const std::string &filename#>)
    //pe->settext
}

LandMine::~LandMine()
{
    gameScene->removeChild(armature, true);
    gameWorld->DestroyBody(body_);
}
