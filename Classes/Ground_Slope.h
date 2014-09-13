//
//  Ground_Slope.h
//  Animal_Squad
//
//  Created by cong ku on 8/1/14.
//
//

#ifndef __Animal_Squad__Ground_Slope__
#define __Animal_Squad__Ground_Slope__

#define PTM_RATIO 32.0

#include "Terrain.h"

using namespace cocos2d;

class Ground_Slope : public Terrain
{
private:
    
    int groundBaseZ;
    float lastBuildingX;
    
public:
    
    bool isCurveUp = false;
    
    PRFilledPolygon* terrain;
    PRFilledPolygon* surface;
    
    /**
     Returns an autoreleased polygon.  Default triangulator is used (Ratcliff's).
     */
    
    static Ground_Slope* create(Point pos, bool curveUp,double _lastTexCoordX);
    virtual bool init(Point pos, bool curveUp,double _lastTexCoordX);
    ~Ground_Slope();
    
    void setupGroundBuildings(Point pos);
    void setGroundBuildings(Point pos);
    
    virtual void setVertices(Point pos);
    
    virtual void update(float dt, Point pos);
    
    
protected:
    
};

#endif /* defined(__Animal_Squad__Ground_Slope__) */
