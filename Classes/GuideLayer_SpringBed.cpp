#include "GuideLayer_SpringBed.h"
#include "VersionControl.h"

// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_SpringBed::GuideLayer_SpringBed(int seasonId, int sectionId):BaseGuideLayer(seasonId,sectionId)
{
	CCPoint dest = ccp(500, mWinSize.height - 450);

	guide_arrows = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	guide_txt = createShadowLabel(LFStrings::getValue("teach_word_1_12").c_str(), fontStr_KlavikaBold);
	addChild(guide_arrows);
	addChild(guide_txt);

#if Version_Free != 0
	guide_arrows->setRotation(90);
	CCNode* pshadow = guide_arrows->getChildByTag(Tag_Shadow);
	if (pshadow)
	{
		pshadow->setPosition(ccpAdd(pshadow->getPosition(), ccp(3, 0)));
	}
	guide_arrows->setAnchorPoint(ccp(0.2f, 0.0f));
	guide_arrows->setPosition(ccp(dest.x - 50, dest.y - 130));
	guide_arrows->setVisible(false);

	guide_txt->setAnchorPoint(ccp(0.5f, 1.0f));

	guide_txt->setPosition(ccp(guide_arrows->getPositionX() + guide_arrows->getContentSize().width/2 - 10, guide_arrows->getPositionY() + guide_arrows->getContentSize().height/2 - 10));
#else
	guide_arrows->setPosition(dest);
	guide_txt->setPosition(ccp(guide_arrows->getPositionX() + guide_arrows->getContentSize().width/2 - 10, guide_arrows->getPositionY() + guide_arrows->getContentSize().height/2 + 10));
#endif

}

void GuideLayer_SpringBed::startGuide()
{
	guide_arrows->runAction(CCSequence::create(
		CCDelayTime::create(3.0f),
		CCRemoveSelf::create(),
		NULL));
	guide_txt->runAction(CCSequence::create(
		CCDelayTime::create(3.0f),
		CCRemoveSelf::create(),
		NULL));
}

void GuideLayer_SpringBed::lfClick(CCPoint glPoint)
{
}

void GuideLayer_SpringBed::baseContentLayerEffctiveClicked(CCNode* node)
{

}