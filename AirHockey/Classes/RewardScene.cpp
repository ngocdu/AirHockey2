//
//  RewardScene.cpp
//  AirHockey
//
//

#include "RewardScene.h"
#include "cocos2d.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "Menu.h"
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
    CCSprite *background = CCSprite::create("BackGround2.png");
    background->setPosition(ccp(w/2, h/2));
    this->addChild(background);
    //-------------- menu getperesent ---------------
    
    CCHttpRequest* request = new CCHttpRequest();
    string ipAddr = GameManager::sharedGameManager()->getIpAddr();
    request->setUrl((ipAddr+":3000/users.json").c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RewardScene::onHttpRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
    
    //create startMenuItem
    CCMenuItemImage *playItem =
        CCMenuItemImage::create("StartButton.png", "StartButton.png",
                                this, menu_selector(RewardScene::play));
    playItem->setPosition(ccp(w/2, h/8));
    playItem->setScale(0.7f);
    //create bgmItem
    CCMenuItemImage *bgmItem =
        CCMenuItemImage::create("BgmOn.png", "BgmOn.png",
                                this, menu_selector(RewardScene::bgm));
    bgmItem->setPosition(ccp(w*4/5, h/8));
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

void RewardScene::onHttpRequestCompleted(CCNode *sender, void *data) {
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
        string username = CCUserDefault::sharedUserDefault()->getStringForKey("username");
        string email = CCUserDefault::sharedUserDefault()->getStringForKey("email");
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++)
        {
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
    string name = CCUserDefault::sharedUserDefault()->getStringForKey("username");
    char strP[20] = {0};
    sprintf(strP, "%i", point);
    string email  = CCUserDefault::sharedUserDefault()->getStringForKey("email");
    string ipAddr = GameManager::sharedGameManager()->getIpAddr();
    string url    = ipAddr + ":3000/users?name="+name+"&point="+strP+"&email="+email+"&reward=abc&time="+p->getTime();
    request->setUrl(url.c_str());
    request->setRequestType(CCHttpRequest::kHttpPost);
    CCHttpClient::getInstance()->send(request);
    request->release();
    int r = CCUserDefault::sharedUserDefault()->getIntegerForKey("reward");
    CCUserDefault::sharedUserDefault()->setIntegerForKey("reward", r - 1);
    bt_send_email->removeFromParentAndCleanup(true);
}
void RewardScene::bgm(CCObject* pSender) {
    GameManager *game = GameManager::sharedGameManager();
    bgm_off->setVisible(!game->getBgm());
    game->setBgm(!game->getBgm());
    if (game->getBgm()) {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
    } else {
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
    }
}

void RewardScene::play(CCObject* pSender) {
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.7f, Difficulty::scene()));
}

void RewardScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
    celltouch = cell->getIdx();
}

CCSize RewardScene::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
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
    CCLabelTTF *Pointlabel = CCLabelTTF::create(string->getCString(), "Helvetica", 48);
    Pointlabel->setAnchorPoint(ccp(1, 0));
    Pointlabel->setPosition(ccp(500,0));
    Pointlabel->setTag(123);
    cell->addChild(Pointlabel);
    //time
    CCLabelTTF *timeLabel = CCLabelTTF::create(p->getTime().c_str(), "Helvetica", 13);
    timeLabel->setAnchorPoint(ccp(1, 0));
    timeLabel->setPosition(ccp(330,20));
    cell->addChild(timeLabel);

    // Player Name
    std::string name = p->getName();
    CCLabelTTF *Namelabel = CCLabelTTF::create(p->getName().c_str(), "Helvetica", 48);
    Namelabel->setAnchorPoint(CCPointZero);
    Namelabel->setPosition(ccp(100, 0));
    cell->addChild(Namelabel);
    
    // Player Rank
    char rankBuf[3];
    sprintf(rankBuf, "%i.png", idx+1);
    CCSprite *rank = CCSprite::create(rankBuf);
    rank->setAnchorPoint(CCPointZero);
    rank->setPosition(CCPointZero);
    cell->addChild(rank);
    
    {
        int rewardLocal = CCUserDefault::sharedUserDefault()->getIntegerForKey("reward");
        std::string nameLocal = GameManager::sharedGameManager()->getName();
        int pointMax = CCUserDefault::sharedUserDefault()->getIntegerForKey("pointMax");
        if (p->getReward() != 0 && rewardLocal != 0) {
            CCMenuItemImage *bt_send_email =
                CCMenuItemImage::create("Present.png","Present.png",
                                        this, menu_selector(RewardScene::clickBtSendEmail));
            bt_send_email->setTag(idx + 100);
            menu = CCMenu::create(bt_send_email, NULL);
            menu->setPosition(ccp(550, 30));
            cell->addChild(menu);
        }
    }
    return cell;
}

unsigned int RewardScene::numberOfCellsInTableView(CCTableView *table)
{
    return players->count();
}
Player1::Player1(string name , int point, string email, string time, int reward)
{
    this->_point = point;
    this->_name = name;
    this->_email = email;
    this->_reward = reward;
    this->_time = time;
}
Player1::~Player1(){}
