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
    CCSprite *background = CCSprite::create("Menu.png");
    background->setPosition(ccp(w/2, h/2));
    this->addChild(background);
    
    CCSprite *ranking_bg = CCSprite::create("RankingBG.png");
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
        CCMenuItemImage::create("StartButton.png", "StartButton.png",
                                this, menu_selector(RankingScene::play));
    playItem->setPosition(ccp(w/2, h/6));
    playItem->setScale(0.7f);
    //create bgmItem
    CCMenuItemImage *bgmItem =
        CCMenuItemImage::create("BgmOn.png", "BgmOn.png",
                                this, menu_selector(RankingScene::bgm));
    bgmItem->setPosition(ccp(w*4/5, h/6));
    CCMenu* pMenu = CCMenu::create(playItem, bgmItem, NULL);
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

    rapidjson::Document document;
    if(data2 != NULL && !document.Parse<0>(data2).HasParseError())
    {   
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++)
        {
            Player *player = new Player(document[i]["name"].GetString(),
                                        document[i]["point"].GetInt());
            players->addObject(player);
        }
    } else {
        CCLog(document.GetParseError());
    }
    d = -1;
    CCTableView *tableView=CCTableView::create(this, CCSizeMake(700, 350));
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setAnchorPoint(ccp(0, 0));
    tableView->setPosition(ccp(size.width/8, 300));
    tableView->setDelegate(this);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView, 21);
    tableView->reloadData();
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
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.7f, Difficulty::scene()));
}

void RankingScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
}

CCSize RankingScene::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    return CCSizeMake(600, 80);
}

CCTableViewCell* RankingScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell *cell = table->dequeueCell();
    cell = new CCTableViewCell();
    cell->autorelease();
    //
    Player * p = (Player*)players->objectAtIndex(idx);
    CCString *string = CCString::createWithFormat("%d",p->getPoint());
    CCLabelTTF *Pointlabel = CCLabelTTF::create(string->getCString(), "Helvetica", 50.0);
    Pointlabel->setAnchorPoint(ccp(1, 0));
    Pointlabel->setPosition(ccp(500,0));
    Pointlabel->setTag(123);
    cell->addChild(Pointlabel);
    
    std::string name = p->getName();
    CCLabelTTF *Namelabel = CCLabelTTF::create(p->getName().c_str(), "Helvetica", 50.0);
    Namelabel->setAnchorPoint(CCPointZero);
    Namelabel->setPosition(CCPointZero);
    cell->addChild(Namelabel);
    
    if (idx == 0) {
        int rewardLocal = CCUserDefault::sharedUserDefault()->getIntegerForKey("reward");
        std::string nameLocal = GameManager::sharedGameManager()->getName();
        int pointMax = CCUserDefault::sharedUserDefault()->getIntegerForKey("pointMax");
        if (p->getReward() != 0 && rewardLocal != 0 &&
            p->getName() == nameLocal && p->getPoint() == pointMax) {
            CCMenuItemImage *bt_send_email = CCMenuItemImage::create("Present.png",
                                                                     "Present.png", this, menu_selector(RankingScene::clickBtSendEmail));
            CCMenu * menu = CCMenu::create(bt_send_email, NULL);
            menu->setPosition(ccp(550, 20));
            cell->addChild(menu);
        }
        
    }
    return cell;
}

unsigned int RankingScene::numberOfCellsInTableView(CCTableView *table)
{
    return players->count();
}
Player::Player(string name, int point)
{
    this->_point = point;
    this->_name = name;
}
Player::~Player(){}
