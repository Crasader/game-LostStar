#include "GuideLayer_Strobe.h"
#include "VersionControl.h"
// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_Strobe::GuideLayer_Strobe(int seasonId, int sectionId):BaseGuideLayer(seasonId,sectionId)
{
	
	CCPoint dest = ccp(430,mWinSize.height - 300);


	CCSprite* guide_arrows = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	CCLabelTTF* guide_txt = createShadowLabel(LFStrings::getValue("teach_word_1_14").c_str(), fontStr_KlavikaBold);
	addChild(guide_arrows); 
	guide_arrows->setRotation(90);
	CCNode* pshadow = guide_arrows->getChildByTag(Tag_Shadow);
	if (pshadow)
	{
		pshadow->setPosition(ccpAdd(pshadow->getPosition(), ccp(3, 0)));
	}
	addChild(guide_txt);
	guide_txt->setAnchorPoint(ccp(0.0f, 0.5f));
#if Version_Free != 0
	guide_arrows->setPosition(ccp(dest.x + 50,dest.y - 35));
#else
	guide_arrows->setPosition(ccp(dest.x,dest.y));
#endif
	guide_txt->setPosition(ccp(guide_arrows->getPositionX() + guide_arrows->getContentSize().width/2 + 10,guide_arrows->getPositionY() - guide_arrows->getContentSize().height/2));

}
void GuideLayer_Strobe::lfClick(CCPoint glPoint)
{
}

void GuideLayer_Strobe::baseContentLayerEffctiveClicked(CCNode* node)
{
	if (node && node->getTag() == Tag_Box_Strobe)
	{
//		hideAll();
		this->removeAllChildren();
	}
}