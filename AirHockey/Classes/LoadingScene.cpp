//
//  LoadingScene.cpp
//  AirHockey
//
//  Created by Kien MetalRock on 7/29/13.
//
//

#include "LoadingScene.h"
USING_NS_CC;

LoadingScene::LoadingScene()
: m_nNumberOfSprites(11)
, m_nNumberOfLoadedSprites(0) {
    m_pLabelLoading = CCLabelTTF::create("Loading...", "BankGothic Md BT", 24);
    m_pLabelPercent = CCLabelTTF::create("0", "BankGothic Md BT", 24);
    
    m_pLabelLoading->setPosition(ccp(w/2, h/2));
    m_pLabelPercent->setPosition(ccp(w/2, h/2 + 50));
    
    this->addChild(m_pLabelLoading);
    this->addChild(m_pLabelPercent);
    
    CCTextureCache::sharedTextureCache()->addImageAsync("Numbers/0.png", this, callfuncO_selector(LoadingScene::LoadingCallback));
    CCTextureCache::sharedTextureCache()->addImageAsync("Numbers/1.png", this, callfuncO_selector(LoadingScene::LoadingCallback));
    CCTextureCache::sharedTextureCache()->addImageAsync("Numbers/2.png", this, callfuncO_selector(LoadingScene::LoadingCallback));
    CCTextureCache::sharedTextureCache()->addImageAsync("Numbers/3.png", this, callfuncO_selector(LoadingScene::LoadingCallback));
    CCTextureCache::sharedTextureCache()->addImageAsync("Numbers/4.png", this, callfuncO_selector(LoadingScene::LoadingCallback));
    CCTextureCache::sharedTextureCache()->addImageAsync("Numbers/5.png", this, callfuncO_selector(LoadingScene::LoadingCallback));
    CCTextureCache::sharedTextureCache()->addImageAsync("Numbers/6.png", this, callfuncO_selector(LoadingScene::LoadingCallback));
    CCTextureCache::sharedTextureCache()->addImageAsync("Numbers/7.png", this, callfuncO_selector(LoadingScene::LoadingCallback));
    CCTextureCache::sharedTextureCache()->addImageAsync("Numbers/8.png", this, callfuncO_selector(LoadingScene::LoadingCallback));
    CCTextureCache::sharedTextureCache()->addImageAsync("Numbers/9.png", this, callfuncO_selector(LoadingScene::LoadingCallback));
    CCTextureCache::sharedTextureCache()->addImageAsync("Numbers/10.png", this, callfuncO_selector(LoadingScene::LoadingCallback));
}

LoadingScene::~LoadingScene() {
    
}

void LoadingScene::LoadingCallback(CCObject *obj) {
    ++m_nNumberOfLoadedSprites;
    
    char tmp[10];
    sprintf(tmp, "%%%d",(int)(((float)m_nNumberOfLoadedSprites/m_nNumberOfSprites) * 100));
    m_pLabelPercent->setString(tmp);
}

CCScene* LoadingScene::scene() {
    CCScene *scene = CCScene::create();
    LoadingScene *layer = LoadingScene::create();
    scene->addChild(layer);
    return scene;
}