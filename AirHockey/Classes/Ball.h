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

class Ball : public b2Sprite {
public:
    CC_SYNTHESIZE(bool, _inPlay, InPlay);
    CC_SYNTHESIZE(float, _radius, Radius);
    
    ~Ball();
    Ball(GameLayer *game, int type, CCPoint position);
    static Ball * create(GameLayer *game, int type, CCPoint position);
    virtual void reset(void);
    virtual void update(float dt);
    
private:
    void initBall();
    CCPoint _startPosition;
};

#endif /* defined(__AirHockey__Ball__) */
