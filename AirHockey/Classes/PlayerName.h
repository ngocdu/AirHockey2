//
//  PlayerName.h
//  AirHockey
//
//  Created by macbook_016 on 2013/06/21.
//
//

#ifndef __AirHockey__PlayerName__
#define __AirHockey__PlayerName__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;

class PlayerName : public CCLayer, public extension::CCEditBoxDelegate{
private:
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
    void clickBtBack(CCObject* pScene);

    CREATE_FUNC(PlayerName);
};


#endif /* defined(__AirHockey__PlayerName__) */
