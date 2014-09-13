//
//  SkyBuildingRoof.h
//  Animal_Squad
//
//  Created by cong ku on 8/17/14.
//
//

#ifndef __Animal_Squad__SkyBuildingRoof__
#define __Animal_Squad__SkyBuildingRoof__

#include "Building.h"

class SkyBuildingRoof : public Building {
    
private:
public:
    static SkyBuildingRoof* create(Point pos, int groundY);
    virtual bool init(Point pos, int groundY);
    virtual ~SkyBuildingRoof();
    
    virtual void setDead();
    virtual void setVertices(Point pos);
    virtual void update(float dt, Point pos);
    
    
};

#endif /* defined(__Animal_Squad__SkyBuildingRoof__) */
