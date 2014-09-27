//
//  MapGenerator.cpp
//  Animal_Squad
//
//  Created by cong ku on 8/11/14.
//
//
#include <stdexcept>

#include "MapGenerator.h"

#include "Ground.h"
#include "Ground_Curved.h"
#include "Ground_Slope.h"

#include "Building.h"
#include "GroundBuilding.h"
#include "ElevatorShaft.h"
#include "SkyBuilding.h"
#include "SkyBuildingRoof.h"

#include "NormalEnemy.h"
#include "Panzer.h"
#include "Wall.h"
#include "BazookaEnemy.h"
#include "GuardTower.h"
#include "ShieldEnemy.h"
#include "LandMine.h"
#include "StockPiles.h"


static MapGenerator *m_MapGenerator = nullptr;


MapGenerator* MapGenerator::GetInstance()
{
    if (!m_MapGenerator) {
        m_MapGenerator = new MapGenerator();
    }
    
    return m_MapGenerator;
}

void MapGenerator::init(Layer* _gameLayer, b2World* _gameWorld)
{
    gameLayer = _gameLayer;
    gameWorld = _gameWorld;
    
    stageType = onGround;
    terrainStatus = plain;
    
    stageTimer = 3.0;
    enemyTimer = 3.0;
    elevatorTimer = 5.0;
 
    //load scene info
    if (sceneInfos.size() == 0) {
       // sceneInfos["houseRoom0"] =  SceneConstructor::ConstructScene("testhouse.json");
        sceneInfos["groundBack0"] =  SceneConstructor::ConstructScene("ground_frontback.json");
        sceneInfos["groundBack1"] =  SceneConstructor::ConstructScene("ground_frontback1.json");
        sceneInfos["groundBack2"] =  SceneConstructor::ConstructScene("ground_frontback2.json");
        sceneInfos["groundBack3"] =  SceneConstructor::ConstructScene("ground_frontback3.json");
        sceneInfos["groundBack4"] =  SceneConstructor::ConstructScene("ground_frontback4.json");
        sceneInfos["enemySetup1"] = SceneConstructor::ConstructScene("EnemySetup1.json");
        sceneInfos["enemySetup2"] = SceneConstructor::ConstructScene("EnemySetup2.json");
        sceneInfos["enemySetup3"] = SceneConstructor::ConstructScene("EnemySetup3.json");
        sceneInfos["three_panzer_in_a_row"] = SceneConstructor::ConstructScene("three_panzer_in_a_row.json");
        sceneInfos["one_shieldenemy_on_one_guardtower"] = SceneConstructor::ConstructScene("one_shieldenemy_on_one_guardtower.json");
        sceneInfos["three_shieldenemy_in_a_row"] = SceneConstructor::ConstructScene("three_shieldenemy_in_a_row.json");
        sceneInfos["two_bazookaenemy_and_one_shieldenemy"] = SceneConstructor::ConstructScene("two_bazookaenemy_and_one_shieldenemy.json");
        
        sceneInfos["one_steelpiles_in_front_of_one_guardtower_with_a_bazookaenemy_on_it"] = SceneConstructor::ConstructScene("one_steelpiles_in_front_of_one_guardtower_with_a_bazookaenemy_on_it.json");

        sceneInfos["three_mine_in_a_row"] = SceneConstructor::ConstructScene("three_mine_in_a_row.json");

        
        //Apartment scene info
        sceneInfos["apartment_room1"] = SceneConstructor::ConstructScene("Apartment_Room1.json");
        sceneInfos["apartment_room2"] = SceneConstructor::ConstructScene("Apartment_Room2.json");
        sceneInfos["apartment_room3"] = SceneConstructor::ConstructScene("Apartment_Room3.json");

    }

    
    Ground* firstGround = Ground::create(Point(0, 100), 0);
    terrains.push_back(firstGround);
    currentTerrain = firstGround;
    
    
    NormalEnemy* ne = NormalEnemy::create("running_grunt", Point(900, 120), 0.25, 0.25);
    enemies.push_back(ne);
    terrainStatus = inGroundBld;
    
    Panzer* p = Panzer::create("Panzer", Point(900,250), 1, 1);
    enemies.push_back(p);
    
    
    GuardTower *gt = GuardTower::create("GuardTower", Point(1100,270), 1.0, 0.8);
    commonObjs.push_back(gt);
    
    BazookaEnemy *be = BazookaEnemy::create("PAObin", Point(1100,350), 0.3, 0.3);
    enemies.push_back(be);
    
    ShieldEnemy *se = ShieldEnemy::create("DDUNBIN", Point(900,120), 0.3, 0.3);
    enemies.push_back(se);
    
    LandMine *lm = LandMine::create("Mine", Point(1300,250), 0.3, 0.3);
    enemies.push_back(lm);
    
    Item *it = Item::create(gameLayer, gameWorld, Point(1400,200));
    items.push_back(it);
    
    StockPiles *sp = StockPiles::create("Sabaodui", Point(950,120), 0.4, 0.4);
    commonObjs.push_back(sp);
}


//Scene construction
int MapGenerator::setupSceneWithInfo(std::string name, Point pos)
{
    SceneInfo info = sceneInfos[name];
    sceneName = name;
    std::vector<SceneData> vec = info.sceneInfoVec;
    for (int i = 0; i < vec.size(); i++) {
        SceneData d = vec[i];
        addObjectWithData(d, pos);
    }
    
    return info.length;
}


void MapGenerator::addObjectWithData(SceneData data, Point pos)
{
    Node* node;
    if (data.type == 1) {
        Sprite *s = Sprite::create(data.sourceName);
        gameLayer->addChild(s);
        sprites.push_back(s);
        node = (Node*)s;
        if (data.isPhysics) {
            Point pos_ = Point(data.x+pos.x, data.y+pos.y);
            
            //create body
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(pos_.x/PTM_RATIO,pos_.y/PTM_RATIO);
            b2Body* body_ = gameWorld->CreateBody(&bodyDef);
            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox((0.5*data.scalex*s->getContentSize().width/PTM_RATIO), 0.5*data.scaley*s->getContentSize().height/PTM_RATIO);
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 1.0;
            fixtureDef.friction = 0.3f;
            fixtureDef.fixturetype = f_ground;
            body_->CreateFixture(&fixtureDef);
        }
    }
    else if (data.type == 2) {

        Sprite *s = Sprite::createWithSpriteFrameName(data.sourceName);
        gameLayer->addChild(s);
        sprites.push_back(s);
        node = (Node*)s;
        if (data.isPhysics) {
            Point pos_ = Point(data.x+pos.x, data.y+pos.y);
            
            //create body
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(pos_.x/PTM_RATIO,pos_.y/PTM_RATIO);
            b2Body* body_ = gameWorld->CreateBody(&bodyDef);
            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox((0.5*data.scalex*s->getContentSize().width/PTM_RATIO), 0.5*data.scaley*s->getContentSize().height/PTM_RATIO);
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 1.0;
            fixtureDef.friction = 0.3f;
            fixtureDef.fixturetype = f_ground;
            body_->CreateFixture(&fixtureDef);
        }
    }
    else if (data.type == 3) {

        Point pos_ = Point(data.x+pos.x, data.y+pos.y);
        if (data.sourceName.find("GuardTower")!=string::npos) {
            GuardTower *d = GuardTower::create("GuardTower", pos_, data.scalex, data.scaley);
            node = (Node*)d->theBody;
            commonObjs.push_back(d);
        }
        else if (data.sourceName.find("shiguan")!=string::npos){
            StockPiles *sp = StockPiles::create("shiguan", pos_, data.scalex, data.scaley);
            node = (Node*)sp->theBody;
            commonObjs.push_back(sp);
        }
        else{
            CommonObject *d = CommonObject::create(data.sourceName, pos_, data.scalex, data.scaley);
            node = (Node*)d->theBody;
            commonObjs.push_back(d);
        }
    }
    else if (data.type == 4) {

        Point pos_ = Point(data.x+pos.x, data.y+pos.y);
        if (data.sourceName.find("Panzer")!=string::npos) {
            Panzer* p = Panzer::create("Panzer", pos_, 1, 1);
            node = (Node*)p->armature;
            enemies.push_back(p);

        }
        if(data.sourceName.find("DDUNBIN")!=string::npos){
            ShieldEnemy *se = ShieldEnemy::create(data.sourceName.c_str(),pos_,data.scalex, data.scaley);
            node = (Node*)se->armature;
            enemies.push_back(se);
        }
        if(data.sourceName.find("PAObin")!=string::npos){
            BazookaEnemy *be = BazookaEnemy::create(data.sourceName.c_str(),pos_,data.scalex, data.scaley);
            node = (Node*)be->armature;
            enemies.push_back(be);
        }
        if (data.sourceName.find("Mine")!=string::npos) {
            LandMine *lm = LandMine::create(data.sourceName.c_str(),pos_,data.scalex, data.scaley);
            node = (Node*)lm->armature;
            enemies.push_back(lm);
        }

        
        if(data.sourceName.find("running_grunt")!=string::npos){
            NormalEnemy *ne = NormalEnemy::create(data.sourceName.c_str(), Point(pos_.x, pos_.y+60), data.scalex, data.scaley);
            node = (Node*)ne->armature;
            enemies.push_back(ne);
        }
    }
   
    
    node->setPosition(Point(data.x + pos.x, data.y + pos.y));
    node->setScaleX(data.scalex);
    node->setScaleY(data.scaley);
    node->setRotation(data.rotation);
    node->setZOrder(data.zOrder);
    
}



void MapGenerator::terrainEliminator()
{
    for (int i = 0; i < terrains.size(); i++) {
        Terrain *t = terrains.at(i);
        if (t->dead == false) {
            t->setDead();
        }
    }
}


void MapGenerator::setStageType(StageTypes st)
{
    stageType = st;
}



void MapGenerator::update(Point pos, float dt)
{
    updateObjects(pos, dt);
    
    if (currentTerrain->timerStart) {
        stageTimer -= dt;
    }
    enemyTimer -= dt;
    elevatorTimer -= dt;
    
    
    // time base terrain generator
    
    //if time for elevator, try go elevator first
    
    
    if (stageTimer <= 0) {
        
        bool jobDone = false;
        
        bool elevatorAdded = false;
        if (elevatorTimer <=0) {
            elevatorAdded = changeToElevator(terrainStatus);
        }
        
        if (stageType == onGround && (!elevatorAdded)) {
            while (!jobDone) {
                int randSeed = rand()%4; //seed decide the next stage type
                
                TerrainStatus ts = (TerrainStatus)randSeed;
                
                switch (ts) {
                    case plain:
                    {
                        jobDone = changeToPlain(terrainStatus);
                    }
                        break;
                        
                    case goingUp:
                    {
                        jobDone = changeToGroundBuilding(terrainStatus);
                    }
                        break;
                    case goingDown:
                    {
                        jobDone = changeToGroundBuilding(terrainStatus);
                    }
                        
                    case inGroundBld:
                    {
                        jobDone = changeToGroundBuilding(terrainStatus);
                    }
                        break;
                        
                    default:
                        break;
                }
            }
        }
        else if (stageType == onRoof && (!elevatorAdded))
        {
            while (!jobDone) {
                int randSeed = rand()%2+4; //seed decide the next stage type
                
                TerrainStatus ts = (TerrainStatus)randSeed;
                
                switch (ts) {
                    case inSkyBld:
                    {
                        jobDone = changeToSkyBuilding(terrainStatus);
                    }
                        break;
                        
                    case onSkyRoof:
                    {
                        jobDone = changeToSkyRoof(terrainStatus);
                    }
                        break;
                        
                    default:
                        break;
                }
            }

        }
        
        
        
    }
    
    
}



void MapGenerator::updateObjects(Point pos, float dt)
{
    std::vector<Sprite*> usedSprite;
    for (int i = 0; i < sprites.size(); i++) {
        Sprite *s = sprites.at(i);
        if ((pos.x - s->getPositionX()) > 1524) {
            usedSprite.push_back(s);
        }
    }
    for (int i = 0; i < usedSprite.size(); i++) {
        Sprite *s = sprites.at(i);
        sprites.erase(std::remove(sprites.begin(), sprites.end(), s), sprites.end());
        gameLayer->removeChild(s, true);
    }
    
    
    std::vector<Terrain*> usedTerrain;
    for (int i = 0; i < terrains.size(); i++) {
        Terrain *t = terrains.at(i);
        if (t->afterDeath) {
            //this->removeChild(g);
            usedTerrain.push_back(t);
        }
        else {
            t->update(dt, pos);
        }
    }
    for (int i = 0; i < usedTerrain.size(); i++) {
        Terrain* t = usedTerrain.at(i);
        terrains.erase(std::remove(terrains.begin(), terrains.end(), t), terrains.end());
        delete t;
    }
    
    
    std::vector<CommonObject*> usedObj;
    for (int i = 0; i < commonObjs.size(); i++) {
        try {
            CommonObject* g = commonObjs.at(i);
            g->update(pos, dt);
            if (g->destroyed) {
                usedObj.push_back(g);
            }
        } catch (std::out_of_range &exc) {
            std::cerr << exc.what() << " Line:" << __LINE__ << " File:" << __FILE__ << endl;
        }
    }
    for (int i = 0; i < usedObj.size(); i++) {
        try {
            CommonObject* g = usedObj.at(i);
            commonObjs.erase(std::remove(commonObjs.begin(), commonObjs.end(), g), commonObjs.end());
            delete g;
        } catch (std::out_of_range &exc) {
            std::cerr << exc.what() << " Line:" << __LINE__ << " File:" << __FILE__ << endl;
        }
    }

    std::vector<Enemy*> usedDummy;
    for (int i = 0; i < enemies.size(); i++) {
        try {
            Enemy* e = enemies.at(i);
            e->update(dt);
            if ((pos.x - e->position.x) > 500 || (pos.y - e->position.y) > 1000) {
                usedDummy.push_back(e);
            }
        } catch (std::out_of_range &exc) {
            std::cerr << exc.what() << " Line:" << __LINE__ << " File:" << __FILE__ << endl;
        }
    }
    for (int i = 0; i < usedDummy.size(); i++) {
        try {
            Enemy* e = usedDummy.at(i);
            enemies.erase(std::remove(enemies.begin(), enemies.end(), e), enemies.end());
            delete e;
        } catch (std::out_of_range &exc) {
        std::cerr << exc.what() << " Line:" << __LINE__ << " File:" << __FILE__ << endl;
        }
    }

    std::vector<Item*>useditems;
    for (int i=0; i<items.size(); i++) {
        try {
            Item *item = items.at(i);
            item->update(dt);
            if (pos.x-item->armature->getPositionX()>550 || (pos.y-item->armature->getPositionY())>1000) {
                useditems.push_back(item);
            }

        } catch (std::out_of_range &exc) {
            std::cerr << exc.what() << " Line:" << __LINE__ << " File:" << __FILE__ << endl;
        }
    }
    for (int i=0; i<useditems.size(); i++) {
        
        try {
            Item *item = useditems.at(i);
            items.erase(std::remove(items.begin(),items.end(),item), items.end());
            delete item;
            
        } catch (std::out_of_range &exc) {
            std::cerr << exc.what() << " Line:" << __LINE__ << " File:" << __FILE__ << endl;
        }
    }

}


void MapGenerator::cleanup()
{
    sprites.clear();
    
    for (int i = 0; i < terrains.size(); i++) {
        Terrain *t = terrains.at(i);
        delete t;
    }
    terrains.clear();
    
    
    for (int i = 0; i < commonObjs.size(); i++) {
        CommonObject* g = commonObjs.at(i);
        delete g;
    }
    commonObjs.clear();
    
    for (int i = 0; i < enemies.size(); i++) {
        Enemy* e = enemies.at(i);
        delete e;
    }
    enemies.clear();
    
    for (int i=0; i<items.size(); i++) {
        Item* item = items[i];
        delete item;
    }
    items.clear();
    
}


//=================Terrain Changing logic===================
bool MapGenerator::changeToPlain(TerrainStatus previous)
{
    
    if (previous == plain) {
        return false;
    }
    
    switch (previous) {
        case plain:
            return false;
            break;
        
        case goingUp:
        {
            makeGroundPlain(true, true);
            return true;
        }
            break;
            
        case goingDown:
        {
            makeGroundPlain(true, false);
            return true;

        }
            break;
            
        default:
        {
            makeGroundPlain(false, false);
            return true;

        }
            break;
    }
    
    
    return false;
}

bool MapGenerator::changeToUp(TerrainStatus previous)
{
    if (previous == goingUp) {
        return false;
    }
    
    
    switch (previous) {
        case goingDown:
            return false;
            break;
        case plain:
        {
            makeGroundSlope(true);
            return true;
        }
            break;
        default:
            return false;
            break;
    }
    
    
    return false;
}

bool MapGenerator::changeToDown(TerrainStatus previous)
{
    if (previous == goingDown) {
        return false;
    }
    
    switch (previous) {
        case goingUp:
            return false;
            break;
        case plain:
        {
            makeGroundSlope(false);
            return true;
        }
            break;
        default:
            return false;
            break;
    }
    
    return false;
}

bool MapGenerator::changeToGroundBuilding(TerrainStatus previous)
{
    if (previous == inGroundBld) {
        return false;
    }
    
    switch (previous) {
        case plain:
        {
            makeGroundBuilding();
            return true;
        }
            break;
            
        default:
            return false;
            break;
    }
}

bool MapGenerator::changeToSkyBuilding(TerrainStatus previous)
{
    makeSkyBuilding();
    
    return true;
}

bool MapGenerator::changeToSkyRoof(TerrainStatus previous)
{
    makeRoofBuilding();
    
    return true;
}

bool MapGenerator::changeToElevator(TerrainStatus previous)
{
    if (previous == inGroundBld) {
        makeEvelator(true);
        return true;
    }
    else if (previous == inSkyBld) {
        makeEvelator(false);
        return true;
    }
    
    return false;
}

//=================Terrain Generation===================

void MapGenerator::makeGroundPlain(bool fromSlope, bool wentUp)
{
    terrainEliminator();

    Point lastTerrainPos = currentTerrain->lastPos;
    float lastTexCoordX = currentTerrain->lastTexCoordX;
    
    
    if (fromSlope) {
        if (wentUp) {
            Ground_Curved *gc = Ground_Curved::create(lastTerrainPos, true, false,lastTexCoordX);
            terrains.push_back(gc);
            Ground *g = Ground::create(gc->lastPos, gc->lastTexCoordX);
            terrains.push_back(g);
            currentTerrain = g;
        }
        else {
            Ground_Curved *gc = Ground_Curved::create(lastTerrainPos, false, true,lastTexCoordX);
            terrains.push_back(gc);
            Ground *g = Ground::create(gc->lastPos, gc->lastTexCoordX);
            terrains.push_back(g);
            currentTerrain = g;
        }
    }
    else {
        Ground *g = Ground::create(lastTerrainPos, lastTexCoordX);
        terrains.push_back(g);
        currentTerrain = g;
    }
    
    terrainStatus = plain;
    stageType = onGround;
    stageTimer = 8.0;
}

void MapGenerator::makeGroundSlope(bool goUp)
{
    terrainEliminator();

    Point lastTerrainPos = currentTerrain->lastPos;
    float lastTexCoordX = currentTerrain->lastTexCoordX;

    
    if (goUp) {
        Ground_Curved *gc = Ground_Curved::create(lastTerrainPos, true, true,lastTexCoordX);
        terrains.push_back(gc);
        Ground_Slope *gs = Ground_Slope::create(gc->lastPos, true, gc->lastTexCoordX);
        terrains.push_back(gs);
        
        terrainStatus = goingUp;
        stageType = onGround;
        currentTerrain = gs;
    }
    else {
        Ground_Curved *gc = Ground_Curved::create(lastTerrainPos, false, false,lastTexCoordX);
        terrains.push_back(gc);
        Ground_Slope *gs = Ground_Slope::create(gc->lastPos, false, gc->lastTexCoordX);
        terrains.push_back(gs);
        
        terrainStatus = goingDown;
        stageType = onGround;
        currentTerrain = gs;
    }
    
    stageTimer = 1.0;

}

void MapGenerator::makeGroundBuilding()
{
    terrainEliminator();

    Point lastTerrainPos = currentTerrain->lastPos;
    
    GroundBuilding *gb = GroundBuilding::create(lastTerrainPos);
    terrains.push_back(gb);
    
    terrainStatus = inGroundBld;
    stageType = onGround;
    currentTerrain = gb;
    
     stageTimer = 8.0;
}

void MapGenerator::makeSkyBuilding()
{
    terrainEliminator();

    Point lastTerrainPos = currentTerrain->lastPos;
    Point pos = Point(lastTerrainPos.x +300, lastTerrainPos.y-240);
    
    SkyBuilding *skb = SkyBuilding::create(pos, pos.y - 512*3);
    terrains.push_back(skb);
    
    terrainStatus = inSkyBld;
    stageType = onRoof;
    currentTerrain = skb;
    
    stageTimer = 5.0;
}

void MapGenerator::makeRoofBuilding()
{
    terrainEliminator();

    Point lastTerrainPos = currentTerrain->lastPos;
    Point pos = Point(lastTerrainPos.x +300, lastTerrainPos.y-200);
    
    SkyBuildingRoof *skbf = SkyBuildingRoof::create(pos, pos.y - 512*3);
    terrains.push_back(skbf);
    
    terrainStatus = onSkyRoof;
    stageType = onRoof;
    currentTerrain = skbf;
    
    stageTimer = 2.0;
}

void MapGenerator::makeEvelator(bool up)
{
    elevatorTimer = 15;
    
    terrainEliminator();

    Point lastTerrainPos = currentTerrain->lastPos;
    
    int height;
    if (up) {
        height = 512*3;
        ElevatorShatf *es = ElevatorShatf::create(lastTerrainPos, height);
        SkyBuilding *skb = SkyBuilding::create(es->lastPos, lastTerrainPos.y);
        terrains.push_back(es);
        terrains.push_back(skb);
        
        terrainStatus = inSkyBld;
        currentTerrain = skb;

    }
    else {
        height = -512*3;
        ElevatorShatf *es = ElevatorShatf::create(lastTerrainPos, height);
        GroundBuilding *gb = GroundBuilding::create(es->lastPos);
        terrains.push_back(es);
        terrains.push_back(gb);
        
        terrainStatus = inGroundBld;
        currentTerrain = gb;
    }
    
    
    stageTimer = 8;
}