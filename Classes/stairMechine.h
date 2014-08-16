//
//  stairMechine.h
//  Zombie_Project
//
//  Created by cong ku on 14-3-17.
//
//

#ifndef __Zombie_Project__stairMechine__
#define __Zombie_Project__stairMechine__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Character.h"

using namespace cocos2d;
using namespace std;


class stairMechine {
    
public:
    static stairMechine* create(Rect _rect, b2Body *body);
    bool init(Rect _rect, b2Body *body);
    
    bool isFromTop = false;
    
    Rect rect;
    b2Body *stairBody;
    vector<b2Body*>bodysToIgnore;
    
    void stairProcess(Character* c);
    void addBodyToIgnore(b2Body* b);
    void removeBodyToIgnore(b2Body* b);
    
    bool shouldCancelContact(b2Body* b);
    
private:
    
};

#endif /* defined(__Zombie_Project__stairMechine__) */
