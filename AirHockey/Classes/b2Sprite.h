//
//  b2Sprite.h
//  AirHockey
//
//  Created by Trung Kien Do on 13/07/09.
//
//

#ifndef __AirHockey__b2Sprite__
#define __AirHockey__b2Sprite__

#define PTM_RATIO 32.0

#include <iostream>
#include "cocos2d.h"
#include "Box2D.h"

class GameLayer;

enum {
    kSpritePlayer,
    kSpriteBall
};

enum {
    playerRadius = 160 / 2,
    ballRadius = 85 / 2
};

USING_NS_CC;

class b2Sprite : public CCSprite {
public:
    b2Sprite (GameLayer *game, int type);
    
    CC_SYNTHESIZE(b2Body *, _body, Body);
    CC_SYNTHESIZE(GameLayer *, _game, Game);
    CC_SYNTHESIZE(int, _type, Type);
    
    virtual void setSpritePosition(CCPoint position);
    virtual void update(float dt);
    virtual void hide(void);
    virtual void reset(void);
    virtual float mag();
    
private:
};


#endif /* defined(__AirHockey__b2Sprite__) */
