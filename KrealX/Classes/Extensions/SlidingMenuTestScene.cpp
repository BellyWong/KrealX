//
//  SlidingMenuTestScene.cpp
//  KrealX
//
//  Created by kreal on 6/26/13.
//
//

#include "SlidingMenuTestScene.h"
#include "cocos2d.h"
#include "SlidingMenu.h"

using namespace cocos2d;

SlidingMenuTestScene::~SlidingMenuTestScene()
{
    
}
SlidingMenuTestScene::SlidingMenuTestScene()
{
    
    ////////////////////////
    // BEGIN OF TESTING
    ////////////////////////
    CCArray* menuArray = CCArray::createWithCapacity(24);
    
    CCSize windowSize = CCDirector::sharedDirector()->getOpenGLView()->getDesignResolutionSize();
    
    float w= 0;
    float h=0;
    for (int i = 0;i<24;i++)
    {
        CCSprite* spr = CCSprite::create("CloseNormal.png");
        CCSprite* selspr = CCSprite::create("CloseSelected.png");
        CCSprite* dis = CCSprite::create("CloseSelected.png");
        CCMenuItemSprite* itmspr = CCMenuItemSprite::create(spr, selspr,dis, this, menu_selector(SlidingMenuTestScene::menuItemCallback));
        itmspr->setTag(i);
                                         
        if (i > 3) itmspr->setEnabled(false);
                                         
        CCLOG ("tag for item %d" , itmspr->getTag());
                                         
        menuArray->addObject(itmspr);
                                         
    }
    int row = 3;
    int col = 4;
    CCPoint p = ccp(80,80);
    //some trick to center menu
    float eWidth =  (col-1)*(w+p.x);
    float eHeight = (row-1)*(h+p.y);
    CCPoint menuPosition = ccp(
                               windowSize.width/2.0f -eWidth/2.0f ,
                               windowSize.height/2.0f -eHeight/2.0f);
                                         
    SlidingMenuGrid* sliderMenu = SlidingMenuGrid::menuWithArray(menuArray, col, row, menuPosition,p);
                                         
    sliderMenu->setAnchorPoint(ccp(0.5, 0.5));
    //    sliderMenu->setPosition(100,100);
                                         
    this->addChild(sliderMenu);

    ////////////////////////
    // END OF TESTING
    ////////////////////////
    
    scheduleUpdate();
}
// returns a Scene that contains the HelloWorld as the only child
CCScene* SlidingMenuTestScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new SlidingMenuTestScene();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

void SlidingMenuTestScene::menuItemCallback(CCObject* pSender)
{
    
}
void SlidingMenuTestScene::draw()
{
    
}
void SlidingMenuTestScene::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    
}
void SlidingMenuTestScene::update(float dt)
{
    
}
