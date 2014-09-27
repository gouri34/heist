//
//  skyBuilding.h
//  Animal_Squad
//
//  Created by cong ku on 8/12/14.
//
//

#ifndef __Animal_Squad__skyBuilding__
#define __Animal_Squad__skyBuilding__

#include "Building.h"

class SkyBuilding : public Building {
private:
    
public:
    static SkyBuilding* create(Point pos, int groundY);
    virtual bool init(Point pos, int groundY);
    virtual ~SkyBuilding();
    
    virtual void setDead();
    virtual void setVertices(Point pos);
    
    virtual void terrainSceneArrangement();
    virtual void update(float dt, Point pos);
    
    
    
};

#endif /* defined(__Animal_Squad__skyBuilding__) */
