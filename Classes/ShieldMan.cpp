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
                fixtureDef.fixturetype = f_zbody_head;
                fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | ARROW | BULLET;
            }
            else if (boneName.compare("bodybone") == 0||boneName.compare("body")==0) {
                fixtureDef.fixturetype = f_zbody_body;
                fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | ARROW | BULLET;
            }
            else if (boneName.compare("leglbone") == 0 || boneName.compare("right_leg") == 0|| boneName.compare("right_foreleg")|| boneName.compare("foreleg_rbone") ||boneName.compare("left_leg")==0||boneName.compare("left_foreleg")==0) {
                fixtureDef.fixturetype = f_zbody_leg;
                fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | ARROW | BULLET;
            }
            // add shield into b2body
            else if(boneName.compare("shield")==0){
                fixtureDef.fixturetype = f_spikes;
                fixtureDef.filter.maskBits = BASE_GROUND | UPPER_GROUND | ARROW | BULLET;
            }
            
            else {
                fixtureDef.fixturetype = f_zbody_void;
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

void ShieldMan::setB2bodyPartPosition()
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
                if (name.compare("head")==0 || name.compare("body")==0 || name.compare("left_leg")==0 || name.compare("left_foreleg")==0 || name.compare("right_leg")==0 || name.compare("right_foreleg")==0) {
                    
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

void ShieldMan::update(float dt,Bear *bear)
{
    
    if (dead) {
        Sprite* s = deadSpriteArray.at(0);
        position = s->getPosition();
    }
    else {
        position = armature->getPosition();
    }
    
    
    if (!dead) {
        
        
        
        updateArmatureBody();
        
        //zombie movement
        if (footBody != NULL)
        {
            
            float offsetX = HEIGHTDIFFX;
            if (waypointOnRight) {
                offsetX = -HEIGHTDIFFX;
            }
            
            armature->setPosition(Point(footBody->GetPosition().x*
                                        PTM_RATIO+offsetX, footBody->GetPosition().y*PTM_RATIO+HEIGHTDIFFY));
            
            //detect shield spikes and the player
            MyQueryCallback queryCallback; //see "World querying topic"
            b2AABB aabb;
            //b2Vec2 explosionCenterVec = b2Vec2(explo->posX/PTM_RATIO, explo->posY/PTM_RATIO);
            Bone* shieldBone = armature->getBone("shield");
            Mat4 shieldPosMat = shieldBone->_getNodeToParentTransform();
            Point shieldPos = Point(shieldPosMat.m[12], shieldPosMat.m[13]);
            b2Vec2 detectionVec = b2Vec2(shieldPos.x/PTM_RATIO, shieldPos.y/PTM_RATIO);
            aabb.lowerBound = detectionVec - b2Vec2( shieldBone->getContentSize().width/2, shieldBone->getContentSize().height/2);
            aabb.upperBound = detectionVec + b2Vec2( shieldBone->getContentSize().width/2, shieldBone->getContentSize().height/2);

            gameWorld->QueryAABB( &queryCallback, aabb );
            
            for (int j = 0; j < queryCallback.foundBodies.size(); j++) {
                b2Body* body = queryCallback.foundBodies[j];
                b2Fixture* f = body->GetFixtureList();
                if (f) {
                    FixtureType t = f->GetFixtureType();
                    
                    //if it's the player, player get hurt
                    if (t == f_bear_body) {
                        printf("Bear hit the spikes!\n");
                    }
                }
            }
            
            
            
        }

    }
}
