//
//  GroundBuilding.h
//  Animal_Squad
//
//  Created by cong ku on 8/5/14.
//
//

#ifndef __Animal_Squad__GroundBuilding__
#define __Animal_Squad__GroundBuilding__

#include "Building.h"

class GroundBuilding : public Building {
private:
    
    
public:
    static GroundBuilding* create(Point pos);
    virtual bool init(Point pos);
    virtual ~GroundBuilding();
    
    virtual void setDead();
    virtual void setVertices(Point pos);
    virtual void update(float dt, Point pos);
    
};

#endif /* defined(__Animal_Squad__GroundBuilding__) */
