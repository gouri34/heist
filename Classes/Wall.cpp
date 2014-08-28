//
//  Wall.cpp
//  Bear
//
//  Created by cong ku on 14-6-27.
//
//

#include "Wall.h"

Wall* Wall::create(Layer *gameScene_, b2World *gameWorld_, Point pos)
{
    Wall*a = new Wall();
    if (a&&a->init(gameScene_,gameWorld_, pos)) {
        return a;
    }
    return NULL;
}

bool Wall::init(cocos2d::Layer *gameScene_, b2World *gameWorld_, cocos2d::Point pos)
{
    gameScene = gameScene_;
    gameWorld = gameWorld_;
    
    //load the armature for reference first
    Armature* theBody = Armature::create("b_wall");
    theBody->setPosition(Point(pos.x, pos.y-9));
    theBody->setVisible(true);
    theBody->setAnchorPoint(Point(0.5,0));
    theBody->setScale(2.0);
    gameScene_->addChild(theBody, 12);
    
    //use the armature put blocks at right positions
    Vector<Node*> bonearr = theBody->getChildren();
    for(int i = 0; i< bonearr.size();i++)
    {
        Bone *bone = (Bone*)bonearr.at(i);
        std::string boneName = bone->getName();
        Skin *skin = (Skin*)bone->getDisplayRenderNode();
        
        //printf("%s\n", boneName.c_str());
        
        if (skin !=NULL) {
            skin->isphysicsObject = true;
            skin->parentScale = theBody->getScale();
            Rect a = skin->getTextureRect();
            
            //create sprite
            Sprite *dumpSprite = (Sprite*)Sprite::createWithTexture(skin->getTexture(), skin->getTextureRect());
            dumpSprite->setPosition(skin->getParentRelatePosition());
            dumpSprite->setScale(theBody->getScale());
            dumpSprite->setVisible(true);
            dumpSprite->setZOrder(bone->getZOrder());
            gameScene_->addChild(dumpSprite);

            //create b2body
            Point partpos = skin->getParentRelatePosition();
            float partrotation = skin->getWorldRotation();
            float bodyrotation = partrotation*M_PI/180.0;
            Size partSize = Size((a.getMaxX()-a.getMinX())/PTM_RATIO*theBody->getScale(), (a.getMaxY()-a.getMinY())/PTM_RATIO*theBody->getScale());
            
            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            
            bodyDef.position.Set(partpos.x/PTM_RATIO, partpos.y/PTM_RATIO);
            b2Body *body_ = gameWorld_->CreateBody(&bodyDef);
            
            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(partSize.width/2.0, partSize.height/2.0);//These are mid points for our 1m box
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 2.4f;
            fixtureDef.friction = 0.3f;
            fixtureDef.filter.categoryBits = ZOMBIE;
            
            // printf("bonename = %s\n", boneName.c_str());
            
            fixtureDef.fixturetype = f_wallblock;
            fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND;
            
            
            body_->CreateFixture(&fixtureDef);
            body_->SetUserData(dumpSprite);
            body_->SetTransform(b2Vec2(partpos.x/PTM_RATIO, partpos.y/PTM_RATIO), bodyrotation);
            body_->SetAngularDamping(2.0);
            
            skin->body = body_;
            
            
            wallBlocks.push_back(body_);
        }
        /*Bone *bone = armature->getBone(key->getCString());
         printf("boneX = %f\n", bone->getPositionX());
         printf("boneY = %f\n", bone->getPositionY());*/
    }
    
    //create the static body
    Rect wallRect = theBody->getBoundingBox();
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    
    bodyDef.position.Set((wallRect.origin.x+wallRect.size.width/2)/PTM_RATIO, (wallRect.origin.y+wallRect.size.height/2)/PTM_RATIO);
    staticWall = gameWorld_->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(wallRect.size.width/2.0/PTM_RATIO, wallRect.size.height/2.0/PTM_RATIO);//These are mid points for our 1m box

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.2f;
    fixtureDef.friction = 0.3f;
    fixtureDef.fixturetype = f_wall;
    staticWall->CreateFixture(&fixtureDef);
    staticWall->SetUserData(this);

    //remove the armature after used
    gameScene->removeChild(theBody, true);
    
    return true;
}

void Wall::destroyWall()
{
    //destroy the static wall first
    gameWorld->DestroyBody(staticWall);
    
    //set the wall blocks;
    for (int i = 0; i < wallBlocks.size(); i++) {
        b2Body *b = wallBlocks.at(i);
        b->SetType(b2_dynamicBody);
        
        float xforce = (0.018+(float)(rand()%100)/6500.0)*1400;
        float yforce = ((50.0 - rand()%100)/3000.0)*1400;
        float torque = ((float)(50.0-rand()%100)/320000.0)*16000;
        
        b->ApplyLinearImpulse(b2Vec2(xforce*3, yforce*3), b->GetWorldCenter(), true);
        b->ApplyAngularImpulse(torque*3, true);
    }
    
    auto e1 = Shaky3D::create(0.4, cocos2d::Size(5, 5), 5, 0);
    a->GetInstance()->menuGrid->runAction(e1);

}

void Wall::update(cocos2d::Point pos, float dt)
{
    
    std::vector<b2Body*>usedbody;
    
    for (int i = 0; i < wallBlocks.size(); i++) {
        b2Body *b = wallBlocks.at(i);
        Sprite *s = (Sprite*)b->GetUserData();
        
        if ((pos.x - s->getPositionX())>300) {
            usedbody.push_back(b);
        }
    }
    
    for (int i = 0; i < usedbody.size(); i++) {
        b2Body *b = usedbody.at(i);
        Sprite *s = (Sprite*)b->GetUserData();
        gameWorld->DestroyBody(b);
        gameScene->removeChild(s, true);
        
        wallBlocks.erase(std::remove(wallBlocks.begin(), wallBlocks.end(), b), wallBlocks.end());
    }
    
    if ( wallBlocks.size() <= 0) {
        destroyed = true;
    }
}

Wall::~Wall()
{
    
}
