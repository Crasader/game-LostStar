#include "GuideLayer_DoubleTable.h"
#include "MenuLayer.h"

// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_DoubleTable::GuideLayer_DoubleTable(int seasonId, int sectionId):BaseGuideLayer(seasonId,sectionId)
{
	guide_txt = createShadowLabel(LFStrings::getValue("teach_word_1_6").c_str());
	guide_txt->setAnchorPoint(ccp(0.5f, 0.5f));
	addChild(guide_txt);
	guide_txt->setPosition(ccp(512, 320));

	spriteMask1 = CCSprite::create(getImageFile("jiaoxue_table144_24.png").c_str());
	addChild(spriteMask1);
	spriteMask1->setPosition(ccp(680, 400));

	spriteMask2 = CCSprite::create(getImageFile("jiaoxue_table192_24.png").c_str());
	addChild(spriteMask2);
	spriteMask2->setPosition(ccp(368, 208));

	CCSprite* a1 = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	addChild(a1);
//	a1->setScale(1.2f);
	a1->setPosition(ccp(340, 270));
	a1->setRotation(165);
	CCNode* pshadow = a1->getChildByTag(Tag_Shadow);
	if (pshadow)
	{
		pshadow->setPosition(ccpAdd(pshadow->getPosition(), ccp(-3, 3)));
	}

	CCSprite* a2 = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	addChild(a2);
	a2->setPosition(ccp(700, 350));


	CCLayerColor* pLayercolor = CCLayerColor::create(ccc4(0.0f, 0.0f, 0.0f, 75.0f));
	pLayercolor->setPosition(CCPointZero);
	addChild(pLayercolor, -1);

}

void GuideLayer_DoubleTable::lfClick(CCPoint glPoint)
{
 	BaseScense::getCurrentScene()->getMenuLayer()->getGameMenu()->setEnabled(true);
 	BaseScense::getCurrentScene()->getMenuLayer()->getGameMenu()->setVisible(true);
	BaseScense::getCurrentScene()->setCollisionEnable(true);
// 	resumeBaseContentLayer();
	stopGuide();
}

void GuideLayer_DoubleTable::startGuide()
{
	BaseScense::getCurrentScene()->setCollisionEnable(false);
	BaseScense::getCurrentScene()->getMenuLayer()->getGameMenu()->setEnabled(false);
	BaseScense::getCurrentScene()->getMenuLayer()->getGameMenu()->setVisible(false);
	float time = 0.6f;
	spriteMask1->setOpacity(0);
	spriteMask1->runAction(
		CCSequence::create(
			CCRepeat::create(
				CCSequence::create(
				CCSpawn::create(
					CCFadeIn::create(time), 
					CCScaleTo::create(time, 1.05f, 1.1f), 
					NULL), 
				CCDelayTime::create(0.3f),
				CCSpawn::create(
					CCFadeOut::create(time), 
					CCScaleTo::create(time, 1.0f), 
					NULL),
				NULL)
				, -1),
				CCCallFunc::create(this, callfunc_selector(GuideLayer_DoubleTable::stopGuide)),
				NULL));

	spriteMask2->setOpacity(0);
	spriteMask2->runAction(
		CCSequence::create(
			CCRepeat::create(
				CCSequence::create(
				CCSpawn::create(
					CCFadeIn::create(time), 
					CCScaleTo::create(time, 1.05f, 1.1f), 
					NULL), 
				CCDelayTime::create(0.3f),
				CCSpawn::create(
					CCFadeOut::create(time), 
					CCScaleTo::create(time, 1.0f), 
					NULL),
				NULL)
				, -1),
				NULL));
}

void GuideLayer_DoubleTable::stopGuide()
{
	this->removeAllChildren();
}
