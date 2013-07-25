//
//  GameLayer.cpp
//  AirHockey
//
//  Created by Trung Kien Do on 13/07/09.
//  Copyright __FRAMGIA__ 2013年. All rights reserved.
//

#include "GameLayer.h"

#pragma mark SCENE
CCScene* GameLayer::scene() {
    CCScene *scene = CCScene::create();
    GameLayer *layer = new GameLayer();
    scene->addChild(layer);
    layer->release();
    return scene;
}

#pragma mark CONSTRUCTOR
GameLayer::GameLayer() {
    setTouchEnabled(true);
    setAccelerometerEnabled(true);
    SimpleAudioEngine::sharedEngine()->preloadEffect("hitPuck.wav");
    _level = GameManager::sharedGameManager()->getLevel();
    CCLOG("gamelayer name: %s", GameManager::sharedGameManager()->getName().c_str());

//    float x=0, y=0, z=0;
//    this->getCamera()->getCenterXYZ(&x, &y, &z);
//    this->getCamera()->setCenterXYZ(x, y+0.0000001, z);
//    this->getCamera()->setEyeXYZ(x, y, 70);
    CCSprite *backGroundImg = CCSprite::create("Court3.png");
    backGroundImg->setPosition(ccp(w/2, h/2));
    this->addChild(backGroundImg);
    
    // Score Counter
    _scoreLabel1 = CCLabelTTF::create("0", "BankGothic Md BT", 48);
    _scoreLabel1->setColor(ccBLACK);
    _scoreLabel2 = CCLabelTTF::create("0", "BankGothic Md BT", 48);
    _scoreLabel2->setColor(ccBLACK);
    _scoreLabel1->setPosition(ccp(w - 70, h/2 - 100));
    _scoreLabel1->setRotation(90);
    _scoreLabel2->setPosition(ccp(w - 70, h/2 + 100));
    _scoreLabel2->setRotation(90);
    this->addChild(_scoreLabel1);
    this->addChild(_scoreLabel2);

    // Pause Button
    _pauseButton = CCSprite::create("PauseButton.png");
    _pauseButton->setPosition(ccp(w - 70, h/2));
    this->addChild(_pauseButton);
    
    // End Game
    _endLayerBg = CCSprite::create("EndGameBG.png");
    _endLayerBg->setPosition(ccp(w/2, h/2));
    this->addChild(_endLayerBg, 5);
    ew = _endLayerBg->getContentSize().width;
    eh = _endLayerBg->getContentSize().height;
    _quitButton     = CCSprite::create("Quit.png");
    _restartButton  = CCSprite::create("Restart.png");
    _continueButton = CCSprite::create("Continue.png");
    resultLabel     = CCLabelTTF::create("DRAW", "BankGothic Md BT", 64);

    _quitButton->setPosition(ccp(ew/2, eh/4));
    _restartButton->setPosition(ccp(ew/2, eh/2));
    _continueButton->setPosition(ccp(ew/2, eh*3/4));
    resultLabel->setPosition(ccp(ew/2, eh*3/4));
    
    _endLayerBg->addChild(_quitButton);
    _endLayerBg->addChild(_restartButton);
    _endLayerBg->addChild(_continueButton);
    _endLayerBg->addChild(resultLabel);
    _endLayerBg->setVisible(false);
    
    // Timer
    _minutes = 3;
    _seconds = 00;
    _playing = false;
    char timeBuf[20] = {0};
	sprintf(timeBuf, "0%i:0%i", _minutes, _seconds);

    _time = CCLabelTTF::create(timeBuf, "BankGothic Md BT", 40);
    _time->setColor(ccBLACK);
	_time->setPosition(ccp(60, h/2));
    _time->setRotation(90);
	this->addChild(_time);
    
    // Physics
    this->initPhysics();
    
    CCSprite *startGame = CCSprite::create("OnStart.png");
    startGame->setPosition(ccp(-w/2, h/2));
    this->addChild(startGame, 9, 1);
    
    CCFiniteTimeAction *move1  = CCMoveTo::create(1, ccp(w/2, h/2));
    CCFiniteTimeAction *move2  = CCMoveTo::create(1, ccp(w*3/2, h/2));
    CCFiniteTimeAction *delay = CCDelayTime::create(0.2);
    CCFiniteTimeAction *start = CCCallFuncN::create(this, callfuncN_selector(GameLayer::onStart));
    startGame->runAction(CCSequence::create(move1, delay, move2, start, NULL));
    
    // Scheduler
    scheduleUpdate();
    this->schedule(schedule_selector(GameLayer::Timer), 1);
    this->schedule(schedule_selector(GameLayer::handleProcess), 0.025);
}

#pragma mark DESTRUCTOR
GameLayer::~GameLayer() {
    delete _world;
    _world = NULL;
    
    delete m_debugDraw;
}

#pragma mark INIT PHYSICS
void GameLayer::initPhysics() {
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
    _player1 = Ball::create(this, humanPlayer, "Mallet1_2.png");
    _player1->setStartPos(ccp(w/2, _player1->getRadius()*2));
    _player1->setSpritePosition(_player1->getStartPos());
    this->addChild(_player1);
    _player2 = Ball::create(this, aiPlayer, "Mallet2_2.png");
    _player2->setStartPos(ccp(w/2, h - _player2->getRadius()*2));
    _player2->setSpritePosition(_player2->getStartPos());
    this->addChild(_player2);
    _puck = Ball::create(this, puck, "Puck.png");
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
    groundEdgeDef.filter.groupIndex = groupIndex;
    _groundBody->CreateFixture(&groundEdgeDef);
}

void GameLayer::onStart() {
    _playing = true;
    this->removeChildByTag(1);
}

#pragma mark DRAW
void GameLayer::draw() {
    CCLayer::draw();
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    kmGLPushMatrix();
    _world->DrawDebugData();
    kmGLPopMatrix();
}

#pragma mark UPDATE
void GameLayer::update(float dt) {
    if (_playing) {
        _world->Step(dt, 8, 3);
        _player1->update(dt);
        _player2->update(dt);
        _puck->update(dt);
    }
    
    if ((_minutes == 0 && _seconds == 0) || _score1 == 1 || _score2 == 1) {
        _playing = false ;
        _isEnd = true;
        this->pauseSchedulerAndActions();
        this->endGame();
    }

    // Apply impluse when the puck is near the edges
    if (_puck->getPositionX() <= 15 + _puck->getRadius())
        _puck->getBody()->ApplyLinearImpulse(b2Vec2(10, 0),
                                             _puck->getBody()->GetWorldCenter());

    if (_puck->getPositionX() >= w - 15 - _puck->getRadius())
        _puck->getBody()->ApplyLinearImpulse(b2Vec2(-10, 0),
                                             _puck->getBody()->GetWorldCenter());
    
    if (_puck->getPositionX() <= 260 || _puck->getPositionX() >= w - 260) {
        if (_puck->getPositionY() >= h - 15 - _puck->getRadius())
            _puck->getBody()->ApplyLinearImpulse(b2Vec2(0, -10),
                                             _puck->getBody()->GetWorldCenter());
        if (_puck->getPositionY() <= 15 + _puck->getRadius())
            _puck->getBody()->ApplyLinearImpulse(b2Vec2(0, 10),
                                             _puck->getBody()->GetWorldCenter());
    }
    
    // Gloal !!!
    if (_puck->getPositionY() > h + _puck->getRadius()) {
        SimpleAudioEngine::sharedEngine()->playEffect("score.wav");
        this->scoreCounter(1);
        this->newTurn();
        _puck->setPosition(ccp(w/2, h/2 + 3 * _puck->getRadius()));
    }
    
    if (_puck->getPositionY() < -_puck->getRadius()) {
        SimpleAudioEngine::sharedEngine()->playEffect("score.wav");
        this->scoreCounter(2);
        this->newTurn();
        _puck->setPosition(ccp(w/2, h/2 - 3 * _puck->getRadius()));
    }
    
    // Collision Detection
    std::vector<MyContact>::iterator pos;
    for(pos = _contactListener->_contacts.begin();
        pos != _contactListener->_contacts.end(); ++pos) {
        MyContact contact = *pos;
        
        if (contact.fixtureA == _puck->getFixture() ||
            contact.fixtureB == _puck->getFixture()) {
            SimpleAudioEngine::sharedEngine()->playEffect("hitPuck.wav");
        }
        
        if ((contact.fixtureA == _puck->getFixture()     &&
             contact.fixtureB == _player2->getFixture()) ||
            (contact.fixtureA == _player2->getFixture()  &&
             contact.fixtureB == _puck->getFixture())) {
            lastHit = 0;
            this->defenseCenter();
        }
    }
    if (lastHit >= 1200/_level) {
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
        
    if (lastHit >= 1200/_level) {
        if ((y >= h/2 - pr && y <= 3*h/4) ||
            (y > 3*h/4 && x > w/2 && x < 3*w/4)) {
            this->attack();
        }
    } else {
        this->defenseCenter();
    }
}

void GameLayer::defenseLeft() {
    this->getStateInfo();
    _player2->getBody()->ApplyLinearImpulse(
        this->ptm2(15*_level*(w*3/8 - px), 15*_level*(h - pr - 10 - py)),
        _player2->getBody()->GetWorldCenter());
}

void GameLayer::defenseRight() {
    this->getStateInfo();
    _player2->getBody()->ApplyLinearImpulse(
        this->ptm2(15*_level*(w*5/8 - px), 15*_level*(h - pr - 10 - py)),
        _player2->getBody()->GetWorldCenter());
}

void GameLayer::defenseCenter() {
    this->getStateInfo();
    _player2->getBody()->ApplyLinearImpulse(
        this->ptm2(15*_level*(w/2 - px), 15*_level*(h - pr - 10 - py)),
        _player2->getBody()->GetWorldCenter());
}

void GameLayer::attack() {
    float cx = (h - 10 - pr - y)*vx/vy + x;
    if ((cx > w/4 && cx < w*3/4) || (vx < 10 && vy < 10))
        _player2->getBody()->ApplyLinearImpulse(
            b2Vec2(5*_level*(x - px), 5*_level*(10 + y - py)),
            _player2->getBody()->GetWorldCenter());
    else _player2->getBody()->SetLinearVelocity(b2Vec2((x/2 + w/4 - px)/5, 0));

    
}

#pragma mark TOUCHES HANDLE
void GameLayer::ccTouchesBegan(CCSet* touches, CCEvent* event) {
    if (_playing) {
        if (_mouseJoint != NULL) return;
        CCTouch *touch = (CCTouch *)touches->anyObject();
        CCPoint tap = touch->getLocation();
        b2Vec2 target = this->ptm(tap);
        CCRect pauseRect = _pauseButton->boundingBox();
        
        if (pauseRect.containsPoint(tap)) {
            _isPauseClicked = true;
            _continueButton->setVisible(true);
        } else {
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
    } else {
        CCTouch *touch = (CCTouch *)touches->anyObject();
        CCPoint tap = touch->getLocation();
        
        CCPoint p1 = _endLayerBg->convertToWorldSpace(_restartButton->getPosition());
        CCPoint p2 = _endLayerBg->convertToWorldSpace(_quitButton->getPosition());
        CCPoint p3 = _endLayerBg->convertToWorldSpace(_continueButton->getPosition());
        
        float rmw = _restartButton->getContentSize().width;
        float rmh = _restartButton->getContentSize().height;
        
        float qw  = _quitButton->getContentSize().width;
        float qh  = _quitButton->getContentSize().height;
        
        float cw = _continueButton->getContentSize().width;
        float ch = _continueButton->getContentSize().height;
        
        CCRect restartRect  = CCRectMake(p1.x - rmw/2, p1.y - rmh/2, rmw, rmh);
        CCRect quitRect     = CCRectMake(p2.x - qw/2, p2.y - qh/2, qw, qh);
        CCRect continueRect = CCRectMake(p3.x - cw/2, p3.y - ch/2, cw, ch);
        
        CCRect p1Rect = _player1->boundingBox();
        
        if (continueRect.containsPoint(tap)) {
            _playing = true;
            this->resumeSchedulerAndActions();
            _endLayerBg->setVisible(false);
        }
        
        if (quitRect.containsPoint(tap)) {
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, RankingScene::scene()));
        }
        if (restartRect.containsPoint(tap)) {
            _playing = true;
            this->resumeSchedulerAndActions();
            _endLayerBg->setVisible(false);
            this->gameReset();
        }
        if (p1Rect.containsPoint(tap)) {
            b2MouseJointDef md;
            md.bodyA =  _groundBody;
            md.bodyB = _player1->getBody();
            md.target = this->ptm(tap);
            md.collideConnected = true;
            md.maxForce = 100000.0f * _player1->getBody()->GetMass();
            md.dampingRatio = 0;
            md.frequencyHz = 1000;
            
            _mouseJoint = (b2MouseJoint *)_world->CreateJoint(&md);
            _player1->getBody()->SetAwake(true);
        }
    }
}

void GameLayer::ccTouchesMoved(CCSet* touches, CCEvent* event) {
    if (_mouseJoint == NULL) return;
    if (_playing) {
        CCTouch *touch = (CCTouch *)touches->anyObject();
        CCPoint tap = touch->getLocation();
        if (tap.y > h/2 || tap.y < 10 ||
            tap.x > w - 10 || tap.x < 10) {
            return;
        }
        b2Vec2 target = this->ptm(tap);
        _mouseJoint->SetTarget(target);
    }
}

void GameLayer::ccTouchesEnded(CCSet* touches, CCEvent* event) {
    if (_isPauseClicked) {
        CCTouch *touch = (CCTouch *)touches->anyObject();
        CCPoint tap = touch->getLocation();
        CCRect pauseRect = _pauseButton->boundingBox();
        
        if (pauseRect.containsPoint(tap)) {
            _endLayerBg->setVisible(true);
            this->pauseSchedulerAndActions();
            _playing = false;
            if (!_isEnd)    resultLabel->setVisible(false);
        }
    } else {
        if (_mouseJoint != NULL) {
            _world->DestroyJoint(_mouseJoint);
            _mouseJoint = NULL;
        }
    }
}

#pragma mark New Turn
void GameLayer::newTurn() {
    _player1->reset();
    _player2->reset();
    _puck->getBody()->SetLinearVelocity(b2Vec2_zero);
    
    if (_mouseJoint != NULL) {
        _world->DestroyJoint(_mouseJoint);
        _mouseJoint = NULL;
    }
}

#pragma mark GAME RESET
void GameLayer::gameReset() {
    _player1->reset();
    _player2->reset();
    _puck->reset();
    
    _isEnd = false;
    _score1 = _score2 = 0;
    _minutes = 3;
    _seconds = 0;
    
    _scoreLabel1->setString("0");
    _scoreLabel2->setString("0");
    
    char timeBuf[20] = {0};
	if(_minutes < 10 && _seconds < 10)
        sprintf(timeBuf, "0%i:0%i", _minutes, _seconds);
	else if(_minutes < 10 && _seconds >= 10)
        sprintf(timeBuf, "0%i:%i", _minutes, _seconds);
    
    _time->setString(timeBuf);
    
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
			_seconds = 59;
		}
	}
    
	char timeBuf[20] = {0};
	if(_minutes < 10 && _seconds < 10)
        sprintf(timeBuf, "0%i:0%i", _minutes, _seconds);
	else if(_minutes < 10 && _seconds >= 10)
        sprintf(timeBuf, "0%i:%i", _minutes, _seconds);
    
    _time->setString(timeBuf);
}

#pragma mark Check High Score
void GameLayer::checkHighScore() {
    CCHttpRequest* request = new CCHttpRequest();
    string ipAddr = GameManager::sharedGameManager()->getIpAddr();
    request->setUrl((ipAddr + ":3000/users.json").c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(GameLayer::onHttpRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void GameLayer::onHttpRequestCompleted(CCNode *sender, void *data) {
    CCHttpResponse *response = (CCHttpResponse*)data;
    if (!response)
    {
        return;
    }
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode,
            response->getHttpRequest()->getTag());
    
    if (!response->isSucceed())
    {
        return;
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    char * data2 = (char*)(malloc(buffer->size() *  sizeof(char)));
    int d = -1;
    printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        d++ ;
        data2[d] = (*buffer)[i];
    }
    data2[d + 1] = '\0';
    //-----------------------
    rapidjson::Document document;
    if(data2 != NULL && !document.Parse<0>(data2).HasParseError())
    {
        string name = GameManager::sharedGameManager()->getName();
        int point = (_score1 + 1) * (180 - (_minutes * 60 + _seconds)) *
        (GameManager::sharedGameManager()->getLevel() * 2000);
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++)
        {
            if (point > document[i]["point"].GetInt()) {
                CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, GetPresent::scene()));
                break;
            }
        }
    }
    else
    {
        CCLog(document.GetParseError());
    }
    d = -1;
    delete []data2;
}

void GameLayer::endGame() {
    if (_score1 > _score2) {
        int p = (_score1 + 1) * (180 - (_minutes * 60 + _seconds)) *
                (GameManager::sharedGameManager()->getLevel() * 2000);
        resultLabel->setString("YOU WIN");
        resultLabel->runAction(CCBlink::create(2.0f, 5.0f));
        CCPoint effectPoint =
            _endLayerBg->convertToWorldSpace(resultLabel->getPosition());
        this->addEffect(ccp(effectPoint.x - 100, effectPoint.y));
        this->addEffect(ccp(effectPoint.x + 100, effectPoint.y));
        
        GameManager::sharedGameManager()->setPoint(p);
        this->checkHighScore();
    }
    else if (_score1 == _score2) resultLabel->setString("DRAW");
    else resultLabel->setString("YOU LOSE");
    
    _endLayerBg->setVisible(true);
    resultLabel->setVisible(true);
    _continueButton->setVisible(false);
}


void GameLayer::addEffect(CCPoint point) {
    CCParticleSystemQuad *pop = new CCParticleSystemQuad;
    pop = CCParticleGalaxy::create();
    pop->setTexture(CCTextureCache::sharedTextureCache()->addImage("Star.png"));
    pop->setPosition(point);
    pop->setGravity(CCPointZero);
    
    pop->setAngle(140);
    pop->setAngleVar(360);
    
    pop->setSpeed(80);
    //pop->setSpeedVar(360);
    
    pop->setLife(2.0f);
    pop->setStartSize(40);
    
    pop->setPositionType(kCCPositionTypeRelative);
    
    pop->setRadialAccel(-50);
    //pop->setRadialAccelVar(-100);
    
    //pop->setTangentialAccel(-50);
    pop->setTotalParticles(9);
    //pop->setRotatePerSecond(0);
    pop->setAutoRemoveOnFinish(true);
    pop->setAtlasIndex(0);
    pop->setBlendAdditive(false);
    //pop->setOrderOfArrival(0);
    pop->setOpacityModifyRGB(false);
    pop->setDuration(0.5);
    pop->setEmissionRate(200);
    //pop->setEndRadius(50);
    this->addChild(pop, 100, 777);
}
