#include "GuideLayer_FreazyBomb.h"

// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_FreazyBomb::GuideLayer_FreazyBomb(int seasonId, int sectionId):BaseGuideLayer(seasonId,sectionId)
{
	
//	mFinger->setPosition(ccp(700 - Finger_Pos_Offset_X,-20 + Finger_Pos_Offset_Y));

	CCPoint dest = ccp(512 + 45 ,mWinSize.height - 440 + 69);


	CCSprite* guide_arrows = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	CCSprite* guide_txt = createShadowLabel(LFStrings::getValue("teach_word_4_1").c_str(), fontStr_KlavikaBold);
	addChild(guide_arrows);
	addChild(guide_txt);

	guide_arrows->setPosition(ccp(dest.x + 20, dest.y - 35));
	guide_txt->setPosition(ccp(guide_arrows->getPositionX() + guide_arrows->getContentSize().width/2,
		guide_arrows->getPositionY() + guide_arrows->getContentSize().height/2 + 10));
}

void GuideLayer_FreazyBomb::startGuide()
{
// 	mFinger->runAction(
// 		CCRepeat::create(
// 		CCSequence::create(
// 		CCFadeTo::create(0.3f,255),
// 		CCMoveTo::create(0.9f,ccp(530,mWinSize.height - 435 +Finger_Pos_Offset_Y)),
// 		CCDelayTime::create(1.3f),
// 		CCFadeTo::create(0.5f,0),
// 		CCMoveTo::create(0.1f,ccp(570, -20 + Finger_Pos_Offset_Y+5)),NULL),3));
}

void GuideLayer_FreazyBomb::lfClick(CCPoint glPoint)
{
}

void GuideLayer_FreazyBomb::baseContentLayerEffctiveClicked(CCNode* node)
{
	if (node && node->getTag() == Tag_Timer_Dec_Bomb)
	{
//		hideAll();
		this->removeAllChildren();
	}
}