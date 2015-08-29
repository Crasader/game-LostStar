#include "BaseGuideLayer.h"
#include "SpriteHelper.h"
#include "BackgroudLayerFactory.h"
#include "BaseScense.h"


USING_NS_CC_EXT;

BaseGuideLayer::BaseGuideLayer(int seasonId,int sectionId)
{
	mBaseScene = NULL;
	mBaseContentLayer = NULL;
	mSeasonId = seasonId;
	mBaseContentLayerTouchEff = true;
	mSectionId = sectionId;
	resMgr = ResManager::getManager();
	mWinSize = CCDirector::sharedDirector()->getWinSize();

	mFinger = CCArmature::create("STAR_FALL_UI_jiaoxue");
	mFinger->getAnimation()->play(Finger_Armature_Finger);
	addChild(mFinger,1);
	mFinger->setPosition(ccp(700,0));
}

CCSprite* BaseGuideLayer::createShadowArrow(const char *string)
{
	CCSprite* pArrow = CCSprite::create(string);
	CCSprite* pArrowShadow = CCSprite::create(string);
	pArrowShadow->setColor(ccBLACK);
	pArrow->addChild(pArrowShadow, -1, Tag_Shadow);
	pArrowShadow->setPosition(ccp(pArrow->getContentSize().width/2, pArrow->getContentSize().height/2 - 3));

	return pArrow;
}

CCLabelTTF* BaseGuideLayer::createShadowLabel(const char *string, const char *fontName, float fontSize/* = 40*/)
{
	CCLabelTTF* pLabel = CCLabelTTF::create(string, fontName, fontSize);
	pLabel->setAnchorPoint(ccp(0.5f, 0.0f));
	CCLabelTTF* pLabelShadow = CCLabelTTF::create(string, fontName, fontSize);
	pLabelShadow->setColor(ccBLACK);
	pLabel->addChild(pLabelShadow, -1, Tag_Shadow);
	pLabelShadow->setPosition(ccp(pLabel->getContentSize().width/2, pLabel->getContentSize().height/2 - 3));

	return pLabel;
}

void BaseGuideLayer::labelRunAction(CCLabelTTF* pTraget, CCAction* pAction)
{
	if (!pTraget)
	{
		return;
	}
	pTraget->runAction(pAction);
	if (CCLabelTTF* pLabel = (CCLabelTTF*)pTraget->getChildByTag(Tag_Shadow))
	{
		CCAction* p = (CCAction*)pAction->copy()->autorelease();
		pLabel->runAction(p);
	}
}

string BaseGuideLayer::getTxtImageFile(std::string file)
{
	return resMgr->getFilePathByLanguageType(g_gameinPath+file);
}
string BaseGuideLayer::getImageFile(std::string file)
{
	return resMgr->getSharedFilePath(g_gameinPath+file);
}	

void BaseGuideLayer::setBaseScene(BaseScense* scene)
{
	mBaseScene = scene;
}

void BaseGuideLayer::pauseBaseContentLayer()
{
	mBaseContentLayer->setTouchEnabled(false);
	mBaseContentLayer->setCCBladeVisible(false);
	mBaseContentLayer->setWaitingPause(true);
	SpriteHelper::pauseNodeAndChildren(mBaseContentLayer);

	SpriteHelper::pauseSubNodeAndChildrenInParentNodeByTag(mBaseContentLayer,Tag_Box_RedStar);

	// 暂停碎片
	{
// 		//SpriteHelper::pauseSubNodeAndChildrenInParentNodeByTag(BackgroudLayerFactory::getCurrentBgLayer(),Tag_Fragment);
// 		SpriteHelper::pauseSubNodeAndChildrenInParentNodeByTag(mBaseScene->getBackgroudCoverLayer(),Tag_Fragment);
		SpriteHelper::pauseSubNodeAndChildrenInParentNodeByTag(mBaseContentLayer,Tag_Fragment);
	}

	SpriteHelper::pauseSubNodeAndChildrenInParentNodeByTag(mBaseContentLayer,Tag_Box_Boiler);
}
void BaseGuideLayer::resumeBaseContentLayer()
{
	mBaseContentLayer->setTouchEnabled(true);
	SpriteHelper::resumeNodeAndChildren(mBaseContentLayer);
	mBaseContentLayer->setCCBladeVisible(true);
	mBaseContentLayer->setWaitingPause(false);

	SpriteHelper::resumeSubNodeAndChildrenInParentNodeByTag(mBaseContentLayer,Tag_Box_RedStar);
	// 恢复碎片
	{
// 		//SpriteHelper::resumeSubNodeAndChildrenInParentNodeByTag(BackgroudLayerFactory::getCurrentBgLayer(),Tag_Fragment);
// 		SpriteHelper::resumeSubNodeAndChildrenInParentNodeByTag(mBaseScene->getBackgroudCoverLayer(),Tag_Fragment);
		SpriteHelper::resumeSubNodeAndChildrenInParentNodeByTag(mBaseContentLayer,Tag_Fragment);
	}
	SpriteHelper::resumeSubNodeAndChildrenInParentNodeByTag(mBaseContentLayer,Tag_Box_Boiler);
}

void BaseGuideLayer::hideAll()
{
	CCArray* children = getChildren();
	int num = getChildrenCount();
	for (int i=0;i<num;i++)
	{
		CCNode* node = (CCNode*)children->objectAtIndex(i);
		node->stopAllActions();
		node->runAction(CCFadeTo::create(0.3f,0));
	}
}

void BaseGuideLayer::stopGuide()
{
	setVisible(false);
	setTouchEnabled(true);
}

void BaseGuideLayer::pause()
{
	setTouchEnabled(false);
	SpriteHelper::pauseNodeAndChildren(this);
}
void BaseGuideLayer::resume()
{
	setTouchEnabled(true);
	SpriteHelper::resumeNodeAndChildren(this);
}

void BaseGuideLayer::setBaseContentLayer(BaseContentLayer* layer)
{
	mBaseContentLayer = layer;
}
