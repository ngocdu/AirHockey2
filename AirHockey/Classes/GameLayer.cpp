//
//  GameLayerScene.cpp
//  AirHockey
//
//  Created by Trung Kien Do on 13/07/09.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#include "GameLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

GameLayer::GameLayer()
{
    setTouchEnabled( true );

    CCSize s = CCDirector::sharedDirector()->getWinSize();

    this->initPhysics();
    scheduleUpdate();
}

GameLayer::~GameLayer()
{
    delete _world;
    _world = NULL;
    
    delete m_debugDraw;
}

void GameLayer::initPhysics()
{
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    _world = new b2World(gravity);

    // Do we want to let bodies sleep?
    _world->SetAllowSleeping(true);

    _world->SetContinuousPhysics(true);

    m_debugDraw = new GLESDebugDraw( PTM_RATIO );
    _world->SetDebugDraw(m_debugDraw);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    flags += b2Draw::e_aabbBit;
    flags += b2Draw::e_pairBit;
    flags += b2Draw::e_centerOfMassBit;
    m_debugDraw->SetFlags(flags);


    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
    b2Body* groundBody = _world->CreateBody(&groundBodyDef);
    
    b2EdgeShape groundBox;

    // bottom
    groundBox.Set(b2Vec2(0,0), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);

    // top
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);

    // left
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));
    groundBody->CreateFixture(&groundBox,0);

    // right
    groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
    
    _player1 = Ball::create(this, kSpritePlayer,
                            ccp(w/2, playerRadius));
    _player2 = Ball::create(this, kSpritePlayer,
                            ccp(w/2, h - playerRadius));
    _ball = Ball::create(this, kSpriteBall,
                            ccp(w/2, h/2));
    
}

void GameLayer::draw()
{
    CCLayer::draw();

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    kmGLPushMatrix();

    _world->DrawDebugData();

    kmGLPopMatrix();
}

void GameLayer::update(float dt)
{
    _world->Step(dt, 8, 3);
    
}

void GameLayer::ccTouchesBegan(CCSet* touches, CCEvent* event) {
    
}

void GameLayer::ccTouchesMoved(CCSet* touches, CCEvent* event) {
    
}

void GameLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    CCSetIterator it;
    CCTouch* touch;
    
    for( it = touches->begin(); it != touches->end(); it++) 
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint location = touch->getLocationInView();
        
        location = CCDirector::sharedDirector()->convertToGL(location);
        
    
    }
}

CCScene* GameLayer::scene()
{
    CCScene *scene = CCScene::create();
    CCLayer* layer = new GameLayer();
    scene->addChild(layer);
    layer->release();
    return scene;
}
