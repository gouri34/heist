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
    int lastSeed;
    
public:
    
    
    PRFilledPolygon* terrain;
    PRFilledPolygon* surface;

    
    /**
     Returns an autoreleased polygon.  Default triangulator is used (Ratcliff's).
     */
   
    static Ground* create(Point pos, double _lastTexCoordX);
    virtual bool init(Point pos, double _lastTexCoordX);
    ~Ground();

    virtual void setVertices(Point pos);
    virtual void terrainSceneArrangement();

    
    virtual void update(float dt, Point pos);
    

protected:

};

#endif /* defined(__Bear__gound__) */
