//
//  ScreenShaker.h
//  Animal_Squad
//
//  Created by wd on 9/22/14.
//
//screen shaker

#ifndef __Animal_Squad__ScreenShaker__
#define __Animal_Squad__ScreenShaker__

#include <iostream>
#include "cocos2d.h"
#include "UniversalAttributes.h"

using namespace cocos2d;

class ScreenShaker : public ActionInterval
{
public:
    ScreenShaker();
    
    // Create the action with a time and a strength (same in x and y)
    // @param d vib time
    // @param strength vib amplitude
    static ScreenShaker* create(float d, float strength);
    // Create the action with a time and strengths (different in x and y)
    static ScreenShaker* create(float d, float strength_x, float strength_y);
    bool initWithDuration(float d, float strength_x, float strength_y);
    
    //Override the parent functions(requirred)
    virtual ScreenShaker* clone() const override;
    virtual ScreenShaker* reverse(void) const override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    virtual void stop(void);
    
private:
    float sceneinitposx = 0;
    float sceneinitposy = 0;
protected:
    // Initial position of the shaked node
    float _initial_x, _initial_y;
    // Strength of the action
    float _strength_x, _strength_y;
};

#endif /* defined(__Animal_Squad__ScreenShaker__) */
