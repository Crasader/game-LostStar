#include "DoorLayer.h"
#include "CurtainLayer.h"
#include "ResManager.h"
#include "LevelManager.h"

#define X_INSERT	12		// 相交12像素

DoorLayer::DoorLayer(int seasonId,int sectionId,bool isOpened /* = false */)
{
	return;

	init();

	callback = NULL;
	autoRemove = false;
	initIsOpened = isOpened;
	mSeasonId = seasonId;
	mSectionId = sectionId;

// 	leftDoor = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"door_left.png").c_str());
// 	rightDoor = CCSprite::createWithTexture(leftDoor->getTexture());
// 	rightDoor->setFlipX(true);
// 
// 
// 	addChild(leftDoor);
// 	addChild(rightDoor);
// 
// 
// 	if (initIsOpened)
// 	{
// 		leftDoor->setPosition(ccp(getLeftMinPosX(),leftDoor->getContentSize().height/2));
// 		rightDoor->setPosition(ccp(getRightMaxPosX(),rightDoor->getContentSize().height/2));
// 	}else
// 	{
// 		leftDoor->setPosition(ccp(getLeftMaxPosX(),leftDoor->getContentSize().height/2));
// 		rightDoor->setPosition(ccp(getRightMinPosX(),rightDoor->getContentSize().height/2));
// 	}
}


void DoorLayer::setAutoRemve(bool remove)
{
	autoRemove = remove;
}
void DoorLayer::action(CurtainLayerCallback* callback /*= NULL*/)
{
	this->callback = callback;

	if (initIsOpened)
	{
		close();
	}else
	{
		open();
	}
}

#define  MOVE_TIME	0.3f
void DoorLayer::open()
{
	actionEnd(this);
// 	leftDoor->runAction(CCSequence::create(
// 		CCMoveTo::create(MOVE_TIME,ccp(getLeftMinPosX(),leftDoor->getPositionY())),
// 		CCCallFuncN::create(this,SEL_CallFuncN(&DoorLayer::actionEnd)),
// 		NULL));
// 	rightDoor->runAction(CCMoveTo::create(MOVE_TIME,ccp(getRightMaxPosX(),rightDoor->getPositionY())));
}
void DoorLayer::close()
{
	actionEnd(this);
// 	leftDoor->runAction(CCSequence::create(
// 		CCMoveTo::create(MOVE_TIME,ccp(getLeftMaxPosX(),leftDoor->getPositionY())),
// 		CCCallFuncN::create(this,SEL_CallFuncN(&DoorLayer::actionEnd)),
// 		NULL));
// 	rightDoor->runAction(CCMoveTo::create(MOVE_TIME,ccp(getRightMinPosX(),rightDoor->getPositionY())));
}

void DoorLayer::actionEnd(CCNode* node)
{
	if (callback)
		callback->curtainActionEnd();

	if (autoRemove)
		removeFromParentAndCleanup(true);
}

float DoorLayer::getLeftMinPosX()
{
//	float x_ret = getContentSize().width/2 - 1024/2.0f - leftDoor->getContentSize().width/2 + X_INSERT;
//	return x_ret;
	return 0;

	//float x_offset = (getContentSize().width - 1024)/2.0f + X_INSERT;
	//// 根据1024,768的外延，差多少就盖多少
	//return -leftDoor->getContentSize().width/2 + x_offset;
}
float DoorLayer::getLeftMaxPosX()
{
//	float x_ret = getContentSize().width/2 - leftDoor->getContentSize().width/2 + X_INSERT;
//	return x_ret;
	return 0;

	//float x_offset = (getContentSize().width - 1024)/2.0f + X_INSERT;
	//// 根据1024,768的外延，差多少就盖多少
	//return leftDoor->getContentSize().width/2 + x_offset;
}

float DoorLayer::getRightMinPosX()
{
//	float x_ret = getContentSize().width/2 + rightDoor->getContentSize().width/2 - X_INSERT;
//	return x_ret;

	return 0;

	//float x_offset = (getContentSize().width - 1024)/2.0f + X_INSERT;
	//// 根据1024,768的外延，差多少就盖多少
	//return getContentSize().width/2 + rightDoor->getContentSize().width/2 - x_offset;
}
float DoorLayer::getRightMaxPosX()
{
//	float x_ret = getContentSize().width/2 + 1024/2.0f + rightDoor->getContentSize().width/2 - X_INSERT;
//	return x_ret;
	return 0;

	//float x_offset = (getContentSize().width - 1024)/2.0f + X_INSERT;
	//// 根据1024,768的外延，差多少就盖多少
	//return getContentSize().width + rightDoor->getContentSize().width/2 - x_offset;

}

DoorLayer::~DoorLayer(void)
{
}
