#include "GameScene.h"
#include "MapGenerator.h"

USING_NS_CC;

const float32 FIXED_TIMESTEP = 1.0f / 60.0f;

// Minimum remaining time to avoid box2d unstability caused by very small delta times
// if remaining time to simulate is smaller than this, the rest of time will be added to the last step,
// instead of performing one more single step with only the small delta time.
const float32 MINIMUM_TIMESTEP = 1.0f / 100.0f;

const int32 VELOCITY_ITERATIONS = 6;
const int32 POSITION_ITERATIONS = 6;

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
    Director::getInstance()->getTextureCache()->addImage("terrain.png");
    Director::getInstance()->getTextureCache()->addImage("testbuilding_view.png");
    Director::getInstance()->getTextureCache()->addImage("testbuilding_wall.png");
    Director::getInstance()->getTextureCache()->addImage("ele_shaft_wall.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("apartment_spritesheet0.plist", "apartment_spritesheet0.png");


    
    //load armatures
    ArmatureDataManager::getInstance()->addArmatureFileInfo("tauren.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("wall.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("b_wall.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("glassWindow.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("agent.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("running_grunt.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("FlammerMonster.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Panzer.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("PAObin.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("DDUNBIN.ExportJson");

    ArmatureDataManager::getInstance()->addArmatureFileInfo("GuardTower.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Mine.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Meteo.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("shiguan.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Sabaodui.ExportJson");
    
    //groundbuilding objects
    ArmatureDataManager::getInstance()->addArmatureFileInfo("apartment_chair.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("apartment_door.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("apartment_table.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("apartment_teatable.ExportJson");

    
    //physics setup
    b2Vec2 gravity;
	gravity.Set(0.0f, -70.0f);
	world = new b2World(gravity);
	world->SetAutoClearForces(true);
	// Do we want to let bodies sleep?
    world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);
    _contactListener = new MyContactListener();
    world->SetContactListener(_contactListener);
    
    
    //load character
    monster = Monster::create(this, world, Point(50, visibleSize.height/2));
    monster->creatfootBody();
    
    a->GetInstance()->setMonsterData(monster);
    
    MapGenerator::GetInstance()->cleanup();
    MapGenerator::GetInstance()->init(this, world);
    
    return true;
}


void GameScene::update(float dt)
{
    //update physics world
    physicsUpdate(dt);
    
    //update characters
    monster->update(dt);
    Point bearPos = monster->theBody->getPosition();
    
    //update terrain
    MapGenerator::GetInstance()->update(bearPos, dt);
    
    
    //camera
    this->setPosition(Point(-monster->theBody->getPositionX()+300, -bearPos.y+294));
    
    
    
    
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
    delete monster;
}

