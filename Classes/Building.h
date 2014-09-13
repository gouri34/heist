//
//  Building.h
//  Animal_Squad
//
//  Created by cong ku on 14-7-4.
//
//

#ifndef __Animal_Squad__Building__
#define __Animal_Squad__Building__

#include "Terrain.h"

using namespace cocos2d;

class Building : public Terrain
{
public:
    
    b2Body *roofBody = NULL;
    b2Body *ceilingBody = NULL;
    
    
    int buildingType = 0;
    int height;
    
    
    PRFilledPolygon* higherFrontView;
    PRFilledPolygon* lowerFrontView;
    PRFilledPolygon* wall;
    PRFilledPolygon* terrain;
    
    Size floorTextureSize;
    Size wallTextureSize;
    Size viewTextureSize;
    
    static Building* create(Point pos);
    virtual bool init(Point pos);
    virtual ~Building();
    
    
    
    
};

#endif /* defined(__Animal_Squad__Building__) */
