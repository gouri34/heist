//
//  RoomConstructor.h
//  Animal_Squad
//
//  Created by cong ku on 8/31/14.
//
//

#ifndef __Animal_Squad__RoomConstructor__
#define __Animal_Squad__RoomConstructor__

#import "cocos2d.h"
#include "Box2D/Box2D.h"

#include "json/rapidjson.h"
#include "json/document.h"
USING_NS_CC;
using namespace rapidjson;

typedef struct {
    float x;
    float y;
    int zOrder;
    float scalex;
    float scaley;
    int rotation;
    
    int type; // 1:sprite, 2:armature
    bool isPhysics;
    
    std::string sourceName;
    
}RoomData;

typedef struct {
    std::vector<RoomData> roomInfoVec;
    int width;
} RoomInfo;

class RoomConstructor {

private:
    
public:
    
    static RoomInfo ConstructRoom(std::string fileName);
    static void arrayProcess(rapidjson::Value &array, std::vector<RoomData> &roomInfoVec);
    static RoomData componentProcess(rapidjson::Value &object);
    
    static std::vector<std::string> Split(const std::string& str, const char* del);

};


#endif /* defined(__Animal_Squad__RoomConstructor__) */
