//
//  Menu.cpp
//  AirHockey
//
//  Created by macbook_016 on 2013/06/20.
//
//

#include "Menu.h"
#include "Difficulty.h"
#include "GameManager.h"
#include "RankingScene.h"

using namespace cocos2d;

CCScene* Menu::scene() {
    CCScene *scene = CCScene::create();
    Menu *layer = Menu::create();
    scene->addChild(layer);
    return scene;
}

bool Menu::init() {
    
    _start = CCMenuItemImage::create("Start.png", "Start.png",
                                     this, menu_selector(Menu::startGame));
    
    _start->setPosition(ccp(w/2.10989011, h/1.5147929));

    _ranking = CCMenuItemImage::create("Rank.png", "Rank.png",
                                        this, menu_selector(Menu::ranking));
    _ranking->setPosition(ccp(w/2.10989011, h/1.90689013));

    _bgm = CCMenuItemImage::create("Bgm.png", "Bgm.png",
                                    this, menu_selector(Menu::bgm));
    _bgm->setPosition(ccp(w/2.10989011, h/2.63239075));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(_start, _ranking, _bgm, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    CCSprite* pSprite = CCSprite::create("Menu.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(size.width / 2, size.height / 2));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    GameManager::sharedGameManager()->setBgm(true);
    return true;
}
void Menu::bgm(CCObject* pSender)
{
    GameManager *game = GameManager::sharedGameManager();
    if (game->getBgm()) {
        _bgm->setColor(ccc3(32, 32, 64));
        game->setBgm(false);
    } else {
        _bgm->setColor(ccc3(255, 255, 255));
        game->setBgm(true);
    }

}
void Menu::ranking(CCObject* pSender)
{
    CCScene *rankScene = CCScene::create();
    RankingScene *rankLayer = RankingScene::create();
    rankScene->addChild(rankLayer);
    CCScene *pScene = CCTransitionFadeTR::create(2, rankScene);
    CCDirector::sharedDirector()->replaceScene(pScene);
}
void Menu::startGame(CCObject* pSender)
{
    CCScene *DifficultyScene = Difficulty::scene();
    CCDirector::sharedDirector()->replaceScene(DifficultyScene);
}


