//
//  stairMechine.cpp
//  Zombie_Project
//
//  Created by cong ku on 14-3-17.
//
//

#include "stairMechine.h"

stairMechine* stairMechine::create(cocos2d::Rect _rect, b2Body *body)
{
    stairMechine *stair = new stairMechine();
    if (stair && stair->init(_rect, body)) {
        return stair;
    }
    
    return NULL;
}

bool stairMechine::init(cocos2d::Rect _rect, b2Body *body)
{
    rect = _rect;
    stairBody = body;
    
    return true;
}

void stairMechine::stairProcess(Character *c)
{
    if (rect.intersectsRect(c->footRect)) {
        
        
        
        if (!isFromTop) {
            if (c->chType == player) {
                if (c->analogY > 0.1) {
                    c->want_stair_down = false;
                }
                else {
                    c->want_stair_down = true;
                }
            }
        }
        else {
            if (c->chType == player) {
                if (c->analogY < -0.1) {
                    c->want_stair_down = true;
                }
                else {
                    c->want_stair_down = false;
                }
            }
        }
        
        if (c->want_stair_down) {
            addBodyToIgnore(c->footBody);
        }
        else {
            removeBodyToIgnore(c->footBody);
        }
    }
    
    //printf("size of array = %lu\n", bodysToIgnore.size());
}

void stairMechine::addBodyToIgnore(b2Body *b)
{
    if (std::find(bodysToIgnore.begin(), bodysToIgnore.end(), b) == bodysToIgnore.end()) {
        bodysToIgnore.push_back(b);
        //printf("body added = %p\n", b);
    }
}

void stairMechine::removeBodyToIgnore(b2Body* b)
{
    if (std::find(bodysToIgnore.begin(), bodysToIgnore.end(), b) != bodysToIgnore.end()) {
        bodysToIgnore.erase(std::remove(bodysToIgnore.begin(), bodysToIgnore.end(), b), bodysToIgnore.end());
    }
}

bool stairMechine::shouldCancelContact(b2Body *b)
{
    if (!bodysToIgnore.empty()) {
        if (std::find(bodysToIgnore.begin(), bodysToIgnore.end(), b) != bodysToIgnore.end()) {
            return true;
        }
    }
   
    
    return false;
}