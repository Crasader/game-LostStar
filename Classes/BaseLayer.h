#pragma once
#include "LFLayer.h"
#include "ResManager.h"
#include "BaseSprite.h"


class BaseLayer : public LFLayer
{
protected:
	CCSize mWinSize;
	CCSize mVisibleSize;
	CCPoint mVisibleOrigin;

	CCPoint mCenterPoint;
	ResManager* mResMgr;

	vector<string> msgVec;

	// 添加消息监听
	void addObserver(SEL_CallFuncO selector,const char *name);
	// 移除消息监听
	void removeObserver(const char *name);

	virtual void onExit();
public:
	// 禁用触摸
	virtual void disableAllTouchBegin();
	// 启用触摸
	virtual void disableAllTouchEnd();

	//刷新本页面
	virtual void refreshLayer(){};

	// 移除一个Node
	void removeNode(CCNode* toRemove);

	// 修改menu的优先级，upOrdown为true就升高
	void changeMenuHandlerPriority(CCMenu* menu,bool upOrDown);

	// 降低menu的优先级
	void jiangDiMenuHandlerPriority(CCNode* menu);

	// 升高menu的优先级
	void shengGaoMenuHandlerPriority(CCNode* menu);

	

	BaseLayer(void);
	~BaseLayer(void);
};
