#include "CurtainLayerClosedFrame.h"

CurtainLayerClosedFrame* CurtainLayerClosedFrame::create()
{
	CurtainLayerClosedFrame* frame = new CurtainLayerClosedFrame();
	frame->autorelease();
	return frame;
}

CurtainLayerClosedFrame::CurtainLayerClosedFrame()
{
	ResManager* resMgr = ResManager::getManager();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	float x_offset = (winSize.width - 1024)/2.0f;

	int tempSeasonId = 1;	// 只用s1的

	if(CC_CONTENT_SCALE_FACTOR() == 2)
	{
		CCSprite* leftCurtain = CCSprite::create(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtainHD_s%d_left-ipadhd.png",tempSeasonId)).c_str());
		addChild(leftCurtain);
		leftCurtain->setPosition(ccp(leftCurtain->getContentSize().width/2 + x_offset,winSize.height/2));

		CCSprite*rightCurtain = CCSprite::create(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtainHD_s%d_right-ipadhd.png",tempSeasonId)).c_str());
		addChild(rightCurtain);
		rightCurtain->setPosition(ccp(winSize.width - rightCurtain->getContentSize().width/2 - x_offset,winSize.height/2));
	}else
	{
		CCSprite* leftCurtain = CCSprite::create(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_open_s%d_left.png",tempSeasonId)).c_str());
		addChild(leftCurtain);
		leftCurtain->setPosition(ccp(leftCurtain->getContentSize().width/2 + x_offset,winSize.height/2));

		CCSprite* rightCurtain = CCSprite::create(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_open_s%d_right.png",tempSeasonId)).c_str());
		addChild(rightCurtain);
		rightCurtain->setPosition(ccp(winSize.width - rightCurtain->getContentSize().width/2 - x_offset,winSize.height/2));
	}
}