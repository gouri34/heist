//
//  gound.h
//  Bear
//
//  Created by cong ku on 14-6-25.
//
//

#ifndef __Bear__gound__
#define __Bear__gound__

#define PTM_RATIO 32.0

#include "Terrain.h"


using namespace cocos2d;

class Ground : public Terrain
{
private:
    
    int groundBaseZ;
    float lastBuildingX;
    
    float texCoordXoffset;
    
public:
    
    
    PRFilledPolygon* terrain;
    std::vector<Sprite*>groundBuildings;

    
    /**
     Returns an autoreleased polygon.  Default triangulator is used (Ratcliff's).
     */
   
    static Ground* create(Layer *gameScene_, b2World *gameWorld_, Point pos, double _lastTexCoordX);
    virtual bool init(Layer *gameScene_, b2World *gameWorld_, Point pos, double _lastTexCoordX);
    ~Ground();

    void setupGroundBuildings(Point pos);
    void setGroundBuildings(Point pos);

    virtual void setVertices(Point pos);
    
    virtual void update(float dt, Point pos);
    

protected:

};

#endif /* defined(__Bear__gound__) */
