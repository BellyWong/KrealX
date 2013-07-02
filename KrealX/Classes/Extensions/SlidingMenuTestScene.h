//
//  SlidingMenuTestScene.h
//  KrealX
//
//  Created by kreal on 6/26/13.
//
//

#ifndef KrealX_SlidingMenuTestScene_h
#define KrealX_SlidingMenuTestScene_h

#include "cocos2d.h"
using namespace cocos2d;


class SlidingMenuTestScene : public CCLayer {
public:
    ~SlidingMenuTestScene();
    SlidingMenuTestScene();
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    void menuItemCallback(CCObject* pSender);
    virtual void draw();
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void update(float dt);
    
private:

};

#endif
