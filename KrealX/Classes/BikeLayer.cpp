//
//  BikeLayer.cpp
//  KrealX
//
//  Created by kreal on 6/23/13.
//
//

#include "BikeLayer.h"
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


BikeLayer::~BikeLayer() {
}
BikeLayer::BikeLayer() {}

CCScene* BikeLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    // add layer as a child to scene
    
    // 'layer' is an autorelease object
    BikeLayer *layer = new BikeLayer();
    scene->addChild(layer);
    layer->init();
    layer->release();
    
    return scene;
}

void BikeLayer::afterLoadProcessing(b2dJson* json)
{
    BaseRubeLayer::afterLoadProcessing(json);
    std::vector<b2Body *> bodies;
    json->getBodiesByName("terrain", bodies);
    
    _terrainBody = bodies[0];
    
    // build hillKeyPoints for terrain texture gen
    for (b2Fixture* fixture = _terrainBody->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        b2Shape::Type shapeType = fixture->GetType();
        
        if ( shapeType == b2Shape::e_chain )
        {
            b2ChainShape* chainShape = (b2ChainShape*)fixture->GetShape();
            
            int numEdges = chainShape->GetChildCount();
            int numVertices = chainShape->m_hasNextVertex ? numEdges : numEdges + 1;
            
            _nHillVertices = numVertices;
            
            b2EdgeShape edgeShape;
            for (int i = 0; i < numEdges; i++) {
                chainShape->GetChildEdge( &edgeShape, i );
                b2Vec2 vertex1_from = edgeShape.m_vertex1;
                
                _hillKeyPoints[i] = CCPointMake(vertex1_from.x, vertex1_from.y);
                
                //b2Vec2 vertex2_to = edgeShape.m_vertex2;
                
            }
        }
    }
    
    _offsetX = 0;
    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
    
    
    std::string n = "Terrain.png";
    setStripes(CCSprite::create(n.c_str()));
    
    resetHillVertices();
    
}

void BikeLayer::draw()
{
    /*
    CC_NODE_DRAW_SETUP();
    
    ccGLBlendFunc( CC_BLEND_SRC, CC_BLEND_DST );
    
    CCSprite * sprite = getStripes();
    CCTexture2D * texture = sprite->getTexture();
    
    
    ccGLBindTexture2D(texture->getName());
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, _hillVertices);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, _hillTexCoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)_nHillVertices);
     */
    
    
    for(int i = 1; i < 8; ++i) {
        ccDrawColor4B(255,255,255, 255);
        ccDrawLine(_hillKeyPoints[i-1], _hillKeyPoints[i]);
        ccDrawColor4B(255,0,0,255);
    }
    
    BaseRubeLayer::draw();
    
}

void BikeLayer::resetHillVertices() {
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    static int prevFromKeyPointI = -1;
    static int prevToKeyPointI = -1;
    
    
    
    // key points interval for drawing
    while (_hillKeyPoints[_fromKeyPointI+1].x < _offsetX-winSize.width/8/getScale()) {
        _fromKeyPointI++;
    }
    while (_hillKeyPoints[_toKeyPointI].x < _offsetX+winSize.width*9/8/getScale()) {
        _toKeyPointI++;
    }
    
    if (prevFromKeyPointI != _fromKeyPointI || prevToKeyPointI != _toKeyPointI) {
        
        // vertices for visible area
        _nHillVertices = 0;
        //_nBorderVertices = 0;
        CCPoint p0, p1, pt0, pt1;
        p0 = _hillKeyPoints[_fromKeyPointI];
        for (int i=_fromKeyPointI+1; i<_toKeyPointI+1; i++) {
            p1 = _hillKeyPoints[i];
            
            // triangle strip between p0 and p1
            int hSegments = floorf((p1.x-p0.x)/kHillSegmentWidth);
            float dx = (p1.x - p0.x) / hSegments;
            float da = M_PI / hSegments;
            float ymid = (p0.y + p1.y) / 2;
            float ampl = (p0.y - p1.y) / 2;
            pt0 = p0;
            //_borderVertices[_nBorderVertices++] = pt0;
            for (int j=1; j<hSegments+1; j++) {
                pt1.x = p0.x + j*dx;
                pt1.y = ymid + ampl * cosf(da*j);
                //_borderVertices[_nBorderVertices++] = pt1;
                
                _hillVertices[_nHillVertices] = CCPointMake(pt0.x, 0);
                _hillTexCoords[_nHillVertices++] = CCPointMake(pt0.x/512, 1.0f);
                _hillVertices[_nHillVertices] = CCPointMake(pt1.x, 0);
                _hillTexCoords[_nHillVertices++] = CCPointMake(pt1.x/512, 1.0f);
                
                _hillVertices[_nHillVertices] = CCPointMake(pt0.x, pt0.y);
                _hillTexCoords[_nHillVertices++] = CCPointMake(pt0.x/512, 0);
                _hillVertices[_nHillVertices] = CCPointMake(pt1.x, pt1.y);
                _hillTexCoords[_nHillVertices++] = CCPointMake(pt1.x/512, 0);
                
                pt0 = pt1;
            }
            
            p0 = p1;
        }
        
        prevFromKeyPointI = _fromKeyPointI;
        prevToKeyPointI = _toKeyPointI;
        
    }
}


CCPoint BikeLayer::initialWorldOffset()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    return CCPointMake( 0, s.height/2 );
}

float BikeLayer::initialWorldScale()
{
    // This method should return the number of pixels for one physics unit.
    // When creating the scene in RUBE I can see that the jointTypes scene
    // is about 8 units high, so I want the height of the view to be about
    // 10 units, which for iPhone in landscape (480x320) we would return 32.
    // But for an iPad in landscape (1024x768) we would return 76.8, so to
    // handle the general case, we can make the return value depend on the
    // current screen height.
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    return s.height / 12; //screen will be 10 physics units high
    //return 20;
}

string BikeLayer::getFilename()
{
    return "bike.json";
}
