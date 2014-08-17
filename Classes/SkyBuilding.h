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
#include "glassWindow.h"

class SkyBuilding : public Building {
private:

public:
    static SkyBuilding* create(Layer *gameScene_, b2World *gameWorld_, Point pos, int groundY);
    virtual bool init(Layer *gameScene_, b2World *gameWorld_, Point pos, int groundY);
    virtual ~SkyBuilding();
    
    virtual void setDead();
    virtual void setVertices(Point pos);
    virtual void update(float dt, Point pos);
    
    std::vector<GlassWindow*>windows;

    
};

#endif /* defined(__Animal_Squad__skyBuilding__) */
