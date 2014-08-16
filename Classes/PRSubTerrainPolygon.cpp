//
//  PRSubTerrainPolygon.cpp
//  fusion
//
//  Created by cong ku on 11/27/13.
//
//

#include "PRSubTerrainPolygon.h"
#include "PRRatcliffTriangulator.h"


#define __GLES2 1

PRSubTerrainPolygon* PRSubTerrainPolygon::filledPolygonWithPointsAndTexture(Vector2dVector &mainPoints, Vector2dVector &subPoints, Texture2D * fillTexture) {
    
    PRSubTerrainPolygon *pRet = new PRSubTerrainPolygon();
    if(pRet && pRet->initWithPointsAndTexture(mainPoints, subPoints,fillTexture))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
    
}


PRSubTerrainPolygon* PRSubTerrainPolygon::filledPolygonWithPointsAndTextureUsingTriangulator(Vector2dVector &mainPoints, Vector2dVector &subPoints, Texture2D *fillTexture, PRRatcliffTriangulator *polygonTriangulator) {
    
    PRSubTerrainPolygon *pRet = new PRSubTerrainPolygon();
    if(pRet && pRet->initWithPointsandTextureusingTriangulator(mainPoints, subPoints, fillTexture,polygonTriangulator))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool PRSubTerrainPolygon::initWithPointsAndTexture(Vector2dVector &mainPoints, Vector2dVector &subPoints, Texture2D * fillTexture) {
    
    
    PRRatcliffTriangulator *polygonTriangulator = new PRRatcliffTriangulator();
    return initWithPointsandTextureusingTriangulator(mainPoints, subPoints, fillTexture, polygonTriangulator);;
}


bool PRSubTerrainPolygon::initWithPointsandTextureusingTriangulator(Vector2dVector &mainPoints, Vector2dVector &subPoints,Texture2D *fillTexture, PRRatcliffTriangulator* polygonTriangulator) {
    
    triangulator = polygonTriangulator;
    
#ifdef __GLES2
    setGLProgram(CCShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
#endif
    
   // self.shaderProgram = [[CCShaderCache sharedShaderCache] programForKey:GLProgram::SHADER_NAME_POSITION_TEXTURE];
    setTexture(fillTexture);
    setPoints(mainPoints, subPoints);
    
	return true;
}

void PRSubTerrainPolygon::setPoints(Vector2dVector &mainPoints, Vector2dVector &subPoints) {
   // printf("sdfdsf");
    int n = 0;
    
    float atlasWidth = texture->getPixelsWide();
    
    float mainDistance = 0;
    float subDistance = 0;
    
    CC_SAFE_FREE(areaTrianglePoints);
    CC_SAFE_FREE(textureCoordinates);
    
    //Vector2dVector maintriPoints = PRRatcliffTriangulator::triangulateVertices(mainPoints);
    Vector2dVector maintriPoints = mainPoints;
    //Vector2dVector subtriPoints = PRRatcliffTriangulator::triangulateVertices(subPoints);
    Vector2dVector subtriPoints = subPoints;

   // printf("size of sub = %lu\n", subPoints.size());
    areaTrianglePointCount = (int)(mainPoints.size()-1)*4;
    areaTrianglePoints = (Point*) malloc(sizeof(Point) * (mainPoints.size()-1)*4);
    textureCoordinates = (Point*) malloc(sizeof(Point) * (mainPoints.size()-1)*4);
    

    
    for (int i = 0; i < mainPoints.size()-1; i++) {

        //Vector2d v = (Vector2d)triangulatedPoints.at(i);
        //areaTrianglePoints[i] = PointMake(v.GetX(), v.GetY());
        Vector2d v1 = subtriPoints.at(i);
        Vector2d v2 = subtriPoints.at(i+1);

        areaTrianglePoints[n] = Point(v1.GetX(), v1.GetY());
        textureCoordinates[n++] = Point(subDistance/atlasWidth, 1.0);
        subDistance += Point(v2.GetX() - v1.GetX(), v2.GetY() - v1.GetY()).getLength();
        areaTrianglePoints[n] = Point(v2.GetX(), v2.GetY());
        textureCoordinates[n++] = Point(subDistance/atlasWidth, 1.0);
        //printf("v1 x = %f", v1.GetX());
        //printf("v1 x = %f", v2.GetX());
        
        Vector2d v3 = maintriPoints.at(i);
        Vector2d v4 = maintriPoints .at(i+1);
        areaTrianglePoints[n] = Point(v3.GetX(), v3.GetY());
        textureCoordinates[n++] = Point(mainDistance/atlasWidth, 0);
        mainDistance += Point(v2.GetX()-v1.GetX(), v2.GetY() - v1.GetY()).getLength();
        areaTrianglePoints[n] = Point(v4.GetX(), v4.GetY());
        textureCoordinates[n++] = Point(mainDistance/atlasWidth, 0);
        
    }
    
    //printf("n = %d\n", n);
    
   // calculateTextureCoordinates();
}

void PRSubTerrainPolygon::cleanup()
{
    CC_SAFE_RELEASE_NULL  (texture);
	CC_SAFE_FREE(triangulator);
    CC_SAFE_FREE(areaTrianglePoints);
    CC_SAFE_FREE(textureCoordinates);
    CCNode::cleanup();
}

void PRSubTerrainPolygon::calculateTextureCoordinates() {
    int minX = 0;
    float minY = 0;
    minX = (int)(areaTrianglePoints[0].x / textureWidth);
    minY = (float)(areaTrianglePoints[0].y / textureWidth);
    
    for (int j = 0; j < areaTrianglePointCount; j++) {
        textureCoordinates[j].x = areaTrianglePoints[j].x*(1.0f / texture->getPixelsWide() * CC_CONTENT_SCALE_FACTOR()) - minX;
        textureCoordinates[j].y = areaTrianglePoints[j].y*(1.0f / texture->getPixelsWide() * CC_CONTENT_SCALE_FACTOR());

        textureCoordinates[j].y = 1.0f - textureCoordinates[j].y;
    }
}

void PRSubTerrainPolygon::updateBlendFunc() {
    // it's possible to have an untextured sprite
    if( !texture || !texture->hasPremultipliedAlpha() ) {
        blendFunc.src = GL_SRC_ALPHA;
        blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    } else {
        blendFunc.src = CC_BLEND_SRC;
        blendFunc.dst = CC_BLEND_DST;
    }
}

void PRSubTerrainPolygon::setBlendFunc(BlendFunc blendFuncIn) {
    blendFunc = blendFuncIn;
}

void PRSubTerrainPolygon::setTexture(Texture2D* texture2D) {
    
    CCAssert(texture2D, "NO TEXTURE SET");
    CC_SAFE_RELEASE(texture);
	
    texture = texture2D;
    CC_SAFE_RETAIN(texture);
    
	cocos2d::Texture2D::TexParams texParams = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	texture->setTexParameters(texParams);
    textureWidth = texture->getPixelsWide();

    
    updateBlendFunc();
	calculateTextureCoordinates();
    
}

Texture2D* PRSubTerrainPolygon::getTexture() {
    return texture;
}

void PRSubTerrainPolygon::draw(Renderer* renderer, const Mat4 &transform, bool transformUpdated) {
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(PRSubTerrainPolygon::onDraw, this, transform, transformUpdated);
    renderer->addCommand(&_customCommand);
}

void PRSubTerrainPolygon::onDraw(const Mat4 &transform, bool transformUpdated)
{
    //kmGLPushMatrix();
    //kmGLLoadMatrix(&transform);

    //draw
    CC_NODE_DRAW_SETUP();
    
    cocos2d::GL::bindTexture2D(texture->getName());
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    GL::blendFunc(blendFunc.src, blendFunc.dst);
    
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
    
    if(sizeof(Point) == sizeof(Vec2)) {
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, areaTrianglePoints);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, textureCoordinates);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)areaTrianglePointCount);
    } else {
        Vec2* newAreaTrianglePoints = new Vec2[areaTrianglePointCount];
        Vec2* newTextureCoordinates = new Vec2[areaTrianglePointCount];
        
        if(newAreaTrianglePoints != NULL && newTextureCoordinates != NULL) {
            
            for( unsigned int i = 0; i < areaTrianglePointCount; i++) {
                newTextureCoordinates[i].x = textureCoordinates[i].x;
                newTextureCoordinates[i].y = textureCoordinates[i].y;
                
                newAreaTrianglePoints[i].x = areaTrianglePoints[i].x;
                newAreaTrianglePoints[i].y = areaTrianglePoints[i].y;
            }
            glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, newAreaTrianglePoints);
            glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, newTextureCoordinates);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)areaTrianglePointCount);
            
            CC_SAFE_DELETE_ARRAY(newAreaTrianglePoints);
            CC_SAFE_DELETE_ARRAY(newTextureCoordinates);
        }
    }
    
    CC_INCREMENT_GL_DRAWS(1);

}