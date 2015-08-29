#include "GuideLayer_Score.h"
#include "VersionControl.h"

// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_Score::GuideLayer_Score(int seasonId, int sectionId):BaseGuideLayer(seasonId,sectionId)
{
	CCPoint dest = ccp(1024, 650);

	guide_txt = createShadowLabel(LFStrings::getValue("teach_word_1_3").c_str(), fontStr_KlavikaBold);
	guide_txt->setPosition(dest/2);
	this->addChild(guide_txt);
}

void GuideLayer_Score::startGuide()
{
	guide_txt->runAction(CCSequence::create(
		CCDelayTime::create(3.0f),
		CCRemoveSelf::create(),
		NULL));
}

void GuideLayer_Score::lfClick(CCPoint glPoint)
{
}

void GuideLayer_Score::baseContentLayerEffctiveClicked(CCNode* node)
{

}