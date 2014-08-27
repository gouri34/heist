//
//  ElevatorShaft.h
//  Animal_Squad
//
//  Created by cong ku on 8/11/14.
//
//

#ifndef __Animal_Squad__ElevatorShaft__
#define __Animal_Squad__ElevatorShaft__

#include "Building.h"

typedef  enum {
    Waiting, GoingUp, GoingDown, Done, AfterDone
} ElevatorStatus;


class ElevatorShatf : public Building {
private:
    Sprite *elevator;
    b2Body *elevator_body;
    ElevatorStatus elevator_status;
    int destinationY;
    int shaft_height;
    
public:
    
    static ElevatorShatf* create(Layer *gameScene_, b2World *gameWorld_, Point pos, int s_heigth);
    virtual bool init(Layer *gameScene_, b2World *gameWorld_, Point pos, int s_heigth);
    virtual ~ElevatorShatf();
    
    virtual void setDead();
    void setupElevator();
    virtual void setVertices(Point pos);
    virtual void update(float dt, Point pos);
    
};

#endif /* defined(__Animal_Squad__ElevatorShaft__) */