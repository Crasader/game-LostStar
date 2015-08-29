#pragma once

#include "cocos2d.h"
#include <string>
#include <vector>

using namespace std;
using namespace cocos2d;

// 对话框消失的消息
#define MSG_BUY_SECTION_FRAME_AUTO_DISSMISS	"MSG_BUY_SSECTION_FRAME_AUTO_DISSMISS"
#define MSG_BUY_SEASON_FRAME_AUTO_DISSMISS	"MSG_BUY_SEASON_FRAME_AUTO_DISSMISS"
#define MSG_BUY_HELP_FRAME_AUTO_DISSMISS	"MSG_BUY_HELP_FRAME_AUTO_DISSMISS"
#define MSG_BUY_TOOL_FRAME_AUTO_DISSMISS	"MSG_BUY_TOOL_FRAME_AUTO_DISSMISS"
// 等待时间流逝的对话框消失的消息
#define MSG_WAIT_TIME_FRAME_AUTO_DISSMISS	"MSG_WAIT_TIME_FRAME_AUTO_DISSMISS"
// 商城关闭
#define MSG_STORE_FRAME_AUTO_DISSMISS		"MSG_STORE_FRAME_AUTO_DISSMISS"

// 购买游戏的弹框关闭
#define MSG_BUYGAME_FRAME_AUTO_DISSMISS		"MSG_BUYGAME_FRAME_AUTO_DISSMISS"


// 购买
#define BUY_ONE_PLAY_TIME_SUC				"BUY_ONE_PLAY_TIME_SUC"
#define BUY_ONE_HELP_TIME_USESTAR_SUC		"BUY_ONE_HELP_TIME_USESTAR_SUC"

// 其他消息
#define MSG_USE_SUPERTOOL					"MSG_USE_SUPERTOOL"

// 要移除对话框，发送这个消息即可
#define MSG_REMOVE_BUY_ALERTS				"MSG_REMOVE_BUY_ALERTS"


// 如果是付费的情况，那么付费成功与否需要监听PayManager的数据


struct ObserverItem
{
	CCObject *target;
	SEL_CallFuncO selector;
	string name;
	CCObject *obj;

	ObserverItem()
	{
		target = NULL;
		name = "";
		obj = NULL;
	};
	ObserverItem(CCObject *_target,SEL_CallFuncO _selector,string _name)
	{
		target = _target;
		_selector = _selector;
		name = _name;
		obj = NULL;
	}
};
class LFAlert;

#define TAG_LABLE_IN_MENUITEM	1234321

// alter的父类，他的生命周期跟LFAlert一致，是作为LFAlert的子类保存的
class BaseBuyAlert : public CCNode
{
protected:
	LFAlert* mAlertNode;
	vector<ObserverItem> mAutoRemovePool;

	void alertClosed(CCNode* alter);
	void removeAllObserver();
	// 准备完毕,
	// 当子类数据准备好了，然后可以窗口alert的时候调用
	// 如果调用了callAutoRelease，那么就会把BaseBuyAlert帮到LFAlert上面
	// 让他们生命周期一致，子类就不用管理BaseBuyAlert的release问题
	// 推荐true
	void prepareComplete(bool callAutoRelease);

	virtual void onExit();

	virtual void recivedRemoveAlertMsg(CCObject* not_use);

	// menu里面的Label的Tag是：TAG_LABLE_IN_MENUITEM
	virtual CCMenuItem* creatMenuItem(CCObject* target, SEL_MenuHandler selector,string lable);

	//void 
protected:
	// 子类实现：工厂方法
	virtual LFAlert* alertNodeFactory() = 0;
public:
	BaseBuyAlert();
	void setEnbale(bool enable);
	BaseBuyAlert* addAutoRemoveObserver(CCObject *_target,SEL_CallFuncO _selector,string _name);
	LFAlert* getLFAlert();

	static void removeAllBuyAlerts();
};

/**
 * 免费的玩的次数完毕了.但是还可以购买，用星星或者用钱
 */
class LFAlert_FreeTimeOut : public BaseBuyAlert
{
protected:
	int seasonId;
	int sectionId;
	LFAlert_FreeTimeOut(int seasonId,int sectionId);

	virtual void buyOneTime(CCObject* sender);
	virtual void buyTimeLimite(CCObject* sender);

protected:
	virtual string getBuySectionUseStarOrMoneyString(int starNum,int money);
	virtual string getBuySectionUseMoneyString(int money);
	virtual LFAlert* alertNodeFactory();
public:

	// 在玩关卡之前是否需要提示没有次数了
	static bool isTimeToShowBeforePlay(int seasonId,int sectionId);
	// 显示没有次数的对话框
	static LFAlert_FreeTimeOut* showAlert(int seasonId,int sectionId);
};

/**
 * 在BaseContentLayer里面，游戏当中，失败了就会自动刷新，那么这时需要弹出对话框提示
 * 这个对话框就是这个类
 */
class LFAlert_FreeTimeOut_InGameFaild : public LFAlert_FreeTimeOut
{
protected:
	LFAlert_FreeTimeOut_InGameFaild(int seasonId,int sectionId);
	virtual void goSectionSelected(CCObject* sender);
	virtual void goMainSelected(CCObject* sender);
protected:
	virtual LFAlert* alertNodeFactory();
public:
	static LFAlert_FreeTimeOut_InGameFaild* showAlert(int seasonId,int sectionId);
};

/**
 * 点击下一关，要计划进入下一关时，如果发现下一关没有次数了，那么就是这提示框
 */
class LFAlert_FreeTimeOut_GoNext : public LFAlert_FreeTimeOut
{
protected:
	LFAlert_FreeTimeOut_GoNext(int seasonId,int sectionId);
protected:
	virtual string getBuySectionUseStarOrMoneyString(int starNum,int money);
	virtual string getBuySectionUseMoneyString(int money);
public:
	static LFAlert_FreeTimeOut_GoNext* showAlert(int seasonId,int sectionId);
};


/**
 * 等待,没有星星自动支付了，那么就需要等待
 */
class LFAlert_WaitTimeOver : public BaseBuyAlert
{
protected:
	int mSeasonId;
	int mSectionId;

	CCLabelTTF* mTimeLable;
	int mTime;

	LFAlert_WaitTimeOver(int seasonId,int sectionId);
	virtual LFAlert* alertNodeFactory();

	virtual string getInforString();
	virtual void updateTimeDisplay(CCNode* lable);
protected:
	virtual void buyStarClicked(CCObject* sender);
public:
	// 在玩关卡之前是否需要提示没有次数了
	static bool isTimeToShowBeforePlay(int seasonId,int sectionId);
	static LFAlert_WaitTimeOver* showAlert(int seasonId,int sectionId);
};


class LFAlert_WaitTimeOver_GoNext : public LFAlert_WaitTimeOver
{
protected:
	LFAlert_WaitTimeOver_GoNext(int seasonId,int sectionId);
protected:
	virtual string getInforString();
public:
	static LFAlert_WaitTimeOver_GoNext* showAlert(int seasonId,int sectionId);
};


//////////////////////////////////////////////////////////////////////////
// 商城，买season+买星星
//////////////////////////////////////////////////////////////////////////
class LFAlert_Store : public BaseBuyAlert
{
protected: 
	int buyStarOrByGame; // buyStar = 0, buyGame = 1
	int starNumToBuy;


	bool mShowBuyGame;
	LFAlert_Store(bool showBuyGame);
	virtual LFAlert* alertNodeFactory();
protected:
	// tag就是购买的个数
	virtual void buyStarClicked(CCObject* sender);
	// 购买全部关卡
	virtual void buyAllSeasonClicked(CCObject* sender);

public:
	static LFAlert_Store* showAlert(bool showBuyGame);
};

// 用于2-1时提示购买整个游戏
class LFAlert_BuyGame : public LFAlert_Store
{
protected: 
	LFAlert_BuyGame();
	virtual LFAlert* alertNodeFactory();
	virtual CCMenuItem* creatMenuItem(CCObject* target, SEL_MenuHandler selector,string lable);
public:
	static LFAlert_BuyGame* showAlert();
};



//////////////////////////////////////////////////////////////////////////
// 购买Season以及他下面的section的对话框
//////////////////////////////////////////////////////////////////////////
class LFAlert_BuySeasonAndItsSections : public BaseBuyAlert
{
protected:
	int seasonId;
	bool buyAllSeason;

	LFAlert_BuySeasonAndItsSections(int seasonId);

	virtual void buyAllSeasonClicked(CCObject* sender);
	virtual void buyOneSeasonClicked(CCObject* sender);

protected:
	virtual LFAlert* alertNodeFactory();
public:
	static LFAlert_BuySeasonAndItsSections* showAlert(int seasonId);
};


/**
 * 购买答案的对话框
 */
class LFAlert_BuySolution : public BaseBuyAlert
{
protected:
	LFAlert_BuySolution();

	int numToBuy;

	void buyOneUseStar(CCObject* sender);
	// Sender是CCmenuItem，getTag就是对应的购买的次数
	void buySolutionClicked(CCObject* sender);
protected:
	virtual LFAlert* alertNodeFactory();
public:
	static LFAlert_BuySolution* showAlert();
};

/**
 * 购买答案的对话框
 */
class LFAlert_BuySuperTool : public BaseBuyAlert
{
protected:

	int buyNum;

	LFAlert_BuySuperTool();
	// Sender是CCmenuItem，getTag就是对应的购买的个数
	void buySuperToolClicked(CCObject* sender);

	// 使用点击了
	void useSuperToolClicked(CCObject* sender);

	CCMenuItem* useItem;
	void buySuperToolRsp(CCObject* payRspWrapper);
protected:
	virtual LFAlert* alertNodeFactory();
public:
	static LFAlert_BuySuperTool* showAlert();
};


// 选择支付方式的时候，发出的信息
// class ChooseInfor : public CCObject
// {
// private:
// 	PaymentTag payTag;
// public:
// 	ChooseInfor(PaymentTag tag)
// 	{
// 		this->payTag = tag;
// 	}
// 
// 	PaymentTag getPayTag()
// 	{
// 		return payTag;
// 	}
// };

/**
 * 选择支付方式的对话框：比如电信支付，移动支付，联通支付等
 */
class LFAlerts_ChoosePayWay : public BaseBuyAlert
{
protected:
	LFAlerts_ChoosePayWay(bool removeWhenChoosed);

	CCMenu* menu;

	bool mRemoveWhenChoosed;

	// tag就是paytag
	virtual void chooseClicked(CCObject* sender);
	virtual LFAlert* alertNodeFactory();
public:
	static LFAlerts_ChoosePayWay* showAlert(bool removeWhenChoosed);
};

/**
 * 选择购买本关卡的对话框
 */
class LFAlerts_UnLockSeason : public BaseBuyAlert
{
protected:

	int buyNum;

	LFAlerts_UnLockSeason(int seasonIndex);
	~LFAlerts_UnLockSeason();
	// Sender是CCmenuItem，getTag就是对应的购买的个数
	void buyUnLockSeasonClicked(CCObject* sender);

	void buyUnLockSeasonRsp(CCObject* payRspWrapper);
protected:
	virtual LFAlert* alertNodeFactory();
public:
	static LFAlerts_UnLockSeason* showAlert(int seasonIndex);
private:
	int				seasonIndex;
	int				price;
};