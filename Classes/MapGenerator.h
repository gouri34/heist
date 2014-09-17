//
//  MapGenerator.h
//  Animal_Squad
//
//  Created by cong ku on 8/11/14.
//
//

#ifndef __Animal_Squad__MapGenerator__
#define __Animal_Squad__MapGenerator__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

#include "SceneConstructor.h"

#include "Terrain.h"

#include "CommonObject.h"
#include "Enemy.h"
#include "Item.h"

using namespace cocos2d;


typedef  enum {
    onGround, onRoof, onElevator
} StageTypes;

typedef  enum {
    plain = 0, goingUp = 1, goingDown = 2, inGroundBld = 3, inSkyBld = 4, onSkyRoof = 5
} TerrainStatus;


class MapGenerator : Ref {
private:
    
    
    StageTypes stageType;
    TerrainStatus terrainStatus;
    
    Terrain* currentTerrain;
    
    float stageTimer;
    float elevatorTimer;
    
    std::vector<Sprite*> sprites;
    std::vector<Terrain*> terrains;
    std::vector<CommonObject*>commonObjs;
    std::vector<Enemy*> enemies;
    std::vector<Item*> items;
    
    std::map<string, SceneInfo> sceneInfos;
    
    std::string sceneName;
    
    //terrain changing logic
    bool changeToPlain(TerrainStatus previous);
    bool changeToUp(TerrainStatus previous);
    bool changeToDown(TerrainStatus previous);
    
    //terrain generation
    void makeGroundPlain(bool fromSlope, bool wentUp);
    void makeGroundSlope(bool goUp);
    
public:
    
    Layer *gameLayer;
    b2World *gameWorld;
    
    float enemyTimer; //just for testing

    
    static MapGenerator* GetInstance();
    void init(Layer* _gameLayer, b2World* _gameWorld);
    
    int setupSceneWithInfo(std::string name, Point pos);
    void addObjectWithData(SceneData data, Point pos);

    void terrainEliminator();
    
    void setStageType(StageTypes st);
    
    void update(Point pos, float dt);
    void updateObjects(Point pos, float dt);
    
    void cleanup();
    
};

#endif /* defined(__Animal_Squad__MapGenerator__) */
