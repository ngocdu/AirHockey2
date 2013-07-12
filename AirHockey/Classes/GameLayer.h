//
//  GameLayer.h
//  AirHockey
//
//  Created by Trung Kien Do on 13/07/09.
//  Copyright __FRAMGIA__ 2013年. All rights reserved.
//
#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "Ball.h"
#include "GLES-Render.h"
#include "MyContactListener.h"

USING_NS_CC;

class GameLayer : public CCLayer {
public:
    ~GameLayer();
    GameLayer();
    
    CC_SYNTHESIZE(b2World *, _world, World);
    
    static CCScene* scene();
    
    void initPhysics();
    void createEdge(float x1, float y1, float x2, float y2, int groupIndex);
    
    virtual void draw();
    void update(float dt);
    
    void getStateInfo();
    void handleProcess();
    void defenseCenter();
    void defenseLeft();
    void defenseRight();
    void attack();
    
    virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
    virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);
    virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
    
    void gameReset();
    void scoreCounter(int player);
    
    void Timer();
    
    b2Vec2 ptm(CCPoint point) {
        return b2Vec2(point.x / PTM_RATIO, point.y / PTM_RATIO);
    }
    b2Vec2 ptm2(float x, float y) {
        return b2Vec2(x / PTM_RATIO, y / PTM_RATIO);
    }
private:
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float w = s.width;
    float h = s.height;
    b2Body *_groundBody;
    CCSprite *_p1;
    CCSprite *_p2;    
    GLESDebugDraw *m_debugDraw;
    
    Ball *_player1;
    Ball *_player2;
    Ball *_puck;

    b2MouseJoint *_mouseJoint;
    
    int _score1  = 0;
    int _score2  = 0;
    int lastHit = 0;
    int _minutes, _seconds;
    
    bool _playing;
    
    float x, y, px, py;
    float vx, vy, vpx, vpy;
    float pr;
    
    CCLabelTTF *_scoreLabel1;
    CCLabelTTF *_scoreLabel2;
    CCLabelTTF *_time;
    
    MyContactListener *_contactListener;
};

#endif 
