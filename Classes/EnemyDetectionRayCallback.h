//
//  EnemyDetectionRayCallback.h
//  Zombie_Project
//
//  Created by cong ku on 14-3-29.
//
//

#ifndef Zombie_Project_EnemyDetectionRayCallback_h
#define Zombie_Project_EnemyDetectionRayCallback_h

#include "Box2D/Box2D.h"
#include "Character.h"

class EnemyDetectionRayCallback : public b2RayCastCallback
{
public:
    //std::vector<b2Body*> foundBodies;
    b2Vec2 cloestPoint;
    Character *EnemyAimed;
    /*virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
     foundBodies.push_back( fixture->GetBody() );
     return true;//keep going to find all fixtures in the query area
     }*/
    
    virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
    {
        
        FixtureType ft = fixture->GetFixtureType();
        
        if (ft == f_foot) {
            b2Body *b = fixture->GetBody();
            EnemyAimed = (Character*)b->GetUserData();
            
            return fraction;
        }
        else {
            return -1;
        }
       
        
    }
    
};


#endif
