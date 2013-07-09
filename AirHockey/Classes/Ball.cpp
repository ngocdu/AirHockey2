//
//  Ball.cpp
//  AirHockey
//
//  Created by Trung Kien Do on 13/07/09.
//
//

#include "Ball.h"
#include "GameLayer.h"

Ball::~Ball() {
    
}

Ball::Ball(GameLayer * game, int type, CCPoint position) : b2Sprite(game, type) {
    _type = type;
    _startPosition = position;
    _inPlay = true;
}

Ball * Ball::create(GameLayer *game, int type, CCPoint position) {
    Ball *sprite = new Ball(game, type, position);
    
    if (sprite) {
        sprite->initBall();
        sprite->autorelease();
        return sprite;
    }
    
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void Ball::initBall() {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    _body = _game->getWorld()->CreateBody(&bodyDef);
    _body->SetSleepingAllowed(true);

    b2CircleShape circle;
    b2FixtureDef fixtureDef;
    
    if (_type == kSpriteBall) {
        _body->SetBullet(true);
        fixtureDef.filter.groupIndex = -10;
        circle.m_radius = ballRadius / PTM_RATIO;
    } else if (_type == kSpritePlayer) {
        fixtureDef.filter.groupIndex = 0;
        circle.m_radius = playerRadius / PTM_RATIO;
    }

    fixtureDef.shape =  &circle;
    fixtureDef.density = 5;
    fixtureDef.restitution = 0.7;
    
    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(this);
    
    setSpritePosition(_startPosition);
}

void Ball::reset() {
    if (_body) {
        _body->SetLinearVelocity(b2Vec2_zero);
        _body->SetAngularVelocity(0);
        _body->SetAwake(true);
    }
    setSpritePosition(_startPosition);
    _inPlay = true;
    setVisible(true);
}

void Ball::update(float dt) {
    if (_body && isVisible()) {
        setPositionX(_body->GetPosition().x * PTM_RATIO);
        setPositionY(_body->GetPosition().y * PTM_RATIO);
    }
}


