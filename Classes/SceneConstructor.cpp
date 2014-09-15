//
//  RoomConstructor.cpp
//  Animal_Squad
//
//  Created by cong ku on 8/31/14.
//
//

#include "SceneConstructor.h"

SceneInfo SceneConstructor::ConstructScene(std::string fileName)
{
    Document doc;
    std::string data = CCFileUtils::getInstance()->getStringFromFile(fileName);
    
    doc.Parse<kParseDefaultFlags>(data.c_str());
    rapidjson::Value &array = doc["gameobjects"];
    
    SceneInfo info;
    info.length = 0;
    arrayProcess(array, info);
    
    return info;
}

void SceneConstructor::arrayProcess(rapidjson::Value &array, SceneInfo &info)
{
    
    if (array.IsArray()) {
        if (array.Size() >=1) {
            for (int i = 0; i < array.Size(); i++) {
                rapidjson::Value & v = array[i];
                rapidjson::Value & arr = v["gameobjects"];
                if (!arr.IsNull()) {
                    arrayProcess(arr, info);
                }
                
                SceneData data = componentProcess(v);
                info.sceneInfoVec.push_back(data);
                
                if (data.lastPos > info.length) {
                    info.length = data.lastPos;
                }
            }
        }
    }
    else {
        return;
    }
    
}


SceneData SceneConstructor::componentProcess(rapidjson::Value &object)
{
    SceneData data;
    
    float rotation = (float)object["rotation"].GetDouble();
    float scalex = object["scalex"].GetDouble();
    float scaley = object["scaley"].GetDouble();
    float x = object["x"].GetDouble();
    float y = object["y"].GetDouble();
    float zorder = object["zorder"].GetInt();
    
    data.x = x;
    data.y = y;
    data.zOrder = zorder;
    data.scalex = scalex;
    data.scaley = scaley;
    data.rotation = rotation;
    
    
    rapidjson::Value & components = object["components"];
    if (components.Size() > 1) {
        data.isPhysics = true;
    }
    int i = 0;
    rapidjson::Value & component = components[i];
    
    std::string type = component["classname"].GetString();
    std::string filenamePath = component["fileData"]["path"].GetString();
    std::vector<std::string>filePathVec = Split(filenamePath, "/");
    std::string filename = filePathVec[filePathVec.size()-1];
    
    if (type.compare("CCArmature") == 0) {
        filename = Split(filename, ".")[0];
        if (filename.compare("running_grunt")==0 || filename.compare("agent")==0) {
            data.type = 3;
        }
        else {
            data.type = 2;
        }
        data.lastPos = data.x;
    }
    else {
        data.type = 1;
        
        Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(filename);
        data.lastPos = data.x + texture->getContentSize().width/2;
    }
    
    data.sourceName = filename;
    
    return data;
}

//----------string process related-----------------
std::vector<std::string> SceneConstructor::Split(const std::string& str, const char* del)
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
