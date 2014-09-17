//
//  Terrain.cpp
//  Animal_Squad
//
//  Created by cong ku on 7/31/14.
//
//

#include "Terrain.h"

void Terrain::setVertices(Point pos){
    
}

void Terrain::update(float dt, Point pos)
{
    
}

void Terrain::setDead()
{
    dead = true;
}

Vector2dVector Terrain::pointsToVector(Point p1, Point p2, Point p3, Point p4)
{
    Vector2dVector v;
    v.push_back(Vector2d(p1.x, p1.y));
    v.push_back(Vector2d(p2.x, p2.y));
    v.push_back(Vector2d(p3.x, p3.y));
    v.push_back(Vector2d(p4.x, p4.y));
    
    return v;
}

Vector2dVector Terrain::makeVector(Point v1, Point v2, Point v3, Point v4)
{
    Vector2dVector v;
    v.push_back(Vector2d(v2.x, v2.y));
    v.push_back(Vector2d(v3.x, v3.y));
    v.push_back(Vector2d(v1.x, v1.y));
    v.push_back(Vector2d(v1.x, v1.y));
    v.push_back(Vector2d(v3.x, v3.y));
    v.push_back(Vector2d(v4.x, v4.y));
    
    return v;
}
void Terrain::setPhysicsTerrain(Vector2dVector v, b2Body **b)
{
    b2BodyDef abodyDef;
    
	abodyDef.bullet = false;
	abodyDef.type = b2_staticBody;
    b2EdgeShape astaticBox;
    
    b2FixtureDef fixtureDef;
	fixtureDef.shape = &astaticBox;
	fixtureDef.density = 5.0;
	fixtureDef.friction = 2.5;
	fixtureDef.restitution = 0;
    fixtureDef.isSensor = false;
    fixtureDef.fixturetype = f_ground;
    
    
    if (*b) {
        gWorld->DestroyBody(*b);
        *b = NULL;
        if (groundBody == NULL) {
        }
    }
    
    
    *b = gWorld->CreateBody(&abodyDef);
    for (int i = 1; i < v.size(); i++) {
        Vector2d vec1 = v[i-1];
        Vector2d vec2 = v[i];
        
        b2Vec2 vv1 = b2Vec2(vec1.GetX()/PTM_RATIO, vec1.GetY()/PTM_RATIO);
        b2Vec2 vv2 = b2Vec2(vec2.GetX()/PTM_RATIO, vec2.GetY()/PTM_RATIO);
        
        astaticBox.Set(vv1, vv2);
        (*b)->CreateFixture(&fixtureDef);
    }
    
}

void Terrain::terrainSceneArrangement()
{
    
}


Terrain::~Terrain()
{
    
}