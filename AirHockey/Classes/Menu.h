//
//  Menu.h
//  AirHockey
//
//  Created by macbook_016 on 2013/06/20.
//
//

#ifndef __AirHockey__Menu__
#define __AirHockey__Menu__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
class Menu : public CCLayer
{
private:
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float w = size.width;
    float h = size.height;
    CCMenuItemImage *_start;
    CCMenuItemImage *_ranking;
    CCMenuItemImage *_bgm;
public:
    virtual bool init();
    static CCScene* scene();
    
    // a selector callback
    void bgm(CCObject* pSender);
    void ranking(CCObject* pSender);
    void startGame(CCObject* pSender);
    CREATE_FUNC(Menu);
};

#endif /* defined(__AirHockey__Menu__) */
