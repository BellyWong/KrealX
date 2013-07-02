//
//  DynamicTerrain.h
//  KrealX
//
//  Created by kreal on 6/29/13.
//
//

#ifndef KrealX_DynamicTerrain_h
#define KrealX_DynamicTerrain_h


#include "cocos2d.h"
#include "Box2D.h"
#include "GLES-Render.h"

#define kMaxHillKeyPoints 1000
#define kHillSegmentWidth 5
#define kMaxHillVertices 4000
#define kMaxBorderVertices 800


using namespace cocos2d;

class DynamicTerrain : public cocos2d::CCNode {
    
    int _offsetX;
    CCPoint _hillKeyPoints[kMaxHillKeyPoints];
    CCSprite *_stripes;
    int _fromKeyPointI;
    int _toKeyPointI;
    
    
    int _nHillVertices;
    CCPoint _hillVertices[kMaxHillVertices];
    CCPoint _hillTexCoords[kMaxHillVertices];
    int _nBorderVertices;
    CCPoint _borderVertices[kMaxBorderVertices];
    
    // box2d stuff
    b2World * _world;
    b2Body * _body;
    GLESDebugDraw * _debugDraw;
    CCSpriteBatchNode * _batchNode;
    int screenW;
    int screenH;
    //int textureSize;
    
    
protected: float offsetX;
public: virtual float getOffsetX(void) const { return offsetX; }
public: virtual void setOffsetX(float var);
    
public:
    CC_SYNTHESIZE_RETAIN(CCSprite *, stripes, Stripes);
    
    ~DynamicTerrain();
    void draw(void);
    
    static DynamicTerrain * terrainWithWorld(b2World* w);
    bool initWithWorld(b2World* w);
    
private:
    void resetHillVertices();
    ccColor3B generateDarkColor();
    ccColor3B generateLightColorFrom(ccColor3B c);
    void resetBox2DBody();
    void generateHills();
    void setupDebugDraw();
};



#endif
