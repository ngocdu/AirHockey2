//
//  Ball.h
//  AirHockey
//
//  Created by Trung Kien Do on 13/07/09.
//
//

#ifndef __AirHockey__Ball__
#define __AirHockey__Ball__

#include <iostream>
#include "b2Sprite.h"

enum {
    playerRadius    = 160 / 2,
    ballRadius      = 85 / 2
};


enum {
    humanPlayer,
    aiPlayer,
    puck
};

class Ball : public b2Sprite {
public:
    CC_SYNTHESIZE(bool, _inPlay, InPlay);
    CC_SYNTHESIZE(float, _radius, Radius);
    CC_SYNTHESIZE(CCPoint, _startPosition, StartPos);
    
    ~Ball();
    Ball(GameLayer *game, int type);
    static Ball * create(GameLayer *game, int type, const char *pszFileName);
    virtual void reset(void);
    virtual void update(float dt);
    
private:
    void initBall(const char *pszFileName);
};

#endif /* defined(__AirHockey__Ball__) */
