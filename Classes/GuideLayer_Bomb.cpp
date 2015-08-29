#include "GuideLayer_Bomb.h"
#include "VersionControl.h"

// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_Bomb::GuideLayer_Bomb(int seasonId, int sectionId):BaseGuideLayer(seasonId,sectionId)
{
	
	CCPoint dest = ccp(515,mWinSize.height - 537);


	CCSprite* guide_arrows = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	CCLabelTTF* guide_txt = createShadowLabel(LFStrings::getValue("teach_word_1_8").c_str(), fontStr_KlavikaBold);
	addChild(guide_arrows); 

	addChild(guide_txt);
	guide_txt->setAnchorPoint(ccp(0.0f, 0.5f));
	guide_arrows->setRotation(90);
	CCNode* pshadow = guide_arrows->getChildByTag(Tag_Shadow);
	if (pshadow)
	{
		pshadow->setPosition(ccpAdd(pshadow->getPosition(), ccp(3, 0)));
	}
#if Version_Free != 0
	guide_arrows->setPosition(ccp(dest.x - 42, mWinSize.height - 200));
	guide_txt->setAnchorPoint(ccp(0.0f, 0.8f));
#else
	guide_arrows->setPosition(ccp(dest.x + 30, dest.y - 60));
#endif
	guide_txt->setPosition(ccp(guide_arrows->getPositionX() + guide_arrows->getContentSize().width/2 + 10, guide_arrows->getPositionY() - guide_arrows->getContentSize().height/2));

}
void GuideLayer_Bomb::lfClick(CCPoint glPoint)
{
}

void GuideLayer_Bomb::baseContentLayerEffctiveClicked(CCNode* node)
{
// #if Version_Free != 0
// #else
	if (node && node->getTag() == Tag_Box_Bomb)
	{
		hideAll();
		this->removeAllChildren();
	}
//#endif
}