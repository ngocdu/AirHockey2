//
//  GameLayer.cpp
//  AirHockey
//
//  Created by Trung Kien Do on 13/07/09.
//  Copyright __FRAMGIA__ 2013年. All rights reserved.
//

#include "GameLayer.h"
#include "SimpleAudioEngine.h"

#pragma mark CONSTRUCTOR

GameLayer::GameLayer() {
    setTouchEnabled(true);
    setAccelerometerEnabled(true);
    CCSprite *backGroundImg = CCSprite::create("court.png");
    backGroundImg->setPosition(ccp(w/2, h/2));
    this->addChild(backGroundImg);
    
    // Score Counter
    _scoreLabel1 = CCLabelTTF::create("0", "Arial", 40);
    _scoreLabel2 = CCLabelTTF::create("0", "Arial", 40);
    _scoreLabel1->setPosition(ccp(w - 50, h/2 - 50));
    _scoreLabel1->setRotation(90);
    _scoreLabel2->setPosition(ccp(w - 50, h/2 + 50));
    _scoreLabel2->setRotation(90);
    this->addChild(_scoreLabel1);
    this->addChild(_scoreLabel2);
    
    // Timer
    _minutes = 2;
    _seconds = 60;
    _playing = true;
    char timeBuf[20] = {0};
	sprintf(timeBuf, "0%i:%i", _minutes, _seconds);

    _time = CCLabelTTF::create(timeBuf, "Times New Roman", 34);
	_time->setPosition(ccp(40, h/2));
    _time->setRotation(-90);
	this->addChild(_time, 1);
    
    // Physics
    this->initPhysics();
    
    // Scheduler
    scheduleUpdate();
    this->schedule(schedule_selector(GameLayer::Timer), 1);
    this->schedule(schedule_selector(GameLayer::handleProcess), 0.025);
}

#pragma mark DESTRUCTOR
GameLayer::~GameLayer()
{
    delete _world;
    _world = NULL;
    
//    delete m_debugDraw;
}

#pragma mark INIT PHYSICS
void GameLayer::initPhysics()
{
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    _world = new b2World(gravity);
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    
    _mouseJoint = NULL;

    _contactListener = new MyContactListener();
    _world->SetContactListener(_contactListener);
    
    m_debugDraw = new GLESDebugDraw( PTM_RATIO );
    _world->SetDebugDraw(m_debugDraw);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
//    flags += b2Draw::e_jointBit;
//    flags += b2Draw::e_aabbBit;
//    flags += b2Draw::e_pairBit;
//    flags += b2Draw::e_centerOfMassBit;
    m_debugDraw->SetFlags(flags);


    // Create Play Ground
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
    _groundBody = _world->CreateBody(&groundBodyDef);
    
    // Bottom Left
    this->createEdge(0, 10, 210, 10, 0);
    // Bottom Right
    this->createEdge(w - 210, 10, w, 10, 0);
    // Bottom Center
    this->createEdge(210, 10, w - 210, 10, -10);
    
    // Top Left
    this->createEdge(0, h - 10, 210, h - 10, 0);
    // Top Right
    this->createEdge(w - 210, h - 10, w, h - 10, 0);
    // Top Center
    this->createEdge(210, h - 10, w - 210, h - 10, -10);
        
    // Left
    this->createEdge(10, 0, 10, h, 0);
    // Right
    this->createEdge(w - 10, h, w - 10, 0, 0);
    // Center
    this->createEdge(0, h/2, w, h/2, -10);
    
    // Create 2 Player and Puck
    _player1 = Ball::create(this, humanPlayer, "mallet1.png");
    _player1->setStartPos(ccp(w/2, _player1->getRadius()*2));
    _player1->setSpritePosition(_player1->getStartPos());
    this->addChild(_player1);
    _player2 = Ball::create(this, aiPlayer, "mallet2.png");
    _player2->setStartPos(ccp(w/2, h - _player2->getRadius()*2));
    _player2->setSpritePosition(_player2->getStartPos());
    this->addChild(_player2);
    _puck = Ball::create(this, puck, "puck.png");
    _puck->setStartPos(ccp(w/2, h/2));
    _puck->setSpritePosition(_puck->getStartPos());
    this->addChild(_puck);
}

void GameLayer::createEdge(float x1, float y1,
                           float x2, float y2,
                           int groupIndex) {
    b2EdgeShape groundEdgeShape;
    groundEdgeShape.Set(b2Vec2(x1 / PTM_RATIO, y1 / PTM_RATIO),
                        b2Vec2(x2 / PTM_RATIO, y2 / PTM_RATIO));
    b2FixtureDef groundEdgeDef;
    groundEdgeDef.shape = &groundEdgeShape;
    groundEdgeDef.restitution = 0.2;
    groundEdgeDef.filter.groupIndex = groupIndex;
    _groundBody->CreateFixture(&groundEdgeDef);
}

#pragma mark DRAW
void GameLayer::draw()
{
    CCLayer::draw();

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    kmGLPushMatrix();

    _world->DrawDebugData();

    kmGLPopMatrix();
}

#pragma mark UPDATE
void GameLayer::update(float dt)
{
    if (_playing) {
        _world->Step(dt, 8, 3);
        _player1->update(dt);
        _player2->update(dt);
        _puck->update(dt);
    }
    
    if (_minutes == 0 || _seconds == 0 || _score1 == 7 || _score2 == 7) {
        _playing = false;
        _score1 = _score2 = 0;
        _scoreLabel1->setString("0");
        _scoreLabel2->setString("0");
        this->unscheduleAllSelectors();
        this->unscheduleUpdate();
    }
    
    
    if (_puck->getBody()->GetPosition().y*PTM_RATIO > h + _puck->getRadius()) {
        this->scoreCounter(1);
        this->gameReset();
    }
    
    if (_puck->getBody()->GetPosition().y*PTM_RATIO < -_puck->getRadius()) {
        this->scoreCounter(2);
        this->gameReset();
    }
    
    std::vector<MyContact>::iterator pos;
    for(pos = _contactListener->_contacts.begin();
        pos != _contactListener->_contacts.end(); ++pos) {
        MyContact contact = *pos;
        
        if ((contact.fixtureA == _puck->getFixture() &&
             contact.fixtureB == _player2->getFixture()) ||
            (contact.fixtureA == _player2->getFixture() &&
             contact.fixtureB == _puck->getFixture())) {
            lastHit = 0;
            this->defenseCenter();
        }
    }
    if (lastHit >= 450) {
        this->defenseCenter();
    }
}

#pragma mark Get Game State Info
void GameLayer::getStateInfo() {
    x   = _puck->getPositionX();
    y   = _puck->getPositionY();
    px  = _player2->getPositionX();
    py  = _player2->getPositionY();
    vx  = _puck->getBody()->GetLinearVelocity().x;
    vy  = _puck->getBody()->GetLinearVelocity().y;
    vpx = _player2->getBody()->GetLinearVelocity().x;
    vpy = _player2->getBody()->GetLinearVelocity().y;
    pr = _puck->getRadius();
}

#pragma mark AI PLAYER
void GameLayer::handleProcess() {
    lastHit+= 25;
    this->getStateInfo();
        
    if (lastHit >= 450) {
        if ((y >= h / 2 - pr && y <= 3 * h /4) ||
            (y > 3 * h / 4 && x > w / 2 && x < 3 * w / 4)) {
            this->attack();
        }
    } else {
        this->defenseCenter();
    }
}

void GameLayer::defenseLeft() {
    this->getStateInfo();
    _player2->getBody()->ApplyLinearImpulse(
        this->ptm2(7*(w*3/8 - px), 7*(h - pr - 10 - py)),
        _player2->getBody()->GetWorldCenter());

}

void GameLayer::defenseRight() {
    this->getStateInfo();
    _player2->getBody()->ApplyLinearImpulse(
        this->ptm2(7*(w*5/8 - px), 7*(h - pr - 10 - py)),
        _player2->getBody()->GetWorldCenter());
}

void GameLayer::defenseCenter() {
    this->getStateInfo();
    _player2->getBody()->ApplyLinearImpulse(
        this->ptm2(17*(w/2 - px), 17*(h - pr - 10 - py)),
        _player2->getBody()->GetWorldCenter());
}

void GameLayer::attack() {
    float cx = (h - 10 - pr - y)*vx/vy + x;
    if ((cx > w / 4 && cx < w * 3 / 4) || (vx < 5 && vy < 5))
        _player2->getBody()->ApplyLinearImpulse(
            b2Vec2(17*(x - px), 17*(10 + y - py)),
            _player2->getBody()->GetWorldCenter());
    else _player2->getBody()->SetLinearVelocity(b2Vec2((x/2 + w/4 - px) / 5, 0));

    
}

#pragma mark TOUCHES HANDLE
void GameLayer::ccTouchesBegan(CCSet* touches, CCEvent* event) {
    if (_mouseJoint != NULL) return;
    CCTouch *touch = (CCTouch *)touches->anyObject();
    CCPoint tap = touch->getLocation();
    b2Vec2 target = this->ptm(tap);

    if (tap.y < h/2 - _player1->getRadius() &&
        tap.y > 10 + _player1->getRadius()  &&
        tap.x > 10 + _player1->getRadius()  &&
        tap.x < w - _player1->getRadius()) {
        b2MouseJointDef md;
        md.bodyA =  _groundBody;
        md.bodyB = _player1->getBody();
        md.target = target;
        md.collideConnected = true;
        md.maxForce = 100000.0f * _player1->getBody()->GetMass();
        md.dampingRatio = 0;
        md.frequencyHz = 1000;
        
        _player1->setSpritePosition(tap);
        _mouseJoint = (b2MouseJoint *)_world->CreateJoint(&md);
        _player1->getBody()->SetAwake(true);
    }
    
}

void GameLayer::ccTouchesMoved(CCSet* touches, CCEvent* event) {
    if (_mouseJoint == NULL) return;
    CCTouch *touch = (CCTouch *)touches->anyObject();
    CCPoint tap = touch->getLocation();
    if (tap.y > h/2 || tap.y < 10 ||
        tap.x > w - 10 || tap.x < 10) {
//        _world->DestroyJoint(_mouseJoint);
//        _mouseJoint = NULL;
        return;
    }
    b2Vec2 target = this->ptm(tap);
    _mouseJoint->SetTarget(target);
}

void GameLayer::ccTouchesEnded(CCSet* touches, CCEvent* event) {
    if (_mouseJoint != NULL) {
        _world->DestroyJoint(_mouseJoint);
        _mouseJoint = NULL;
    }
}

#pragma mark GAME RESET
void GameLayer::gameReset() {
    _player1->reset();
    _player2->reset();
    _puck->getBody()->SetLinearVelocity(b2Vec2_zero);
    
    if (_mouseJoint != NULL) {
        _world->DestroyJoint(_mouseJoint);
        _mouseJoint = NULL;
    }
}

#pragma mark SCORE COUNTER
void GameLayer::scoreCounter(int player) {
    char scoreBuf[10];
    
    if (player == 1) {
        _score1++;
        sprintf(scoreBuf, "%d", _score1);
        _scoreLabel1->setString(scoreBuf);
        _puck->setSpritePosition(ccp(w/2, h/2 + 2*_puck->getRadius()));
    }
    
    if (player == 2) {
        _score2++;
        sprintf(scoreBuf, "%d", _score2);
        _scoreLabel2->setString(scoreBuf);
        _puck->setSpritePosition(ccp(w/2, h/2 - 2*_puck->getRadius()));
    }
}

#pragma mark Timer
void GameLayer::Timer() {
    if(_playing && _minutes >= 0) {
		if(_seconds > 0)	_seconds--;
		else {
			if(_minutes > 0) _minutes--;
			else  _minutes = 0;
			_seconds = 60;
		}
	}
    
	char timeBuf[20] = {0};
	if(_minutes < 10 && _seconds < 10)
        sprintf(timeBuf, "0%i:0%i", _minutes, _seconds);
	else if(_minutes < 10 && _seconds >= 10)
        sprintf(timeBuf, "0%i:%i", _minutes, _seconds);
    
    _time->setString(timeBuf);
}

#pragma mark SCENE
CCScene* GameLayer::scene()
{
    CCScene *scene = CCScene::create();
    GameLayer *layer = new GameLayer();
    scene->addChild(layer);
    layer->release();
    return scene;
}
