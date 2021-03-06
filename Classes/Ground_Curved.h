//
//  Ground_Curved.h
//  Animal_Squad
//
//  Created by cong ku on 8/1/14.
//
//

#ifndef __Animal_Squad__Ground_Curved__
#define __Animal_Squad__Ground_Curved__

#define PTM_RATIO 32.0

#include "Terrain.h"

using namespace cocos2d;

class Ground_Curved : public Terrain
{
private:
    
    int groundBaseZ;
    float lastBuildingX;
    
    bool isCurveUp = false;
    bool isLowerOne = false;

    
public:
    
    
    
    PRFilledPolygon* ground;
    PRFilledPolygon* terrain;
    std::vector<Sprite*>groundBuildings;
    
    
    /**
     Returns an autoreleased polygon.  Default triangulator is used (Ratcliff's).
     */
    
    static Ground_Curved* create(Layer *gameScene_, b2World *gameWorld_, Point pos, bool curveUp, bool lowerOne,double _lastTexCoordX);
    virtual bool init(Layer *gameScene_, b2World *gameWorld_, Point pos, bool curveUp, bool lowerOne,double _lastTexCoordX);
    ~Ground_Curved();
    
    void setupGroundBuildings(Point pos);
    void setGroundBuildings(Point pos);
    
    virtual void setVertices(Point pos);
    
    virtual void update(float dt, Point pos);
    
    
protected:
    
};


#endif /* defined(__Animal_Squad__Ground_Curved__) */
