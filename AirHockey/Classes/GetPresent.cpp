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
    
    // email
    m_pUserEmail =
    extension::CCEditBox::create(editBoxSize,
                                 extension::CCScale9Sprite::create("GreenBox.png"));
    m_pUserEmail->setPosition(ccp(visibleOrigin.x + visibleSize.width / 2,
                                 visibleOrigin.y + visibleSize.height * 3 / 4));
    m_pUserEmail->setFontSize(40);
    m_pUserEmail->setFontColor(ccRED);
    m_pUserEmail->setPlaceHolder("input your Email to receive reward:");
    m_pUserEmail->setPlaceholderFontColor(ccWHITE);
    m_pUserEmail->setMaxLength(55);
    m_pUserEmail->setReturnType(cocos2d::extension::kKeyboardReturnTypeDone);
    m_pUserEmail->setDelegate(this);
    this->addChild(m_pUserEmail);
    
    // name
    m_pUserName =
    extension::CCEditBox::create(editBoxSize,
                                 extension::CCScale9Sprite::create("GreenBox.png"));
    m_pUserName->setPosition(ccp(visibleOrigin.x + visibleSize.width / 2,
                                 visibleOrigin.y + visibleSize.height * 2 / 4));
    m_pUserName->setFontSize(40);
    m_pUserName->setFontColor(ccRED);
    m_pUserName->setPlaceHolder("input your username");
    m_pUserName->setPlaceholderFontColor(ccWHITE);
    m_pUserName->setMaxLength(55);
    m_pUserName->setReturnType(cocos2d::extension::kKeyboardReturnTypeDone);
    m_pUserName->setDelegate(this);
    this->addChild(m_pUserName);
    
    CCMenuItemFont *sendMenuItem =
        CCMenuItemFont::create("SendEmail", this, menu_selector(GetPresent::menuSendEmail));
    sendMenuItem->setPosition(ccp(size.width / 2, size.height * 0.25));
    sendMenuItem->setFontSizeObj(70);
    sendMenuItem->setFontSize(70);
    
    CCMenuItemFont *backMenuItem =
    CCMenuItemFont::create("Back", this, menu_selector(GetPresent::menuBack));
    backMenuItem->setPosition(ccp(size.width / 2, size.height * 0.15));
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
    GameManager::sharedGameManager()->setEmail(m_pUserEmail->getText());
}
bool GetPresent::is_email(std::string const& address) {
    size_t at_index = address.find_first_of('@', 0);
    return at_index != std::string::npos
      && address.find_first_of('.', at_index) != std::string::npos;
    
}
int GetPresent::spc_email_isvalid(const char *address) {
    int        count = 0;
    const char *c, *domain;
    static char *rfc822_specials = "()<>@,;:\\\"[]!#$&%|*+}{?\'";
    
    /* first we validate the name portion (name@domain) */
    for (c = address;  *c;  c++) {
        if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) ==
                           '\"')) {
            while (*++c) {
                if (*c == '\"') break;
                if (*c == '\\' && (*++c == ' ')) continue;
                if (*c == '&' || *c == '%' || *c == '$' || *c == '#' || *c == '!'
                    || *c <= ' ' || *c >= 127) return 0;
            }
            if (!*c++) return 0;
            if (*c == '@') break;
            if (*c != '.') return 0;
            continue;
        }
        if (*c == '@') break;
        if (*c <= ' ' || *c >= 127) return 0;
        if (strchr(rfc822_specials, *c)) return 0;
    }
    if (c == address || *(c - 1) == '.') return 0;
    
    /* next we validate the domain portion (name@domain) */
    if (!*(domain = ++c)) return 0;
    do {
        if (*c == '.') {
            if (c == domain || *(c - 1) == '.') return 0;
            count++;
        }
        if (*c <= ' ' || *c >= 127) return 0;
        if (strchr(rfc822_specials, *c)) return 0;
    } while (*++c);
    
    return (count >= 1);
}

void GetPresent::menuSendEmail(CCObject *pSender)
{
    if (strcmp(m_pUserName->getText(), "") != 0) {
        //--------------
        GameManager::sharedGameManager()->setName(m_pUserName->getText());
        CCHttpRequest* request = new CCHttpRequest();
        string ipAddr = GameManager::sharedGameManager()->getIpAddr();
        request->setUrl((ipAddr+":3000/users.json").c_str());
        request->setRequestType(CCHttpRequest::kHttpGet);
        request->setResponseCallback(this, callfuncND_selector(GetPresent::onHttpRequestCompleted));
        CCHttpClient::getInstance()->send(request);
        request->release();
    }
}
void GetPresent::menuBack(cocos2d::CCObject *pSender) {
    CCDirector::sharedDirector()->replaceScene(RankingScene::scene());
}
void GetPresent::onHttpRequestCompleted(CCNode *sender, void *data) {
    CCSize w = CCDirector::sharedDirector()->getWinSize();
    CCHttpResponse *response = (CCHttpResponse*)data;
    if (!response)
    {
        return;
    }
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        CCLog("%s completed", response->getHttpRequest()->getTag());
    }
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {0};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode,
            response->getHttpRequest()->getTag());
    
    if (!response->isSucceed())
    {
        CCLabelTTF *notConnectLabel =
        CCLabelTTF::create("Can't load Data", "Time new roman", 20);
        notConnectLabel->setPosition(ccp(w.width/2, w.height/2));
        this->addChild(notConnectLabel);
        return;
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    char * data2 = (char*)(malloc(buffer->size() *  sizeof(char)));
    int d = -1;
    printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        d++ ;
        data2[d] = (*buffer)[i];
    }
    data2[d + 1] = '\0';
    //-----------------------
    
    rapidjson::Document document;
    if(data2 != NULL && !document.Parse<0>(data2).HasParseError())
    {
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++)
        {
            string name = document[i]["name"].GetString();
            CCLOG("%s", name.c_str());
            CCLOG("%s", m_pUserName->getText());
            if (strcmp(m_pUserName->getText(), name.c_str()) == 0) {
                CCLOG("co ten trung nhau");
                m_pUserName->setText("that name was existed");
                return;
            }
        }
    } else {
        CCLog(document.GetParseError());
    }
    if (strcmp(m_pUserEmail->getText(), "") != 0 && this->is_email(m_pUserEmail->getText()) &&
        this->spc_email_isvalid(m_pUserEmail->getText())) {
        CCHttpRequest * request = new CCHttpRequest();
        string name = GameManager::sharedGameManager()->getName();
        int p = GameManager::sharedGameManager()->getPoint();
        char strP[20] = {0};
        sprintf(strP, "%i", p);
        string email  = GameManager::sharedGameManager()->getEmail();
        string ipAddr = GameManager::sharedGameManager()->getIpAddr();
        string url    = ipAddr + ":3000/users?name="+name+"&point="+strP+"&email="+email;
        request->setUrl(url.c_str());
        request->setRequestType(CCHttpRequest::kHttpPost);
        CCHttpClient::getInstance()->send(request);
        request->release();
        m_pUserEmail->setText("email send succses");
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, RankingScene::scene()));
    }else {
        m_pUserEmail->setText("email fail");
    }
    d-=1;
}