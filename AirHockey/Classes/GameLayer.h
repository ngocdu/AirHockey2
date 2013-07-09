//
//  GameLayerScene.h
//  AirHockey
//
//  Created by Trung Kien Do on 13/07/09.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"
#include "Ball.h"
#include "GLES-Render.h"

USING_NS_CC;

class GameLayer : public CCLayer {
public:
    CC_SYNTHESIZE(b2World *, _world, World);
    
    ~GameLayer();
    GameLayer();
    
    static CCScene* scene();
    
    void initPhysics();

    virtual void draw();
    virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
    virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);
    virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
    void update(float dt);
    
private:
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float w = s.width;
    float h = s.height;
    
    GLESDebugDraw *m_debugDraw;
    
    Ball *_player1;
    Ball *_player2;
    Ball *_ball;

};

#endif 
