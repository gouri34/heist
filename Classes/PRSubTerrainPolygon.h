//
//  PRSubTerrainPolygon.h
//  fusion
//
//  Created by cong ku on 11/27/13.
//
//

#ifndef __fusion__PRSubTerrainPolygon__
#define __fusion__PRSubTerrainPolygon__

#include "cocos2d.h"
#include "PRRatcliffTriangulator.h"

using namespace cocos2d;

class PRSubTerrainPolygon : public Node {
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
    
    PRSubTerrainPolygon() {
        areaTrianglePointCount = 0;
        texture = NULL;
        areaTrianglePoints = NULL;
        textureCoordinates = NULL;
        triangulator = NULL;
    }
    
    /**
     Returns an autoreleased polygon.  Default triangulator is used (Ratcliff's).
     */
    static PRSubTerrainPolygon* filledPolygonWithPointsAndTexture(Vector2dVector &mainPoints, Vector2dVector &subPoints, Texture2D * fillTexture);
    
    /**
     Returns an autoreleased filled poly with a supplied triangulator.
     */
    static PRSubTerrainPolygon* filledPolygonWithPointsAndTextureUsingTriangulator(Vector2dVector &mainPoints, Vector2dVector &subPoints, Texture2D *fillTexture, PRRatcliffTriangulator *polygonTriangulator);
    
    /**
     Initialize the polygon.  polygonPoints will be triangulated.  Default triangulator is used (Ratcliff).
     */
    bool initWithPointsAndTexture(Vector2dVector &mainPoints, Vector2dVector &subPoints, Texture2D * fillTexture);
    
    /**
     Initialize the polygon.  polygonPoints will be triangulated using the supplied triangulator.
     */
    bool initWithPointsandTextureusingTriangulator(Vector2dVector &mainPoints, Vector2dVector &subPoints,Texture2D *fillTexture, PRRatcliffTriangulator* polygonTriangulator);
    
    void setPoints(Vector2dVector &mainPoints, Vector2dVector &subPoints);
    
    void setTexture(Texture2D* texture2D);
	Texture2D* getTexture();
    void updateBlendFunc();
    void setBlendFunc(BlendFunc blendFuncIn);
    virtual void cleanup();
	virtual void draw(Renderer* renderer, const kmMat4 &transform, bool transformUpdated);
    
protected:
    void onDraw(const Mat4 &transform, bool transformUpdated);
    CustomCommand _customCommand;

    
};




#endif /* defined(__fusion__PRSubTerrainPolygon__) */
