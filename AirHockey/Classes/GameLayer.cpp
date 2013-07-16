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
    
    // End Game
    _endLayerBg = CCSprite::create("EndGameBG.png");
    _endLayerBg->setPosition(ccp(w/2, h/2));
    this->addChild(_endLayerBg, 5);
    ew = _endLayerBg->getContentSize().width;
    eh = _endLayerBg->getContentSize().height;
    _rematchButton = CCSprite::create("rematch.png");
    _rematchButton->setPosition(ccp(ew/2, eh/2));
    _endLayerBg->addChild(_rematchButton);
    
    _quitButton = CCSprite::create("quit.png");
    _quitButton->setPosition(ccp(ew/2, eh/4));
    _endLayerBg->addChild(_quitButton);
    
    resultLabel = CCLabelTTF::create("DRAW", "Arial", 64);
    resultLabel->setPosition(ccp(ew/2, eh*3/4));
    _endLayerBg->addChild(resultLabel);
    _endLayerBg->setVisible(false);
    
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
    groundEdgeDef.filter.groupIndex = groupIndex;
    _groundBody->CreateFixture(&groundEdgeDef);
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
    
    if ((_minutes == 0 && _seconds == 0) || _score1 == 3 || _score2 == 3) {
        _playing = false ;
        this->pauseSchedulerAndActions();
        this->endGame();
    }

    // Apply impluse when the puck is near the edges
    if (_puck->getPositionX() <= 15 + _puck->getRadius()) {
        _puck->getBody()->ApplyLinearImpulse(b2Vec2(10, 0),
                             _puck->getBody()->GetWorldCenter());
    }

    if (_puck->getPositionX() >= w - 15 - _puck->getRadius()) {
        _puck->getBody()->ApplyLinearImpulse(b2Vec2(-10, 0),
                             _puck->getBody()->GetWorldCenter());
    }
    
    if (_puck->getPositionX() <= 260 || _puck->getPositionX() >= w - 260) {
        if (_puck->getPositionY() >= h - 15 - _puck->getRadius()) {
            _puck->getBody()->ApplyLinearImpulse(b2Vec2(0, -10),
                                _puck->getBody()->GetWorldCenter());
        }
        if (_puck->getPositionY() <= 15 + _puck->getRadius()) {
            _puck->getBody()->ApplyLinearImpulse(b2Vec2(0, 10),
                                 _puck->getBody()->GetWorldCenter());
        }
    }
    
    // Gloal !!!
    if (_puck->getPositionY() > h + _puck->getRadius()) {
        this->scoreCounter(1);
        this->newTurn();
    }
    
    if (_puck->getPositionY() < -_puck->getRadius()) {
        this->scoreCounter(2);
        this->newTurn();
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
        this->ptm2(50*(w*3/8 - px), 50*(h - pr - 10 - py)),
        _player2->getBody()->GetWorldCenter());
}

void GameLayer::defenseRight() {
    this->getStateInfo();
    _player2->getBody()->ApplyLinearImpulse(
        this->ptm2(50*(w*5/8 - px), 50*(h - pr - 10 - py)),
        _player2->getBody()->GetWorldCenter());
}

void GameLayer::defenseCenter() {
    this->getStateInfo();
    _player2->getBody()->ApplyLinearImpulse(
        this->ptm2(50*(w/2 - px), 50*(h - pr - 10 - py)),
        _player2->getBody()->GetWorldCenter());
}

void GameLayer::attack() {
    float cx = (h - 10 - pr - y)*vx/vy + x;
    if ((cx > w/4 && cx < w*3/4) || (vx < 10 && vy < 10))
        _player2->getBody()->ApplyLinearImpulse(
            b2Vec2(15*(x - px), 15*(10 + y - py)),
            _player2->getBody()->GetWorldCenter());
    else _player2->getBody()->SetLinearVelocity(b2Vec2((x/2 + w/4 - px)/5, 0));

    
}

#pragma mark TOUCHES HANDLE
void GameLayer::ccTouchesBegan(CCSet* touches, CCEvent* event) {
    if (_playing) {
        CCLOG("%s", CCUserDefault::sharedUserDefault()->getStringForKey("Difficulty").c_str());
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
    } else {
        CCTouch *touch = (CCTouch *)touches->anyObject();
        CCPoint tap = touch->getLocation();
        
        CCPoint p1 = _endLayerBg->convertToWorldSpace(_rematchButton->getPosition());
        CCPoint p2 = _endLayerBg->convertToWorldSpace(_quitButton->getPosition());
        
        float rmw = _rematchButton->getContentSize().width;
        float rmh = _rematchButton->getContentSize().height;
        
        float qw  = _quitButton->getContentSize().width;
        float qh  = _quitButton->getContentSize().height;
        
        CCRect rematchRect = CCRectMake(p1.x - rmw/2, p1.y - rmh/2, rmw, rmh);
        CCRect quitRect    = CCRectMake(p2.x - qw/2, p2.y - qh/2, qw, qh);
                                        
        if (quitRect.containsPoint(tap)) {
            CCDirector::sharedDirector()->replaceScene(Menu::scene());
        }
        if (rematchRect.containsPoint(tap)) {
            _playing = true;
            this->resumeSchedulerAndActions();
            _endLayerBg->setVisible(false);
            this->gameReset();
        }
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
    _score1 = _score2 = 0;
    _minutes = 2;
    _seconds = 60;
    
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

#pragma mark Check High Score
void GameLayer::checkHighScore() {
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://192.168.1.104:3000/users.json");
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
            if (document[i]["name"].GetString() == name &&
                point == document[i]["point"].GetInt() &&
                document[i]["reward"].GetInt() != 0) {
                int rewark = CCUserDefault::sharedUserDefault()->getIntegerForKey("reward");
                CCUserDefault::sharedUserDefault()->setIntegerForKey("reward", rewark + 1);
                CCUserDefault::sharedUserDefault()->setIntegerForKey("pointMax", point);
                CCDirector::sharedDirector()->replaceScene(GetPresent::scene());
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
        resultLabel->setString("YOU WIN");
        //---------send request to server ------------
        CCHttpRequest* request = new CCHttpRequest();
        int p = (_score1 + 1) * (180 - (_minutes * 60 + _seconds)) *
                (GameManager::sharedGameManager()->getLevel() * 2000);
        GameManager::sharedGameManager()->setPoint(p);
        string name = GameManager::sharedGameManager()->getName();
        char strP[20] = {0};
        sprintf(strP, "%i", p);
        string url = "http://192.168.1.104:3000/users?name="+name+"&point="+strP+"&email=ngocduk54a2@gmail.com"+"&reward=0";
        request->setUrl(url.c_str());
        request->setRequestType(CCHttpRequest::kHttpPost);
        CCHttpClient::getInstance()->send(request);
        request->release();
        this->checkHighScore();
    }
    else if (_score1 == _score2) resultLabel->setString("DRAW");
    else resultLabel->setString("YOU LOSE");
    
    _endLayerBg->setVisible(true);
}
