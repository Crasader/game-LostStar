#include "CurtainLayer.h"
#include "SpriteHelper.h"
#include "CurtainLayerClosedFrame.h"
#include "Music.h"

CurtainLayer::CurtainLayer(int seasonId,int sectionId,bool toClose/* = false*/)
{
	//CCDirector::sharedDirector()->getScheduler()->setTimeScale(0.3f);
	mSeasonId = seasonId;
	mSectionId = sectionId;
    hdLayerMask = NULL;
	callback = NULL;
	this->toClose = toClose;
	initCurtain();
	autoRemove = true;

	//Music::sharedMusicPlayer()->loadEffectSound("curtain.wav");

	if (CC_CONTENT_SCALE_FACTOR() == 2)
	{
        leftCurtain->setAnchorPoint(ccp(0.25f,0.5f));
        rightCurtain->setAnchorPoint(ccp(0.75f,0.5f));
        leftCurtain->setScale(2.0f);
        rightCurtain->setScale(2.0f);
	}
}
void CurtainLayer::initCurtain()
{
	ResManager* resMgr = ResManager::getManager();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	//int tempSeasonId = mSeasonId;
	int tempSeasonId = 1;	// 只用s1的

	float x_offset = (winSize.width - 1024)/2.0f;

	if (toClose)
	{
		leftCurtain = CCSprite::create(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_s%d_left.png",tempSeasonId)).c_str());
		addChild(leftCurtain);
		leftCurtain->setPosition(ccp(-leftCurtain->getContentSize().width/2 + x_offset,winSize.height/2));
		//leftCurtain->setPosition(ccp(-leftCurtain->getContentSize().width/2,winSize.height/2));

		rightCurtain = CCSprite::create(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_s%d_right.png",tempSeasonId)).c_str());
		addChild(rightCurtain);
		//rightCurtain->setPosition(ccp(winSize.width + rightCurtain->getContentSize().width/2,winSize.height/2));
		rightCurtain->setPosition(ccp(winSize.width + rightCurtain->getContentSize().width/2 - x_offset,winSize.height/2));

		hdLayerMask = CurtainLayerClosedFrame::create();
		addChild(hdLayerMask,2);
		hdLayerMask->setVisible(false);
	}else
	{
		leftCurtain = CCSprite::create(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_open_s%d_left.png",tempSeasonId)).c_str());
		addChild(leftCurtain);
		//leftCurtain->setPosition(ccp(leftCurtain->getContentSize().width/2,winSize.height/2));
		leftCurtain->setPosition(ccp(leftCurtain->getContentSize().width/2 + x_offset,winSize.height/2));

		rightCurtain = CCSprite::create(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_open_s%d_right.png",tempSeasonId)).c_str());
		addChild(rightCurtain);
		//rightCurtain->setPosition(ccp(winSize.width - rightCurtain->getContentSize().width/2,winSize.height/2));
		rightCurtain->setPosition(ccp(winSize.width - rightCurtain->getContentSize().width/2 - x_offset,winSize.height/2));
	}
}
void CurtainLayer::action(CurtainLayerCallback* callback/* = NULL*/)
{
	this->callback = callback;
	toClose?close():open();

	//Music::sharedMusicPlayer()->playEffectSound("curtain.wav");
}
void CurtainLayer::close()
{
	Music::sharedMusicPlayer()->playEffectSound("curtain_close.wav");
	//int tempSeasonId = mSeasonId;
	int tempSeasonId = 1;	// 只用s1的

	ResManager* resMgr = ResManager::getManager();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	int framNum = 6;
	string fileNamesLeft[] = {
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_s%d_left.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain2_s%d_left.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain3_s%d_left.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain4_s%d_left.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain5_s%d_left.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain6_s%d_left.png",tempSeasonId))
	};
	string fileNamesRight[] = {
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_s%d_right.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain2_s%d_right.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain3_s%d_right.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain4_s%d_right.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain5_s%d_right.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain6_s%d_right.png",tempSeasonId))
	};
	CCFiniteTimeAction* actionLeft = (CCFiniteTimeAction*)SpriteHelper::createAction(fileNamesLeft,framNum,0.07f,false);
	CCFiniteTimeAction* actionCallBackLeft = SpriteHelper::createAction(actionLeft,this,SEL_CallFuncN(&CurtainLayer::actionEnd));
	CCFiniteTimeAction* actionRight = (CCFiniteTimeAction*)SpriteHelper::createAction(fileNamesRight,framNum,0.07f,false);

	float x_offset = (winSize.width - 1024)/2.0f;

	leftCurtain->runAction(CCSequence::create(
		CCMoveTo::create(0.2f,ccp(leftCurtain->getContentSize().width/2 + x_offset,winSize.height/2)),
		actionCallBackLeft,NULL));

	//rightCurtain->setFlipX(true);
	rightCurtain->runAction(CCSequence::create(
		CCMoveTo::create(0.2f,ccp(winSize.width - rightCurtain->getContentSize().width/2 - x_offset,winSize.height/2)),
		actionRight,
		NULL));
}
void CurtainLayer::open()
{
	Music::sharedMusicPlayer()->playEffectSound("curtain_open.wav");

	//int tempSeasonId = mSeasonId;
	int tempSeasonId = 1;	// 只用s1的

	ResManager* resMgr = ResManager::getManager();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	int framNum = 4;
	string fileNamesLeft[] = {
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_open_s%d_left.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain2_open_s%d_left.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain3_open_s%d_left.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain4_open_s%d_left.png",tempSeasonId))
	};
	string fileNamesRight[] = {
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_open_s%d_right.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain2_open_s%d_right.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain3_open_s%d_right.png",tempSeasonId)),
		resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain4_open_s%d_right.png",tempSeasonId))
	};

    
    float factor = 1.0f;
	float x_offset = (winSize.width - 1024)/2.0f;
    
	CCFiniteTimeAction* actionLeft = (CCFiniteTimeAction*)SpriteHelper::createAction(fileNamesLeft,framNum,0.04f / factor,false);
	CCFiniteTimeAction* actionRight = (CCFiniteTimeAction*)SpriteHelper::createAction(fileNamesRight,framNum,0.04f / factor,false);

    if (CC_CONTENT_SCALE_FACTOR() == 2)
	{
        factor = 2.0f;
	}
    leftCurtain->runAction(CCSequence::create(
                            actionLeft,
                            CCMoveTo::create(0.4f,ccp( (-leftCurtain->getContentSize().width/2 + x_offset )*factor,winSize.height/2)),
                            CCCallFuncN::create(this, SEL_CallFuncN(&CurtainLayer::actionEnd)),NULL));
    
    rightCurtain->runAction(CCSequence::create(
                            actionRight,
                            CCMoveTo::create(0.4f,ccp( (winSize.width + leftCurtain->getContentSize().width/2 - x_offset)*factor,winSize.height/2)),NULL));

}

void CurtainLayer::setAutoRemve(bool remove)
{
	autoRemove = remove;
}

void CurtainLayer::actionEnd(CCNode* node)
{
	//int tempSeasonId = 1;	// 只用s1的
	//ResManager* resMgr = ResManager::getManager();
	if (toClose)
	{
		// 显示高清的图
		hdLayerMask->setVisible(true);
	}else
	{
//		leftCurtain->initWithFile(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_open_s%d_left.png",tempSeasonId)).c_str());
//		rightCurtain->initWithFile(resMgr->getSharedFilePath(SpriteHelper::splicingNum_String("curtain1_open_s%d_right.png",tempSeasonId)).c_str());
//        
//        if (CC_CONTENT_SCALE_FACTOR() == 2)
//        {
//            leftCurtain->setAnchorPoint(ccp(0.25f,0.5f));
//            rightCurtain->setAnchorPoint(ccp(0.75f,0.5f));
//            leftCurtain->setScale(2.0f);
//            rightCurtain->setScale(2.0f);
//        }
	}
	if (callback)
	{
		callback->curtainActionEnd();
	}

	if (autoRemove)
		removeFromParentAndCleanup(true);
}