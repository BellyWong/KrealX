//
//  BaseRubeLayer.h
//  KrealX
//
//  Created by kreal on 6/20/13.
//
//

#ifndef KrealX_BaseRubeLayer_h
#define KrealX_BaseRubeLayer_h

#include "cocos2d.h"
#include "Box2D.h"
#include "SimpleAudioEngine.h"
#include "GLES-Render.h"
#include "b2dJson/b2dJson.h"
#include "RubeImageInfo.h"


USING_NS_CC;
using namespace CocosDenshion;
using namespace std;
using namespace cocos2d;

class BaseRubeLayer : public CCLayer {
public:
    
    CC_SYNTHESIZE(b2World *, m_world, World);
    CC_SYNTHESIZE(int, m_gravity, Gravity);
    
    ~BaseRubeLayer(void);
    BaseRubeLayer();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    static CCLayer* create();
    virtual void update (float dt);
    virtual void afterLoadProcessing(b2dJson* json);
    virtual void draw();
    virtual string getFilename();
    virtual CCPoint initialWorldOffset();
    virtual float initialWorldScale();
    void loadWorld();
    void clear();
    b2Vec2 screenToWorld(CCPoint screenPos);
    CCPoint worldToScreen(b2Vec2 worldPos);
    void setImagePositionsFromPhysicsBodies();
    //CREATE_FUNC(BaseRubeLayer);
    
private:
    
    GLESDebugDraw * m_debugDraw;
    b2MouseJoint* m_mouseJoint;
    b2Body* m_mouseJointGroundBody;
    CCArray * m_imageInfos;
};

#endif
