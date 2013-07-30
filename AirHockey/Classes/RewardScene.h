//
//  RewardScene.h
//  AirHockey
//
//

#ifndef __AirHockey__RewardScene__
#define __AirHockey__RewardScene__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCTableView.h"
#include "CCScrollView.h"
#include "CCTableViewCell.h"
#include <set>
#include <vector>
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;


class Player1 : public CCObject {
private:
    CC_SYNTHESIZE(string, _name, Name);
    CC_SYNTHESIZE(int, _point, Point);
    CC_SYNTHESIZE(int, _reward, Reward);
    CC_SYNTHESIZE(string, _time, Time);
    CC_SYNTHESIZE(string, _email, Email);
public:
    Player1(string name , int mark, string email, string time, int reward);
    ~Player1();
};


class RewardScene : public CCLayer, public CCTableViewDelegate, CCTableViewDataSource {
private:
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float w = size.width;
    float h = size.height;
    CCMenu * rewardMenu;
    
    CCArray *players = new CCArray();
    int celltouch;
public:
    virtual bool init();
    static CCScene* scene();
    void onHttpRequestCompleted(CCNode *sender, void *data);
    
    void back(CCObject* pSender);
    void clickBtSendEmail(CCObject* pSender);
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){}
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    void convertName(char *str_name);
    CREATE_FUNC(RewardScene); 
};
#endif /* defined(__AirHockey__RewardScene__) */