#include "LFAlerts.h"
#include "LFAlert.h"
#include "LFStrings.h"
#include "GameInforEditer.h"
#include "font.h"
#include "LFToast.h"
#include "LevelManager.h"
#include "MenuLayer.h"
#include "PriceDefine.h"
#include "VersionControl.h"
#include "MarketLayer.h"
#include "IOSStoreLayer.h"
#include "Debug.h"
#include "VersionControl.h"
#include "GameInforEditer.h"
#define  MSG_CHOOSE_PAYWAY	"MSG_CHOOSE_PAYWAY"

BaseBuyAlert::BaseBuyAlert()
{
	mAlertNode = NULL;
	init();
}

void BaseBuyAlert::setEnbale(bool enable)
{
	mAlertNode->setMenuItemEnable(enable);
}

void BaseBuyAlert::alertClosed(CCNode* alter)
{
	mAlertNode = NULL;
	removeAllObserver();
}
void BaseBuyAlert::removeAllObserver()
{
	for (int i=0,count = mAutoRemovePool.size(); i<count; i++)
	{
		CCNotificationCenter::sharedNotificationCenter()->removeObserver(mAutoRemovePool[i].target,mAutoRemovePool[i].name.c_str());
	}
	mAutoRemovePool.clear();
}

CCMenuItem* BaseBuyAlert::creatMenuItem(CCObject* target, SEL_MenuHandler selector,string lable)
{
	CCMenuItemImage* menuItem = CCMenuItemImage::create(
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu.png").c_str(),
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu_select.png").c_str(),
		target,selector);
	CCLabelTTF* _lable = CCLabelTTF::create(lable.c_str(),fontStr_katong,30,CCSizeMake(340,0),kCCTextAlignmentCenter);
	menuItem->addChild(_lable,0,TAG_LABLE_IN_MENUITEM);
	_lable->setAnchorPoint(ccp(0.5f,0.5f));
	_lable->setPosition(ccp(menuItem->getContentSize().width/2,menuItem->getContentSize().height/2));
	return menuItem;
}

void BaseBuyAlert::onExit()
{
	CCNode::onExit();
	removeAllObserver();
}


void BaseBuyAlert::recivedRemoveAlertMsg(CCObject* not_use)
{
	if (mAlertNode != NULL)
	{
		mAlertNode->removeFromParent();
		mAlertNode = NULL;
	}
}
BaseBuyAlert* BaseBuyAlert::addAutoRemoveObserver(CCObject *_target,SEL_CallFuncO _selector,string _name)
{
	CCNotificationCenter::sharedNotificationCenter()->addObserver(_target,_selector,_name.c_str(),NULL);
	ObserverItem item(_target,_selector,_name);
	mAutoRemovePool.push_back(item);
	return this;
}
LFAlert* BaseBuyAlert::getLFAlert()
{
	return mAlertNode;
}

void BaseBuyAlert::removeAllBuyAlerts()
{
	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REMOVE_BUY_ALERTS,NULL);
}

// 准备完毕
void BaseBuyAlert::prepareComplete(bool callAutoRelease)
{
// 	mAlertNode = alertNodeFactory();
// 	addAutoRemoveObserver(this,SEL_CallFuncO(&BaseBuyAlert::recivedRemoveAlertMsg),MSG_REMOVE_BUY_ALERTS);
// 	// 监听选择支付的消息
// 	addAutoRemoveObserver(this,SEL_CallFuncO(&BaseBuyAlert::paywayChoosed),MSG_CHOOSE_PAYWAY);
// 
// 	if (callAutoRelease)
// 	{
// 		mAlertNode->addChild(this); // 保持引用，避免被释放
// 		autorelease();				// 自动释放
// 	}
}



//////////////////////////////////////////////////////////////////////////
// LFAlert_FreeTimeOut
//////////////////////////////////////////////////////////////////////////
LFAlert_FreeTimeOut::LFAlert_FreeTimeOut(int seasonId,int sectionId)
{
	this->seasonId = seasonId;
	this->sectionId = sectionId;
}

LFAlert_FreeTimeOut* LFAlert_FreeTimeOut::showAlert(int seasonId,int sectionId)
{
	LFAlert_FreeTimeOut* temp = new LFAlert_FreeTimeOut(seasonId,sectionId);
	temp->prepareComplete(true);
	return temp;
}

bool LFAlert_FreeTimeOut::isTimeToShowBeforePlay(int seasonId,int sectionId)
{
	if (!GameInforEditer::getGameInfor()->isIgnoreAllLock() && // 不忽略
		!GameInforEditer::getGameInfor()->isSectionTimeLimiteBuyed(seasonId,sectionId) && // 没有购买
		GameInforEditer::getGameInfor()->getSectionFreePlayTimeLeft(seasonId,sectionId) <= 0)
	{
		return true;
	}
	return false;
}

void LFAlert_FreeTimeOut::buyOneTime(CCObject* sender)
{
	CCLog("%s",__FUNCTION__);
	int starNum = GameInforEditer::getGameInfor()->getSectionPlayToCostStarNum(seasonId,sectionId);
	if (GameInforEditer::getGameInfor()->getTotalFreeScore() < starNum)
	{
		LFToast::makeToast("Your star is not enough!");
		return;
	}

	GameInforEditer::getGameInfor()->subMyStarNum(starNum);

	//然后
	mAlertNode->setVisible(false);
	CCNotificationCenter::sharedNotificationCenter()->postNotification(BUY_ONE_PLAY_TIME_SUC,mAlertNode);

	setEnbale(false);
}
void LFAlert_FreeTimeOut::buyTimeLimite(CCObject* sender)
{
	CCLog("%s",__FUNCTION__);
	LFAlerts_ChoosePayWay::showAlert(true);
	// 不应该移除
	// 因为他身上还帮别人监听了很多消息
	// 移除了，那消息就没有了
	//mAlertNode->removeFromParent(); 
	// 
}
LFAlert* LFAlert_FreeTimeOut::alertNodeFactory()
{
	CCSprite* base = CCSprite::create(ResManager::getManager()->getSharedFilePath("messagebox_bg.png").c_str());
	CCMenu* menu = NULL;
	char temp[1024];
	// 只能用人民币购买
	if (GameInforEditer::getGameInfor()->getSectionPlayTimeLeft(seasonId,sectionId) <= 0)
	{
		int money = GameInforEditer::getGameInfor()->getBuySectionTimeLimiteCostMoney();
		CCMenuItemImage* buyLimitItem = CCMenuItemImage::create(
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu_select.png").c_str(),
			this,SEL_MenuHandler(&LFAlert_FreeTimeOut::buyTimeLimite)
			);
		{
			sprintf(temp,LFStrings::getValue("buyItemUseMoney").c_str(),money);
			CCLabelTTF* lable = CCLabelTTF::create(temp,fontStr_katong,30,CCSizeMake(340,0),kCCTextAlignmentCenter);
			buyLimitItem->addChild(lable);
			lable->setAnchorPoint(ccp(0.5f,0.5f));
			lable->setPosition(ccp(buyLimitItem->getContentSize().width/2,buyLimitItem->getContentSize().height/2));
		}

		CCLabelTTF* buyString = CCLabelTTF::create(getBuySectionUseMoneyString(money).c_str(),fontStr_katong,27,CCSizeMake(360,0),kCCTextAlignmentCenter);
		buyString->setAnchorPoint(ccp(0.5f,0.5f));
		base->addChild(buyString);
		buyString->setPosition(ccp(240,280));
		buyString->setColor(ccc3(157,107,0));

		menu = CCMenu::create(buyLimitItem,NULL);

		buyLimitItem->setPosition(ccp(240,120));
	}
	else
	{// 可以用人民币购买，也可以用星星购买
		int starNum = GameInforEditer::getGameInfor()->getSectionPlayToCostStarNum(seasonId,sectionId);
		int money = GameInforEditer::getGameInfor()->getBuySectionTimeLimiteCostMoney();
		CCMenuItemImage* buyOneTimeItem = CCMenuItemImage::create(
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu_select.png").c_str(),
			this,SEL_MenuHandler(&LFAlert_FreeTimeOut::buyOneTime)
			);
		{
			sprintf(temp,LFStrings::getValue("buyItemUseStar").c_str(),starNum);
			CCLabelTTF* lable = CCLabelTTF::create(temp,fontStr_katong,30,CCSizeMake(340,0),kCCTextAlignmentCenter);
			buyOneTimeItem->addChild(lable);
			lable->setAnchorPoint(ccp(0.5f,0.5f));
			lable->setPosition(ccp(buyOneTimeItem->getContentSize().width/2,buyOneTimeItem->getContentSize().height/2));

			if (GameInforEditer::getGameInfor()->getTotalFreeScore() < starNum)
			{
				lable->setColor(ccRED);
			}
		}


		CCMenuItemImage* buyLimitItem = CCMenuItemImage::create(
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu_select.png").c_str(),
			this,SEL_MenuHandler(&LFAlert_FreeTimeOut::buyTimeLimite)
			);
		{
			sprintf(temp,LFStrings::getValue("buyItemUseMoney").c_str(),money);
			CCLabelTTF* lable = CCLabelTTF::create(temp,fontStr_katong,30,CCSizeMake(340,0),kCCTextAlignmentCenter);
			buyLimitItem->addChild(lable);
			lable->setAnchorPoint(ccp(0.5f,0.5f));
			lable->setPosition(ccp(buyLimitItem->getContentSize().width/2,buyLimitItem->getContentSize().height/2));
		}

		CCLabelTTF* buyString = CCLabelTTF::create(getBuySectionUseStarOrMoneyString(starNum,money).c_str(),fontStr_katong,27,CCSizeMake(360,0),kCCTextAlignmentCenter);
		buyString->setAnchorPoint(ccp(0.5f,0.5f));
		base->addChild(buyString);
		buyString->setPosition(ccp(240,290));
		buyString->setColor(ccc3(157,107,0));

		menu = CCMenu::create(buyOneTimeItem,buyLimitItem,NULL);

		buyOneTimeItem->setPosition(ccp(140,120));
		buyLimitItem->setPosition(ccp(340,120));
	}

	LFAlert* alert = new LFAlert();
	alert->setBaseContainer(base,menu);
	alert->setAutoDismiss(MSG_BUY_SECTION_FRAME_AUTO_DISSMISS,true);
	alert->show();
	alert->release();

	addAutoRemoveObserver(this,SEL_CallFuncO(&LFAlert_FreeTimeOut::alertClosed),MSG_BUY_SECTION_FRAME_AUTO_DISSMISS);

	return alert;
}
string LFAlert_FreeTimeOut::getBuySectionUseStarOrMoneyString(int starNum,int money)
{
	char temp[1024];
	sprintf(temp,LFStrings::getValue("buyPlayTime").c_str(),money,starNum);
	return string(temp);
}
string LFAlert_FreeTimeOut::getBuySectionUseMoneyString(int money)
{
	char temp[1024];
	sprintf(temp,LFStrings::getValue("buyTimeLimite").c_str(),money);
	return string(temp);
}


//////////////////////////////////////////////////////////////////////////
// LFAlert_FreeTimeOut_InGameFaild
//////////////////////////////////////////////////////////////////////////
LFAlert_FreeTimeOut_InGameFaild::LFAlert_FreeTimeOut_InGameFaild(int seasonId,int sectionId):LFAlert_FreeTimeOut(seasonId,sectionId)
{
}
LFAlert_FreeTimeOut_InGameFaild* LFAlert_FreeTimeOut_InGameFaild::showAlert(int seasonId,int sectionId)
{
	LFAlert_FreeTimeOut_InGameFaild* temp = new LFAlert_FreeTimeOut_InGameFaild(seasonId,sectionId);
	temp->prepareComplete(true);
	return temp;
}

void LFAlert_FreeTimeOut_InGameFaild::goSectionSelected(CCObject* sender)
{
	BaseScense::getCurrentScene()->getMenuLayer()->gotoSelectLevelPage();
	//LevelManager::getManager()->go
	CCLog("%s",__FUNCTION__);

	setEnbale(false);
	mAlertNode->removeFromParent();
}
void LFAlert_FreeTimeOut_InGameFaild::goMainSelected(CCObject* sender)
{
	BaseScense::getCurrentScene()->getMenuLayer()->gotoMainPage();
	CCLog("%s",__FUNCTION__);

	setEnbale(false);
	mAlertNode->removeFromParent();
}

LFAlert* LFAlert_FreeTimeOut_InGameFaild::alertNodeFactory()
{
	CCSprite* base = CCSprite::create(ResManager::getManager()->getSharedFilePath("messagebox_bg.png").c_str());
	CCMenu* menu = NULL;
	char temp[1024];
	// 只能用人民币购买
	if (GameInforEditer::getGameInfor()->getSectionPlayTimeLeft(seasonId,sectionId) <= 0)
	{
		int money = GameInforEditer::getGameInfor()->getBuySectionTimeLimiteCostMoney();
		CCMenuItemImage* buyLimitItem = CCMenuItemImage::create(
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu_select.png").c_str(),
			this,SEL_MenuHandler(&LFAlert_FreeTimeOut_InGameFaild::buyTimeLimite)
			);
		{
			sprintf(temp,LFStrings::getValue("buyItemUseMoney").c_str(),money);
			CCLabelTTF* lable = CCLabelTTF::create(temp,fontStr_katong,30,CCSizeMake(340,0),kCCTextAlignmentCenter);
			buyLimitItem->addChild(lable);
			lable->setAnchorPoint(ccp(0.5f,0.5f));
			lable->setPosition(ccp(buyLimitItem->getContentSize().width/2,buyLimitItem->getContentSize().height/2));
		}

		CCLabelTTF* buyString = CCLabelTTF::create(getBuySectionUseMoneyString(money).c_str(),fontStr_katong,27,CCSizeMake(360,0),kCCTextAlignmentCenter);
		buyString->setAnchorPoint(ccp(0.5f,0.5f));
		base->addChild(buyString);
		buyString->setPosition(ccp(240,280));
		buyString->setColor(ccc3(157,107,0));

		menu = CCMenu::create(buyLimitItem,NULL);

		buyLimitItem->setPosition(ccp(240,80));
	}
	else
	{// 可以用人民币购买，也可以用星星购买
		int starNum = GameInforEditer::getGameInfor()->getSectionPlayToCostStarNum(seasonId,sectionId);
		int money = GameInforEditer::getGameInfor()->getBuySectionTimeLimiteCostMoney();
		CCMenuItemImage* buyOneTimeItem = CCMenuItemImage::create(
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu_select.png").c_str(),
			this,SEL_MenuHandler(&LFAlert_FreeTimeOut_InGameFaild::buyOneTime)
			);
		{
			sprintf(temp,LFStrings::getValue("buyItemUseStar").c_str(),starNum);
			CCLabelTTF* lable = CCLabelTTF::create(temp,fontStr_katong,30,CCSizeMake(340,0),kCCTextAlignmentCenter);
			buyOneTimeItem->addChild(lable);
			lable->setAnchorPoint(ccp(0.5f,0.5f));
			lable->setPosition(ccp(buyOneTimeItem->getContentSize().width/2,buyOneTimeItem->getContentSize().height/2));

			if (GameInforEditer::getGameInfor()->getTotalFreeScore() < starNum)
			{
				lable->setColor(ccRED);
			}
		}


		CCMenuItemImage* buyLimitItem = CCMenuItemImage::create(
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu_select.png").c_str(),
			this,SEL_MenuHandler(&LFAlert_FreeTimeOut_InGameFaild::buyTimeLimite)
			);
		{
			sprintf(temp,LFStrings::getValue("buyItemUseMoney").c_str(),money);
			CCLabelTTF* lable = CCLabelTTF::create(temp,fontStr_katong,30,CCSizeMake(340,0),kCCTextAlignmentCenter);
			buyLimitItem->addChild(lable);
			lable->setAnchorPoint(ccp(0.5f,0.5f));
			lable->setPosition(ccp(buyLimitItem->getContentSize().width/2,buyLimitItem->getContentSize().height/2));
		}

		CCLabelTTF* buyString = CCLabelTTF::create(getBuySectionUseStarOrMoneyString(money,starNum).c_str(),fontStr_katong,27,CCSizeMake(360,0),kCCTextAlignmentCenter);
		buyString->setAnchorPoint(ccp(0.5f,0.5f));
		base->addChild(buyString);
		buyString->setPosition(ccp(240,290));
		buyString->setColor(ccc3(157,107,0));

		menu = CCMenu::create(buyOneTimeItem,buyLimitItem,NULL);

		buyOneTimeItem->setPosition(ccp(140,80));
		buyLimitItem->setPosition(ccp(340,80));
	}

	{
		// 主页
		CCMenuItemImage* homeItem = CCMenuItemImage::create(
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu_select.png").c_str(),
			this,SEL_MenuHandler(&LFAlert_FreeTimeOut_InGameFaild::goMainSelected)
			);
		{
			CCLabelTTF* lable = CCLabelTTF::create(LFStrings::getValue("home").c_str(),fontStr_katong,30,CCSizeMake(340,0),kCCTextAlignmentCenter);
			homeItem->addChild(lable);
			lable->setAnchorPoint(ccp(0.5f,0.5f));
			lable->setPosition(ccp(homeItem->getContentSize().width/2,homeItem->getContentSize().height/2));
		}
		menu->addChild(homeItem);

		// 关卡选择
		CCMenuItemImage* sectionSelectItem = CCMenuItemImage::create(
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu.png").c_str(),
			ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu_select.png").c_str(),
			this,SEL_MenuHandler(&LFAlert_FreeTimeOut_InGameFaild::goSectionSelected)
			);
		{
			CCLabelTTF* lable = CCLabelTTF::create(LFStrings::getValue("select_section").c_str(),fontStr_katong,30,CCSizeMake(340,0),kCCTextAlignmentCenter);
			sectionSelectItem->addChild(lable);
			lable->setAnchorPoint(ccp(0.5f,0.5f));
			lable->setPosition(ccp(sectionSelectItem->getContentSize().width/2,sectionSelectItem->getContentSize().height/2));
		}

		menu->addChild(sectionSelectItem);


		homeItem->setPosition(ccp(140,160));
		sectionSelectItem->setPosition(ccp(340,160));
	}

	LFAlert* alert = new LFAlert();
	alert->setBaseContainer(base,menu);
	//alert->setAutoDismiss(MSG_BUY_SECTION_FRAME_AUTO_DISSMISS,true);
	//不能自动移除，必须要用户选择
	alert->setAutoDismiss(MSG_BUY_SECTION_FRAME_AUTO_DISSMISS,false);
	alert->show();
	alert->release();

	addAutoRemoveObserver(this,SEL_CallFuncO(&LFAlert_FreeTimeOut_InGameFaild::alertClosed),MSG_BUY_SECTION_FRAME_AUTO_DISSMISS);

	return alert;
}



//////////////////////////////////////////////////////////////////////////
// LFAlert_FreeTimeOut_GoNext
//////////////////////////////////////////////////////////////////////////
LFAlert_FreeTimeOut_GoNext::LFAlert_FreeTimeOut_GoNext(int seasonId,int sectionId):LFAlert_FreeTimeOut(seasonId,sectionId)
{
}
LFAlert_FreeTimeOut_GoNext* LFAlert_FreeTimeOut_GoNext::showAlert(int seasonId,int sectionId)
{
	LFAlert_FreeTimeOut_GoNext* temp = new LFAlert_FreeTimeOut_GoNext(seasonId,sectionId);
	temp->prepareComplete(true);
	return temp;
}
string LFAlert_FreeTimeOut_GoNext::getBuySectionUseStarOrMoneyString(int starNum,int money)
{
	char temp[1024];
	sprintf(temp,LFStrings::getValue("buyNextPlayTime").c_str(),money,starNum);
	return string(temp);
}
string LFAlert_FreeTimeOut_GoNext::getBuySectionUseMoneyString(int money)
{
	char temp[1024];
	sprintf(temp,LFStrings::getValue("buyNextTimeLimite").c_str(),money);
	return string(temp);
}


//////////////////////////////////////////////////////////////////////////
// LFAlert_WaitTimeOver
//////////////////////////////////////////////////////////////////////////
LFAlert_WaitTimeOver::LFAlert_WaitTimeOver(int seasonId,int sectionId)
{
	mSeasonId = seasonId;
	mSectionId = sectionId;
}
// 在玩关卡之前是否需要提示没有次数了
bool LFAlert_WaitTimeOver::isTimeToShowBeforePlay(int seasonId,int sectionId)
{
	// 1 星星不够,等待时间没到
	//if (GameInforEditer::getGameInfor()->getTotalFreeScore() < GameInforEditer::getGameInfor()->getStarCostForFirstPlay(seasonId,sectionId) &&
	//	GameInforEditer::getGameInfor()->getTimeToWaitForFirstPlay(seasonId,sectionId) > 0)
	//{
	//	return true;
	//}
	return false;
}
LFAlert_WaitTimeOver* LFAlert_WaitTimeOver::showAlert(int seasonId,int sectionId)
{
	LFAlert_WaitTimeOver* temp = new LFAlert_WaitTimeOver(seasonId,sectionId);
	temp->prepareComplete(true);
	return temp;
}
string LFAlert_WaitTimeOver::getInforString()
{
	char temp[512];
	int starNeed = GameInforEditer::getGameInfor()->getStarCostForFirstPlay(mSeasonId,mSectionId);
	int currentStar = GameInforEditer::getGameInfor()->getTotalFreeScore();
	sprintf(temp,LFStrings::getValue("xingxingbugou_this").c_str(),currentStar,starNeed);

	return string(temp);
}

void LFAlert_WaitTimeOver::updateTimeDisplay(CCNode* lable)
{
	mTime --;
	char temp[256];
	sprintf(temp,"%d%s",mTime,LFStrings::getValue("miao").c_str());
	((CCLabelTTF*)lable)->setString(temp);
	if (mTime <= 0)
	{
		lable->stopAllActions();
	}
}

void LFAlert_WaitTimeOver::buyStarClicked(CCObject* sender)
{
	CCLog("%s",__FUNCTION__);
	mAlertNode->removeFromParent(); 

	LFAlert_Store::showAlert(false);
}

LFAlert* LFAlert_WaitTimeOver::alertNodeFactory()
{
	CCSprite* base = CCSprite::create(ResManager::getManager()->getSharedFilePath("messagebox_bg.png").c_str());
	CCMenu* menu = CCMenu::create();
	menu->setPosition(CCPointZero);
	char temp[1024];

	CCLabelTTF* buyString = CCLabelTTF::create(getInforString().c_str(),fontStr_katong,27,CCSizeMake(370,0),kCCTextAlignmentCenter);
	buyString->setAnchorPoint(ccp(0.5f,0.5f));
	base->addChild(buyString);
	buyString->setPosition(ccp(240,290));
	buyString->setColor(ccc3(157,107,0));

	mTime = GameInforEditer::getGameInfor()->getTimeToWaitForFirstPlay(mSeasonId,mSectionId);
	sprintf(temp,"%d%s",mTime,LFStrings::getValue("miao").c_str());
	mTimeLable = CCLabelTTF::create(temp,fontStr_katong,25,CCSizeMake(0,0),kCCTextAlignmentLeft);
	mTimeLable->setAnchorPoint(ccp(0.5f,0.5f));
	base->addChild(mTimeLable);
	mTimeLable->setPosition(ccp(buyString->getPositionX(),230));
	mTimeLable->setColor(ccc3(157,107,0));

	mTimeLable->runAction(CCRepeat::create(CCSequence::create(
		CCDelayTime::create(1.0f),
		CCCallFuncN::create(this,SEL_CallFuncN(&LFAlert_WaitTimeOver::updateTimeDisplay)),
		NULL
		),-1));

	
	CCMenuItem* item = creatMenuItem(this,SEL_MenuHandler(&LFAlert_WaitTimeOver::buyStarClicked),LFStrings::getValue("buyStar"));
	menu->addChild(item);
	item->setPosition(ccp(buyString->getPositionX(),103));

	LFAlert* alert = new LFAlert();
	alert->setBaseContainer(base,menu);
	alert->setAutoDismiss(MSG_WAIT_TIME_FRAME_AUTO_DISSMISS,true);
	alert->show();
	alert->release();

	addAutoRemoveObserver(this,SEL_CallFuncO(&LFAlert_WaitTimeOver::alertClosed),MSG_WAIT_TIME_FRAME_AUTO_DISSMISS);

	return alert;
}


//////////////////////////////////////////////////////////////////////////
// LFAlert_WaitTimeOver_GoNext
//////////////////////////////////////////////////////////////////////////
LFAlert_WaitTimeOver_GoNext::LFAlert_WaitTimeOver_GoNext(int seasonId,int sectionId)
	:LFAlert_WaitTimeOver(seasonId,sectionId)
{

}
string LFAlert_WaitTimeOver_GoNext::getInforString()
{
	char temp[512];
	int starNeed = GameInforEditer::getGameInfor()->getStarCostForFirstPlay(mSeasonId,mSectionId);
	int currentStar = GameInforEditer::getGameInfor()->getTotalFreeScore();
	sprintf(temp,LFStrings::getValue("xingxingbugou_next").c_str(),currentStar,starNeed);

	return string(temp);
}
LFAlert_WaitTimeOver_GoNext* LFAlert_WaitTimeOver_GoNext::showAlert(int seasonId,int sectionId)
{
	LFAlert_WaitTimeOver_GoNext* temp = new LFAlert_WaitTimeOver_GoNext(seasonId,sectionId);
	temp->prepareComplete(true);
	return temp;
}


//////////////////////////////////////////////////////////////////////////
// LFAlert_Store
//////////////////////////////////////////////////////////////////////////
LFAlert_Store::LFAlert_Store(bool showBuyGame)
{
	mShowBuyGame = showBuyGame;
	//mShowBuyGame = true;
}
void LFAlert_Store::buyStarClicked(CCObject* sender)
{
	CCMenuItem* item = (CCMenuItem*)sender;
	CCLog("%s",__FUNCTION__);

	// 保存
	buyStarOrByGame = 0;
	starNumToBuy = item->getTag();
	
	// 先选择支付方式
	LFAlerts_ChoosePayWay::showAlert(true);
	// 不移除
	//mAlertNode->removeFromParent();
}
void LFAlert_Store::buyAllSeasonClicked(CCObject* sender)
{
	buyStarOrByGame = 1;
	// 先选择支付方式
	LFAlerts_ChoosePayWay::showAlert(true);
	//PaymentFactory::getFactory()->getCurrentPaymentManager(true)->buySeasonAndItsSections(-1);
	CCLog("%s",__FUNCTION__);
}
LFAlert_Store* LFAlert_Store::showAlert(bool showBuyGame)
{
	LFAlert_Store* temp = new LFAlert_Store(showBuyGame);
	temp->prepareComplete(true);
	return temp;
}

LFAlert* LFAlert_Store::alertNodeFactory()
{
	CCSprite* base = CCSprite::create(ResManager::getManager()->getSharedFilePath("messagebox_bg.png").c_str());
	CCMenu* menu = CCMenu::create();
	menu->setPosition(CCPointZero);
	char temp[1024];
	CCLabelTTF* infor = CCLabelTTF::create(LFStrings::getValue("storeInfor").c_str(),fontStr_katong,27,CCSizeMake(370,0),kCCTextAlignmentLeft);
	infor->setAnchorPoint(ccp(0.5f,0.5f));
	base->addChild(infor);
	infor->setPosition(ccp(273,350));
	infor->setColor(ccc3(157,107,0));

	float lable_pos_x = 56;
	float menu_item_pos_x = (base->getContentSize().width - 120);
	float pos_y = infor->getPositionY()-120;
	float y_distance = (mShowBuyGame?70:85);
	ccColor3B color = ccc3(157,107,0);
	// 购买X份
	{
		int buy_num[] = {10,25};
		float buy_cost[] = {
			BUY_STAR_10_COST,
			BUY_STAR_25_COST};

		for (int i=0,count=sizeof(buy_num)/sizeof(int); i<count; i++)
		{
			sprintf(temp,LFStrings::getValue("buystar_x").c_str(),buy_num[i]);
			CCLabelTTF* useStarBuy = CCLabelTTF::create(temp,fontStr_katong,22,CCSizeMake(360,0),kCCTextAlignmentLeft);
			useStarBuy->setAnchorPoint(ccp(0.0f,0.5f));
			base->addChild(useStarBuy);
			useStarBuy->setPosition(ccp(lable_pos_x,pos_y));
			useStarBuy->setColor(color);
			sprintf(temp,LFStrings::getValue("d_yuan").c_str(),(int)buy_cost[i]);

			CCMenuItem* item = creatMenuItem(this,SEL_MenuHandler(&LFAlert_Store::buyStarClicked),temp);
			item->setPosition(ccp(menu_item_pos_x,pos_y));
			menu->addChild(item);
			item->setTag(buy_num[i]);
			pos_y -= y_distance;
		}
	}

	// 购买整个游戏
	if(mShowBuyGame)
	{
		CCLabelTTF* useStarBuy = CCLabelTTF::create(LFStrings::getValue("buygame").c_str(),
			fontStr_katong,22,CCSizeMake(360,0),kCCTextAlignmentLeft);
		useStarBuy->setAnchorPoint(ccp(0.0f,0.5f));
		base->addChild(useStarBuy);
		useStarBuy->setPosition(ccp(lable_pos_x,pos_y));
		useStarBuy->setColor(color);

		sprintf(temp,LFStrings::getValue("d_yuan").c_str(),BUY_ALLSEASON_COST);
		CCMenuItem* item = creatMenuItem(this,SEL_MenuHandler(&LFAlert_Store::buyAllSeasonClicked),temp);
		item->setPosition(ccp(menu_item_pos_x,pos_y));
		menu->addChild(item);
		pos_y -= y_distance;
	}

	LFAlert* alert = new LFAlert();
	alert->setBaseContainer(base,menu);
	alert->setAutoDismiss(MSG_STORE_FRAME_AUTO_DISSMISS,true);
	alert->show();
	alert->release();

	addAutoRemoveObserver(this,SEL_CallFuncO(&LFAlert_Store::alertClosed),MSG_STORE_FRAME_AUTO_DISSMISS);

	return alert;
}


//////////////////////////////////////////////////////////////////////////
// LFAlert_Store
//////////////////////////////////////////////////////////////////////////
LFAlert_BuyGame::LFAlert_BuyGame():LFAlert_Store(true)
{
}
LFAlert_BuyGame* LFAlert_BuyGame::showAlert()
{
	LFAlert_BuyGame* temp = new LFAlert_BuyGame();
	temp->prepareComplete(true);
	return temp;
}
CCMenuItem* LFAlert_BuyGame::creatMenuItem(CCObject* target, SEL_MenuHandler selector,string lable)
{
	CCMenuItemImage* menuItem = CCMenuItemImage::create(
		ResManager::getManager()->getSharedFilePath("goumaiyouxi_anniu.png").c_str(),
		ResManager::getManager()->getSharedFilePath("goumaiyouxi_anniu_select.png").c_str(),
		target,selector);
	CCLabelTTF* _lable = CCLabelTTF::create(lable.c_str(),fontStr_katong,30,CCSizeMake(340,0),kCCTextAlignmentCenter);
	menuItem->addChild(_lable,0,TAG_LABLE_IN_MENUITEM);
	_lable->setAnchorPoint(ccp(0.5f,0.5f));
	_lable->setPosition(ccp(menuItem->getContentSize().width/2,menuItem->getContentSize().height/2));
	return menuItem;
}
LFAlert* LFAlert_BuyGame::alertNodeFactory()
{
	CCSprite* base = CCSprite::create(ResManager::getManager()->getSharedFilePath("goumaiyouxi.png").c_str());
	CCMenu* menu = CCMenu::create();
	menu->setPosition(CCPointZero);
	char temp[1024];
	//CCLabelTTF* infor = CCLabelTTF::create(LFStrings::getValue("storeInfor").c_str(),fontStr_katong,27,CCSizeMake(370,0),kCCTextAlignmentLeft);
	//infor->setAnchorPoint(ccp(0.5f,0.5f));
	//base->addChild(infor);
	//infor->setPosition(ccp(273,350));
	//infor->setColor(ccc3(157,107,0));

	float lable_pos_x = 56;
	float menu_item_pos_x = (base->getContentSize().width - 120);
	//float pos_y = infor->getPositionY()-120;
	float pos_y = 230;
	float y_distance = (mShowBuyGame?70:85);
	ccColor3B color = ccc3(157,107,0);
	// 购买整个游戏
	if(mShowBuyGame)
	{
		sprintf(temp,LFStrings::getValue("d_yuan").c_str(),BUY_ALLSEASON_COST);
		CCMenuItem* item = creatMenuItem(this,SEL_MenuHandler(&LFAlert_BuyGame::buyAllSeasonClicked),temp);
		item->setPosition(ccp(210,60));
		menu->addChild(item);
		pos_y -= y_distance;
	}

	LFAlert* alert = new LFAlert();
	alert->setBaseContainer(base,menu);
	alert->setAutoDismiss(MSG_BUYGAME_FRAME_AUTO_DISSMISS,true);
	alert->show();
	alert->release();

	addAutoRemoveObserver(this,SEL_CallFuncO(&LFAlert_BuyGame::alertClosed),MSG_BUYGAME_FRAME_AUTO_DISSMISS);

	return alert;
}







//////////////////////////////////////////////////////////////////////////
// LFAlert_BuySeasonAndItsSections
//////////////////////////////////////////////////////////////////////////
LFAlert_BuySeasonAndItsSections::LFAlert_BuySeasonAndItsSections(int seasonId)
{
	this->seasonId = seasonId;
}
LFAlert_BuySeasonAndItsSections* LFAlert_BuySeasonAndItsSections::showAlert(int seasonId)
{
	LFAlert_BuySeasonAndItsSections* ret = new LFAlert_BuySeasonAndItsSections(seasonId);
	ret->prepareComplete(true);
	return ret;
}
void LFAlert_BuySeasonAndItsSections::buyAllSeasonClicked(CCObject* sender)
{
	CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(30002);

#if ((CC_TARGET_PLATFORM == CC_PLATFORM_IOS && Version_Platform == 0) || DEBUG_IOSMarket)
	IOSStoreLayer *layer = IOSStoreLayer::create();
	layer->setComefrom(from_SeasonSelector);
	CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 200);
#else
	MarketLayer* m = new MarketLayer(MS_SpecialItem);
	CCDirector::sharedDirector()->getRunningScene()->addChild(m,1024, marketlayer_tag);
	m->release();
#endif
	//buyAllSeason = true;
	//LFAlerts_ChoosePayWay::showAlert(true);

	//PaymentFactory::getFactory()->getCurrentPaymentManager(true)->buySeasonAndItsSections(-1);
	// 花钱的都不应该移除
	//mAlertNode->removeFromParent();
}
void LFAlert_BuySeasonAndItsSections::buyOneSeasonClicked(CCObject* sender)
{
	buyAllSeason = false;
	LFAlerts_ChoosePayWay::showAlert(true);

	//PaymentFactory::getFactory()->getCurrentPaymentManager(true)->buySeasonAndItsSections(seasonId);
	// 花钱的都不应该移除
	//mAlertNode->removeFromParent();
}

LFAlert* LFAlert_BuySeasonAndItsSections::alertNodeFactory()
{
	CCSprite* base = CCSprite::create(ResManager::getManager()->getSharedFilePath("messagebox_bg.png").c_str());

	CCMenuItemImage* buyAllItem = CCMenuItemImage::create(
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu.png").c_str(),
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu_select.png").c_str(),
		this,SEL_MenuHandler(&LFAlert_BuySeasonAndItsSections::buyAllSeasonClicked)
		);
	CCMenuItemImage* buyThisSeasonItem = CCMenuItemImage::create(
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu.png").c_str(),
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu_select.png").c_str(),
		this,SEL_MenuHandler(&LFAlert_BuySeasonAndItsSections::buyOneSeasonClicked)
		);


	char temp[1024];
	sprintf(temp,LFStrings::getValue("buy_all").c_str(),BUY_ALLSEASON_COST,"RMB");

	bool hasAll = true;
	//屏蔽剧情道具相关
/*
	vector<JuqingDaoJu> vec_ = JuqingDaoJuManager::getManager()->getAllItemsInSeason(GameInforEditer::getGameInfor()->getLastPlayedSeasonId());
	for (int i=0; i<vec_.size(); ++i)
	{
		if (vec_.at(i).state == DJS_NotGetted)
		{
			hasAll = false;
			break;
		}
	}
*/
	CCLabelTTF* buyAll;
	if (hasAll)
	{
		buyAll = CCLabelTTF::create(LFStrings::getValue("unlock_card_ALL").c_str(),fontStr_katong,26,CCSizeMake(360,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
		buyAllItem->setVisible(false);
	}
	else 
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS && Version_Platform == 0)
		buyAll = CCLabelTTF::create(LFStrings::getValue("OpenBox_SeasonSelector").c_str(),fontStr_katong,26,CCSizeMake(360,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
#else
		buyAll = CCLabelTTF::create(LFStrings::getValue("unlock_card_android").c_str(),fontStr_katong,26,CCSizeMake(360,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
#endif
	}
	buyAll->setAnchorPoint(ccp(0.5f,0.5f));
	base->addChild(buyAll);
	buyAll->setPosition(ccp(240,140+buyAll->getContentSize().height/2));
	buyAll->setColor(ccc3(157,107,0));

	sprintf(temp,LFStrings::getValue("buy_this").c_str(),BUY_ONESEASON_COST,"RMB");
	CCLabelTTF* buyThis = CCLabelTTF::create(temp,fontDefault,26,CCSizeMake(360,0),kCCTextAlignmentCenter);
	buyThis->setAnchorPoint(ccp(0.5f,0.5f));
	base->addChild(buyThis);
	buyThis->setPosition(ccp(240,320+buyThis->getContentSize().height/2));
	buyThis->setColor(ccc3(157,107,0));

	CCLabelTTF* buy = CCLabelTTF::create(LFStrings::getValue("buy").c_str(),fontStr_katong,45,CCSizeMake(360,0),kCCTextAlignmentCenter);
	if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS && Version_Platform == 0)
	{
		buy = CCLabelTTF::create(LFStrings::getValue("OpenBox").c_str(),fontStr_katong,45,CCSizeMake(360,0),kCCTextAlignmentCenter);
	}
	buy->setAnchorPoint(ccp(0.5f,0.5f));
	buyThisSeasonItem->addChild(buy);
	buy->setPosition(ccp(buyThisSeasonItem->getContentSize().width/2,buyThisSeasonItem->getContentSize().height/2));
	buy->setColor(ccc3(255,255,255));

	buy = CCLabelTTF::create(LFStrings::getValue("buy").c_str(),fontStr_katong,45,CCSizeMake(340,0),kCCTextAlignmentCenter);
	if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS && Version_Platform == 0)
	{
		buy = CCLabelTTF::create(LFStrings::getValue("OpenBox").c_str(),fontStr_katong,45,CCSizeMake(340,0),kCCTextAlignmentCenter);
	}
	buy->setAnchorPoint(ccp(0.5f,0.5f));
	buyAllItem->addChild(buy);
	buy->setPosition(ccp(buyAllItem->getContentSize().width/2,buyAllItem->getContentSize().height/2));
	buy->setColor(ccc3(255,255,255));

	CCMenu* menu = CCMenu::create(buyAllItem,buyThisSeasonItem,NULL);

	buyAllItem->setPosition(ccp(240,90));
	buyThisSeasonItem->setPosition(ccp(240,270));

#if Version_Platform == 9
    buyAll->setString(LFStrings::getValue("unlock_card_android_One").c_str());
    buy->setString("");
    menu->setVisible(false);
#endif
    
    

#if Not_Need_Buy_Season == 1
	buyThis->removeFromParent();
	buyThisSeasonItem->removeFromParent();

	buyAllItem->setPositionY(164+10);
	buyAll->setPositionY(290-30);
#endif

	LFAlert* alert = new LFAlert();
	alert->setBaseContainer(base,menu);
	alert->setAutoDismiss(MSG_BUY_SEASON_FRAME_AUTO_DISSMISS,true);
	alert->show();
	alert->setTag(30002);
	alert->release();

	return alert;
}


//////////////////////////////////////////////////////////////////////////
// LFAlert_BuySolution
//////////////////////////////////////////////////////////////////////////
LFAlert_BuySolution::LFAlert_BuySolution()
{

}
// Sender是CCmenuItem，getTag就是对应的购买的次数
void LFAlert_BuySolution::buySolutionClicked(CCObject* sender)
{
	CCMenuItem* item = (CCMenuItem*)sender;
	numToBuy = item->getTag();
	// 不移除
	//mAlertNode->removeFromParent();
}

void LFAlert_BuySolution::buyOneUseStar(CCObject* sender)
{
	CCLog("%s",__FUNCTION__);
	if (GameInforEditer::getGameInfor()->getTotalFreeScore() < BUY_SOLUTION_1_COST_Star)
	{
		LFToast::makeToast("Your star is not enough!");
		return;
	}

	GameInforEditer::getGameInfor()->subMyStarNum(BUY_SOLUTION_1_COST_Star);
	GameInforEditer::getGameInfor()->addHelpNum(1);

	CCNotificationCenter::sharedNotificationCenter()->postNotification(BUY_ONE_HELP_TIME_USESTAR_SUC,mAlertNode);
	//然后
	mAlertNode->setVisible(false);
	setEnbale(false);
	mAlertNode->removeFromParent();
}
LFAlert_BuySolution* LFAlert_BuySolution::showAlert()
{
	LFAlert_BuySolution* ret = new LFAlert_BuySolution();
	ret->prepareComplete(true);
	return ret;
}
LFAlert* LFAlert_BuySolution::alertNodeFactory()
{
	CCSprite* base = CCSprite::create(ResManager::getManager()->getSharedFilePath("messagebox_bg.png").c_str());
	CCMenu* menu = CCMenu::create();
	menu->setPosition(CCPointZero);
	char temp[1024];
	CCLabelTTF* infor = CCLabelTTF::create(LFStrings::getValue("buySolutionInfor").c_str(),fontStr_katong,25,CCSizeMake(500,0),kCCTextAlignmentLeft);
	infor->setAnchorPoint(ccp(0.5f,0.5f));
	base->addChild(infor);
	infor->setPosition(ccp(331,530));
	infor->setColor(ccc3(157,107,0));


	float lable_pos_x = 140;
	float menu_item_pos_x = base->getContentSize().width - 180;
	float pos_y = infor->getPositionY()-infor->getContentSize().height-20;;
	float y_distance = 70;
	ccColor3B color = ccc3(157,107,0);
	{
		{
			CCLabelTTF* useStarBuy = CCLabelTTF::create(LFStrings::getValue("buySolutionUseStar").c_str(),fontStr_katong,20,CCSizeMake(360,0),kCCTextAlignmentLeft);
			useStarBuy->setAnchorPoint(ccp(0.0f,0.5f));
			base->addChild(useStarBuy);
			useStarBuy->setPosition(ccp(lable_pos_x,pos_y));
			useStarBuy->setColor(color);

			sprintf(temp,LFStrings::getValue("d_star").c_str(),BUY_SOLUTION_1_COST_Star);
			CCMenuItem* item = creatMenuItem(this,SEL_MenuHandler(&LFAlert_BuySolution::buyOneUseStar),temp);
			item->setPosition(ccp(menu_item_pos_x,pos_y));
			menu->addChild(item);

			if (GameInforEditer::getGameInfor()->getTotalFreeScore() < BUY_SOLUTION_1_COST_Star)
			{
				CCLabelTTF* starNotEnough = CCLabelTTF::create(LFStrings::getValue("starNotEnough").c_str(),fontStr_katong,20,CCSizeMake(360,0),kCCTextAlignmentLeft);
				starNotEnough->setAnchorPoint(ccp(0.0f,0.5f));
				base->addChild(starNotEnough);
				starNotEnough->setPosition(ccp(useStarBuy->getPositionX() + useStarBuy->getContentSize().width,pos_y));
				starNotEnough->setColor(ccRED);
				((CCLabelTTF*)item->getChildByTag(TAG_LABLE_IN_MENUITEM))->setColor(ccRED);
			}

			pos_y -= y_distance;
		}
		// 购买X份
		{
			int buy_num[] = {1,2,4,12,25};
			float buy_cost[] = {
				BUY_SOLUTION_1_COST,
				BUY_SOLUTION_2_COST,
				BUY_SOLUTION_4_COST,
				BUY_SOLUTION_12_COST,
				BUY_SOLUTION_25_COST};
			bool is_float[] = {true,false,false,false,false};

			for (int i=0,count=sizeof(buy_num)/sizeof(int); i<count; i++)
			{
				sprintf(temp,LFStrings::getValue("buySolution_x").c_str(),buy_num[i]);
				CCLabelTTF* useStarBuy = CCLabelTTF::create(temp,fontStr_katong,20,CCSizeMake(360,0),kCCTextAlignmentLeft);
				useStarBuy->setAnchorPoint(ccp(0.0f,0.5f));
				base->addChild(useStarBuy);
				useStarBuy->setPosition(ccp(lable_pos_x,pos_y));
				useStarBuy->setColor(color);
				if(is_float[i])
				{
					sprintf(temp,LFStrings::getValue("f_yuan").c_str(),buy_cost[i]);
				}else
				{
					sprintf(temp,LFStrings::getValue("d_yuan").c_str(),(int)buy_cost[i]);
				}
				
				CCMenuItem* item = creatMenuItem(this,SEL_MenuHandler(&LFAlert_BuySolution::buySolutionClicked),temp);
				item->setPosition(ccp(menu_item_pos_x,pos_y));
				menu->addChild(item);
				item->setTag(buy_num[i]);
				pos_y -= y_distance;
			}
		}
	}

	LFAlert* alert = new LFAlert();
	alert->setBaseContainer(base,menu);
	alert->setAutoDismiss(MSG_BUY_HELP_FRAME_AUTO_DISSMISS,true);
	alert->show();
	alert->release();

	addAutoRemoveObserver(this,SEL_CallFuncO(&LFAlert_BuySolution::alertClosed),MSG_BUY_HELP_FRAME_AUTO_DISSMISS);

	return alert;
}



LFAlert_BuySuperTool::LFAlert_BuySuperTool()
{

}
// Sender是CCmenuItem，getTag就是对应的购买的个数
void LFAlert_BuySuperTool::buySuperToolClicked(CCObject* sender)
{
//	addAutoRemoveObserver(this,SEL_CallFuncO(&LFAlert_BuySuperTool::buySuperToolRsp),MSG_PP_SuperToolRsp);
	CCMenuItem* item = (CCMenuItem*)sender;
	buyNum = item->getTag();
	LFAlerts_ChoosePayWay::showAlert(true);
	// 不移除
	//mAlertNode->removeFromParent();
}

void LFAlert_BuySuperTool::buySuperToolRsp(CCObject* payRspWrapper)
{
// 	PPCmd_BuySuperToolRsp* rsp = (PPCmd_BuySuperToolRsp*)(((PayRspWrapper*)payRspWrapper)->data);
// 	if (rsp->rspCode == PP_Success)
// 	{
// 		if (GameInforEditer::getGameInfor()->getSuperToolCount(ST_SuperTool) <= 0)
// 			((CCLabelTTF*)useItem->getChildByTag(TAG_LABLE_IN_MENUITEM))->setColor(ccRED);
// 		else
// 			((CCLabelTTF*)useItem->getChildByTag(TAG_LABLE_IN_MENUITEM))->setColor(ccWHITE);
// 	}
}
void LFAlert_BuySuperTool::useSuperToolClicked(CCObject* sender)
{
	CCScene* currentScene = CCDirector::sharedDirector()->getRunningScene();
	CCPoint pos = currentScene->convertToNodeSpace(((CCMenuItem*)sender)->getPosition());

	if (BaseContentLayer::shareBaseContentLayer()->isUsingSuperTool())
	{
		LFToast::makeToast(LFStrings::getValue("superToolIsUsing"),ccp(0,pos.y));
		return;
	}

//	if (GameInforEditer::getGameInfor()->getSuperToolCount(ST_SuperTool) <= 0)
	{
		LFToast::makeToast(LFStrings::getValue("superToolIsNotEnough"),ccp(0,pos.y));
		return;
	}

	LFToast::makeToast(LFStrings::getValue("clickScreenToUse"),ccp(0,pos.y+50),2.0f);

	// 扣个数
//	GameInforEditer::getGameInfor()->subSuperTool(ST_SuperTool,1);
	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USE_SUPERTOOL,mAlertNode);

	mAlertNode->removeFromParent();
}

LFAlert_BuySuperTool* LFAlert_BuySuperTool::showAlert()
{
	LFAlert_BuySuperTool* ret = new LFAlert_BuySuperTool();
	ret->prepareComplete(true);
	return ret;
}
LFAlert* LFAlert_BuySuperTool::alertNodeFactory()
{
	CCSprite* base = CCSprite::create(ResManager::getManager()->getSharedFilePath("messagebox_bg.png").c_str());
	CCMenu* menu = CCMenu::create();
	menu->setPosition(CCPointZero);
	char temp[1024];
	CCLabelTTF* infor = CCLabelTTF::create(LFStrings::getValue("superToolInfor").c_str(),fontStr_katong,25,CCSizeMake(500,0),kCCTextAlignmentLeft);
	infor->setAnchorPoint(ccp(0.5f,0.5f));
	base->addChild(infor);
	infor->setPosition(ccp(331,530));
	infor->setColor(ccc3(157,107,0));

	useItem = creatMenuItem(this,SEL_MenuHandler(&LFAlert_BuySuperTool::useSuperToolClicked),LFStrings::getValue("useOne"));
	menu->addChild(useItem);
	useItem->setPosition(ccp(331,infor->getPositionY()-80));
//	if (GameInforEditer::getGameInfor()->getSuperToolCount(ST_SuperTool) <= 0)
		((CCLabelTTF*)useItem->getChildByTag(TAG_LABLE_IN_MENUITEM))->setColor(ccRED);

	float lable_pos_x = 140;
	float menu_item_pos_x = base->getContentSize().width - 180;
	float pos_y = useItem->getPositionY()-useItem->getContentSize().height-20;
	float y_distance = 70;
	ccColor3B color = ccc3(157,107,0);
	// 购买X份
	{
		//int buy_num[] = {1,2,6,14,30};
		//float buy_cost[] = {
		//	BUY_SUPERTOOL_1_COST,
		//	BUY_SUPERTOOL_2_COST,
		//	BUY_SUPERTOOL_6_COST,
		//	BUY_SUPERTOOL_14_COST,
		//	BUY_SUPERTOOL_30_COST};

		int buy_num[] = {1,2,4};
		float buy_cost[] = {
			BUY_SUPERTOOL_2_COST,
			BUY_SUPERTOOL_6_COST,
			BUY_SUPERTOOL_14_COST};

		for (int i=0,count=sizeof(buy_num)/sizeof(int); i<count; i++)
		{
			sprintf(temp,LFStrings::getValue("buytool_x").c_str(),buy_num[i]);
			CCLabelTTF* useStarBuy = CCLabelTTF::create(temp,fontStr_katong,20,CCSizeMake(360,0),kCCTextAlignmentLeft);
			useStarBuy->setAnchorPoint(ccp(0.0f,0.5f));
			base->addChild(useStarBuy);
			useStarBuy->setPosition(ccp(lable_pos_x,pos_y));
			useStarBuy->setColor(color);
			sprintf(temp,LFStrings::getValue("d_yuan").c_str(),(int)buy_cost[i]);

			CCMenuItem* item = creatMenuItem(this,SEL_MenuHandler(&LFAlert_BuySuperTool::buySuperToolClicked),temp);
			item->setPosition(ccp(menu_item_pos_x,pos_y));
			menu->addChild(item);
			item->setTag(buy_num[i]);
			pos_y -= y_distance;
		}
	}

	LFAlert* alert = new LFAlert();
	alert->setBaseContainer(base,menu);
	alert->setAutoDismiss(MSG_BUY_TOOL_FRAME_AUTO_DISSMISS,true);
	alert->show();
	alert->release();

	addAutoRemoveObserver(this,SEL_CallFuncO(&LFAlert_BuySuperTool::alertClosed),MSG_BUY_TOOL_FRAME_AUTO_DISSMISS);

	return alert;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
LFAlerts_ChoosePayWay::LFAlerts_ChoosePayWay(bool removeWhenChoosed)
{
	mRemoveWhenChoosed = removeWhenChoosed;
	menu = NULL;
}
LFAlerts_ChoosePayWay* LFAlerts_ChoosePayWay::showAlert(bool removeWhenChoosed)
{
	LFAlerts_ChoosePayWay* ret = new LFAlerts_ChoosePayWay(removeWhenChoosed);
	ret->prepareComplete(true);

// #if FixedPaymentTag == Choose_PaymentTag // 选择支付方式
// #else
// 	IPaymentManager* pay = PaymentFactory::getFactory()->getCurrentPaymentManager(false);
// 	CCAssert(pay != NULL,"");
// 	int configPayTag = pay->getPaymentTag();
// 	ret->chooseClicked(ret->menu->getChildByTag(configPayTag));
// #endif
	return ret;
}
void LFAlerts_ChoosePayWay::chooseClicked(CCObject* sender)
{
// 	CCMenuItem* item = (CCMenuItem*)sender;
// 
// 	ChooseInfor infor((PaymentTag)item->getTag());
// 	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHOOSE_PAYWAY,&infor);
// 	if (mRemoveWhenChoosed && mAlertNode != NULL)
// 	{
// 		mAlertNode->removeFromParent();
// 	}

}
LFAlert* LFAlerts_ChoosePayWay::alertNodeFactory()
{
	CCSprite* base = CCSprite::create(ResManager::getManager()->getSharedFilePath("messagebox_bg.png").c_str());
	menu = CCMenu::create();
	menu->setPosition(CCPointZero);
	CCLabelTTF* infor = CCLabelTTF::create(LFStrings::getValue("chossPayway").c_str(),fontStr_katong,25,CCSizeMake(500,0),kCCTextAlignmentLeft);
	infor->setAnchorPoint(ccp(0.5f,0.5f));
	base->addChild(infor);
	infor->setPosition(ccp(331,530));
	infor->setColor(ccc3(157,107,0));

	float lable_pos_x = 320;
	float pos_y = infor->getPositionY()-infor->getContentSize().height-40;
	float y_distance = 90;
	ccColor3B color = ccc3(157,107,0);

	string lable[] = {
		LFStrings::getValue("youxijidi"),
		LFStrings::getValue("liantongduanxin"),
		LFStrings::getValue("dianxinduanxin"),
		LFStrings::getValue("yidongduanxin"),
		LFStrings::getValue("tianyikongjian"),
	};
// 	PaymentTag tag[] = {
// 		PT_YOUXIJIDI_SDK,
// 		PT_UNISMSPAY_SDK,
// 		PT_EGameSMSOffLine,
// 		PT_MMSMSBillingSDK,
// 		PT_CTEStoreSDKLocalSms,
// 	};

// 	for (int i=0;i<sizeof(tag)/sizeof(PaymentTag);i++)
// 	{
// 		CCMenuItem* item = creatMenuItem(this,SEL_CallFuncO(&LFAlerts_ChoosePayWay::chooseClicked),lable[i]);
// 		item->setTag(tag[i]);
// 		item->setPosition(ccp(lable_pos_x,pos_y));
// 		pos_y -= y_distance;
// 		menu->addChild(item);
// 	}

	
#define MSG_DISMMISE	"MSG_DISMMISE"

	LFAlert* alert = new LFAlert(true,Default_Priority-1);
	alert->setBaseContainer(base,menu);
	alert->setAutoDismiss(MSG_DISMMISE,true);
	alert->show(1030);
	alert->release();

	addAutoRemoveObserver(this,SEL_CallFuncO(&LFAlerts_ChoosePayWay::alertClosed),MSG_DISMMISE);

	return alert;
}

//////////////////////////////////////////////////////////////////////////
// LFAlert_BuyUnLockSeason
//////////////////////////////////////////////////////////////////////////
LFAlerts_UnLockSeason::LFAlerts_UnLockSeason(int index)
{
	seasonIndex = index;
	price = 6;
	//
//	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,	callfuncO_selector(LFAlerts_UnLockSeason::buyUnLockSeasonRsp), MSG_PayEndRsp, NULL);
}
LFAlerts_UnLockSeason::~LFAlerts_UnLockSeason()
{
//	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_PayEndRsp);
}
// Sender是CCmenuItem，getTag就是对应的购买的次数
void LFAlerts_UnLockSeason::buyUnLockSeasonClicked(CCObject* sender)
{
	CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(30002);
	CCMenuItem* item = (CCMenuItem*)sender;
	int index = item->getTag();

	//监听购买返回的消息
	//	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
	//		callfuncO_selector(MarketContaintLayer::buyRsp), MSG_PayEndRsp, NULL);
	//弹出购买支付框
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS && Version_Platform == 0)
	if (GameInforEditer::getGameInfor()->getTotalFreeScore() >= price)
	{
		GameInforEditer::getGameInfor()->subMyStarNum(price);
		GameInforEditer::getGameInfor()->unLockSeason(seasonIndex);
	}
	else
	{
		IOSStoreLayer *layer = IOSStoreLayer::create();
		layer->setComefrom(from_MarketStore);
		CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 200);
	}
#endif
	// 不移除
	//mAlertNode->removeFromParent();
}

void LFAlerts_UnLockSeason::buyUnLockSeasonRsp(CCObject* payRspWrapper)
{
//	NetworkWaitingUI::hideWaiting();
//	GameInforEditer::getGameInfor()->unLockSeason(seasonIndex);
}
LFAlerts_UnLockSeason* LFAlerts_UnLockSeason::showAlert(int seasonIndex)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS && Version_Platform == 0) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && Version_Platform == 5) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	LFAlerts_UnLockSeason* ret = new LFAlerts_UnLockSeason(seasonIndex);
	ret->prepareComplete(true);
	return ret;
#else
//	GameInforEditer::getGameInfor()->unLockSeason(seasonIndex);
	return NULL;
#endif
}
LFAlert* LFAlerts_UnLockSeason::alertNodeFactory()
{
	
	CCSprite* base = CCSprite::create(ResManager::getManager()->getSharedFilePath("messagebox_bg.png").c_str());

	CCMenuItemImage* buyAllItem = CCMenuItemImage::create(
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu.png").c_str(),
		ResManager::getManager()->getSharedFilePath(g_buttonPath+"goumai_tanchukuang_anniu_select.png").c_str(),
		this,SEL_MenuHandler(&LFAlerts_UnLockSeason::buyUnLockSeasonClicked)
		);


	CCLabelTTF *buyAll = NULL;
	buyAll = CCLabelTTF::create(LFStrings::getValue("UnLockThisSeason").c_str(),fontStr_katong,26,CCSizeMake(360,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);

	buyAll->setAnchorPoint(ccp(0.5f,0.5f));
	base->addChild(buyAll);
	buyAll->setPosition(ccp(240,140+buyAll->getContentSize().height/2));
	buyAll->setColor(ccc3(157,107,0));

	

	CCLabelTTF* buy = CCLabelTTF::create(LFStrings::getValue("buy").c_str(),fontStr_katong,45,CCSizeMake(360,0),kCCTextAlignmentCenter);
	if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS && Version_Platform == 0)
	{
		buy = CCLabelTTF::create(LFStrings::getValue("OpenBox").c_str(),fontStr_katong,45,CCSizeMake(360,0),kCCTextAlignmentCenter);
	}
	buy->setAnchorPoint(ccp(0.5f,0.5f));
	buyAllItem->addChild(buy);
	buy->setPosition(ccp(buyAllItem->getContentSize().width/2,buyAllItem->getContentSize().height/2));
	buy->setColor(ccc3(255,255,255));

	CCMenu* menu = CCMenu::create(buyAllItem,NULL);

	buyAllItem->setPosition(ccp(240,90));

#if Not_Need_Buy_Season == 1
	
	buyAllItem->setPositionY(164+10);
	buyAll->setPositionY(290-30);
#endif

	LFAlert* alert = new LFAlert();
	alert->setBaseContainer(base,menu);
	alert->setAutoDismiss(MSG_BUY_SEASON_FRAME_AUTO_DISSMISS,true);
	alert->show();
	alert->setTag(30002);
	alert->release();

	return alert;
}
