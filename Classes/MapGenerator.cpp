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

#include "Wall.h"
#include "glassWindow.h"
#include "NormalEnemy.h"

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
    
    trapTimer = 3.0;
    dummyTimer = 1.0;
    elevatorTimer = 5.0;
 
    
    Ground* firstGround = Ground::create(gameLayer, gameWorld, Point(0, 100), 0);
    terrains.push_back(firstGround);

}



Point MapGenerator::getLastTerrainPos(bool death,float *lastTexCoord_x, int *groundYPos)
{
    Point lastPos = Point(0,0);
    
    for (int i = 0; i < terrains.size(); i++) {
        Terrain *t = terrains.at(i);
        if ((death == true) && (t->dead == false)) {
            t->setDead();
        }
        if (lastPos.x < t->lastPos.x) {
            lastPos = t->lastPos;
        }
        if (lastTexCoord_x != nullptr) {
            *lastTexCoord_x = t->lastTexCoordX;
        }
        if (groundYPos != nullptr) {
            *groundYPos = t->groundYpos;

        }
    }
    
    return lastPos;
    
}


void MapGenerator::setStageType(StageTypes st)
{
    stageType = st;
}


void MapGenerator::update(Point pos, float dt)
{
    updateObjects(pos, dt);
    
    
    trapTimer -= dt;
    elevatorTimer -= dt;
    
    if (trapTimer < 0) {
        trapTimer = 2.4;
        
        //add a new ground
        int randNum = rand()%100;
        //int buildingType = rand()%3;
        if (stageType == onGround) {

            if (randNum<50){
                
                if (terrainStatus != inGroundBld) {
                    Point lastPos = getLastTerrainPos(true, nullptr, nullptr);
                    
                    GroundBuilding *gb = GroundBuilding::create(gameLayer, gameWorld, lastPos);
                    terrains.push_back(gb);
                    GlassWindow *g = GlassWindow::create(gameLayer, gameWorld, Point(lastPos.x+10,lastPos.y));
                    windows.push_back(g);
                    
                    terrainStatus = inGroundBld;
                }
                else {
                    if (elevatorTimer >0) {
                        Point lastPos = getLastTerrainPos(false, nullptr, nullptr);
                        
                        GlassWindow *g = GlassWindow::create(gameLayer, gameWorld, Point(lastPos.x+10,lastPos.y));
                        windows.push_back(g);
                        
                        terrainStatus = inGroundBld;
                    }
                    else {
                        elevatorTimer = 12.0;
                        
                        Point lastPos = getLastTerrainPos(true, nullptr, nullptr);
                        ElevatorShatf *es = ElevatorShatf::create(gameLayer, gameWorld, lastPos);
                        terrains.push_back(es);
                        lastPos = es->lastPos;
                        SkyBuilding *sb = SkyBuilding::create(gameLayer, gameWorld, lastPos, es->groundYpos);
                        terrains.push_back(sb);
                        
                        stageType = onElevator;
                        terrainStatus = inSkyBld;
                    }
                    
                    
                }
                
                
            }
            else if (randNum < 75){
                
                //up or from down to plain
                
                float lastTexCoordX;
                Point lastPos = getLastTerrainPos(true, &lastTexCoordX, nullptr);
                
                
                if (terrainStatus == plain|| terrainStatus == inGroundBld) {
                    
                    Ground_Curved *gc = Ground_Curved::create(gameLayer, gameWorld, Point(lastPos.x - 5, lastPos.y), true, true,lastTexCoordX);
                    terrains.push_back(gc);
                    Ground_Slope *gs = Ground_Slope::create(gameLayer, gameWorld, gc->lastPos, true, gc->lastTexCoordX);
                    terrains.push_back(gs);
                    
                    terrainStatus =  goingUp;
                }
                else if (terrainStatus == goingDown) {
                    
                    Ground_Curved *gc = Ground_Curved::create(gameLayer, gameWorld, Point(lastPos.x, lastPos.y), false, true,lastTexCoordX);
                    terrains.push_back(gc);
                    Ground *g = Ground::create(gameLayer, gameWorld, Point(gc->lastPos.x - 5, gc->lastPos.y), gc->lastTexCoordX - 5/512);
                    terrains.push_back(g);
                    
                    terrainStatus = plain;
                }
                else if(terrainStatus == goingUp) {
                    Ground_Curved *gc = Ground_Curved::create(gameLayer, gameWorld, Point(lastPos.x, lastPos.y), true, false,lastTexCoordX);
                    terrains.push_back(gc);
                    Ground *g = Ground::create(gameLayer, gameWorld, Point(gc->lastPos.x - 5, gc->lastPos.y), gc->lastTexCoordX - 5/512);
                    terrains.push_back(g);
                    
                    terrainStatus = plain;
                }
                
            }
            else {
                //down or up to plain
                
                
                float lastTexCoordX;
                Point lastPos = getLastTerrainPos(true, &lastTexCoordX, nullptr);
                
                if (terrainStatus == plain || terrainStatus == inGroundBld) {
                    
                    Ground_Curved *gc = Ground_Curved::create(gameLayer, gameWorld, Point(lastPos.x - 5, lastPos.y), false, false,lastTexCoordX);
                    terrains.push_back(gc);
                    Ground_Slope *gs = Ground_Slope::create(gameLayer, gameWorld, gc->lastPos, false, gc->lastTexCoordX);
                    terrains.push_back(gs);
                    
                    terrainStatus =  goingDown;
                }
                else if (terrainStatus == goingDown) {
                    
                    Ground_Curved *gc = Ground_Curved::create(gameLayer, gameWorld, Point(lastPos.x, lastPos.y), false, true,lastTexCoordX);
                    terrains.push_back(gc);
                    Ground *g = Ground::create(gameLayer, gameWorld, Point(gc->lastPos.x - 5, gc->lastPos.y), gc->lastTexCoordX - 5/512);
                    terrains.push_back(g);
                    
                    terrainStatus = plain;
                    
                }
                else if(terrainStatus == goingUp) {
                    Ground_Curved *gc = Ground_Curved::create(gameLayer, gameWorld, Point(lastPos.x, lastPos.y), true, false,lastTexCoordX);
                    terrains.push_back(gc);
                    Ground *g = Ground::create(gameLayer, gameWorld, Point(gc->lastPos.x - 5, gc->lastPos.y), gc->lastTexCoordX - 5/512);
                    terrains.push_back(g);
                    
                    terrainStatus = plain;
                    
                }
                
            }
            
            
        }
        else if (stageType == onRoof)
        {
            if (randNum < 50) {
                
                if (terrainStatus == inSkyBld) {
                    Point lastPos = getLastTerrainPos(false, nullptr, nullptr);
                    
                    GlassWindow *g = GlassWindow::create(gameLayer, gameWorld, Point(lastPos.x+10,lastPos.y));
                    windows.push_back(g);
                }
                else if (terrainStatus == onSkyRoof) {
                    int lastY;
                    
                    Point lastPos = getLastTerrainPos(true, nullptr, &lastY);
                    
                    SkyBuilding *sb = SkyBuilding::create(gameLayer, gameWorld, Point(lastPos.x + 420, lastPos.y - 200), lastY);
                    terrains.push_back(sb);
                    
                    terrainStatus = inSkyBld;
                }
                
                
            }
            else {
                if (terrainStatus == inSkyBld) {
                    int lastY;

                    Point lastPos = getLastTerrainPos(true, nullptr, &lastY);
                    
                    SkyBuildingRoof *sbr = SkyBuildingRoof::create(gameLayer, gameWorld, Point(lastPos.x+420, lastPos.y -200), lastY);
                    terrains.push_back(sbr);
                    
                    terrainStatus = onSkyRoof;
                }
                else if (terrainStatus == onSkyRoof) {
                    Point lastPos = getLastTerrainPos(false, nullptr, nullptr);
                    
                    Wall *w = Wall::create(gameLayer, gameWorld, lastPos);
                    walls.push_back(w);
                }
            }
        }

    }
    else {
        dummyTimer -= dt;
        if (dummyTimer <=0) {
            
            dummyTimer = 0.02 + (float)(rand()%100)/160.0;
            
            //find the last position to put dummy in place
            Point lastPos;
            
            if (stageType == onRoof) {
                for (int i = 0; i < terrains.size(); i++) {
                    Terrain *t = terrains.at(i);
                    lastPos = t->lastPos;
                }

            }
            else if (stageType == onGround)
            {
                for (int i = 0; i < terrains.size(); i++) {
                    Terrain *t = terrains.at(i);
                    lastPos = t->lastPos;
                }
                
            }
            
            
            NormalEnemy *e = NormalEnemy::create((Scene*)gameLayer, gameWorld, "agent", Point(lastPos.x-20, lastPos.y+50), 0.3);
            enemies.push_back(e);
            
        }
    }
    
    
    
    
}

void MapGenerator::updateObjects(Point pos, float dt)
{
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
    
    
    std::vector<Wall*> usedWall;
    for (int i = 0; i < walls.size(); i++) {
        Wall* w = walls.at(i);
        w->update(pos, dt);
        if (w->destroyed) {
            usedWall.push_back(w);
        }
    }
    for (int i = 0; i < usedWall.size(); i++) {
        Wall* w = usedWall.at(i);
        walls.erase(std::remove(walls.begin(), walls.end(), w), walls.end());
        delete w;
    }
    
    
    std::vector<GlassWindow*> usedWindow;
    for (int i = 0; i < windows.size(); i++) {
        GlassWindow* g = windows.at(i);
        g->update(pos, dt);
        if (g->destroyed) {
            usedWindow.push_back(g);
        }
    }
    for (int i = 0; i < usedWindow.size(); i++) {
        GlassWindow* g = usedWindow.at(i);
        windows.erase(std::remove(windows.begin(), windows.end(), g), windows.end());
        delete g;
    }
    
    
    std::vector<NormalEnemy*> usedDummy;
    for (int i = 0; i < enemies.size(); i++) {
        NormalEnemy* e = enemies.at(i);
        e->update(dt);
        if ((pos.x - e->position.x) > 300) {
            usedDummy.push_back(e);
        }
    }
    for (int i = 0; i < usedDummy.size(); i++) {
        NormalEnemy* e = usedDummy.at(i);
        enemies.erase(std::remove(enemies.begin(), enemies.end(), e), enemies.end());
        delete e;
    }

}


void MapGenerator::cleanup()
{
    for (int i = 0; i < terrains.size(); i++) {
        Terrain *t = terrains.at(i);
        delete t;
    }
    terrains.clear();
    
    
    for (int i = 0; i < walls.size(); i++) {
        Wall *w = walls.at(i);
        delete w;
    }
    walls.clear();
    
    for (int i = 0; i < windows.size(); i++) {
        GlassWindow* g = windows.at(i);
        delete g;
    }
    windows.clear();
    
    for (int i = 0; i < enemies.size(); i++) {
        NormalEnemy* e = enemies.at(i);
        delete e;
    }
    enemies.clear();
    
}
