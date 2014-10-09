//
//  ScreenShaker.cpp
//  Animal_Squad
//
//  Created by wd on 9/22/14.
//
//

#include "ScreenShaker.h"

// not really useful, but I like clean default constructors
ScreenShaker::ScreenShaker() : _strength_x(0), _strength_y(0), _initial_x(0), _initial_y(0)
{
}

ScreenShaker* ScreenShaker::create(float d, float strength )
{
    // call other construction method with twice the same strength
    return create( d, strength, strength );
}

ScreenShaker* ScreenShaker::create(float duration, float strength_x, float strength_y)
{
    ScreenShaker *p_action = new ScreenShaker();
    p_action->initWithDuration(duration, strength_x, strength_y);
    p_action->autorelease();
    
    return p_action;
}

bool ScreenShaker::initWithDuration(float duration, float strength_x, float strength_y)
{
    if (CCActionInterval::initWithDuration(duration)&&UniversalAttributes::GetInstance()->inMenuMode==false)
    {
        _strength_x = strength_x;
        _strength_y = strength_y;
        
        return true;
    }
    
    return false;
}

// Helper function. I included it here so that you can compile the whole file
// it returns a random value between min and max included
float fgRangeRand( float min, float max )
{
    float rnd = ((float)rand()/(float)RAND_MAX);
    return rnd*(max-min)+min;
}

void ScreenShaker::update(float time)
{
    float randx = fgRangeRand( -_strength_x, _strength_x );
    float randy = fgRangeRand( -_strength_y, _strength_y );
    
    // move the target to a ScreenShakerd position
    _target->setPosition(Vec2(_initial_x + randx,
                              _initial_y + randy));
}

ScreenShaker* ScreenShaker::clone(void) const
{
    auto a = new ScreenShaker();
    a->initWithDuration(_duration, _strength_x, _strength_y);
    a->autorelease();
    return a;
}

ScreenShaker* ScreenShaker::reverse() const
{
    return ScreenShaker::create(_duration, -_strength_x, -_strength_y);
}

void ScreenShaker::startWithTarget(Node *target)
{
    CCActionInterval::startWithTarget(target);
    
    // save the initial position
    _initial_x = target->getPosition().x;
    _initial_y = target->getPosition().y;
}

void ScreenShaker::stop(void)
{
    // Action is done, reset clip position
    //_target->setPosition(Vec2( _initial_x, _initial_y ) );
    _target->setPosition(Vec2( sceneinitposx, sceneinitposy ) );

    
    CCActionInterval::stop();
}

