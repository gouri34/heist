#include "GameScene.h"
#include "MapGenerator.h"

USING_NS_CC;

const float32 FIXED_TIMESTEP = 1.0f / 60.0f;

// Minimum remaining time to avoid box2d unstability caused by very small delta times
// if remaining time to simulate is smaller than this, the rest of time will be added to the last step,
// instead of performing one more single step with only the small delta time.
const float32 MINIMUM_TIMESTEP = 1.0f / 100.0f;

const int32 VELOCITY_ITERATIONS = 3;
const int32 POSITION_ITERATIONS = 3;

// maximum number of steps per tick to avoid spiral of death
const int32 MAXIMUM_NUMBER_OF_STEPS = 15;


// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    //preload textures
    cocos2d::Texture2D::TexParams params = {GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT};
//    Texture2D* groundTexture = Director::getInstance()->getTextureCache()->addImage("GlassBuilding.png");
//    groundTexture->setTexParameters(params);
//    Texture2D* wallTexture = Director::getInstance()->getTextureCache()->addImage("GlassBuilding_Wall.png");
//    wallTexture->setTexParameters(params);
    
    
    Texture2D* wallTexture = Director::getInstance()->getTextureCache()->addImage("testbuilding_wall.png");
    wallTexture->setTexParameters(params);
    
    Texture2D* viewTexture = Director::getInstance()->getTextureCache()->addImage("testbuilding_view.png");
    viewTexture->setTexParameters(params);
    
    Texture2D* terrainTexture = Director::getInstance()->getTextureCache()->addImage("terrain.png");
    terrainTexture->setTexParameters(params);
    
    
    //load armatures
    ArmatureDataManager::getInstance()->addArmatureFileInfo("tauren.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("wall.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("b_wall.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("glassWindow.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("agent.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("GUAIWUvvvvvvvvvvvvvvv.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("running_grunt.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("TrashCan.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("StopSign.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("BeastTrap.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("BeastCaptureMobile.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Object1.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Object2.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Object3.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("drill_grunt.ExportJson");


    
    //physics setup
    b2Vec2 gravity;
	gravity.Set(0.0f, -40.0f);
	world = new b2World(gravity);
	world->SetAutoClearForces(true);
	// Do we want to let bodies sleep?
    world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);
    _contactListener = new MyContactListener();
    world->SetContactListener(_contactListener);
    
    
    //load character
    bear = Bear::create(this, world, Point(visibleSize.width*0.8, visibleSize.height/2));
    bear->creatfootBody();
    
    MapGenerator::GetInstance()->cleanup();
    MapGenerator::GetInstance()->init(this, world);
    
    return true;
}


void GameScene::update(float dt)
{
    //update physics world
    physicsUpdate(dt);
    
    //update characters
    bear->update(dt);
    Point bearPos = bear->theBody->getPosition();
    
    //update terrain
    MapGenerator::GetInstance()->update(bearPos, dt,bear);
    
    
    //camera
    this->setPosition(Point(-bear->theBody->getPositionX()+200, -bearPos.y+294));
    
    
    
}



void GameScene::physicsUpdate(float dt)
{
    ////physics process
    //fixed time step
    float32 frameTime = dt;
    int stepsPerformed = 0;
    while ( (frameTime > 0.0) && (stepsPerformed < MAXIMUM_NUMBER_OF_STEPS) ){
        float32 deltaTime = std::min( frameTime, FIXED_TIMESTEP );
        frameTime -= deltaTime;
        if (frameTime < MINIMUM_TIMESTEP) {
            deltaTime += frameTime;
            frameTime = 0.0f;
        }
        
        this->beforeStep();
        
        world->Step(deltaTime,VELOCITY_ITERATIONS,POSITION_ITERATIONS);
        
        
        stepsPerformed++;
        this->afterStep(); // process collisions and result from callbacks called by the step
    }
    
    
    world->ClearForces ();
    for(b2Body *b = world->GetBodyList(); b; b=b->GetNext()) {
        if (b->GetUserData() != NULL) {
            if (b->GetFixtureList()->GetFixtureType() == f_wallblock || b->GetFixtureList()->GetFixtureType() == f_glassblock) {
                Sprite *s = (Sprite*)b->GetUserData();
                s->setPosition(Point(b->GetPosition().x*PTM_RATIO, b->GetPosition().y*PTM_RATIO));
                s->setRotation(-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));
            }
        }
    }

}

void GameScene::beforeStep()
{
    
}

void GameScene::afterStep()
{
    
}

GameScene::~GameScene()
{
    delete bear;
}

