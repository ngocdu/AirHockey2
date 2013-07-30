//
//  RankingScene.h
//  AirHockey
//
//

#ifndef __AirHockey__RankingScene__
#define __AirHockey__RankingScene__

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


class Player : public CCObject {
private:
    CC_SYNTHESIZE(string, _name, Name);
    CC_SYNTHESIZE(int, _point, Point);
    CC_SYNTHESIZE(int, _reward, Reward);
    
public:
    Player(string name , int mark);
    ~Player();
};


class RankingScene : public CCLayer, public CCTableViewDelegate, CCTableViewDataSource {
private:
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float w = size.width;
    float h = size.height;
    
    CCSprite *bgm_off;
    
    CCTableView *tableView;
    CCArray *players = new CCArray();
public:
    virtual bool init();
    static CCScene* scene();
    void onHttpRequestCompleted(CCNode *sender, void *data);
    
    void play(CCObject* pSender);
    void bgm(CCObject* pSender);
    void reward(CCObject* pSender);
    
    CREATE_FUNC(RankingScene);
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){}
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    void convertName(char *str_name);
};
#endif /* defined(__AirHockey__RankingScene__) */