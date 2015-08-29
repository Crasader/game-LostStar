#include "GuideLayer_CannotDestory.h"
#include "VersionControl.h"

// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_CannotDestory::GuideLayer_CannotDestory(int seasonId, int sectionId):BaseGuideLayer(seasonId,sectionId)
{
	
	CCPoint dest = ccp(612,mWinSize.height - 516);

	guide_arrows = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	guide_txt = createShadowLabel(LFStrings::getValue("teach_word_1_4").c_str(), fontStr_KlavikaBold);
	addChild(guide_arrows);
	addChild(guide_txt);
#if Version_Free != 0 
	guide_arrows->setPosition(ccp(dest.x, dest.y + 60));
#else
	guide_arrows->setPosition(ccp(dest.x + 50, dest.y + 36));
#endif
	guide_txt->setPosition(ccp(guide_arrows->getPositionX() + guide_arrows->getContentSize().width/2 - 10,guide_arrows->getPositionY() + guide_arrows->getContentSize().height/2 + 10));
}
void GuideLayer_CannotDestory::lfClick(CCPoint glPoint)
{
}

void GuideLayer_CannotDestory::startGuide()
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