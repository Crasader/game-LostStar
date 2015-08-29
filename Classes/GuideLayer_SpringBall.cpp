#include "GuideLayer_SpringBall.h"

// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_SpringBall::GuideLayer_SpringBall(int seasonId, int sectionId):BaseGuideLayer(seasonId,sectionId)
{
	CCPoint dest = ccp(480,mWinSize.height - 220);

	guide_arrows = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	guide_txt = createShadowLabel(LFStrings::getValue("teach_word_1_11").c_str(), fontStr_KlavikaBold);
	guide_txt->setAnchorPoint(ccp(0.0f, 0.0f));
	addChild(guide_arrows);
	addChild(guide_txt);

	guide_arrows->setPosition(ccp(345, mWinSize.height - 262));
	guide_txt->setPosition(ccp(guide_arrows->getPositionX() + guide_arrows->getContentSize().width/2 - 10, guide_arrows->getPositionY() + guide_arrows->getContentSize().height/2 + 10));
}

void GuideLayer_SpringBall::startGuide()
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

void GuideLayer_SpringBall::lfClick(CCPoint glPoint)
{
}

void GuideLayer_SpringBall::baseContentLayerEffctiveClicked(CCNode* node)
{

}