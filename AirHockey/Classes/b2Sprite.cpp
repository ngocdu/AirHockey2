//
//  b2Sprite.cpp
//  AirHockey
//
//  Created by Trung Kien Do on 13/07/09.
//
//

#include "b2Sprite.h"
#include "Gamelayer.h"

b2Sprite::b2Sprite (GameLayer *game, int type) {
    _game = game;
    _type = type;
}

void b2Sprite::setSpritePosition(CCPoint position) {
    setPosition(position);
    
    if (_body) {
        _body->SetTransform(b2Vec2(position.x / PTM_RATIO,
                                   position.y / PTM_RATIO),
                            _body->GetAngle());
    }
}

void b2Sprite::update(float dt){
    if (_body && isVisible()) {
        setPositionX(_body->GetPosition().x * PTM_RATIO);
        setPositionY(_body->GetPosition().y * PTM_RATIO);
        setRotation(CC_RADIANS_TO_DEGREES(-1 * _body->GetAngle()));
    }
}

void b2Sprite::hide(void) {
    if (_body) {
        _body->SetLinearVelocity(b2Vec2_zero);
        _body->SetAngularVelocity(0);
        _body->SetTransform(b2Vec2_zero, 0.0);
        _body->SetAwake(false);
    }
}

void b2Sprite::reset() {}

float b2Sprite::mag() {
    if (_body) {
        return pow(_body->GetLinearVelocity().x, 2) +
        pow(_body->GetLinearVelocity().y, 2);
    }
    return 0.0;
}