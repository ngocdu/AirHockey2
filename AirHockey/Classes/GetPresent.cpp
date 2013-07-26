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
CCScene* GetPresent::scene() {
    CCScene *scene = CCScene::create();
    GetPresent *layer = GetPresent::create();
    scene->addChild(layer);
    return scene;
}

bool GetPresent::init() {
    CCSize editBoxSize = CCSizeMake(w - 100, 60);
    
    // Congrats 
    CCLabelTTF *congrats = CCLabelTTF::create("BEST SCORE !!!", "BankGothic Md BT", 60);
    congrats->setPosition(ccp(w/2, h*6/8));
    this->addChild(congrats);
    
    
    // email
    CCLabelTTF *emailLabel1 = CCLabelTTF::create("Please enter your Email", "BankGothic Md BT", 40);
    emailLabel1->setPosition(ccp(w/2, h*5/8));
    this->addChild(emailLabel1);
    CCLabelTTF *emailLabel2 = CCLabelTTF::create("to get Presents:", "BankGothic Md BT", 40);
    emailLabel2->setPosition(ccp(w/2, emailLabel1->getPositionY() - 45));
    this->addChild(emailLabel2);
    
    m_pUserEmail =
    extension::CCEditBox::create(editBoxSize,
                                 extension::CCScale9Sprite::create("GreenBox.png"));
    m_pUserEmail->setPosition(ccp(w/2, emailLabel2->getPositionY() - 60));
    m_pUserEmail->setFontSize(40);
    m_pUserEmail->setFontColor(ccWHITE);
    m_pUserEmail->setMaxLength(55);
    m_pUserEmail->setReturnType(cocos2d::extension::kKeyboardReturnTypeDone);
    m_pUserEmail->setInputMode(kEditBoxInputModeEmailAddr);
    m_pUserEmail->setDelegate(this);
    this->addChild(m_pUserEmail);
    
    // name
    CCLabelTTF *nameLabel = CCLabelTTF::create("Please choose an username:","BankGothic Md BT", 40);
    nameLabel->setPosition(ccp(w/2, h*3/8));
    this->addChild(nameLabel);
    m_pUserName =
    extension::CCEditBox::create(editBoxSize,
                                 extension::CCScale9Sprite::create("GreenBox.png"));
    m_pUserName->setPosition(ccp(w/2, nameLabel->getPositionY() - 60));
    m_pUserName->setFontSize(40);
    m_pUserName->setPlaceholderFontColor(ccWHITE);
    m_pUserName->setMaxLength(55);
    m_pUserName->setReturnType(cocos2d::extension::kKeyboardReturnTypeDone);
    m_pUserName->setInputMode(kEditBoxInputModeAny);
    m_pUserName->setDelegate(this);
    this->addChild(m_pUserName);
    
    
    CCMenuItemImage *sendMenuItem = CCMenuItemImage::create("GetPresent.png", "GetPresentOnClicked.png", this, menu_selector(GetPresent::menuSendEmail));
    sendMenuItem->setPosition(ccp(w/2, 100));
    
//    CCMenuItemImage *backMenuItem =
//    CCMenuItemImage::create("BackButton.png", "BackButton.png", this, menu_selector(GetPresent::menuBack));
//    backMenuItem->setPosition(ccp(w/4, 100));
    
    pMenu = CCMenu::create(sendMenuItem, NULL);
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

#pragma mark valid Email
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


#pragma mark onHttpRequestCompleted
void GetPresent::onHttpRequestCompleted(CCNode *sender, void *data) {
    CCSize w = CCDirector::sharedDirector()->getWinSize();
    CCHttpResponse *response = (CCHttpResponse*)data;
    if (!response) {
        return;
    }
    if (0 != strlen(response->getHttpRequest()->getTag())) {
        CCLog("%s completed", response->getHttpRequest()->getTag());
    }
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {0};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode,
            response->getHttpRequest()->getTag());
    
    if (!response->isSucceed()) {
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
    for (unsigned int i = 0; i < buffer->size(); i++) {
        d++ ;
        data2[d] = (*buffer)[i];
    }
    data2[d + 1] = '\0';
    
    rapidjson::Document document;
    if(data2 != NULL && !document.Parse<0>(data2).HasParseError()) {
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++) {
            string name = document[i]["name"].GetString();
            string email_server = document[i]["email"].GetString();
            CCLOG("%s", name.c_str());
            CCLOG("%s", m_pUserName->getText());
            if (strcmp(m_pUserName->getText(), name.c_str()) == 0) {
               if(strcmp(m_pUserEmail->getText(), "") != 0 && strcmp(m_pUserEmail->getText(), email_server.c_str()) == 0 ){
                   m_pUserName->setText("the name was existed");
                   return;
               } 
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
        int reward = CCUserDefault::sharedUserDefault()->getIntegerForKey("reward");
        string url;
        if (reward != 0) {
            url    = ipAddr + ":3000/users?name="+name+"&point="+strP+"&email="+email;
        }else {
            url    = ipAddr + ":3000/users?name="+name+"&point="+strP+"&email="+email;
        }
        
        request->setUrl(url.c_str());
        request->setRequestType(CCHttpRequest::kHttpPost);
        CCHttpClient::getInstance()->send(request);
        request->release();
        GameManager::sharedGameManager()->setEmail(m_pUserEmail->getText());
        m_pUserEmail->setText("email send succses");
        GameManager::sharedGameManager()->setName(m_pUserName->getText());
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, RankingScene::scene()));
    } else {
        m_pUserEmail->setText("email fail");
    }
    d-=1;
}

#pragma mark Send Email callback
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
#pragma mark Back callback
void GetPresent::menuBack(cocos2d::CCObject *pSender) {
    CCDirector::sharedDirector()->end();
}

