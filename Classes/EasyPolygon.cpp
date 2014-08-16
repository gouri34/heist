//
//  EasyPolygon.cpp
//  Bear
//
//  Created by cong ku on 14-7-2.
//
//

#include "EasyPolygon.h"

#define __GLES2 1

EasyPolygon* EasyPolygon::filledPolygonWithPointsAndTexture(Vector2dVector &polygonPoints, Texture2D * fillTexture) {
    
    EasyPolygon *pRet = new EasyPolygon();
    if(pRet && pRet->initWithPointsAndTexture(polygonPoints, fillTexture))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
    
}


EasyPolygon* EasyPolygon::filledPolygonWithPointsAndTextureUsingTriangulator(Vector2dVector &polygonPoints, Texture2D *fillTexture, PRRatcliffTriangulator *polygonTriangulator) {
    
    EasyPolygon *pRet = new EasyPolygon();
    if(pRet && pRet->initWithPointsandTextureusingTriangulator(polygonPoints, fillTexture,polygonTriangulator))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool EasyPolygon::initWithPointsAndTexture(Vector2dVector &polygonPoints, Texture2D * fillTexture) {
    
    
    PRRatcliffTriangulator *polygonTriangulator = new PRRatcliffTriangulator();
    return initWithPointsandTextureusingTriangulator(polygonPoints, fillTexture, polygonTriangulator);;
}


bool EasyPolygon::initWithPointsandTextureusingTriangulator(Vector2dVector &polygonPoints, Texture2D *fillTexture, PRRatcliffTriangulator* polygonTriangulator) {
    
    triangulator = polygonTriangulator;
    
#ifdef __GLES2
    setGLProgram(CCShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
#endif
    
    setTexture(fillTexture);
    setPoints(polygonPoints);
    
	return true;
}

void EasyPolygon::setPoints(Vector2dVector &points) {
    
    if (points.size() <= 0) {
        areaTrianglePointCount = 0;
        return;
    }
    
    CC_SAFE_FREE(areaTrianglePoints);
    CC_SAFE_FREE(textureCoordinates);
    
    /*Vector2dVector triangulatedPoints = PRRatcliffTriangulator::triangulateVertices(points);
    
    areaTrianglePointCount = (int)triangulatedPoints.size();
    areaTrianglePoints = (Point*) malloc(sizeof(Point) * areaTrianglePointCount);
    textureCoordinates = (Point*) malloc(sizeof(Point) * areaTrianglePointCount);
    
    for (int i = 0; i < areaTrianglePointCount; i++) {
        Vector2d v = (Vector2d)triangulatedPoints.at(i);
        areaTrianglePoints[i] = Point(v.GetX(), v.GetY());
       // printf("x = %f, y = %f\n", v.GetX, v.GetY);
    }*/
    areaTrianglePointCount = 6;
    areaTrianglePoints = (Point*) malloc(sizeof(Point) * areaTrianglePointCount);
    textureCoordinates = (Point*) malloc(sizeof(Point) * areaTrianglePointCount);
    
    Vector2d v = points.at(0);
    areaTrianglePoints[0] = Point(v.GetX(), v.GetY());
    v = points.at(3);
    areaTrianglePoints[1] = Point(v.GetX(), v.GetY());
    v = points.at(2);
    areaTrianglePoints[2] = Point(v.GetX(), v.GetY());
    v = points.at(2);
    areaTrianglePoints[3] = Point(v.GetX(), v.GetY());
    v = points.at(1);
    areaTrianglePoints[4] = Point(v.GetX(), v.GetY());
    v = points.at(0);
    areaTrianglePoints[5] = Point(v.GetX(), v.GetY());

    
    calculateTextureCoordinates();
}

void EasyPolygon::cleanup()
{
    CC_SAFE_RELEASE_NULL(texture);
	CC_SAFE_FREE(triangulator);
    CC_SAFE_FREE(areaTrianglePoints);
    CC_SAFE_FREE(textureCoordinates);
    CCNode::cleanup();
}

void EasyPolygon::calculateTextureCoordinates() {
    int minX = 0;
    float minY = 0;
    minX = (int)(areaTrianglePoints[0].x / textureWidth);
    minY = (float)(areaTrianglePoints[0].y / textureWidth);

    
    for (int j = 0; j < areaTrianglePointCount; j++) {
        //textureCoordinates[j] = areaTrianglePoints[j]*1.0f / texture->getPixelsWide() * CC_CONTENT_SCALE_FACTOR();
        textureCoordinates[j].x = areaTrianglePoints[j].x*1.0f / textureWidth - minX;
        float ty = areaTrianglePoints[j].y*1.0f / textureWidth;
        if (ty > minY) {
            textureCoordinates[j].y = 1.0;
        }
        else {
            textureCoordinates[j].y = 0.0;
        }
        //textureCoordinates[j].y = areaTrianglePoints[j].y*1.0f / textureWidth - minY;
        
        textureCoordinates[j].y = 1.0f - textureCoordinates[j].y;
    }
}

void EasyPolygon::updateBlendFunc() {
    // it's possible to have an untextured sprite
    if( !texture || !texture->hasPremultipliedAlpha() ) {
        blendFunc.src = GL_SRC_ALPHA;
        blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    } else {
        blendFunc.src = CC_BLEND_SRC;
        blendFunc.dst = CC_BLEND_DST;
    }
}

void EasyPolygon::setBlendFunc(BlendFunc blendFuncIn) {
    blendFunc = blendFuncIn;
}

void EasyPolygon::setTexture(Texture2D* texture2D) {
    
    
    CCAssert(texture2D, "NO TEXTURE SET");
    CC_SAFE_RELEASE(texture);
	
    texture = texture2D;
    CC_SAFE_RETAIN(texture);
    
    textureWidth = texture->getPixelsWide();
    
	cocos2d::Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT };
    texture->setTexParameters(texParams);
    
    updateBlendFunc();
	calculateTextureCoordinates();
    
}

Texture2D* EasyPolygon::getTexture() {
    return texture;
}

void EasyPolygon::draw(Renderer* renderer, const Mat4 &transform, bool transformUpdated) {
    
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(EasyPolygon::onDraw, this, transform, transformUpdated);
    renderer->addCommand(&_customCommand);
    
    
}

void EasyPolygon::onDraw(const Mat4 &transform, bool transformUpdated)
{
    //kmGLPushMatrix();
    //kmGLLoadMatrix(&transform);
    
    //draw
    if(areaTrianglePointCount <= 1)
        return;
    
    CC_NODE_DRAW_SETUP();
    
    
    cocos2d::GL::bindTexture2D(texture->getName());
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    GL::blendFunc(blendFunc.src, blendFunc.dst);
    
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
    
    if(sizeof(Point) == sizeof(Vec2)) {
        
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, areaTrianglePoints);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, textureCoordinates);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)areaTrianglePointCount);
        /*short indices1[] = {0, 1, 2};
        short indices2[] = {3, 4, 5};
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, indices1);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, indices2);*/
        
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
            //glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)areaTrianglePointCount);
            
            short indices1[] = {0, 1, 2};
            short indices2[] = {3, 4, 5};
            glDrawElements(GL_TRIANGLES, 3, GL_SHORT, indices1);
            glDrawElements(GL_TRIANGLES, 3, GL_SHORT, indices2);
            
            
            
            CC_SAFE_DELETE_ARRAY(newAreaTrianglePoints);
            CC_SAFE_DELETE_ARRAY(newTextureCoordinates);
        }
    }
    
    CC_INCREMENT_GL_DRAWS(1);
    
     
}