//
//  RoomConstructor.cpp
//  Animal_Squad
//
//  Created by cong ku on 8/31/14.
//
//

#include "RoomConstructor.h"

RoomInfo RoomConstructor::ConstructRoom(std::string fileName)
{
    Document doc;
    std::string data = CCFileUtils::getInstance()->getStringFromFile(fileName);
    
    doc.Parse<kParseDefaultFlags>(data.c_str());
    rapidjson::Value &array = doc["gameobjects"];
    
    std::vector<RoomData> roomInfoVec;
    arrayProcess(array, roomInfoVec);
    
    RoomInfo info;
    info.roomInfoVec = roomInfoVec;
    info.width = 1350;
    return info;
}

void RoomConstructor::arrayProcess(rapidjson::Value &array, std::vector<RoomData> &roomInfoVec)
{
    
    if (array.IsArray()) {
        if (array.Size() >=1) {
            for (int i = 0; i < array.Size(); i++) {
                rapidjson::Value & v = array[i];
                rapidjson::Value & arr = v["gameobjects"];
                if (!arr.IsNull()) {
                    arrayProcess(arr, roomInfoVec);
                }
                
                RoomData info = componentProcess(v);
                roomInfoVec.push_back(info);
            }
            
        }
    }
    else {
        return;
    }
    
}

RoomData RoomConstructor::componentProcess(rapidjson::Value &object)
{
    RoomData info;
    
    int rotation = object["rotation"].GetInt();
    float scalex = object["scalex"].GetDouble();
    float scaley = object["scaley"].GetDouble();
    float x = object["x"].GetDouble();
    float y = object["y"].GetDouble();
    float zorder = object["zorder"].GetInt();
    
    info.x = x;
    info.y = y;
    info.zOrder = zorder;
    info.scalex = scalex;
    info.scaley = scaley;
    info.rotation = rotation;
    
    
    rapidjson::Value & components = object["components"];
    if (components.Size() > 1) {
        info.isPhysics = true;
    }
    int i = 0;
    rapidjson::Value & component = components[i];
    
    std::string type = component["classname"].GetString();
    std::string filenamePath = component["fileData"]["path"].GetString();
    std::vector<std::string>filePathVec = Split(filenamePath, "/");
    std::string filename = filePathVec[filePathVec.size()-1];
    
    if (type.compare("CCArmature") == 0) {
        filename = Split(filename, ".")[0];
        info.type = 2;
        //printf("filename = %s, type = Armature, x = %f, y = %f\n", filename.c_str(), x, y);
    }
    else {
        info.type = 1;
        //printf("filename = %s, type = Sprite, x = %f, y = %f\n", filename.c_str(), x, y);
    }
    
    info.sourceName = filename;
    
    return info;
}

//----------string process related-----------------
std::vector<std::string> RoomConstructor::Split(const std::string& str, const char* del)
{
    size_t first = 0, second = 0;
    size_t end = str.size();
    size_t len = strlen(del);
    std::vector<std::string> tokens;
    while ((second = str.find(del,first)) != (std::string::npos)) {
        size_t dif = second - first;
        if (dif) {
            tokens.push_back(str.substr(first,dif));
        }
        first = second + len;
    }
    if (first != end) {
        tokens.push_back(str.substr(first));
    }
    return tokens;
    
}
