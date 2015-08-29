#pragma once
#include "cocos2d.h"
#include "ICurtain.h"

using namespace cocos2d;


class DoorLayer : public CCLayer,public ICurtain
{
private:
//	CCSprite* leftDoor;
//	CCSprite* rightDoor;
	CurtainLayerCallback* callback;
	bool autoRemove;
	bool initIsOpened;
	int mSeasonId;
	int mSectionId;

	// 获取左右门板的X位置
	float getLeftMinPosX();
	float getLeftMaxPosX();

	float getRightMinPosX();
	float getRightMaxPosX();

	void open();
	void close();
	void actionEnd(CCNode* node);
public:
	DoorLayer(int seasonId,int sectionId,bool isOpened = false);
	virtual void setAutoRemve(bool remove);
	virtual void action(CurtainLayerCallback* callback = NULL);


	~DoorLayer(void);
};

