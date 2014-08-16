//
//  MyQueryCallback.h
//  fusion
//
//  Created by cong ku on 14-1-29.
//
//

#ifndef __fusion__MyQueryCallback__
#define __fusion__MyQueryCallback__

#include "Box2D/Box2D.h"

class MyQueryCallback : public b2QueryCallback {
public:
    std::vector<b2Body*> foundBodies;
    
    bool ReportFixture(b2Fixture* fixture) {
        foundBodies.push_back( fixture->GetBody() );
        return true;//keep going to find all fixtures in the query area
    }
};

#endif /* defined(__fusion__MyQueryCallback__) */
