//
//  DynamicTerrain.cpp
//  KrealX
//
//  Created by kreal on 6/29/13.
//
//

#include "DynamicTerrain.h"
#include "Box2D.h"

using namespace std;

#define PTM_RATIO 32 // pixel to metre ratio

bool DynamicTerrain::initWithWorld(b2World* w) {
    
    _world = w;
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    screenW = size.width;
    screenH = size.height;
    
    //scrolling = false;
    //textureSize = 512;
    
    
    setupDebugDraw();
    generateHills();
    resetHillVertices();
    
    
    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
    
    offsetX = 0;
    
    
    std::string n = "Terrain.png";
    setStripes(CCSprite::create(n.c_str()));
    
    //scheduleUpdate();
    
    return true;
}

void DynamicTerrain::generateHills() {
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    float minDX = 160;
    float minDY = 60;
    int rangeDX = 80;
    int rangeDY = 40;
    
    float x = -minDX;
    float y = winSize.height/2-minDY;
    
    float dy, ny;
    float sign = 1; // +1 - going up, -1 - going  down
    float paddingTop = 20;
    float paddingBottom = 20;
    
    for (int i=0; i<kMaxHillKeyPoints; i++) {
        _hillKeyPoints[i] = CCPointMake(x, y);
        if (i == 0) {
            x = 0;
            y = winSize.height/2;
        } else {
            x += rand()%rangeDX+minDX;
            while(true) {
                dy = rand()%rangeDY+minDY;
                ny = y + dy*sign;
                if(ny < winSize.height-paddingTop && ny > paddingBottom) {
                    break;
                }
            }
            y = ny;
        }
        sign *= -1;
    }
}


void DynamicTerrain::draw() {
    
    
    CC_NODE_DRAW_SETUP();
    
    ccGLBlendFunc( CC_BLEND_SRC, CC_BLEND_DST );
    
    
    CCSprite * sprite = getStripes();
    
    
    CCTexture2D * texture = sprite->getTexture();
    
    
    ccGLBindTexture2D(texture->getName());
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, _hillVertices);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, _hillTexCoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)_nHillVertices);
    
    
    for(int i = MAX(_fromKeyPointI, 1); i <= _toKeyPointI; ++i) {
        ccDrawColor4B(255,255,255, 255);
        ccDrawLine(_hillKeyPoints[i-1], _hillKeyPoints[i]);
        ccDrawColor4B(255,0,0,255);
        
        CCPoint p0 = _hillKeyPoints[i-1];
        CCPoint p1 = _hillKeyPoints[i];
        int hSegments = floorf((p1.x-p0.x)/kHillSegmentWidth);
        float dx = (p1.x - p0.x) / hSegments;
        float da = M_PI / hSegments;
        float ymid = (p0.y + p1.y) / 2;
        float ampl = (p0.y - p1.y) / 2;
        
        CCPoint pt0, pt1;
        pt0 = p0;
        for (int j = 0; j < hSegments+1; ++j) {
            
            pt1.x = p0.x + j*dx;
            pt1.y = ymid + ampl * cosf(da*j);
            
            ccDrawLine(pt0, pt1);
            
            pt0 = pt1;
            
        }
        
    }
    
    // draw debug
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
	
	kmGLPushMatrix();
	
	_world->DrawDebugData();
	
	kmGLPopMatrix();
    
    //CC_NODE_DRAW_SETUP();
    
    //ccGLBlendFunc( CC_BLEND_SRC, CC_BLEND_DST );
    
    
    
    
    // end draw debug
}


void DynamicTerrain::setOffsetX(float newOffsetX) {
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    _offsetX = newOffsetX;
    setPosition(CCPointMake(winSize.width/8-_offsetX*getScale(), 0));
    resetHillVertices();
}

void DynamicTerrain::resetHillVertices() {
    
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
        _nBorderVertices = 0;
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
            _borderVertices[_nBorderVertices++] = pt0;
            for (int j=1; j<hSegments+1; j++) {
                pt1.x = p0.x + j*dx;
                pt1.y = ymid + ampl * cosf(da*j);
                _borderVertices[_nBorderVertices++] = pt1;
                
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
        
        resetBox2DBody();
    }
}

void DynamicTerrain::resetBox2DBody() {
    
    if(_body) {
        _world->DestroyBody(_body);
    }
    
    b2BodyDef bd;
    bd.position.Set(0, 0);
    
    _body = _world->CreateBody(&bd);
    
    b2EdgeShape shape;
    
    b2Vec2 p1, p2;
    for (int i=0; i<_nBorderVertices-1; i++) {
        p1 = b2Vec2(_borderVertices[i].x/PTM_RATIO,_borderVertices[i].y/PTM_RATIO);
        p2 = b2Vec2(_borderVertices[i+1].x/PTM_RATIO,_borderVertices[i+1].y/PTM_RATIO);
        shape.Set(p1, p2);
        _body->CreateFixture(&shape, 0);
    }
    
    //_nBorderVertices = 0;
    /*
    for (b2Fixture* fixture = _body->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        b2Shape::Type shapeType = fixture->GetType();
        
        if(shapeType == b2Shape::e_edge)
        {
            b2EdgeShape* edgeShape = (b2EdgeShape*)fixture->GetShape();
            b2Vec2 vertex1 = edgeShape->m_vertex1;
            b2Vec2 vertex2 = edgeShape->m_vertex2;
            
            //_borderVertices[_nBorderVertices++]
            
        }
    }
     */
}

void DynamicTerrain::setupDebugDraw() {
    _debugDraw = new GLESDebugDraw(PTM_RATIO*CC_CONTENT_SCALE_FACTOR());
    _world->SetDebugDraw(_debugDraw);
    
    uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	//		flags += b2Draw::e_jointBit;
	//		flags += b2Draw::e_aabbBit;
	//		flags += b2Draw::e_pairBit;
	//		flags += b2Draw::e_centerOfMassBit;
	_debugDraw->SetFlags(flags);
}

DynamicTerrain::~DynamicTerrain()
{
    CC_SAFE_RELEASE(_batchNode);
    /*
    if( stripes )
    {
        stripes->release();
        stripes = NULL;
    }
     */
    delete _world;
    delete _debugDraw;
    
    _world = NULL;
    _debugDraw = NULL;
}

