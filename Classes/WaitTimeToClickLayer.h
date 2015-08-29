#pragma once
#include "cocos2d.h"
USING_NS_CC;

class WaitTimeToClickLayer : public CCLayer
{
private:
	unsigned int mSoundId;
	bool isDone;
	CCSize mScreenSize;
	void waitingDone(CCNode* null_obj);	// 等待结束
	void setWaitingSoundPause(bool pause);
public:
	WaitTimeToClickLayer(void);

	void viewWaitingAnimation();//显示等待的动画;
	void viewWaitingPrompt();// 显示提示

	bool isWaitingDone();

	void setPause(bool pause = true);

	void removeNode(CCNode* node);

	~WaitTimeToClickLayer(void);
};

