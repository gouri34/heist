//
//  ElevatorShaft.cpp
//  Animal_Squad
//
//  Created by cong ku on 8/11/14.
//
//

#include "ElevatorShaft.h"
#include "MapGenerator.h"


#define UPPER_GROUND_WIDTH 20.0
#define LOWER_GROUND_WIDTH 21.0
#define TRUE_GROUND_OFFSET 4

#define ELEVATOR_LENGTH 100
#define ELEVATOR_SPEED 12.0



ElevatorShatf* ElevatorShatf::create(Point pos, int s_heigth)
{
    ElevatorShatf *a = new ElevatorShatf();
    if (a&&a->init(pos, s_heigth)) {
        return a;
    }
    return NULL;
}

bool ElevatorShatf::init(Point pos, int s_heigth)
{
    gLayer = MapGenerator::GetInstance()->gameLayer;
    gWorld = MapGenerator::GetInstance()->gameWorld;
    
    
    startPos = pos;
    shaft_height = s_heigth;
    destinationY = startPos.y + shaft_height;
    groundYpos = (int)pos.y;
    
    cocos2d::Texture2D::TexParams params = {GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT};
    
    
    Texture2D* wallTexture = Director::getInstance()->getTextureCache()->addImage("ele_shaft_wall.png");
    wallTexture->setTexParameters(params);
    wallTextureSize = Size(wallTexture->getPixelsWide(), wallTexture->getPixelsHigh());
    
    
    Texture2D* terrainTexture = Director::getInstance()->getTextureCache()->addImage("ground_terrain.png");
    terrainTexture->setTexParameters(params);
    
    
    Vector2dVector empty;
    
    wall = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, wallTexture);
    terrain = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, terrainTexture);
    elevator = Sprite::create("elevator.png");
    elevator->setAnchorPoint(Point(0.5, 0));
    gLayer->addChild(wall, 2);
    gLayer->addChild(terrain,2);
    gLayer->addChild(elevator, 10);
    
    elevator_status = Waiting;
    
    setupElevator();
    setVertices(pos);
    
    
    return true;
}

void ElevatorShatf::setupElevator()
{
    elevator->setPosition(Point(startPos.x+ELEVATOR_LENGTH/2, startPos.y));
    
    Rect a = elevator->getTextureRect();
    
    Size partSize = Size((a.getMaxX()-a.getMinX())/PTM_RATIO, 10.0/PTM_RATIO);
    Point partpos = elevator->getPosition();
    
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.fixedRotation = true;
    
    
    bodyDef.position.Set(partpos.x/PTM_RATIO, partpos.y/PTM_RATIO);
    elevator_body = gWorld->CreateBody(&bodyDef);
    
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(partSize.width/2.0, 1.0/PTM_RATIO);//These are mid points for our 1m box
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 10.2f;
    fixtureDef.friction = 0.9f;
    
    
    elevator_body->CreateFixture(&fixtureDef);
    elevator_body->SetTransform(b2Vec2(partpos.x/PTM_RATIO, partpos.y/PTM_RATIO), 0);
    elevator_body->SetAngularDamping(1.2);
    
    
}

void ElevatorShatf::setVertices(Point pos)
{
    
    //set ground texture
    Vector2dVector groundVecs;
    Vector2dVector ceilVecs;
    
    float ex_Y_hight = pos.y+800;
    
    
    Point ground_lp;
    Point ground_rp;
    
    
    Vector2dVector points;
    Vector2dVector texCoords;
    
    if (shaft_height > 0) {
        ground_lp = Point(startPos.x, startPos.y-10);
        ground_rp = Point(startPos.x + ELEVATOR_LENGTH, startPos.y-10);
    }
    else {
        ground_lp = Point(startPos.x, startPos.y + shaft_height);
        ground_rp = Point(startPos.x + ELEVATOR_LENGTH, startPos.y + shaft_height);
    }
    
    
    
    points = pointsToVector(Point(ground_lp.x, ground_lp.y-512), ground_lp, ground_rp, Point(ground_rp.x, ground_rp.y - 512));
    terrain->setPoints(points);
    
    points = pointsToVector(Point(ground_lp.x-5, ground_lp.y), Point(ground_lp.x-5, ex_Y_hight), Point(ground_rp.x, ex_Y_hight), Point(ground_rp.x+5, ground_rp.y));
    wall->setPoints(points);
    
    
    lastPos = Point(ground_rp.x, destinationY);
    
}

void ElevatorShatf::update(float dt, Point pos)
{
    if (!dead) {
        setVertices(pos);
        
        
        //set elevator sprite position
        elevator->setPosition(Point(elevator_body->GetPosition().x*PTM_RATIO-10, elevator_body->GetPosition().y*PTM_RATIO));
        
        if (elevator_status == Waiting)
        {
            MyQueryCallback queryCallback; //see "World querying topic"
            b2AABB aabb;
            b2Vec2 detectionVec = b2Vec2(elevator->getPosition().x/PTM_RATIO, elevator->getPosition().y/PTM_RATIO);
            aabb.lowerBound = detectionVec - b2Vec2( 0.5, .2);
            aabb.upperBound = detectionVec + b2Vec2( 0.5, .2);
            gWorld->QueryAABB( &queryCallback, aabb );
            
            for (int j = 0; j < queryCallback.foundBodies.size(); j++) {
                b2Body* body = queryCallback.foundBodies[j];
                FixtureType t = body->GetFixtureList()->GetFixtureType();
                //if it's a zombie, explosion metters
                if (t == f_monster_foot) {
                    if (shaft_height > 0) {
                        elevator_status = GoingUp;
                    }
                    else {
                        elevator_status = GoingDown;
                    }
                    MapGenerator::GetInstance()->setStageType(onElevator);
                }
            }
        }
        else if (elevator_status == GoingUp)
        {
            /*b2Vec2 vel = elevator_body->GetLinearVelocity();
             float velChange = ELEVATOR_SPEED - vel.x;
             float impulse = velChange*elevator_body->GetMass()*50;
             elevator_body->ApplyLinearImpulse(b2Vec2(0, impulse), elevator_body->GetWorldCenter(), true);
             */
            
            b2Vec2 ele_pos = elevator_body->GetPosition();
            ele_pos = b2Vec2(ele_pos.x, ele_pos.y + ELEVATOR_SPEED/PTM_RATIO);
            elevator_body->SetTransform(ele_pos, 0);
            
            if (abs(destinationY - elevator->getPosition().y) <= 10)
            {
                elevator_status = Done;
                MapGenerator::GetInstance()->setStageType(onRoof);
            }
        }
        else if (elevator_status == GoingDown) {
            b2Vec2 ele_pos = elevator_body->GetPosition();
            ele_pos = b2Vec2(ele_pos.x, ele_pos.y - ELEVATOR_SPEED/PTM_RATIO);
            elevator_body->SetTransform(ele_pos, 0);
            
            if (abs(destinationY - elevator->getPosition().y) <= 10)
            {
                elevator_status = Done;
                MapGenerator::GetInstance()->setStageType(onGround);
            }
        }
        else if (elevator_status == Done)
        {
            elevator_body->SetTransform(b2Vec2(elevator->getPosition().x/PTM_RATIO, destinationY/PTM_RATIO), 0);
            elevator_status = AfterDone;
        }
        else if (elevator_status == AfterDone)
        {
            setDead();
        }
    }
    else {
        if ((pos.x - lastPos.x) > MonsterOffset) {
            if (elevator_status == AfterDone) {
                afterDeath = true;
            }
        }
    }
}

void ElevatorShatf::setDead()
{
    dead = true;
}


ElevatorShatf::~ElevatorShatf()
{
    gLayer->removeChild(wall, true);
    gLayer->removeChild(terrain, true);
    gLayer->removeChild(elevator, true);
    
    if (groundBody != NULL) {
        gWorld->DestroyBody(groundBody);
        groundBody = NULL;
    }
    if (elevator_body != NULL) {
        gWorld->DestroyBody(elevator_body);
        elevator_body = NULL;
    }
}