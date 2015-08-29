#include "GuideLayer_CutRope.h"
#include "VersionControl.h"

// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_CutRope::GuideLayer_CutRope(int seasonId, int sectionId):BaseGuideLayer(seasonId,sectionId)
{
	mFinger->setPosition(ccp(327 + Finger_Pos_Offset_X ,mWinSize.height - 230 + Finger_Pos_Offset_Y));
	mFinger->setOpacity(255);
	


	CCSprite* guide_arrows = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	CCLabelTTF* guide_txt = createShadowLabel(LFStrings::getValue("teach_word_1_7").c_str(), fontStr_KlavikaBold);

	addChild(guide_arrows);
	addChild(guide_txt);

#if Version_Free != 0
	guide_arrows->setPosition(ccp(560, mWinSize.height - 80 + Finger_Pos_Offset_Y));
	mFinger->setPosition(ccp(350 + Finger_Pos_Offset_X ,mWinSize.height - 30 + Finger_Pos_Offset_Y));
	guide_txt->setAnchorPoint(ccp(1.0f, 1.0f));
#else
	guide_arrows->setPosition(ccp(560, mWinSize.height - 330));
	guide_txt->setAnchorPoint(ccp(0.5f, 1.0f));
#endif

	guide_arrows->setRotation(-90);
	CCNode* pshadow = guide_arrows->getChildByTag(Tag_Shadow);
	if (pshadow)
	{
		pshadow->setPosition(ccpAdd(pshadow->getPosition(), ccp(3, 0)));
	}
	
	guide_txt->setPosition(ccp(guide_arrows->getPositionX() + guide_arrows->getContentSize().width/2 + 90,guide_arrows->getPositionY() - guide_arrows->getContentSize().width/2 - 10));

	mFinger->runAction(
		CCRepeat::create(
		CCSequence::create(
		CCFadeTo::create(0.1f,255),
		CCDelayTime::create(0.4f),
		CCMoveBy::create(1.5f,ccp(250,0)),
		CCDelayTime::create(0.2f),
		CCFadeTo::create(0.3f,0),
		CCMoveBy::create(0.1f,ccp(-250,0)),
		NULL
		),-1));
}

void GuideLayer_CutRope::lfClick(CCPoint glPoint)
{
	//stopGuide();
}

void GuideLayer_CutRope::baseContentLayerEffctiveClicked(CCNode* node)
{
	if (node == NULL)
	{
//		hideAll();
		this->removeAllChildren();
	}
}