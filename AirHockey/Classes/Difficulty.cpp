//
//  Difficulty.cpp
//  AirHockey
//
//  Created by macbook_016 on 2013/06/21.
//
//

#include "Difficulty.h"
#include "GameManager.h"
#include "RankingScene.h"
#include <string.h>
#include "GameLayer.h"

using namespace cocos2d;
CCScene* Difficulty::scene() {
    CCScene *scene = CCScene::create();
    Difficulty *layer = Difficulty::create();
    scene->addChild(layer);
    return scene;
}

bool Difficulty::init() {
    CCSprite *background = CCSprite::create("Menu.png");
    background->setPosition(ccp(w/2, h/2));
    this->addChild(background);
    
    CCPoint visibleOrigin = CCEGLView::sharedOpenGLView()->getVisibleOrigin();
    CCSize visibleSize = CCEGLView::sharedOpenGLView()->getVisibleSize();

    CCSize editBoxSize = CCSizeMake(visibleSize.width/2, 60);
    
    // top
    m_pEditName =
    extension::CCEditBox::create(editBoxSize,
                                 extension::CCScale9Sprite::create("GreenBox.png"));
    m_pEditName->setPosition(ccp(visibleOrigin.x + visibleSize.width/2,
                                 visibleOrigin.y + visibleSize.height*5/8));
    m_pEditName->setFontSize(40);
    m_pEditName->setFontColor(ccRED);
    string p = GameManager::sharedGameManager()->getName();
    if (p == "") m_pEditName->setText("Player");
    else m_pEditName->setText(p.c_str());
    m_pEditName->setPlaceholderFontColor(ccWHITE);
    m_pEditName->setMaxLength(15);
    m_pEditName->setReturnType(cocos2d::extension::kKeyboardReturnTypeDone);
    
    m_pEditName->setDelegate(this);
    this->addChild(m_pEditName);

    CCMenuItemFont *easyMenuItem =
    CCMenuItemFont::create("EASY", this, menu_selector(Difficulty::menuEasy));
    easyMenuItem->setPosition(ccp(w/2, h/2));
    easyMenuItem->setFontSizeObj(70);
    easyMenuItem->setFontSize(70);
    
    CCMenuItemFont *mediumMenuItem =
    CCMenuItemFont::create("MEDIUM", this, menu_selector(Difficulty::menuMedium));
    mediumMenuItem->setPosition(ccp(w/2, h*3/8));
    
    CCMenuItemFont *hardMenuItem =
    CCMenuItemFont::create("HARD", this, menu_selector(Difficulty::menuHard));
    hardMenuItem->setPosition(ccp(w/2, h/4));
    
    CCMenuItemFont *back =
    CCMenuItemFont::create("BACK", this, menu_selector(Difficulty::clickBtBack));
    back->setPosition(ccp(size.width / 2, size.height * 0.1));
    
    pMenu = CCMenu::create(easyMenuItem, mediumMenuItem, hardMenuItem, back, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    
    return true;
}

void Difficulty::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    //CCLog("editBox %p DidBegin !", editBox);
}

void Difficulty::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    //CCLog("editBox %p DidEnd !", editBox);
}

void Difficulty::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox,
                                    const std::string& text)
{
    //CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void Difficulty::editBoxReturn(cocos2d::extension::CCEditBox* editBox) {
    GameManager::sharedGameManager()->setName(m_pEditName->getText());
}


// Game Level Easy=1, Medium=2, Hard=3
void Difficulty::menuHard(CCObject *pSender) {
    GameManager::sharedGameManager()->setName(m_pEditName->getText());
    GameManager::sharedGameManager()->setLevel(3);
    CCScene *game = GameLayer::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, game));
}

void Difficulty::menuMedium(CCObject *pSender) {
    GameManager::sharedGameManager()->setName(m_pEditName->getText());
    GameManager::sharedGameManager()->setLevel(2);
    CCScene *game = GameLayer::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, game));
}

void Difficulty::menuEasy(CCObject *pSender) {
    GameManager::sharedGameManager()->setName(m_pEditName->getText());
    GameManager::sharedGameManager()->setLevel(1);
    CCScene *game = GameLayer::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, game));
}

void Difficulty::clickBtBack(cocos2d::CCObject *pScene) {
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, RankingScene::scene()));
}
