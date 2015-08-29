#include "GuideLayer_ClickMotor.h"
#include "VersionControl.h"
// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_ClickMotor::GuideLayer_ClickMotor(int seasonId, int sectionId):BaseGuideLayer(seasonId,sectionId)
{
	m_pGuide_arrows = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	m_pGuide_txt = createShadowLabel(LFStrings::getValue("teach_word_2_1").c_str(), fontStr_KlavikaBold);

	addChild(m_pGuide_arrows);
	addChild(m_pGuide_txt);


#if Version_Free != 0
	m_pGuide_arrows->setPosition(ccp(720, mWinSize.height - 560));
	m_pGuide_txt->setAnchorPoint(ccp(1.0f, 0.5f));
	m_pGuide_txt->setPosition(ccp(m_pGuide_arrows->getPositionX() - m_pGuide_arrows->getContentSize().width/2 - 10,m_pGuide_arrows->getPositionY() - m_pGuide_arrows->getContentSize().height/2 + 5));

#else
	m_pGuide_arrows->setPosition(ccp(640, mWinSize.height - 500));	
	m_pGuide_txt->setPosition(ccp(m_pGuide_arrows->getPositionX() + m_pGuide_arrows->getContentSize().width/2 - 10,m_pGuide_arrows->getPositionY() + m_pGuide_arrows->getContentSize().height/2 + 10));

#endif
	
}

void GuideLayer_ClickMotor::startGuide()
{
	m_pGuide_arrows->runAction(CCSequence::create(
		CCDelayTime::create(3.0f),
		CCRemoveSelf::create(),
		NULL));
	m_pGuide_txt->runAction(CCSequence::create(
		CCDelayTime::create(3.0f),
		CCRemoveSelf::create(),
		NULL));
}

void GuideLayer_ClickMotor::lfClick(CCPoint glPoint)
{
}