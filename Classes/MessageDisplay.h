#ifndef _MessageDisplay_h_
#define _MessageDisplay_h_

#include "cocos2d.h"
#include "LFAlert.h"
#include "font.h"
#include "LFStrings.h"
#include <string>
#include "IOSStoreLayer.h"
#include "LFUtile.h"
#include "MarketLayer.h"
#include "MenuLayer.h"
#include "Music.h"
using std::string;
using namespace cocos2d;

#define TAG_MessageDisplayer_Layer 4643+10001

class MessageDisplayCallBackInterface
{
public: virtual void messageDialogEndCallback() = 0;
};


//MessageDisplay*layer = MessageDisplay::create("infoStr");
//CCDirector::sharedDirector()->getRunningScene()->addChild(layer);

class MessageDisplay : public LFAlert
{
public:

	CCSprite* m_pDialog;
	OpenBoxComeFrom	m_openComeFrom;
	CCLabelTTF* m_pLabel;
	CCLabelTTF* m_pBtnLabel;

	static MessageDisplay* create(MessageDisplayCallBackInterface* callback = NULL)
	{
		MessageDisplay* layer = new MessageDisplay(callback);
		if(layer != NULL)
		{
			layer->autorelease();
			layer->setAutoDismiss(MSG_MarketClosed, true);
		}
		else
		{
			CC_SAFE_DELETE(layer);
			layer  = NULL;
		}

		return layer;
	}

	~MessageDisplay()
	{
		m_pDialog = NULL;
		CC_SAFE_RELEASE(mLabel);
		CC_SAFE_RELEASE(mMenu);
	}

	MessageDisplay(MessageDisplayCallBackInterface* callback)
	{
		m_openComeFrom = from_MarketStore;
		mCallback = callback;
		m_pDialog = NULL;
		m_pLabel = NULL;
		m_pBtnLabel = NULL;

		CCSize visSize = CCDirector::sharedDirector()->getWinSize();
		m_pDialog = CCSprite::create(ResManager::getManager()->getSharedFilePath("messagebox_bg.png").c_str());
		m_pDialog->setPosition(ccp(visSize.width/2, visSize.height/2));
		//m_pDialog->setScale(0.5f);

		//按钮
		CCMenu* menu = CCMenu::create();
		menu->setPosition(CCPointZero);
		{
			CCMenuItemImage* menuItemOk = CCMenuItemImage::create(
				ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu.png").c_str(),
				ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu_select.png").c_str(),
				this,SEL_MenuHandler(&MessageDisplay::menuItemOkClicked));
			menu->addChild(menuItemOk, 1);
			menuItemOk->setPosition(ccp(m_pDialog->getContentSize().width/2, menuItemOk->getContentSize().height + 50));
			menuItemOk->setScale(1.5f);
			mMenu = menu;
			mMenu->retain();
			menu->setTouchPriority(-1050);

			//按钮上面的字
			m_pBtnLabel = CCLabelTTF::create(LFStrings::getValue("OpenBox").c_str(), fontStr_katong, 36, CCSize(400, 0), kCCTextAlignmentCenter);
			m_pDialog->addChild(m_pBtnLabel, 1);
			m_pBtnLabel->setPosition(menuItemOk->getPosition());
			m_pBtnLabel->setScale(menuItemOk->getScale());
		}

		//label
		{
			m_pLabel = CCLabelTTF::create("", fontStr_katong, 38, CCSize(380, 0), kCCTextAlignmentLeft);
			m_pLabel->setAnchorPoint(ccp(0.5, 1));
			m_pLabel->setPosition(ccp(m_pDialog->getContentSize().width/2 + 10, m_pDialog->getContentSize().height/2 + 70));
			m_pLabel->setColor(fontColor_Store);
			m_pDialog->addChild(m_pLabel, 1);
			mLabel = m_pLabel;
			mLabel->retain();
		}
		//
		{
			this->setTag(TAG_MessageDisplayer_Layer);
			this->setZOrder(1025);
			this->setBaseContainer(m_pDialog,menu);
		}
	}

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		CCPoint pos = LF::lfConvertToLocalPos(pTouch);

		// 如果有添加显示的
		if (mBaseNode)
		{
			if (!canTouch(pos))
			{
				if (mAutoDismiss && !mBaseNode->boundingBox().containsPoint(pos))
				{
					CCNotificationCenter::sharedNotificationCenter()->postNotification(mMessage.c_str(),this);
					CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
					removeFromParent();
					return mModal;
				}
                
                float scale = 1.0f;
                mBaseNode->stopAllActions();
                mBaseNode->runAction(CCSequence::create(
                                                        CCScaleTo::create(0.1f,scale+0.1f),
                                                        CCScaleTo::create(0.1f,scale-0.1f),
                                                        CCScaleTo::create(0.05f,scale),
                                                        NULL
                                                        ));

			}else
			{
				return false;
			}
		}
		return mModal;
	}

	void menuItemOkClicked(CCObject* pSender)
	{
// 		if(mCallback != NULL)
// 		{
			if (m_openComeFrom == from_MarketStore)
			{
				m_pDialog->runAction(CCSequence::create(
					CCScaleTo::create(0.2f, 0.1f), 
					CCCallFuncN::create(this, callfuncN_selector(MessageDisplay::removeCurLayer)),
					NULL));
			}
			else if (m_openComeFrom == from_ExitGame)
			{
				Music::sharedMusicPlayer()->buttonClicked();
				this->removeCurLayer(NULL);
				CCDirector::sharedDirector()->end();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                exit(0);
#endif
			}
			else if (m_openComeFrom == from_ClickMainBtn)
			{
				BaseScense::getCurrentScene()->getMenuLayer()->gotoMainPage();
				this->removeCurLayer(NULL);
			}
			else 
			{
				IOSStoreLayer* pLayer = IOSStoreLayer::create();
				pLayer->setComefrom(m_openComeFrom);
				CCDirector::sharedDirector()->getRunningScene()->addChild(pLayer, 200);
				this->removeCurLayer(NULL);
			}
//		}
	}

	void removeCurLayer(CCNode* pNode)
	{
//		CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MarketClosed, NULL);
		this->removeFromParent();
	}

	void setComeFrom(OpenBoxComeFrom comefrom)
	{
		m_openComeFrom = comefrom;
		if (m_openComeFrom == from_MarketStore)
		{
			m_pLabel->setString("");
		}
		else if (m_openComeFrom == from_GameInHelpMap)
		{
			m_pLabel->setString(LFStrings::getValue("OpenBox_GameInHelpMap").c_str());
		}
		else if (m_openComeFrom == from_GameInSuperTool)
		{
			m_pLabel->setString(LFStrings::getValue("OpenBox_GameInSuperTool").c_str());
		}
		else if (m_openComeFrom == from_SeasonSelector)
		{
			m_pLabel->setString(LFStrings::getValue("OpenBox_SeasonSelector").c_str());
			m_pLabel->setPosition(ccp(m_pLabel->getPositionX(), m_pLabel->getPositionY() + 100));
		}
		else if (m_openComeFrom == from_ClickMainBtn)
		{
			m_pLabel->setString(LFStrings::getValue("Back_To_Main").c_str());
			m_pLabel->setHorizontalAlignment(kCCTextAlignmentCenter);
			m_pBtnLabel->setString(LFStrings::getValue("input_ok").c_str());
		}
		else if (m_openComeFrom == from_ExitGame)
		{
			m_pLabel->setString(LFStrings::getValue("Exit_Game_OK").c_str());
			m_pLabel->setHorizontalAlignment(kCCTextAlignmentCenter);
			m_pBtnLabel->setString(LFStrings::getValue("input_ok").c_str());
		}
	}
    
    void removeGuide(float dt)
    {
//        removeCurLayer(NULL);
        this->setAutoDismiss("", true);
    }

protected:

	CCLabelTTF* mLabel;
	CCMenu* mMenu;

	MessageDisplayCallBackInterface *mCallback;
};

#endif //_MessageDisplay_h_