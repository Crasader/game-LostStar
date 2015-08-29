#include "GuideLayer_ClickBlackHole.h"

// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_ClickBlackHole::GuideLayer_ClickBlackHole(int seasonId, int sectionId):BaseGuideLayer(seasonId,sectionId)
{
	CCPoint dest = ccp(249+271,mWinSize.height - 504);
//	mFinger->setPosition(ccp(804 - Finger_Pos_Offset_X,-20 + Finger_Pos_Offset_Y));
	
	m_pGuide_arrows = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	m_pGuide_txt = createShadowLabel(LFStrings::getValue("teach_word_5_1").c_str(), fontStr_KlavikaBold);
	addChild(m_pGuide_arrows);
	addChild(m_pGuide_txt);

	m_pGuide_arrows->setPosition(ccp(dest.x + 53,dest.y - 10));
	m_pGuide_txt->setPosition(ccp(m_pGuide_arrows->getPositionX() + m_pGuide_arrows->getContentSize().width/2, m_pGuide_arrows->getPositionY() + m_pGuide_arrows->getContentSize().height/2 + 10));
}

void GuideLayer_ClickBlackHole::startGuide()
{
	CCPoint dest = ccp(249+271, 175);
// 	mFinger->runAction(
// 		CCRepeat::create(
// 		CCSequence::create(
// 		CCFadeTo::create(0.3f,255),
// 		CCMoveTo::create(0.9f,ccp(dest.x + 10, dest.y)),
// 		CCDelayTime::create(1.3f),
// 		CCFadeTo::create(0.5f,0),
// 		CCMoveTo::create(0.1f,ccp(dest.x + 50,-20 + Finger_Pos_Offset_Y)),NULL),-1));
}

void GuideLayer_ClickBlackHole::lfClick(CCPoint glPoint)
{
	//if (glPoint.x >= 485 )
	//{
	//}
}
void GuideLayer_ClickBlackHole::baseContentLayerEffctiveClicked(CCNode* node)
{
	if (node && node->getTag() == Tag_Box_Bomb_DarkWhole)
	{
//		hideAll();
		this->removeAllChildren();
	}
}