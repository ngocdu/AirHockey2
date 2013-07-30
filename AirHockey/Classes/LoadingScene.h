//
//  LoadingScene.h
//  AirHockey
//
//  Created by Kien MetalRock on 7/29/13.
//
//

#ifndef __AirHockey__LoadingScene__
#define __AirHockey__LoadingScene__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class LoadingScene : public CCLayer {
public:
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float w = size.width;
    float h = size.height;
    
    LoadingScene();
    ~LoadingScene();
    void LoadingCallback(CCObject *obj);
    static CCScene* scene();
    
private:
    CCLabelTTF *m_pLabelLoading;
    CCLabelTTF *m_pLabelPercent;
    int m_nNumberOfSprites;
    int m_nNumberOfLoadedSprites;
    CREATE_FUNC(LoadingScene);

};


#endif /* defined(__AirHockey__LoadingScene__) */
