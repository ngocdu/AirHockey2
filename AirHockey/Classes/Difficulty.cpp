//
//  Difficulty.cpp
//  AirHockey
//
//  Created by macbook_016 on 2013/06/21.
//
//

#include "Difficulty.h"
#include "GameManager.h"
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
    CCSprite *background = CCSprite::create("menu.png");
    background->setPosition(ccp(w/2, h/2));
    this->addChild(background);
    
    CCPoint visibleOrigin = CCEGLView::sharedOpenGLView()->getVisibleOrigin();
    CCSize visibleSize = CCEGLView::sharedOpenGLView()->getVisibleSize();

    CCSize editBoxSize = CCSizeMake(visibleSize.width/2, 60);
    
    // top
    m_pEditName =
    extension::CCEditBox::create(editBoxSize,
                                 extension::CCScale9Sprite::create("green_edit.png"));
    m_pEditName->setPosition(ccp(visibleOrigin.x + visibleSize.width/2,
                                 visibleOrigin.y + visibleSize.height*5/8));
    m_pEditName->setFontSize(40);
    m_pEditName->setFontColor(ccRED);
    string p = CCUserDefault::sharedUserDefault()->getStringForKey("Difficulty");
    if (p == "") m_pEditName->setPlaceHolder("Player");
    else m_pEditName->setPlaceHolder(p.c_str());
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
    pMenu = CCMenu::create(easyMenuItem, mediumMenuItem, hardMenuItem, NULL);
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

void Difficulty::menuHard(CCObject *pSender) {
    CCUserDefault::sharedUserDefault()->setStringForKey("PlayerName",
                                                        m_pEditName->getText());
    CCUserDefault::sharedUserDefault()->setStringForKey("Difficulty", "Hard");
    CCUserDefault::sharedUserDefault()->flush();
    GameManager::sharedGameManager()->setLevel(3);
    CCScene *game = GameLayer::scene();
    CCDirector::sharedDirector()->replaceScene(game);
}

void Difficulty::menuMedium(CCObject *pSender) {
    CCUserDefault::sharedUserDefault()->setStringForKey("PlayerName",
                                                        m_pEditName->getText());
    CCUserDefault::sharedUserDefault()->setStringForKey("Difficulty", "Medium");
    CCUserDefault::sharedUserDefault()->flush();
    GameManager::sharedGameManager()->setLevel(2);
    CCScene *game = GameLayer::scene();
    CCDirector::sharedDirector()->replaceScene(game);
}
    
void Difficulty::menuEasy(CCObject *pSender) {
    CCUserDefault::sharedUserDefault()->setStringForKey("PlayerName",
                                                        m_pEditName->getText());
    CCUserDefault::sharedUserDefault()->setStringForKey("Difficulty", "Easy");
    CCUserDefault::sharedUserDefault()->flush();
    GameManager::sharedGameManager()->setLevel(1);
    CCScene *game = GameLayer::scene();
    CCDirector::sharedDirector()->replaceScene(game);
}

