//
//  LeaserRayCallback.h
//  Zombie_Project
//
//  Created by cong ku on 14-3-25.
//
//

#ifndef Zombie_Project_LeaserRayCallback_h
#define Zombie_Project_LeaserRayCallback_h

#include "Box2D/Box2D.h"

class LeaserRayCallback : public b2RayCastCallback{
public:
    //std::vector<b2Body*> foundBodies;
    b2Vec2 cloestPoint;
    b2Fixture *lastFixture;
    /*virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
        foundBodies.push_back( fixture->GetBody() );
        return true;//keep going to find all fixtures in the query area
    }*/

    virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
    {
        
        FixtureType ft = fixture->GetFixtureType();
       
        if ((ft != f_ground) && (ft != f_zbody_head) && (ft != f_zbody_body) && (ft != f_zbody_leg))
        {
            return -1;
        }
        
        cloestPoint = point;
        lastFixture = fixture;
        
        return fraction;
    }
};


#endif
