#include "GuideLayer_CanotUseMotor.h"
#include "VersionControl.h"
// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_CanotUseMotor::GuideLayer_CanotUseMotor(int seasonId, int sectionId):BaseGuideLayer(seasonId,sectionId)
{
	m_pGuide_arrows = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	m_pGuide_txt = createShadowLabel(LFStrings::getValue("teach_word_2_6").c_str(), fontStr_KlavikaBold);

	addChild(m_pGuide_arrows);
	addChild(m_pGuide_txt);

#if Version_Free != 0
	m_pGuide_arrows->setRotation(90);
	CCNode* pshadow = m_pGuide_arrows->getChildByTag(Tag_Shadow);
	if (pshadow)
	{
		pshadow->setPosition(ccpAdd(pshadow->getPosition(), ccp(3, 0)));
	}
	m_pGuide_arrows->setPosition(ccp(450, 170));
	m_pGuide_txt->setAnchorPoint(ccp(0.0f, 0.5f));
	m_pGuide_txt->setPosition(ccp(m_pGuide_arrows->getPositionX() + m_pGuide_arrows->getContentSize().width/2, m_pGuide_arrows->getPositionY() - m_pGuide_arrows->getContentSize().height/2 - 10));
#else
	m_pGuide_txt->setAnchorPoint(ccp(0.0f, 0.0f));
	m_pGuide_arrows->setPosition(ccp(420, mWinSize.height - 506 + 220));	
	m_pGuide_txt->setPosition(ccp(m_pGuide_arrows->getPositionX() + m_pGuide_arrows->getContentSize().width/2, m_pGuide_arrows->getPositionY() + m_pGuide_arrows->getContentSize().height/2 + 10));
#endif
}

void GuideLayer_CanotUseMotor::startGuide()
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

void GuideLayer_CanotUseMotor::lfClick(CCPoint glPoint)
{
}