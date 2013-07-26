//
//  RewardScene.cpp
//  AirHockey
//
//

#include "RewardScene.h"
#include "cocos2d.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "GameLayer.h"
#include "Difficulty.h"
#include "GameManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* RewardScene::scene() {
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    RewardScene *layer1 = RewardScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer1);
    
    // return the scene
    return scene;
}

bool RewardScene::init() {
    CCSprite *background = CCSprite::create("RewardBG.png");
    background->setPosition(ccp(w/2, h/2));
    this->addChild(background);
    
    string playerName = GameManager::sharedGameManager()->getName();
    if (playerName != "") {
        char nameBuf[30];
        sprintf(nameBuf, "Player Name: %s", playerName.c_str());
        CCLabelTTF *playerNameLabel = CCLabelTTF::create(nameBuf, "BankGothic Md BT", 30);
        playerNameLabel->setPosition(ccp(w/2, h*3/4));
        this->addChild(playerNameLabel);
    }
    
    CCHttpRequest* request = new CCHttpRequest();
    string ipAddr = GameManager::sharedGameManager()->getIpAddr();
    request->setUrl((ipAddr+":3000/users.json").c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RewardScene::onHttpRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
    
    //create startMenuItem
    CCMenuItemImage *playItem =
        CCMenuItemImage::create("BackButton.png", "BackButtonOnClicked.png",
                                this, menu_selector(RewardScene::back));
    playItem->setPosition(ccp(w/2, h/8));
    
    CCMenu* pMenu = CCMenu::create(playItem, NULL);
    pMenu->setPosition(ccp(0,0));
    this->addChild(pMenu);
        
    return true;
}

void RewardScene::onHttpRequestCompleted(CCNode *sender, void *data) {
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
        CCLabelTTF::create("Can't load Data", "BankGothic Md BT", 20);
        notConnectLabel->setPosition(ccp(w/2, h/2));
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
    //-----------------------

    rapidjson::Document document;
    if(data2 != NULL && !document.Parse<0>(data2).HasParseError()) {
        string username = GameManager::sharedGameManager()->getName();
        string email = GameManager::sharedGameManager()->getEmail();
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++) {
            if (username == document[i]["name"].GetString() &&
                email == document[i]["email"].GetString()){
                string name = document[i]["name"].GetString();
                string mail = document[i]["email"].GetString();
                string time = document[i]["updated_at"].GetString();
                int p = document[i]["point"].GetInt();
                int r = document[i]["reward"].GetInt();
                Player1 *player = new Player1(name,p, mail, time, r);
                players->addObject(player);
            }
        }
    } else {
        CCLog(document.GetParseError());
    }
    d = -1;
    CCTableView *tableView=CCTableView::create(this, CCSizeMake(700, 350));
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setAnchorPoint(ccp(0, 0));
    tableView->setPosition(ccp(size.width/8, 250));
    tableView->setDelegate(this);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView, 21);
    tableView->reloadData();
}

void RewardScene::clickBtSendEmail(cocos2d::CCObject *pSender) {
    CCLOG("cell: %i", celltouch);
    CCMenuItemImage *bt_send_email = (CCMenuItemImage*)pSender;
    CCLOG("tag menu : %i", bt_send_email->getTag());
    CCHttpRequest * request = new CCHttpRequest();
    Player1 *p = (Player1*)players->objectAtIndex(bt_send_email->getTag() - 100);
    int point = p->getPoint();
    string name = GameManager::sharedGameManager()->getName();
    char strP[20] = {0};
    sprintf(strP, "%i", point);
    string email  = GameManager::sharedGameManager()->getName();
    string ipAddr = GameManager::sharedGameManager()->getIpAddr();
    string url    = ipAddr + ":3000/users?name="+name+"&point="+strP+"&email="+email+"&reward=abc&time="+p->getTime();
    request->setUrl(url.c_str());
    request->setRequestType(CCHttpRequest::kHttpPost);
    CCHttpClient::getInstance()->send(request);
    request->release();
    p->setReward(0);
    int r = CCUserDefault::sharedUserDefault()->getIntegerForKey("reward");
    CCUserDefault::sharedUserDefault()->setIntegerForKey("reward", r - 1);
    bt_send_email->removeFromParentAndCleanup(true);
}

void RewardScene::back(CCObject* pSender) {
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.7f, RankingScene::scene()));
}

void RewardScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {
    CCLOG("cell touched at index: %i", cell->getIdx());
    celltouch = cell->getIdx();
}

CCSize RewardScene::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
    return CCSizeMake(600, 80);
}

CCTableViewCell* RewardScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell *cell = table->dequeueCell();
    cell = new CCTableViewCell();
    cell->autorelease();
    
    // Player Point
    Player1 * p = (Player1*)players->objectAtIndex(idx);
    CCString *string = CCString::createWithFormat("%d",p->getPoint());
    CCLabelTTF *Pointlabel = CCLabelTTF::create(string->getCString(), "Helvetica", 36);
    Pointlabel->setAnchorPoint(ccp(1, 0));
    Pointlabel->setPosition(ccp(500, 20));
    Pointlabel->setTag(123);
    cell->addChild(Pointlabel);
    //time
    CCLabelTTF *timeLabel = CCLabelTTF::create(p->getTime().c_str(), "Helvetica", 16);
    timeLabel->setAnchorPoint(CCPointZero);
    timeLabel->setOpacity(70);
    timeLabel->setPosition(ccp(100, 0));
    cell->addChild(timeLabel);

    // Player Name
    std::string name = p->getName();
    CCLabelTTF *Namelabel = CCLabelTTF::create(p->getName().c_str(), "Helvetica", 36);
    Namelabel->setAnchorPoint(CCPointZero);
    Namelabel->setPosition(ccp(100, 20));
    cell->addChild(Namelabel);
    
    // Player Rank
    char rankBuf[3];
    sprintf(rankBuf, "%i.png", idx+1);
    CCSprite *rank = CCSprite::create(rankBuf);
    rank->setAnchorPoint(CCPointZero);
    rank->setPosition(ccp(0, 10));
    cell->addChild(rank);
    
    int rewardLocal = CCUserDefault::sharedUserDefault()->getIntegerForKey("reward");
    std::string nameLocal = GameManager::sharedGameManager()->getName();
    if (p->getReward() != 0 && rewardLocal != 0) {
        CCMenuItemImage *bt_send_email =
            CCMenuItemImage::create("Present.png","Present.png",
                                    this, menu_selector(RewardScene::clickBtSendEmail));
        bt_send_email->setTag(idx + 100);
        menu = CCMenu::create(bt_send_email, NULL);
        menu->setPosition(ccp(550, 30));
        cell->addChild(menu);
    }
    
    return cell;
}

unsigned int RewardScene::numberOfCellsInTableView(CCTableView *table) {
    return players->count();
}

Player1::Player1(string name , int point, string email, string time, int reward) {
    this->_point    = point;
    this->_name     = name;
    this->_email    = email;
    this->_reward   = reward;
    this->_time     = time;
}
Player1::~Player1(){}
