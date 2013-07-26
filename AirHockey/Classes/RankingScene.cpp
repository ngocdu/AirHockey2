//
//  RankingScene.cpp
//  AirHockey
//
//

#include "RankingScene.h"
#include "RewardScene.h"
#include "cocos2d.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
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
    CCSprite *background = CCSprite::create("BackGround2.png");
    background->setPosition(ccp(w/2, h/2));
    this->addChild(background);
    
    CCSprite *topLine = CCSprite::create("line.png");
    topLine->setPosition(ccp(w/2, 602));
    topLine->setOpacity(70);
    this->addChild(topLine);
    
    CCSprite *bottomLine = CCSprite::create("line.png");
    bottomLine->setPosition(ccp(w/2, 248));
    bottomLine->setOpacity(70);
    this->addChild(bottomLine);
    //-------------- menu getperesent ---------------
    
    
    CCMenuItemImage *reward  =
        CCMenuItemImage::create("Present.png","PresentOnClicked.png",
                                this, menu_selector(RankingScene::reward));
    reward->setPosition(ccp(w/5, h/8));

    string playerName = GameManager::sharedGameManager()->getName();
    if (playerName == "") reward->setVisible(false);
    
    CCHttpRequest* request = new CCHttpRequest();
    string ipAddr = GameManager::sharedGameManager()->getIpAddr();
    request->setUrl((ipAddr+":3000/users.json").c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RankingScene::onHttpRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
    
    //create startMenuItem
    CCMenuItemImage *playItem =
        CCMenuItemImage::create("StartButton.png", "StartButtonOnClicked.png",
                                this, menu_selector(RankingScene::play));
    playItem->setPosition(ccp(w/2, h/8 - 10));
    //create bgmItem
    CCMenuItemImage *bgmItem =
        CCMenuItemImage::create("BgmOn.png", "BgmOn.png",
                                this, menu_selector(RankingScene::bgm));
    bgmItem->setPosition(ccp(w*4/5, h/8));
    CCMenu* pMenu = CCMenu::create(playItem, bgmItem, reward, NULL);
    pMenu->setPosition(ccp(0,0));
    this->addChild(pMenu);
    

    bgm_off = CCSprite::create("BgmOff.png");
    bgm_off->setPosition(bgmItem->getPosition());
    bgm_off->setVisible(GameManager::sharedGameManager()->getBgm());
    if (GameManager::sharedGameManager()->getBgm()) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
    } else {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
    }
    this->addChild(bgm_off);
    
        
    return true;
}

void RankingScene::onHttpRequestCompleted(CCNode *sender, void *data) {
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
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++) {
            Player *player = new Player(document[i]["name"].GetString(),
                                        document[i]["point"].GetInt());
            players->addObject(player);
        }
    } else {
        CCLog(document.GetParseError());
    }
    d = -1;
    tableView = CCTableView::create(this, CCSizeMake(700, 350));
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setAnchorPoint(ccp(0, 0));
    tableView->setPosition(ccp(w/8, 250));
    tableView->setDelegate(this);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView, 21);
    tableView->reloadData();
}

void RankingScene::reward(cocos2d::CCObject *pSender) {
    CCDirector::sharedDirector()->replaceScene(RewardScene::scene());
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
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.7f, Difficulty::scene()));
}

void RankingScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {
    CCLOG("cell touched at index: %i", cell->getIdx());
}

CCSize RankingScene::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
    return CCSizeMake(600, 80);
}

CCTableViewCell* RankingScene::tableCellAtIndex(CCTableView *table, unsigned int idx) {
    CCTableViewCell *cell = table->dequeueCell();
    cell = new CCTableViewCell();
    cell->autorelease();
    
    // Player Point
    Player * p = (Player*)players->objectAtIndex(idx);
    CCString *string = CCString::createWithFormat("%d",p->getPoint());

    CCLabelTTF *Pointlabel = CCLabelTTF::create(string->getCString(), "Helvetica", 40);
    Pointlabel->setAnchorPoint(ccp(1, 0));
    Pointlabel->setPosition(ccp(600,0));
    Pointlabel->setTag(123);
    cell->addChild(Pointlabel);
    
    // Player Name
    std::string name = p->getName();

    CCLabelTTF *Namelabel = CCLabelTTF::create(p->getName().c_str(), "Helvetica", 40);
    Namelabel->setAnchorPoint(CCPointZero);
    Namelabel->setPosition(ccp(60, 0));
    cell->addChild(Namelabel);
    
    // Player Rank
    char rankBuf[3];
    sprintf(rankBuf, "%i.png", idx+1);
    CCSprite *rank = CCSprite::create(rankBuf);
    rank->setAnchorPoint(CCPointZero);
    rank->setPosition(CCPointZero);
    cell->addChild(rank);
    
    if (idx == 0) {
        int rewardLocal = CCUserDefault::sharedUserDefault()->getIntegerForKey("reward");
        std::string nameLocal = GameManager::sharedGameManager()->getName();
        int pointMax = CCUserDefault::sharedUserDefault()->getIntegerForKey("pointMax");
        if (p->getReward() != 0 && rewardLocal != 0 &&
            p->getName() == nameLocal && p->getPoint() == pointMax) {
            CCMenuItemImage *bt_send_email =
                CCMenuItemImage::create("Present.png","Present.png",
                                        this, menu_selector(RankingScene::reward));
            CCMenu * menu = CCMenu::create(bt_send_email, NULL);
            menu->setPosition(ccp(550, 30));
            cell->addChild(menu);
        }
    }
    return cell;
}

unsigned int RankingScene::numberOfCellsInTableView(CCTableView *table) {
    return players->count();
}

Player::Player(string name, int point) {
    this->_point = point;
    this->_name = name;
}

Player::~Player(){}
