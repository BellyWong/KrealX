//
//  BikeLayer.h
//  KrealX
//
//  Created by kreal on 6/23/13.
//
//

#ifndef KrealX_BikeLayer_h
#define KrealX_BikeLayer_h


#include "cocos2d.h"
#include "Box2D.h"
#include "SimpleAudioEngine.h"
#include "GLES-Render.h"
#include "b2dJson/b2dJson.h"
#include "RubeImageInfo.h"
#include "BaseRubeLayer.h"

USING_NS_CC;

#define kHillSegmentWidth 5
#define kMaxHillVertices 4000
#define kMaxBorderVertices 800

using namespace CocosDenshion;
using namespace std;

class BikeLayer : public BaseRubeLayer {
public:
    ~BikeLayer(void);
    BikeLayer();
    static cocos2d::CCScene* scene();
    virtual string getFilename();
    virtual CCPoint initialWorldOffset();
    virtual float initialWorldScale();
    virtual void afterLoadProcessing(b2dJson* json);
    virtual void draw();
    void resetHillVertices();
    
    int _fromKeyPointI;
    int _toKeyPointI;
    
    int _nHillVertices;
    CCPoint _hillVertices[kMaxHillVertices];
    CCPoint _hillTexCoords[kMaxHillVertices];

    CCPoint _hillKeyPoints[9];
    
    int _offsetX;
    
    b2Body* _terrainBody;
    
    CC_SYNTHESIZE_RETAIN(CCSprite *, stripes, Stripes);
    //CREATE_FUNC(BikeLayer);
private:
};
#endif