//
//  gound.cpp
//  Bear
//
//  Created by cong ku on 14-6-25.
//
//

//test building lengths
//tall_bld : 352
//mid_bld : 342
//short_bld : 370

//zscales:
//buildings : 10 - 20
//characters : 20 ++




#include "Ground.h"
#include "MapGenerator.h"

#include "StockPiles.h"
#include "BazookaEnemy.h"
#include "ShieldEnemy.h"
#include "Coin.h"
#include "Item.h"
#include "MonsterTrap.h"
#include "Panzer.h"

Ground* Ground::create(Point pos, double _lastTexCoordX)
{
    Ground *a = new Ground();
    if (a&&a->init(pos, _lastTexCoordX)) {
        return a;
    }
    return NULL;
}

bool Ground::init(Point pos, double _lastTexCoordX)
{
    gLayer = MapGenerator::GetInstance()->gameLayer;
    gWorld = MapGenerator::GetInstance()->gameWorld;
    
    startPos = Point(pos.x -1, pos.y);
    backgroundSetupPos = startPos;
    texCoordXoffset = (float)_lastTexCoordX - (int)_lastTexCoordX;
    
    cocos2d::Texture2D::TexParams params = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
    
    Texture2D* terrainTexture = Director::getInstance()->getTextureCache()->addImage("ground_terrain.png");
    terrainTexture->setTexParameters(params);
   
    Texture2D* surfaceTexture = Director::getInstance()->getTextureCache()->addImage("ground_surface.png");
    surfaceTexture->setTexParameters(params);
    surfaceTexWidth = 256;
    
    
    Vector2dVector empty;
    
    
    terrain = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, terrainTexture);
    surface = PRFilledPolygon::filledPolygonWithPointsAndTexture(empty, surfaceTexture);
    gLayer->addChild(terrain,40);
    gLayer->addChild(surface, 40);
    
    setVertices(pos);
    
    lastSetupPos=Point(startPos.x+200, startPos.y);
    
    return true;
}


void Ground::setVertices(Point pos)
{
    Point ground_lp;
    Point ground_rp;
    Point ground_lp_lower;
    Point ground_rp_lower;
    
    if ((pos.x - startPos.x) > MonsterOffset) {
        ground_lp = Point(pos.x - MonsterOffset, startPos.y);
        ground_rp = Point(pos.x + 1024, startPos.y);
    }
    else {
        ground_lp = Point(startPos.x, startPos.y);
        ground_rp = Point(pos.x + 1024, startPos.y);
    }
    
    offScreenPoint = ground_rp;

    if (ground_rp.x < backgroundSetupPos.x) {
        ground_rp = Point(backgroundSetupPos.x, ground_rp.y);
    }
    
    ground_lp_lower = Point(ground_lp.x, ground_lp.y - SURFACE_THICKNESS);
    ground_rp_lower = Point(ground_rp.x, ground_rp.y - SURFACE_THICKNESS);
    
    //Vector2dVector gvecs = makeVector(lp1, lp2, rp1, rp2);
    //ground->setPoints(gvecs);
    Vector2dVector points;
    Vector2dVector texCoords;
    float ground_l_texX = (ground_lp.x - startPos.x)/surfaceTexWidth + texCoordXoffset;
    ground_l_texX = ground_l_texX - (int)ground_l_texX;
    float ground_r_texX = ground_l_texX + (ground_rp.x - ground_lp.x)/surfaceTexWidth;
    points = makeVector(ground_lp_lower, ground_lp, ground_rp, ground_rp_lower);
    texCoords = makeVector(Point(ground_l_texX, 1.0), Point(ground_l_texX, 0.0), Point(ground_r_texX, 0.0), Point(ground_r_texX, 1.0));
    surface->customSetting(points, texCoords);
    
    Vector2dVector tvecs = pointsToVector(Point(ground_lp_lower.x, ground_lp_lower.y-800), ground_lp_lower, ground_rp_lower, Point(ground_rp_lower.x, ground_rp_lower.y - 800));
    terrain->setPoints(tvecs);
    
    //set physics
    Vector2dVector physicsVec;
    physicsVec.push_back(Vector2d(ground_lp.x, ground_lp.y));
    physicsVec.push_back(Vector2d(ground_rp.x, ground_rp.y));
    setPhysicsTerrain(physicsVec, &groundBody);

    lastTexCoordX = ground_r_texX;;
    lastPos = Point(ground_rp.x-1, ground_rp.y);
    groundYpos = ground_rp.y;
}

void Ground::terrainSceneArrangement()
{
    //background arrangement
    while (backgroundSetupPos.x < lastPos.x) {
        int seed = rand()%5;
        while (seed == lastSeed) {
            seed = rand()%5;
        }
        lastSeed = seed;
        
        string sceneName;
        switch (seed) {
            case 0:
                sceneName = "groundBack0";
                break;
            case 1:
                sceneName = "groundBack1";
                break;
            case 2:
                sceneName = "groundBack2";
                break;
            case 3:
                sceneName = "groundBack3";
                break;
            case 4:
                sceneName = "groundBack4";
                break;
            default:
                sceneName = "groundBack0";
                break;
        }
        
        int length = MapGenerator::GetInstance()->setupSceneWithInfo(sceneName, backgroundSetupPos);
        backgroundSetupPos = Point(backgroundSetupPos.x+length, backgroundSetupPos.y);
        validPos = backgroundSetupPos;
    }
    
    //objects arrangement
    if (MapGenerator::GetInstance()->enemyTimer <= 0&&(offScreenPoint.x>=lastSetupPos.x)) {
        MapGenerator::GetInstance()->enemyTimer = UniversalAttributes::GetInstance()->enemyTimer;
        int patternGap = 200;
        int randNum = rand()%11;
        if(randNum==1){
            if (MapGenerator::GetInstance()->enemyTimer>1.1) {
                Panzer *pz = Panzer::create("Panzer",  Point(offScreenPoint.x+40, offScreenPoint.y+100), 1, 1);
                MapGenerator::GetInstance()->enemies.push_back(pz);
                int lastX = offScreenPoint.x+300+patternGap;
                lastSetupPos = Point(lastX, startPos.y);
            }
            else{
                string arrName = "three_panzer_in_a_row";
                int length = MapGenerator::GetInstance()->setupSceneWithInfo(arrName, Point(offScreenPoint.x, offScreenPoint.y+100));
                int lastX = offScreenPoint.x+length+patternGap;
               
                lastSetupPos = Point(lastX, startPos.y);
            }
        }
        else if (randNum==2){
            if (MapGenerator::GetInstance()->enemyTimer>1.1) {
                //generate a sandbag and a bazooka soldier behind it
                StockPiles *sp = StockPiles::create("Sabaodui", Point(offScreenPoint.x,offScreenPoint.y), 0.4, 0.2);
                MapGenerator::GetInstance()->commonObjs.push_back(sp);
                BazookaEnemy *be = BazookaEnemy::create("PAObin", Point(offScreenPoint.x+70,offScreenPoint.y+50), 0.3, 0.35);
                MapGenerator::GetInstance()->enemies.push_back(be);
                int lastX = offScreenPoint.x + patternGap + 300;
                lastSetupPos = Point(lastX,startPos.y);
            }
            else{
                string arrName = "one_shieldenemy_on_one_guardtower";
                int length = MapGenerator::GetInstance()->setupSceneWithInfo(arrName, Point(offScreenPoint.x, offScreenPoint.y+100));
                int lastX = offScreenPoint.x+length+patternGap;
                lastSetupPos = Point(lastX, startPos.y);
            }
        }
        else if (randNum==3){
            if (MapGenerator::GetInstance()->enemyTimer>1.1) {
                ShieldEnemy *se = ShieldEnemy::create("dunbin", Point(offScreenPoint.x, offScreenPoint.y+50), 0.3, 0.3);
                MapGenerator::GetInstance()->enemies.push_back(se);
                int lastX = offScreenPoint.x+350+patternGap;
                lastSetupPos = Point(lastX, startPos.y);
            }
            else{
                string arrName = "three_shieldenemy_in_a_row";
                int length = MapGenerator::GetInstance()->setupSceneWithInfo(arrName, Point(offScreenPoint.x, offScreenPoint.y+100));
                int lastX = offScreenPoint.x+length+patternGap;
                
                lastSetupPos = Point(lastX, startPos.y);
            }
        }
        else if(randNum==4){
            string arrName = "two_bazookaenemy_and_one_shieldenemy";
            int length = MapGenerator::GetInstance()->setupSceneWithInfo(arrName, Point(offScreenPoint.x, offScreenPoint.y+50));
            int lastX = offScreenPoint.x+length+patternGap;
           
            lastSetupPos = Point(lastX, startPos.y);
        }
        else if (randNum==5){
            string arrName = "one_steelpiles_in_front_of_one_guardtower_with_a_bazookaenemy_on_it";
            int length = MapGenerator::GetInstance()->setupSceneWithInfo(arrName, Point(offScreenPoint.x, offScreenPoint.y+50));
            int lastX = offScreenPoint.x+length+patternGap;
            
            lastSetupPos = Point(lastX, startPos.y);
        }
        else if (randNum==6){
            string arrName = "three_mine_in_a_row";
            int length = MapGenerator::GetInstance()->setupSceneWithInfo(arrName, Point(offScreenPoint.x, offScreenPoint.y+50));
            int lastX = offScreenPoint.x+length+patternGap+100;
            lastSetupPos = Point(lastX, startPos.y);
        }
        else if (randNum==7){
            int randN = rand()%3;
            if (randN==1){
                StockPiles *s = StockPiles::create("shiguan", Point(offScreenPoint.x, offScreenPoint.y), 0.5, 0.5);
                MapGenerator::GetInstance()->commonObjs.push_back(s);
            }
            else{
                StockPiles *s = StockPiles::create("Sabaodui", Point(offScreenPoint.x, offScreenPoint.y), 0.4, 0.4);
                MapGenerator::GetInstance()->commonObjs.push_back(s);
            }
            int lastX = offScreenPoint.x+patternGap;
            lastSetupPos = Point(lastX,startPos.y);
        }
        else if (randNum==8&&MapGenerator::GetInstance()->enemyTimer<=1.1){
            MonsterTrap *mt = MonsterTrap::create("ci",  Point(offScreenPoint.x, offScreenPoint.y+80), 0.55, 0.4);
            mt->armature->setColor(Color3B::RED);
            MapGenerator::GetInstance()->enemies.push_back(mt);
            int lastX = offScreenPoint.x+patternGap+300;
            lastSetupPos = Point(lastX,startPos.y);
        }
        
        else if (randNum==9){
            string arrName = "enemySetup1";
            int length = MapGenerator::GetInstance()->setupSceneWithInfo(arrName, Point(offScreenPoint.x, offScreenPoint.y+100));
            int lastX = offScreenPoint.x+length+patternGap;
            
            lastSetupPos = Point(lastX, startPos.y);
            length = MapGenerator::GetInstance()->setupSceneWithInfo(arrName, Point(offScreenPoint.x+length-50, offScreenPoint.y+100));
            lastX = offScreenPoint.x+length+patternGap;
            lastSetupPos = Point(lastX, startPos.y);
        }

        else{
            string arrName = "enemySetup1";
            int length = MapGenerator::GetInstance()->setupSceneWithInfo(arrName, Point(offScreenPoint.x, offScreenPoint.y+100));
            int lastX = offScreenPoint.x+length+patternGap;
            
            lastSetupPos = Point(lastX, startPos.y);

        }


    }
    else {
        coinGenerator();
    }
    
    if (lastSetupPos.x > backgroundSetupPos.x) {
        validPos = lastSetupPos;
    }

}

void Ground::coinGenerator()
{
    //Coin generator is used to generate coins throughout the game, However i think the traditional random method will not do good for this generator,
    //I think Double random mechanism is best suitable for this game, first random will give higher ratio to generate coin between 75-150 on Y-axis.
    //The second random will rand a actual number between two boundries.
    if (offScreenPoint.x > nextCoinX) {
        //calculate the y-axis of the coin
        int rand1 = rand()%15;
        int rand2;
        if (rand1<=11)
            rand2 = rand()%150+65;
        else
            rand2 = rand()%230+160;
        //spawn item
        int rand3 = rand()%55;
        if (rand3>53&&prevItemX<offScreenPoint.x) {
            Item *item = Item::create(gLayer, gWorld, Point(offScreenPoint.x+200,offScreenPoint.y+rand2));
            prevItemX = offScreenPoint.x+2000;
            MapGenerator::GetInstance()->items.push_back(item);
            nextCoinX = offScreenPoint.x + 500;
        }
        else{
            Coin *c = Coin::create(gLayer, gWorld, Point(offScreenPoint.x+200,offScreenPoint.y+rand2));
            MapGenerator::GetInstance()->items.push_back(c);
            nextCoinX = offScreenPoint.x + 500;
        }
    }
}


void Ground::update(float dt, cocos2d::Point pos)
{
    Terrain::update(dt, pos);
    if (!dead) {
        terrainSceneArrangement();
        setVertices(pos);
    }
    else {
        if ((pos.x - lastPos.x) > MonsterOffset) {
            afterDeath = true;
        }
    }
}



Ground::~Ground()
{
    gLayer->removeChild(terrain, true);
    gLayer->removeChild(surface, true);
    
    if (groundBody != NULL) {
        gWorld->DestroyBody(groundBody);
        groundBody = NULL;
    }
}