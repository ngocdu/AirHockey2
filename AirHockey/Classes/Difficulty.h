//
//  Difficulty.h
//  AirHockey
//
//  Created by macbook_016 on 2013/06/21.
//
//

#ifndef __AirHockey__Difficulty__
#define __AirHockey__Difficulty__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;

class Difficulty : public CCLayer, public extension::CCEditBoxDelegate{
private:
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float w = size.width;
    float h = size.height;
    extension::CCEditBox  *m_pEditName;
    CCLabelTTF *m_pTTFShowEditReturn;
    CCMenu *pMenu;
public:
    virtual bool init();
    
    static CCScene* scene();
    void toExtensionsMainLayer(CCObject *sender);
    virtual void editBoxEditingDidBegin(extension::CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(extension::CCEditBox* editBox);
    virtual void editBoxTextChanged(extension::CCEditBox* editBox,
                                    const std::string& text);
    virtual void editBoxReturn(extension::CCEditBox* editBox);
    void menuMedium(CCObject *pSender);
    void menuEasy(CCObject  *pSender);
    void menuHard(CCObject  *pSender);

    CREATE_FUNC(Difficulty);
};


#endif /* defined(__AirHockey__Difficulty__) */
