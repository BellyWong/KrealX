//
//  RubeImageInfo.h
//  KrealX
//
//  Created by kreal on 6/22/13.
//
//

#ifndef KrealX_RubeImageInfo_h
#define KrealX_RubeImageInfo_h

#include "cocos2d.h"
#include "Box2D.h"
#include "SimpleAudioEngine.h"
#include "GLES-Render.h"
#include "b2dJson/b2dJson.h"


USING_NS_CC;
using namespace CocosDenshion;
using namespace std;

class RubeImageInfo: public CCObject {
public:
    
    CCSprite* sprite;   // the image
    string name;        // the file the image was loaded from
    b2Body* b2body;     // the body this image is attached to (can be NULL)
    CCPoint center;     // 'local center' - relative to the position of the body
    float scale;        // a scale of 1 means the image is 1 physics unit high
    float angle;        // 'local angle' - relative to the angle of the body
    float opacity;      // 0 - 1
    bool flip;          // horizontal flip
    int colorTint[4];   // 0 - 255 RGBA values
    
    
    ~RubeImageInfo(void);
    RubeImageInfo();
};
#endif
