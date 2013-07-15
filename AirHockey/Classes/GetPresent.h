//
//  GetPresent.h
//  AirHockey
//
//

#ifndef AirHockey_GetPresent_h
#define AirHockey_GetPresent_h
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
class GetPresent : public CCLayer, public extension::CCEditBoxDelegate{
private:
    extension::CCEditBox* m_pEditName;
    CCLabelTTF* m_pTTFShowEditReturn;
    CCMenu* pMenu;
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    void toExtensionsMainLayer(CCObject *sender);
    virtual void editBoxEditingDidBegin(extension::CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(extension::CCEditBox* editBox);
    virtual void editBoxTextChanged(extension::CCEditBox* editBox,
                                    const std::string& text);
    virtual void editBoxReturn(extension::CCEditBox* editBox);
    void menuSendEmail(CCObject *pSender);
    void menuBack(CCObject *pSender);
    bool is_email(std::string const& address);
    // a selector callback
    CREATE_FUNC(GetPresent);
};
#endif