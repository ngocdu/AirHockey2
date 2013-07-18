//
//  RankingScene.cpp
//  AirHockey
//
//

#include "RankingScene.h"
#include "cocos2d.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "Menu.h"
#include "GameLayer.h"
#include "Difficulty.h"
#include "GameManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* RankingScene::scene() {
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    RankingScene *layer1 = RankingScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer1);
    
    // return the scene
    return scene;
}

bool RankingScene::init() {
    CCSprite *background = CCSprite::create("menu.png");
    background->setPosition(ccp(w/2, h/2));
    this->addChild(background);
    
    CCSprite *ranking_bg = CCSprite::create("ranking_bg.png");
    ranking_bg->setPosition(ccp(w/2, h/2));
    this->addChild(ranking_bg);

    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://192.168.1.59:3000/users.json");
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RankingScene::onHttpRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
    
    //create startMenuItem
    CCMenuItemImage *playItem =
        CCMenuItemImage::create("start_button.png", "start_button.png",
                                this, menu_selector(RankingScene::play));
    playItem->setPosition(ccp(w/2, h/6));
    playItem->setScale(0.7f);
    //create bgmItem
    CCMenuItemImage *bgmItem =
        CCMenuItemImage::create("bgm_on.png", "bgm_on.png",
                                this, menu_selector(RankingScene::bgm));
    bgmItem->setPosition(ccp(w*4/5, h/6));
    CCMenu* pMenu = CCMenu::create(playItem, bgmItem, NULL);
    pMenu->setPosition(ccp(0,0));
    this->addChild(pMenu);
    bgm_off = CCSprite::create("bgm_off.png");
    bgm_off->setPosition(bgmItem->getPosition());
    bgm_off->setVisible(CCUserDefault::sharedUserDefault()->getBoolForKey("BGM"));
    this->addChild(bgm_off);
    return true;
}

void RankingScene::onHttpRequestCompleted(CCNode *sender, void *data) {
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
        notConnectLabel->setPosition(ccp(w/2, h/2));
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
    int count = 0;
    rapidjson::Document document;
    if(data2 != NULL && !document.Parse<0>(data2).HasParseError())
    {   
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++)
        {
            CCLabelTTF *nameLabel = CCLabelTTF::create(document[i]["name"].GetString(),
                                                           "Time New Roman", 50);
            nameLabel->setAnchorPoint(ccp(0, 0)) ;
            nameLabel->setPosition(ccp(w / 6,
                                       h * (12 - count) / 15));
            this->addChild(nameLabel);
            char strP [20] = {0};
            sprintf(strP,"%i", document[i]["point"].GetInt());
            CCLabelTTF *pointLabel = CCLabelTTF::create(strP,
                                                            "Time New Roman", 50);
            pointLabel->setAnchorPoint(ccp(1, 0));
            pointLabel->setPosition(ccp(4 * w / 5,
                                        h * (12 - count) / 15));
            this->addChild(pointLabel);
            int reward = document[i]["reward"].GetInt();
            int rewardLocal = CCUserDefault::sharedUserDefault()->getIntegerForKey("reward");
            string nameLocal = GameManager::sharedGameManager()->getName();
            string name = document[i]["name"].GetString();
            int point = document[i]["point"].GetInt();
            int pointMax = CCUserDefault::sharedUserDefault()->getIntegerForKey("pointMax");
            if (reward != 0 &&  rewardLocal != 0 && name == nameLocal && point == pointMax) {
                CCMenuItemImage *bt_send_email = CCMenuItemImage::create("CloseNormal.png",
                   "CloseNormal.png", this, menu_selector(RankingScene::clickBtSendEmail));
                CCMenu * menu = CCMenu::create(bt_send_email, NULL);
                menu->setPosition(ccp(4.5f * w / 5,
                                      h * (12 - count) / 14.5));
                this->addChild(menu, 20);
            }
            count++;
            }
        }
        else
        {
            CCLog(document.GetParseError());
        }
        d = -1;
        delete []data2;
}
void RankingScene::clickBtSendEmail(cocos2d::CCObject *pSender) {
    CCDirector::sharedDirector()->replaceScene(GetPresent::scene());
}
void RankingScene::bgm(CCObject* pSender) {
    GameManager *game = GameManager::sharedGameManager();
    bgm_off->setVisible(!game->getBgm());
    game->setBgm(!game->getBgm());
    if (game->getBgm()) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
    } else {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
    }
}

void RankingScene::play(CCObject* pSender) {
    CCDirector::sharedDirector()->replaceScene(Difficulty::scene());
}