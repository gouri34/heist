//
//  glassWindow.cpp
//  Animal_Squad
//
//  Created by cong ku on 14-7-4.
//
//

#include "CommonObject.h"
#include "MapGenerator.h"

CommonObject* CommonObject::create(std::string fileName,cocos2d::Point pos, float scalex, float scaley)
{
    CommonObject*a = new CommonObject();
    if (a&&a->init(fileName,pos, scalex, scaley)) {
        return a;
    }
    return NULL;
}

bool CommonObject::init(std::string fileName,cocos2d::Point pos, float scalex, float scaley)
{
    gameScene = MapGenerator::GetInstance()->gameLayer;
    gameWorld = MapGenerator::GetInstance()->gameWorld;
    iscontacting = false;
    //load the armature for reference first
    theBody = Armature::create(fileName.c_str());
    theBody->setPosition(Point(pos.x, pos.y));
    theBody->setVisible(true);
    // theBody->setAnchorPoint(Point(0.5,0));
    theBody->setScaleX(scalex);
    theBody->setScaleY(scaley);
    gameScene->addChild(theBody, 12);
    
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
            dumpSprite->setZOrder(bone->getZOrder()+theBody->getZOrder());
            gameScene->addChild(dumpSprite);
            
            //create b2body
            Point partpos = skin->getParentRelatePosition();
            float partrotation = skin->getWorldRotation();
            float bodyrotation = partrotation*M_PI/180.0;
            Size partSize = Size((a.getMaxX()-a.getMinX())/PTM_RATIO*theBody->getScale(), (a.getMaxY()-a.getMinY())/PTM_RATIO*theBody->getScale());
            
            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            
            bodyDef.position.Set(partpos.x/PTM_RATIO, partpos.y/PTM_RATIO);
            b2Body *body_ = gameWorld->CreateBody(&bodyDef);
            
            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(partSize.width/2.0, partSize.height/2.0);//These are mid points for our 1m box
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.restitution = 0.66;
            fixtureDef.density = 2.4f;
            fixtureDef.friction = 0.3f;
            fixtureDef.filter.categoryBits = ZOMBIE;
            
            // printf("bonename = %s\n", boneName.c_str());
            
            fixtureDef.fixturetype = f_glassblock;
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
    
    //link joints
    const Map<std::string, Bone*>& dic = theBody->getBoneDic();
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
            
            Bone *parentBone = bone->getParentBone();
            if (parentBone && (rand()%4 >= 1)) {
                Skin *parentSkin = (Skin*)parentBone->getDisplayRenderNode();
                b2Body *parentBody = parentSkin->body;
                b2WeldJointDef jdef;
                //CCAffineTransform tran = bone->nodeToParentTransform();
                Mat4 tran = bone->_getNodeToParentTransform();
                Point p = Point(tran.m[12], tran.m[13]);
                // Point p = skin->getWorldPosition();
                //printf("p x = %f y = %f\n", p.x, p.y);
                jdef.Initialize(parentBody, body, b2Vec2(p.x/PTM_RATIO, p.y/PTM_RATIO));
                gameWorld->CreateJoint(&jdef);
            }
            
            
            
        }
    }
    
    //create the static body
    wallRect = theBody->getBoundingBox();
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    
    bodyDef.position.Set((wallRect.origin.x+wallRect.size.width/2)/PTM_RATIO, (wallRect.origin.y+wallRect.size.height/2)/PTM_RATIO);
    staticWall = gameWorld->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(wallRect.size.width/2.0/PTM_RATIO, wallRect.size.height/2.0/PTM_RATIO);//These are mid points for our 1m box
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.2f;
    fixtureDef.friction = 0.3f;
    fixtureDef.fixturetype = f_commonObj;
    staticWall->CreateFixture(&fixtureDef);
    staticWall->SetUserData(this);
    
    //remove the armature after used
    gameScene->removeChild(theBody, true);
    
    return true;
}

void CommonObject::collisionProcess(Monster* monster)
{
    destroy();

}


void CommonObject::destroy()
{
    //destroy the static wall first
    gameWorld->DestroyBody(staticWall);
    
    //set the wall blocks;
    for (int i = 0; i < wallBlocks.size(); i++) {
        b2Body *b = wallBlocks.at(i);
        b->SetType(b2_dynamicBody);
        
        float mass = b->GetMass();
        float xforce = mass*(6+(rand()%6)/2.0);
        float yforce = mass*(4.5+(rand()%4)/2.0);
        float torque = mass*(2+(rand()%2)/2.0);
        
        if (rand()%2 == 0) {
            torque = -torque;
        }
        
        b->ApplyLinearImpulse(b2Vec2(3*xforce, 4*yforce), b->GetWorldCenter(), true);
        b->ApplyAngularImpulse(torque, true);
    }
}

void CommonObject::update(cocos2d::Point pos, float dt)
{
    //-------------------
    
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

CommonObject::~CommonObject()
{
}
