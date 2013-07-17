//
//  RankingScene.h
//  AirHockey
//
//

#ifndef __AirHockey__RankingScene__
#define __AirHockey__RankingScene__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
class RankingScene : public CCLayer
{
private:
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float w = size.width;
    float h = size.height;
    
    CCSprite *bgm_off;
public:
    virtual bool init();
    static CCScene* scene();
    void onHttpRequestCompleted(CCNode *sender, void *data);
    CREATE_FUNC(RankingScene);
    
    void play(CCObject* pSender);
    void bgm(CCObject* pSender);
    void clickBtSendEmail(CCObject* pSender);
};
#endif /* defined(__AirHockey__RankingScene__) */