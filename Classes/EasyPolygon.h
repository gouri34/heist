//
//  EasyPolygon.h
//  Bear
//
//  Created by cong ku on 14-7-2.
//
//

#ifndef __Bear__EasyPolygon__
#define __Bear__EasyPolygon__

#include "cocos2d.h"
#include "PRRatcliffTriangulator.h"

using namespace cocos2d;

class EasyPolygon : public Node {
    
private:
	
    
    int areaTrianglePointCount;
    
	Texture2D *texture;
    float textureWidth;
	BlendFunc blendFunc;
    
	Point *areaTrianglePoints;
	Point *textureCoordinates;
    
    PRRatcliffTriangulator* triangulator;
    void calculateTextureCoordinates();
    
public:
    
    EasyPolygon() {
        areaTrianglePointCount = 0;
        texture = NULL;
        areaTrianglePoints = NULL;
        textureCoordinates = NULL;
        triangulator = NULL;
    }
    
    /**
     Returns an autoreleased polygon.  Default triangulator is used (Ratcliff's).
     */
    static EasyPolygon* filledPolygonWithPointsAndTexture(Vector2dVector &polygonPoints, Texture2D * fillTexture);
    
    /**
     Returns an autoreleased filled poly with a supplied triangulator.
     */
    static EasyPolygon* filledPolygonWithPointsAndTextureUsingTriangulator(Vector2dVector &polygonPoints, Texture2D *fillTexture, PRRatcliffTriangulator *polygonTriangulator);
    
    /**
     Initialize the polygon.  polygonPoints will be triangulated.  Default triangulator is used (Ratcliff).
     */
    bool initWithPointsAndTexture(Vector2dVector &polygonPoints, Texture2D * fillTexture);
    
    /**
     Initialize the polygon.  polygonPoints will be triangulated using the supplied triangulator.
     */
    bool initWithPointsandTextureusingTriangulator(Vector2dVector &polygonPoints, Texture2D *fillTexture, PRRatcliffTriangulator* polygonTriangulator);
    
    void setPoints(Vector2dVector &points);
    
    void setTexture(Texture2D* texture2D);
	Texture2D* getTexture();
    void updateBlendFunc();
    void setBlendFunc(BlendFunc blendFuncIn);
    virtual void cleanup();
	virtual void draw(Renderer* renderer, const Mat4 &transform, bool transformUpdated);
    
protected:
    void onDraw(const Mat4 &transform, bool transformUpdated);
    CustomCommand _customCommand;
    
};

#endif /* defined(__Bear__EasyPolygon__) */
