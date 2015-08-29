#include "GuideLayer_SuperTool.h"
#include "LFToast.h"
#include "LFStrings.h"
#include "font.h"
#include "MenuLayer.h"
#include "History.h"
#include "MaskLayerUI.h"

// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_SuperTool::GuideLayer_SuperTool(int seasonId, int sectionId)
	:BaseGuideLayer(seasonId,sectionId),
	m_nClickCount(0),
	m_bIsUseTeaching(false),
	m_tLabel(NULL),
	m_sGuide_arrowsSpr(NULL)

{
	if (!CCUserDefault::sharedUserDefault()->getBoolForKey(FirstSuperTool,true))
	{
		return;
	}
	m_bIsUseTeaching = true;
	CCUserDefault::sharedUserDefault()->setBoolForKey(FirstSuperTool,false);
	CCUserDefault::sharedUserDefault()->flush();
	BaseScense::getCurrentScene()->getMenuLayer()->beginTeachUseSuperTool();
	BaseScense::getCurrentScene()->getMenuLayer()->setTeachCallBack(this,SEL_CallFunc(&GuideLayer_SuperTool::usedEnd));


	CCMenuItem *item = BaseScense::getCurrentScene()->getMenuLayer()->getRestartItem();
	item->setVisible(false);
	CCPoint dest = ccp(180,500);

	CCSprite* guide_arrows = CCSprite::create(getImageFile("teach_arrows.png").c_str());
	addChild(guide_arrows);
	m_sGuide_arrowsSpr = guide_arrows;
	guide_arrows->setPosition(ccp(dest.x+50, dest.y+65));
	guide_arrows->setRotationX(20);
	guide_arrows->setOpacity(0);

	guide_arrows->runAction(CCSequence::create(
		CCDelayTime::create(0.2f),
		CCFadeTo::create(1.0f,255),
		NULL));
	CCLabelTTF *guide_txt = CCLabelTTF::create(LFStrings::getValue("TeachUseSuperToolOne").c_str(), fontStr_katong,27.0f);//, CCSize(270,0), kCCTextAlignmentCenter);
	guide_txt->setColor(ccBLACK);
	guide_txt->setHorizontalAlignment(kCCTextAlignmentLeft);
	addChild(guide_txt);
	guide_txt->setPosition(ccp(dest.x + 225,guide_arrows->getPositionY() + guide_arrows->getContentSize().height-20));
	m_tLabel = guide_txt;

	BaseScense::getCurrentScene()->getMenuLayer()->getStarState()->teach_scaleChange(m_nClickCount);
}
void GuideLayer_SuperTool::lfClick(CCPoint glPoint)
{
	if (m_bIsUseTeaching)
	{
		if (m_nClickCount == 0)
		{
			m_tLabel->runAction(CCSequence::create(CCFadeOut::create(0.5f),CCCallFunc::create(this,SEL_CallFunc(&GuideLayer_SuperTool::clickEnd)),NULL));
			m_sGuide_arrowsSpr->runAction(CCFadeOut::create(0.4f));
		}
		else if (m_nClickCount == 1)
		{
			m_tLabel->runAction(CCSequence::create(CCFadeOut::create(0.5f),CCCallFunc::create(this,SEL_CallFunc(&GuideLayer_SuperTool::guide_end)),NULL));
			m_sGuide_arrowsSpr->runAction(CCFadeOut::create(0.4f));
		}
	}
	//m_tLabel->setString(LFStrings::getValue("Teach_Click_Finger_Double").c_str());
}
void GuideLayer_SuperTool::clickEnd()
{
	m_nClickCount = 1;
	m_tLabel->setString(LFStrings::getValue("TeachUseSuperToolThree").c_str());
	m_tLabel->runAction(CCFadeIn::create(0.3f));
	m_sGuide_arrowsSpr->setVisible(true);
	m_sGuide_arrowsSpr->setRotationY(160);
	m_sGuide_arrowsSpr->runAction(CCFadeIn::create(0.3f));
	m_tLabel->setPositionX(720);
	m_sGuide_arrowsSpr->setPosition(ccp(m_tLabel->getPositionX() + m_tLabel->getContentSize().width * 0.2f,m_tLabel->getPositionY() - m_tLabel->getContentSize().height * 0.5f));
	//MenuLayer::getCurrentLayer()->getStarState()->teach_scaleChange(m_nClickCount);
}
void GuideLayer_SuperTool::guide_end()
{
	m_nClickCount = 2;
	m_tLabel->removeFromParent();
	m_sGuide_arrowsSpr->removeFromParent();
}
void GuideLayer_SuperTool::usedEnd()
{
	m_tLabel->setString(LFStrings::getValue("TeachUseSuperToolTwo").c_str());
	m_tLabel->runAction(CCFadeIn::create(0.3f));
	m_tLabel->setPosition(ccp(565,550));
	m_sGuide_arrowsSpr->setRotation(0);
	m_sGuide_arrowsSpr->setVisible(false);
	//MenuLayer::getCurrentLayer()->
	//MenuLayer::getCurrentLayer()->getStarState()->teach_scaleChange(m_nClickCount);
}