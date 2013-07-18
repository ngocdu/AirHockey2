//
//  GetPresent.cpp
//  AirHockey
//
//

#include "GetPresent.h"
#include "Difficulty.h"
#include "GameManager.h"
#include "GameLayer.h"
#include "RankingScene.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "HttpClient.h"
using namespace cocos2d::extension;
using namespace cocos2d;
CCScene* GetPresent::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GetPresent *layer = GetPresent::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}
bool GetPresent::init()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint visibleOrigin = CCEGLView::sharedOpenGLView()->getVisibleOrigin();
    CCSize visibleSize = CCEGLView::sharedOpenGLView()->getVisibleSize();
    
    CCSize editBoxSize = CCSizeMake(visibleSize.width - 100, 60);
    
    // top
    m_pEditName =
    extension::CCEditBox::create(editBoxSize,
                                 extension::CCScale9Sprite::create("green_edit.png"));
    m_pEditName->setPosition(ccp(visibleOrigin.x + visibleSize.width / 2,
                                 visibleOrigin.y + visibleSize.height * 3 / 4));
    m_pEditName->setFontSize(40);
    m_pEditName->setFontColor(ccRED);
    m_pEditName->setPlaceHolder("input your Email to give reward:");
    m_pEditName->setPlaceholderFontColor(ccWHITE);
    m_pEditName->setMaxLength(55);
    m_pEditName->setReturnType(cocos2d::extension::kKeyboardReturnTypeDone);
    m_pEditName->setDelegate(this);
    this->addChild(m_pEditName);
    CCMenuItemFont *sendMenuItem =
        CCMenuItemFont::create("SendEmail", this, menu_selector(GetPresent::menuSendEmail));
    sendMenuItem->setPosition(ccp(size.width / 2, size.height * 0.5));
    sendMenuItem->setFontSizeObj(70);
    sendMenuItem->setFontSize(70);
    
    CCMenuItemFont *backMenuItem =
    CCMenuItemFont::create("Back", this, menu_selector(GetPresent::menuBack));
    backMenuItem->setPosition(ccp(size.width / 2, size.height * 0.3));
    backMenuItem->setFontSizeObj(70);
    backMenuItem->setFontSize(70);
    
    pMenu = CCMenu::create(sendMenuItem, backMenuItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    
    return true;
}
void GetPresent::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    //CCLog("editBox %p DidBegin !", editBox);
}

void GetPresent::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    //CCLog("editBox %p DidEnd !", editBox);
}

void GetPresent::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox,
                                    const std::string& text)
{
    //CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void GetPresent::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
{
    //CCLog("editBox %p was returned !");
    GameManager::sharedGameManager()->setEmail(m_pEditName->getText());
}
bool GetPresent::is_email(std::string const& address) {
    size_t at_index = address.find_first_of('@', 0);
    return at_index != std::string::npos
    && address.find_first_of('.', at_index) != std::string::npos;
}
void GetPresent::menuSendEmail(CCObject *pSender)
{
    if (strcmp(m_pEditName->getText(), "") != 0 && this->is_email(m_pEditName->getText())) {
        CCHttpRequest * request = new CCHttpRequest();
        string name = GameManager::sharedGameManager()->getName();
        int p = GameManager::sharedGameManager()->getPoint();
        char strP[20] = {0};
        sprintf(strP, "%i", p);
        string email = GameManager::sharedGameManager()->getEmail();
        string url = "http://192.168.1.59:3000/users?name="+name+"&point="+strP+"&email="+email;
        request->setUrl(url.c_str());
        request->setRequestType(CCHttpRequest::kHttpPost);
        CCHttpClient::getInstance()->send(request);
        request->release();
        m_pEditName->setText("email send succses");
        CCUserDefault::sharedUserDefault()->setIntegerForKey("reward", 0);
    }else {
        m_pEditName->setText("email fail");
    }
}
void GetPresent::menuBack(cocos2d::CCObject *pSender) {
    CCDirector::sharedDirector()->replaceScene(RankingScene::scene());
}