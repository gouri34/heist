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

#include "RoomConstructor.h"

#include "Wall.h"
#include "glassWindow.h"

//enemies
#include "NormalEnemy.h"
#include "Enemy.h"
#include "DrillMan.h"
#include "ShieldMan.h"
#include "BazookaMan.h"

#include "EnemyObject.h"
#include "DestructableObject.h"
#include "StockPiles.h"

//objects
#include "MonsterTrap.h"
#include "Panzer.h"
#include "LandMine.h"


using namespace cocos2d;


typedef  enum {
    onGround, onRoof, onElevator
} StageTypes;

typedef  enum {
    plain, goingUp, goingDown, inGroundBld, inSkyBld, onSkyRoof
} TerrainStatus;


class MapGenerator : Ref {
private:
    
    Layer *gameLayer;
    b2World *gameWorld;
    
    TerrainStatus terrainStatus;
    
    float trapTimer;
    float dummyTimer;
    float elevatorTimer;
    
    std::vector<Terrain*> terrains;
    std::vector<Wall*> walls;
    std::vector<GlassWindow*>windows;
    std::vector<Enemy*> enemies;
    std::vector<DestructableObject*> destructableobjects;
    std::vector<EnemyObject*> enemyobjects;
    std::vector<StockPiles*>stockpiless;
    //暂时创建一个vecto存储没有托管的static body
    std::vector<b2Body*> staticBodys;

    //08.23.2014 added by Weihan
    float lastBuildingx;
    float lastObjectx;
    float lastEnemeyObjectx;
    void backgroundBuildingHandler(Point lastpos);
    void objectHandler(Point lastpos);
    float screenRightEdgeX;
    Size visibleSize;
    //--------
    
    RoomInfo testRoomInfo;
    
public:
    static MapGenerator* GetInstance();
    void init(Layer* _gameLayer, b2World* _gameWorld);
    
    Point getLastTerrainPos(bool death,float *lastTexCoord_x, int *groundYPos);
    
    void setStageType(StageTypes st);
    
    //functions for room construction
    void setupRoomWithInfo(RoomInfo info, Point pos);
    void addObjectWithData(RoomData data, Point pos);
    
    void update(Point pos, float dt, Bear *bear);
    void updateObjects(Point pos, float dt, Bear *bear);
    
    void cleanup();
    
    StageTypes stageType;

};

#endif /* defined(__Animal_Squad__MapGenerator__) */
