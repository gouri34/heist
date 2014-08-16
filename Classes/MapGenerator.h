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

#include "Bear.h"


#include "Terrain.h"

#include "Wall.h"
#include "glassWindow.h"
#include "NormalEnemy.h"

using namespace cocos2d;


typedef  enum {
    onGround, onRoof, onElevator
} StageTypes;

typedef  enum {
    plain, goingUp, goingDown, inGroundBld
} TerrainStatus;


class MapGenerator : Ref {
private:
    
    Layer *gameLayer;
    b2World *gameWorld;
    
    StageTypes stageType;
    TerrainStatus terrainStatus;
    
    float trapTimer;
    float dummyTimer;
    float elevatorTimer;
    
    std::vector<Terrain*> terrains;
    std::vector<Wall*> walls;
    std::vector<GlassWindow*>windows;
    std::vector<NormalEnemy*> enemies;
    
public:
    static MapGenerator* GetInstance();
    void init(Layer* _gameLayer, b2World* _gameWorld);
    
    Point getLastTerrainPos(bool death,float *lastTexCoord_x);
    
    void setStageType(StageTypes st);
    
    void update(Point pos, float dt);
    void updateObjects(Point pos, float dt);
    
    void cleanup();
};

#endif /* defined(__Animal_Squad__MapGenerator__) */
