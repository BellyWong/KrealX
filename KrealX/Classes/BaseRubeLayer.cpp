//
//  BaseRubeLayer.cpp
//  KrealX
//
//  Created by kreal on 6/20/13.
//
//

#include "cocos2d.h"
#include "BaseRubeLayer.h"
#include "b2dJson/b2dJson.h"
#include "QueryCallbacks.h"
#include "b2dJson/b2dJsonImage.h"
#include <string>


using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

USING_NS_CC;

bool BaseRubeLayer::init()
{
    // 1. super init first
    if ( !CCLayer::init() ) {
        return false;
    }

    //ccColor4B tmpColor = { 255, 255, 255, 255 };
    
    //this->se
    //this->setColor(ccc3(255, 255, 255));
    
    setTouchEnabled( true );
    setAccelerometerEnabled( true );
    
    // make screen position values relative to the bottom left of screen
    setAnchorPoint(CCPointMake(0, 0));
    
    // initialize a bunch of things to NULL first
    m_world = NULL;
    m_mouseJoint = NULL;
    m_mouseJointGroundBody = NULL;
    m_debugDraw = NULL;
    m_imageInfos = NULL;
    
    // set the starting scale and offset values from the subclass
    setPosition(this->initialWorldOffset());
    setScale(this->initialWorldScale());
    
    // load the world from RUBE .json file (this will also call afterLoadProcessing)
    loadWorld();
    
    //create main loop
    this->schedule(schedule_selector(BaseRubeLayer::update));

    
    return true;
}
void BaseRubeLayer::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();
    
    if (!m_world) return;
    
    // draw sprites
    setImagePositionsFromPhysicsBodies();
    
    // debug draw
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    kmGLPushMatrix();
    m_world->DrawDebugData();
    kmGLPopMatrix();
}

void BaseRubeLayer::update(float dt) {
    if (!m_world) return;
    
    //m_world->Step(1/60.0, 8, 3);
    
    m_world->Step(dt, 8, 3);
    
    
    //m_world->ClearForces();
}
CCLayer* BaseRubeLayer::create()
{
    BaseRubeLayer *layer = new BaseRubeLayer();
    layer->init();
    layer->autorelease();
    return layer;
}
CCScene* BaseRubeLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    // add layer as a child to scene
    
    // 'layer' is an autorelease object
    BaseRubeLayer *layer = new BaseRubeLayer();
    layer->init();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

CCPoint BaseRubeLayer::initialWorldOffset()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    return CCPointMake( -300, s.height/2 );
}

float BaseRubeLayer::initialWorldScale()
{
    // This method should return the number of pixels for one physics unit.
    // When creating the scene in RUBE I can see that the jointTypes scene
    // is about 8 units high, so I want the height of the view to be about
    // 10 units, which for iPhone in landscape (480x320) we would return 32.
    // But for an iPad in landscape (1024x768) we would return 76.8, so to
    // handle the general case, we can make the return value depend on the
    // current screen height.

    CCSize s = CCDirector::sharedDirector()->getWinSize();
    return s.height / 10; //screen will be 10 physics units high
    //return 20;
}

string BaseRubeLayer::getFilename()
{
    return "bike.json";
}
void BaseRubeLayer::loadWorld()
{
    CCLOG("start loadWorld");
    
    // The clear method should undo anything that is done in this method,
    // and also whatever is done in the afterLoadProcessing method.
    clear();
    
    b2dJson json;
	std::string errorMsg;
    string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(getFilename().c_str());
    
    CCLOG(fullPath.c_str());
    
	m_world = json.readFromFile(fullPath.c_str(), errorMsg);
    
    if ( m_world ) {
        CCLOG("Loaded JSON ok");
        m_debugDraw = new GLESDebugDraw(CC_CONTENT_SCALE_FACTOR());
        m_debugDraw->SetFlags( b2Draw::e_shapeBit );
        m_world->SetDebugDraw(m_debugDraw);
        
        // This body is needed if we want to use a mouse joint to drag things around.
        b2BodyDef bd;
        m_mouseJointGroundBody = m_world->CreateBody( &bd );
        
        afterLoadProcessing(&json);
    }
    else {
         CCLOG(errorMsg.c_str());
    }
    CCLOG("end loadWorld");
}

// Override this in subclasses to do some extra processing (eg. acquire references
// to named bodies, joints etc) after the world has been loaded, and while the b2dJson
// information is still available.
void BaseRubeLayer::afterLoadProcessing(b2dJson* json) {
    std::vector<b2dJsonImage*> b2dImages;
    json->getAllImages(b2dImages);
    
    int imageCount = (int)b2dImages.size();
    
    m_imageInfos = CCArray::createWithCapacity(imageCount);
    m_imageInfos->retain();
    
    // for retina, should make images twice as large in pixels
    float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
    
    for (int i = 0; i < imageCount; i++) {
        RubeImageInfo* imgInfo = new RubeImageInfo();
        b2dJsonImage* img = b2dImages[i];
        
        CCLOG("Loading image: %s", img->file.c_str());
        
        // remove images/ from 'images/image.png'
        string newFileName;
        size_t pos = img->file.find_last_of("/");
        if (pos != std::string::npos)
        {
            newFileName = img->file.substr(pos+1);
        }
        //string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(img->file.c_str());
        CCSprite* sprite = CCSprite::create(newFileName.c_str());
        if( !sprite )
        {
            return;
        }
        // add the sprite to this layer and set the render order
        addChild(sprite, img->renderOrder); //watch out - RUBE render order is float but cocos2d uses integer (why not float?)
        
        // these will not change during simulation so we can set them now
        //sprite->getContentSize().height
        sprite->setScale(img->scale / (sprite->getTexture()->getContentSizeInPixels().height * contentScaleFactor));
        sprite->setFlipX(img->flip);
        ccColor3B colorTint = {img->colorTint[0], img->colorTint[1], img->colorTint[2]};
        sprite->setColor(colorTint);
        sprite->setOpacity(img->colorTint[3]);
        
        // create an info structure to hold the info for this image (body and position etc)
        imgInfo->sprite = sprite;
        imgInfo->name = img->name.c_str();
        imgInfo->b2body = img->body;
        imgInfo->scale = img->scale;
        imgInfo->angle = img->angle;
        imgInfo->center = CCPointMake(img->center.x, img->center.y);
        imgInfo->opacity = img->opacity;
        imgInfo->flip = img->flip;
        for (int n = 0; n < 4; n++) {
            imgInfo->colorTint[n] = img->colorTint[n];
        }
        m_imageInfos->addObject( imgInfo );
    }
    
    setImagePositionsFromPhysicsBodies();
}

// Move all the images to where the physics engine says they should be
void BaseRubeLayer::setImagePositionsFromPhysicsBodies()
{
    int count = m_imageInfos->count();
    
    for (int i=0; i< count; i++) {
        RubeImageInfo *imgInfo = (RubeImageInfo *)m_imageInfos->objectAtIndex(i);
        
        CCPoint pos = imgInfo->center;
        float angle = -imgInfo->angle;
        if ( imgInfo->b2body ) {
            //need to rotate image local center by body angle
            b2Vec2 localPos( pos.x, pos.y );
            b2Rot rot( imgInfo->b2body->GetAngle() );
            localPos = b2Mul(rot, localPos) + imgInfo->b2body->GetPosition();
            pos.x = localPos.x;
            pos.y = localPos.y;
            angle += -imgInfo->b2body->GetAngle();
        }
        imgInfo->sprite->setRotation(CC_RADIANS_TO_DEGREES(angle));
        imgInfo->sprite->setPosition(pos);
    }
}

// Converts a position in screen pixels to a location in the physics world
b2Vec2 BaseRubeLayer::screenToWorld(CCPoint screenPos)
{
    // NOT COMPLETED YET PASS BY REF/VAL ????
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    screenPos.y = winSize.height - screenPos.y;
    
    CCPoint layerOffset = getPosition();
    screenPos.x -= layerOffset.x;
    screenPos.y -= layerOffset.y;
    
    float layerScale = getScale();
    
    return b2Vec2(screenPos.x / layerScale, screenPos.y / layerScale);
}

// Converts a location in the physics world to a position in screen pixels
CCPoint BaseRubeLayer::worldToScreen(b2Vec2 worldPos)
{
    // NOT COMPLETED YET PASS BY REF/VAL ????
    worldPos *= getScale();
    
    CCPoint layerOffset = getPosition();
    
    CCPoint p = CCPointMake(worldPos.x + layerOffset.x, worldPos.y + layerOffset.y);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    p.y = winSize.height - p.y;
    return p;
}



void BaseRubeLayer::clear()
{
    CC_SAFE_RELEASE(m_imageInfos);
    if ( m_world ) {
        CCLOG("Deleting Box2D world");
        delete m_world;
    }
    
    if ( m_debugDraw )
    {
        delete m_debugDraw;
    }
    m_world = NULL;
    m_mouseJoint = NULL;
    m_mouseJointGroundBody = NULL;
    m_debugDraw = NULL;
    m_imageInfos = NULL;
}

BaseRubeLayer::~BaseRubeLayer()
{
    unscheduleUpdate();
    clear();
}

BaseRubeLayer::BaseRubeLayer()
{
   // float i = 0;
}


