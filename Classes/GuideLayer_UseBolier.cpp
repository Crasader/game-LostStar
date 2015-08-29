#include "GuideLayer_UseBolier.h"

// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_UseBolier::GuideLayer_UseBolier(int seasonId, int sectionId):BaseGuideLayer(seasonId,sectionId)
{
	
	mFinger->setPosition(ccp(345 - Finger_Pos_Offset_X, mWinSize.height - 445 + Finger_Pos_Offset_Y));
// 	mFinger->runAction(
// 		CCRepeat::create(
// 			CCSequence::create(
// 			CCFadeTo::create(0.3f,255),
// 			CCMoveTo::create(0.9f,ccp(289 + 20, mWinSize.height - 504 +Finger_Pos_Offset_Y)),
// 			CCDelayTime::create(1.3f),
// 			CCFadeTo::create(0.5f,0),
// 			CCMoveTo::create(0.1f,ccp(289 + 40, -20 + Finger_Pos_Offset_Y)),NULL),-1));
// 
	mFinger->getAnimation()->play(Finger_Armature_ChuiQi);

	CCSprite* guide_arrows = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	CCLabelTTF* guide_txt = createShadowLabel(LFStrings::getValue("teach_word_3_1").c_str(), fontStr_KlavikaBold);
	addChild(guide_arrows);
	addChild(guide_txt);

	guide_arrows->setPosition(ccp(360,mWinSize.height - 460));

	guide_txt->setPosition(ccp(guide_arrows->getPositionX() + guide_arrows->getContentSize().width/2 - 10,guide_arrows->getPositionY() + guide_arrows->getContentSize().height/2 + 10));

}

void GuideLayer_UseBolier::lfClick(CCPoint glPoint)
{
}

void GuideLayer_UseBolier::baseContentLayerEffctiveClicked(CCNode* node)
{
	if (node != NULL && node->getTag() == Tag_Box_Boiler)
	{
//		hideAll();
		this->removeAllChildren();
	}
}