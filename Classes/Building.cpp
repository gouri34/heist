//
//  Building.cpp
//  Animal_Squad
//
//  Created by cong ku on 14-7-4.
//
//

#include "Building.h"
#include "MapGenerator.h"

#define PTM_RATIO 32.0

#define MonsterOffset 300
#define WallHeight 300


Building* Building::create(Point pos)
{
    Building *a = new Building();
    if (a&&a->init(pos)) {
        return a;
    }
    return NULL;
}

bool Building::init(Point pos)
{
    gLayer = MapGenerator::GetInstance()->gameLayer;
    gWorld = MapGenerator::GetInstance()->gameWorld;
    
    startPos = pos;
    
    
    
    Vector2dVector empty;
    
    
    setVertices(pos);
    
    
    
    
    return true;
}



Building::~Building()
{
    
}
