#pragma once
#include "cocos2d.h"
USING_NS_CC;
class Shieldingmanger:public CCNode
{
public:
	Shieldingmanger(void);
	~Shieldingmanger(void);

public:

	static Shieldingmanger* getSielding();

	void setCurrentlayertag(int tag);
	int  getCureentlayertag();
	bool isNomainhot();//true main不响应
	void setNomainhot(bool hot);
	bool isHorizontalActive();
	void setHorizontalActive(bool h);
	void setPingbingbutton(bool wcPingbing);
	bool isPinbingbutton();
	void Fadeoutnodes(CCNode* node);
	void Fadeinnodes(CCNode* node);

	void continuousThetimer_schedule();
	bool continuousThetimer();
	int DaySearch();
	long getCurrentTime();
	void setcontinuousThetimermark(bool mark);
private:
	static Shieldingmanger* manager;
	int tag;
	bool hot;
	bool mHorizontalActive;
	bool wcPingbing_button;


	CCNode* node;
	bool continuousClick_mark;//为true可点击  false不可点击
	float onethetime;//每次时间
	float numtime;//总共时间

	float jointime;
	float actiondonetime;

	
};

