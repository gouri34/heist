//
//  MapGenerator.cpp
//  Animal_Squad
//
//  Created by cong ku on 8/11/14.
//
//

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

#include "Wall.h"

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
        sceneInfos["houseRoom0"] =  SceneConstructor::ConstructScene("testhouse.json");
        sceneInfos["groundBack0"] =  SceneConstructor::ConstructScene("ground_frontback.json");
        sceneInfos["groundBack1"] =  SceneConstructor::ConstructScene("ground_frontback1.json");
        sceneInfos["groundBack2"] =  SceneConstructor::ConstructScene("ground_frontback2.json");
        sceneInfos["groundBack3"] =  SceneConstructor::ConstructScene("ground_frontback3.json");
        sceneInfos["groundBack4"] =  SceneConstructor::ConstructScene("ground_frontback4.json");
        sceneInfos["enemySetup1"] = SceneConstructor::ConstructScene("EnemySetup1.json");
        sceneInfos["enemySetup2"] = SceneConstructor::ConstructScene("EnemySetup2.json");
        sceneInfos["enemySetup3"] = SceneConstructor::ConstructScene("EnemySetup3.json");
    }

    
    Ground* firstGround = Ground::create(Point(0, 100), 0);
    terrains.push_back(firstGround);
    currentTerrain = firstGround;
    
    
    NormalEnemy* ne = NormalEnemy::create("running_grunt", Point(900, 120), 0.25, 0.25);
    enemies.push_back(ne);
    terrainStatus = inGroundBld;
}


//Scene construction
int MapGenerator::setupSceneWithInfo(std::string name, Point pos)
{
    SceneInfo info = sceneInfos[name];
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
        Point pos_ = Point(data.x+pos.x, data.y+pos.y);
        CommonObject *d = CommonObject::create(data.sourceName, pos_, data.scalex, data.scaley);
        node = (Node*)d->theBody;
        commonObjs.push_back(d);
    }
    else if (data.type == 3) {
        Point pos_ = Point(data.x+pos.x, data.y+pos.y);
        NormalEnemy *ne = NormalEnemy::create(data.sourceName.c_str(), pos_, data.scalex, data.scaley);
        node = (Node*)ne->armature;
        enemies.push_back(ne);
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
    
    
    stageTimer -= dt;
    enemyTimer -= dt;
    elevatorTimer -= dt;
    
    
    // time base terrain generator
    if (stageTimer < 0) {
        
        bool jobDone = false;
        while (!jobDone) {
            int randSeed = rand()%3; //seed decide the next stage type
            
            TerrainStatus ts = (TerrainStatus)randSeed;
            
            switch (ts) {
                case plain:
                {
                    jobDone = changeToPlain(terrainStatus);
                }
                    break;
                    
                case goingUp:
                {
                    jobDone = changeToUp(terrainStatus);
                }
                    break;
                case goingDown:
                {
                    jobDone = changeToDown(terrainStatus);
                }
                    break;
                    
                default:
                    break;
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
        CommonObject* g = commonObjs.at(i);
        g->update(pos, dt);
        if (g->destroyed) {
            usedObj.push_back(g);
        }
    }
    for (int i = 0; i < usedObj.size(); i++) {
        CommonObject* g = usedObj.at(i);
        commonObjs.erase(std::remove(commonObjs.begin(), commonObjs.end(), g), commonObjs.end());
        delete g;
    }
    
    std::vector<Enemy*> usedDummy;
    for (int i = 0; i < enemies.size(); i++) {
        Enemy* e = enemies.at(i);
        e->update(dt);
        if ((pos.x - e->position.x) > 300 || (pos.y - e->position.y) > 300) {
            usedDummy.push_back(e);
        }
    }
    for (int i = 0; i < usedDummy.size(); i++) {
        Enemy* e = usedDummy.at(i);
        enemies.erase(std::remove(enemies.begin(), enemies.end(), e), enemies.end());
        delete e;
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

//=================Terrain Generation===================

void MapGenerator::makeGroundPlain(bool fromSlope, bool wentUp)
{
    Point lastTerrainPos = currentTerrain->lastPos;
    float lastTexCoordX = currentTerrain->lastTexCoordX;
    
    terrainEliminator();
    
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
    stageTimer = 5.0;
}

void MapGenerator::makeGroundSlope(bool goUp)
{
    Point lastTerrainPos = currentTerrain->lastPos;
    float lastTexCoordX = currentTerrain->lastTexCoordX;

    terrainEliminator();
    
    if (goUp) {
        Ground_Curved *gc = Ground_Curved::create(lastTerrainPos, true, true,lastTexCoordX);
        terrains.push_back(gc);
        Ground_Slope *gs = Ground_Slope::create(gc->lastPos, true, gc->lastTexCoordX);
        terrains.push_back(gs);
        
        terrainStatus = goingUp;
        currentTerrain = gs;
    }
    else {
        Ground_Curved *gc = Ground_Curved::create(lastTerrainPos, false, false,lastTexCoordX);
        terrains.push_back(gc);
        Ground_Slope *gs = Ground_Slope::create(gc->lastPos, false, gc->lastTexCoordX);
        terrains.push_back(gs);
        
        terrainStatus = goingDown;
        currentTerrain = gs;
    }
    
    stageTimer = 2.0;

}