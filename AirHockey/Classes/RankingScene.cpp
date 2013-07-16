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
#include "PlayerName.h"
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
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF *ranking = CCLabelTTF::create("RANKING", "Time new roman", 60);
    ranking->setPosition(ccp(size.width / 2, 9 * size.height / 10));
    this->addChild(ranking);

    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://localhost:3000/users.json");
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RankingScene::onHttpRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
    
    //create startMenuItem
    CCMenuItemImage *startMenuItem = CCMenuItemImage::create(
                                                             "btn_Continue.png",
                                                             "btn_Continue.png",
                                                             this,
                                                             menu_selector(RankingScene::menuPlay));
    startMenuItem->setScale(4);
    startMenuItem->setPosition(ccp(size.width/4, size.height/8));
    
    //create rankMenuItem
    CCMenuItemImage *rankMenuItem = CCMenuItemImage::create(
                                                            "btn_menu.png",
                                                            "btn_menu.png",
                                                            this,
                                                            menu_selector(RankingScene::menuMenu));
    rankMenuItem->setScale(4);
    rankMenuItem->setPosition(ccp(size.width * 3/4, size.height/8));
    CCMenu* pMenu = CCMenu::create(startMenuItem, rankMenuItem, NULL);
    pMenu->setPosition(ccp(0,0));
    this->addChild(pMenu);
    
    return true;
}

void RankingScene::onHttpRequestCompleted(CCNode *sender, void *data) {
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCHttpResponse *response = (CCHttpResponse*)data;
    
    if (!response)
    {
        return;
    }
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        //CCLog("%s completed", response->getHttpRequest()->getTag());
    }
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode,
            response->getHttpRequest()->getTag());
    CCLog("response code: %d", statusCode);
    
    if (!response->isSucceed())
    {
        CCLabelTTF *notConnectLabel =
        CCLabelTTF::create("PLEASE CHECK YOUR INTERNET CONNECTION", "Time new roman",
                           30);
        notConnectLabel->setPosition(ccp(size.width / 2, size.height / 2));
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
    int dem = 0;
    rapidjson::Document document;
    if(data2 != NULL && !document.Parse<0>(data2).HasParseError())
    {   
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++)
        {
            CCLabelTTF *nameLabel = CCLabelTTF::create(document[i]["name"].GetString(),
                                                           "Time New Roman", 50);
            nameLabel->setAnchorPoint(ccp(0, 0)) ;
            nameLabel->setPosition(ccp(size.width / 6,
                                       size.height * (12 - dem) / 15));
            this->addChild(nameLabel);
            char strP [20] = {0};
            sprintf(strP,"%i", document[i]["point"].GetInt());
            CCLabelTTF *pointLabel = CCLabelTTF::create(strP,
                                                            "Time New Roman", 50);
            pointLabel->setAnchorPoint(ccp(1, 0));
            pointLabel->setPosition(ccp(4 * size.width / 5,
                                        size.height * (12 - dem) / 15));
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
                menu->setPosition(ccp(4.5f * size.width / 5,
                                      size.height * (12 - dem) / 14.5));
                this->addChild(menu);
            }
            dem++;
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
void RankingScene::menuMenu(CCObject* pSender) {
    GameManager *game = GameManager::sharedGameManager();
    if (game->getBgm()) {
        game->setBgm(false);
    } else {
        game->setBgm(true);
    }

}

void RankingScene::menuPlay(CCObject* pSender) {
    CCDirector::sharedDirector()->replaceScene(PlayerName::scene());
}