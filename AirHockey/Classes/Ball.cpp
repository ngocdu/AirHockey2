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

Ball::Ball(GameLayer * game, int type) : b2Sprite(game, type) {
    _type = type;
    _inPlay = true;

}

Ball * Ball::create(GameLayer *game, int type, const char *pszFileName) {
    Ball *sprite = new Ball(game, type);
    
    if (sprite) {
        sprite->initBall(pszFileName);
        return sprite;
    }
    
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void Ball::initBall(const char *pszFileName) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    if (_type == humanPlayer || _type == aiPlayer) {
        bodyDef.linearDamping = 5.0;
    }

    _body = _game->getWorld()->CreateBody(&bodyDef);
    _body->SetSleepingAllowed(true);
    _body->SetBullet(true);
    
    b2CircleShape circle;
    b2FixtureDef fixtureDef;
    
    if (_type == humanPlayer || _type == aiPlayer) {
        fixtureDef.filter.groupIndex = 0;
        fixtureDef.density = 5.0;
        fixtureDef.friction = 0.3;
        fixtureDef.restitution = 0.1;
    } else if (_type == puck) {
        fixtureDef.filter.groupIndex = -10;
        fixtureDef.density = 0.0;
        fixtureDef.friction = 0.0;
        fixtureDef.restitution = 0.7;
    }
    
    this->initWithFile(pszFileName);
    _radius = this->getContentSize().width / 2;
    circle.m_radius = _radius / PTM_RATIO;
    fixtureDef.shape =  &circle;
    _fixture = _body->CreateFixture(&fixtureDef);
    _body->SetUserData(this);
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


