//
//  MyContactListener.m
//  Box2DPong
//
//  Created by Ray Wenderlich on 2/18/10.
//  Copyright 2010 Ray Wenderlich. All rights reserved.
//

#import "MyContactListener.h"

MyContactListener::MyContactListener() {
}

MyContactListener::~MyContactListener() {
    
}



void MyContactListener::BeginContact(b2Contact* contact) {
    // We need to copy out the data because the b2Contact passed in
    // is reused.
   /* b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB(), worldManifold};
    _contacts.push_back(myContact);*/
    bool curGroundSet = false;
   // bool stairCheck = false;
    bool lowestGroundCheck = false;
    //bool deadBodyCheck = false;
    
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    
    b2Fixture* footFixture;
    b2Fixture* gFixture;
    if (fixtureA->GetFixtureType() == f_foot) {
        footFixture = fixtureA;
        gFixture = fixtureB;
        curGroundSet = true;
    }
    else if (fixtureB->GetFixtureType() == f_foot) {
        footFixture = fixtureB;
        gFixture = fixtureA;
        curGroundSet = true;
    }
    
    
    
    b2Fixture *dbodyFixture;
    b2Fixture *groundFixture;
    if (fixtureA->GetFixtureType() == f_bodydead) {
        lowestGroundCheck = true;
        dbodyFixture = fixtureA;
        groundFixture = fixtureB;
    }
    else if (fixtureB->GetFixtureType() == f_bodydead) {
        lowestGroundCheck = true;
        dbodyFixture = fixtureB;
        groundFixture = fixtureA;
    }
    
   /* if (lowestGroundCheck == true) {
        
        Character *ch = (Character*)dbodyFixture->GetBody()->GetUserData();
        b2Body *cur_gournd = groundFixture->GetBody();
        
        b2WorldManifold worldManifold;
        contact->GetWorldManifold(&worldManifold);
        b2Vec2 pos = worldManifold.points[0];
        
        if (ch->groundStandOn == NULL) {
            ch->groundStandOn = cur_gournd;
            ch->lowestGroundContact = pos;
        }
        else {
            if (ch->lowestGroundContact.y > pos.y) {
                ch->groundStandOn = cur_gournd;
                ch->lowestGroundContact = pos;
            }
        }
        
        if (cur_gournd != ch->groundStandOn) {
            contact->SetEnabled(false);
        }
        else {
            contact->SetEnabled(true);
        }
    }
    */
   
   
}


void MyContactListener::EndContact(b2Contact* contact) {
    
   /* b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB(),worldManifold};
    std::vector<MyContact>::iterator pos;
    pos = std::find(_contacts.begin(), _contacts.end(), myContact);
    if (pos != _contacts.end()) {
        _contacts.erase(pos);
    }*/
    
    //contact->SetEnabled(true);
}



void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
    //NSLog(@"preSolve processing");
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    b2Body *b1 = fixtureA->GetBody();
    b2Body *b2 = fixtureB->GetBody();
  /*  if (fixtureA->GetFixtureType() == f_arrow && fixtureB->GetFixtureType() != f_arrow) {
        b2Vec2 speedVec = b1->GetLinearVelocity();
        float arrowSpeed = sqrt(pow(speedVec.x, 2)+pow(speedVec.y, 2));
        if (arrowSpeed > 10) {
            StickyInfo si;
            si.arrowBody = fixtureA->GetBody();
            si.targetBody = fixtureB->GetBody();
            si.normalImpulse = arrowSpeed;
            b2WorldManifold worldManifold;
            contact->GetWorldManifold(&worldManifold);
            si.worldnamifold = worldManifold;
            collisionsMakeStiky.push_back(si);
            
            contact->SetEnabled(false);
        }
    }
    
    if (fixtureB->GetFixtureType() == f_arrow && fixtureA->GetFixtureType() != f_arrow) {
        b2Vec2 speedVec = b2->GetLinearVelocity();
        float arrowSpeed = sqrt(pow(speedVec.x, 2)+pow(speedVec.y, 2));
        if (arrowSpeed > 10) {
            StickyInfo si;
            si.arrowBody = fixtureB->GetBody();
            si.targetBody = fixtureA->GetBody();
            si.normalImpulse = arrowSpeed;
            b2WorldManifold worldManifold;
            contact->GetWorldManifold(&worldManifold);
            si.worldnamifold = worldManifold;
            collisionsMakeStiky.push_back(si);
            
            contact->SetEnabled(false);
        }
    }
    */
    
    
    /*b2Fixture *bulletFixture = NULL;
    b2Fixture *bodyFixture = NULL;
    if (fixtureA->GetFixtureType() == f_bullet) {
        bulletFixture = fixtureA;
        bodyFixture = fixtureB;
    }
    
    if (fixtureB->GetFixtureType() == f_bullet) {
        bulletFixture = fixtureB;
        bodyFixture = fixtureA;
    }
    
    if (bulletFixture) {
        PistolBullet *pb = (PistolBullet*)bulletFixture->GetBody()->GetUserData();
        if (!pb->damageDone) {
            bulletContact bc;
            bc.bulletBody = bulletFixture->GetBody();
            bc.hitBody = bodyFixture->GetBody();
            b2WorldManifold worldManifold;
            contact->GetWorldManifold(&worldManifold);
            bc.worldnamifold = worldManifold;
            bulletContacts.push_back(bc);
            contact->SetEnabled(false);
        }
    }*/

}




void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
    
   /* b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    float normalImpulse = impulse->normalImpulses[0];

    MyContact mycontact = {contact->GetFixtureA(), contact->GetFixtureB(), worldManifold, normalImpulse};
    
    std::vector<MyContact>::iterator pos;
    pos = std::find(_contacts.begin(), _contacts.end(), mycontact);
    (*pos).worldnamifold = worldManifold;
    (*pos).normalImpulse = normalImpulse;*/
    
    
  /*  b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    
    if (fixtureA->GetFixtureType() == f_arrow && fixtureB->GetFixtureType() != f_arrow) {
        if (impulse->normalImpulses[0] > 30) {
            StickyInfo si;
            si.arrowBody = fixtureA->GetBody();
            si.targetBody = fixtureB->GetBody();
            si.normalImpulse = impulse->normalImpulses[0];
            b2WorldManifold worldManifold;
            contact->GetWorldManifold(&worldManifold);
            si.worldnamifold = worldManifold;
            collisionsMakeStiky.push_back(si);
        }
    }
    
    if (fixtureB->GetFixtureType() == f_arrow && fixtureA->GetFixtureType() != f_arrow) {
        if (impulse->normalImpulses[0] > 25) {
            StickyInfo si;
            si.arrowBody = fixtureB->GetBody();
            si.targetBody = fixtureA->GetBody();
            si.normalImpulse = impulse->normalImpulses[0];
            b2WorldManifold worldManifold;
            contact->GetWorldManifold(&worldManifold);
            si.worldnamifold = worldManifold;
            collisionsMakeStiky.push_back(si);
        }
    }*/
    //NSLog(@"the pos = %f, %f", pos.x, pos.y);
}

