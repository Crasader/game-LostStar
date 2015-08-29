#include "GuideLayer.h"
#include "MenuLayer.h"
#include "BaseScense.h"
#include "WaitTimeToClickLayer.h"
#include "VersionControl.h"
#include "font.h"
#include "LFStrings.h"
#include "VersionControl.h"
//=====================下面是对于教学的数据配置======================//

const int repeatTime = 2;

//把二维数组的两个下标分别称为行下标和列下标，在前面的是行下标，在后面的是列下标。
// 需要guide的关卡，第一个元素是seasonId，第二个是sectionId
const int guide_sections[][2] = {
	{1,1},
};
// 手指的位置
const CCPoint fingerPos[] = {
	ccp(512 - 38,768 - 380)
};

//===================================================================//


GuideLayer::GuideLayer(int seasonId,int sectionId):BaseGuideLayer(sectionId,sectionId)
{
	CCAssert(mSeasonId == 1,"");
	CCAssert(mSectionId == 1,"");

	state = 0;	// 代表只能点击最顶上的方块

	//needRepeatNum = repeatTime;
	needRepeatNum = 999999;

	guide_arrows_0 = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	guide_txt_0 = createShadowLabel(LFStrings::getValue("teach_word_1_1_1").c_str(), fontStr_KlavikaBold);
	addChild(guide_arrows_0);
	addChild(guide_txt_0);

	guide_arrows_1 = NULL;
	guide_txt_1 = NULL;
	spriteMask = NULL;
	
}

GuideLayer::~GuideLayer()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
void GuideLayer::guideTimmer()
{

}

void GuideLayer::guideLastOne()
{
	CCPoint dest = ccp(550,mWinSize.height - 430);

	CCSprite* guide_arrows = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	CCLabelTTF* guide_txt = createShadowLabel(LFStrings::getValue("teach_word_1_1_2").c_str(), fontStr_KlavikaBold);
	guide_txt->setAnchorPoint(ccp(0.5f, 0.0f));
	addChild(guide_arrows);
// 	guide_arrows->setRotation(-90);
// 	CCNode* pshadow = guide_arrows->getChildByTag(Tag_Shadow);
// 	if (pshadow)
// 	{
// 		pshadow->setPosition(ccpAdd(pshadow->getPosition(), ccp(-3, 3)));
// 	}

	addChild(guide_txt);
	guide_arrows->setPosition(ccp(dest.x + 100 ,dest.y - 35));
	guide_txt->setPosition(ccp(guide_arrows->getPositionX() + guide_arrows->getContentSize().width/2, guide_arrows->getPositionY() + guide_arrows->getContentSize().height/2 + 10));

	guide_arrows->runAction(CCSequence::create(
		CCDelayTime::create(5.0f),
		CCRemoveSelf::create(),
		NULL));
	guide_txt->runAction(CCSequence::create(
		CCDelayTime::create(5.0f),
		CCRemoveSelf::create(),
		NULL));

#if Version_Free != 0
	guide_arrows->setVisible(false);
	guide_txt->setVisible(false);
#endif
}
void GuideLayer::lfClick(CCPoint glPoint)
{
	switch (state)
	{
	case 1:
#if NEED_GAME_CD_TIME == 0
		guideLastOne();
		state++;
#else
		BaseScense::getCurrentScene()->getMenuLayer()->getGameMenu()->setEnabled(false);
		BaseScense::getCurrentScene()->getMenuLayer()->getGameMenu()->setVisible(false);
		scheduleOnce(schedule_selector(GuideLayer::pauseGame),0);
#endif
		break;
	case 2:
		resumeBaseContentLayer();
		BaseScense::getCurrentScene()->getMenuLayer()->getGameMenu()->setEnabled(true);
		BaseScense::getCurrentScene()->getMenuLayer()->getGameMenu()->setVisible(true);
		if(guide_arrows_1!=NULL)
			//guide_arrows_1->runAction(CCFadeTo::create(0.3f,0));
			guide_arrows_1->removeFromParent();
		if(guide_txt_1!=NULL)
			//guide_txt_1->runAction(CCFadeTo::create(0.3f,0));
			guide_txt_1->removeFromParent();
		if(spriteMask!=NULL)
			spriteMask->removeFromParentAndCleanup(true);

		guideLastOne();
		state ++;
		break;
	}
}
void GuideLayer::pauseGame(float t)
{
	pauseBaseContentLayer();

	BaseScense* scene = dynamic_cast<BaseScense*>(CCDirector::sharedDirector()->getRunningScene());
	CCAssert(scene!=NULL,"");


	// 聚光
	{
		spriteMask = CCSprite::create(getTxtImageFile("teach_word_1_1_2_cover.png").c_str());
		scene->getWaitingLayer()->addChild(spriteMask,-1);
		spriteMask->setPosition(ccp(mWinSize.width/2,mWinSize.height/2));

		// 时钟
		{
			CCPoint dest = ccp(mWinSize.width/2,mWinSize.height - 78);
			guide_arrows_1 = createShadowArrow(getImageFile("teach_arrows.png").c_str());
			guide_txt_1 = createShadowLabel(LFStrings::getValue("teach_word_1_1_2").c_str(), fontStr_KlavikaBold);
			scene->getWaitingLayer()->addChild(guide_arrows_1);
			guide_arrows_1->setRotation(90);
			scene->getWaitingLayer()->addChild(guide_txt_1);
			guide_arrows_1->setPosition(ccp(dest.x + 20,dest.y + 23 + 13 - 60 - 34));
			guide_txt_1->setPosition(ccp(dest.x + 100,guide_arrows_1->getPositionY() - guide_arrows_1->getContentSize().height - 10));
			//guide_txt_1->setOpacity(0);
			//guide_arrows_1->setOpacity(0);
		}
	}
	//guide_arrows_1->runAction(CCFadeTo::create(0.3f,255));
	//guide_txt_1->runAction(CCFadeTo::create(0.3f,255));
	state ++;
}
void GuideLayer::lfTouchesBegan(CCPoint glPoint)
{
	glPoint = convertToNodeSpace(glPoint);
	switch(state)
	{
		// 提示点击第一个方块
	case 0:
		// 只有点击最上面的方块才可以
		if (glPoint.x >= 490 && glPoint.x <= 535 && 
			glPoint.y <= mWinSize.height - 380 && glPoint.y >= mWinSize.height - 435)
		{
			setBaseContentLayerCurrentTouchEffectve(true);
//			mFinger->stopAllActions();
			guide_arrows_0->stopAllActions();
			guide_txt_0->stopAllActions();

//			mFinger->runAction(CCFadeTo::create(0.3f,0));
			mFinger->setVisible(false);

			//guide_arrows_0->runAction(CCFadeTo::create(0.3f,0));
			//labelRunAction(guide_txt_0, CCFadeTo::create(0.3f,0));
			guide_txt_0->removeFromParent();
			guide_arrows_0->removeFromParent();

			state ++;
		}else
		{
			setBaseContentLayerCurrentTouchEffectve(false);
		}
		break;
	}
	
}
void GuideLayer::lfTouchesEnded(CCPoint glPoint)
{

}

void GuideLayer::startGuide()
{
	int index = getGuideIndex();
	if (index == -1)
		return;
	pos = fingerPos[index];

	mFinger->getAnimation()->play(Finger_Armature_DianJi);

	oneGuide(0.5f,pos);
}

void GuideLayer::oneGuide(float delay,CCPoint endPos)
{
	CCPoint dest = endPos;

//	mFinger->stopAllActions();

	//mFinger->setPosition(ccpAdd(endPos,ccp(Finger_Pos_Offset_X - 17,Finger_Pos_Offset_Y + 13)));
// 	mFinger->runAction(CCSequence::create(
// 		CCDelayTime::create(delay),
// 		CCFadeTo::create(0.4f,255),
// 		NULL
// 		));

	//mFinger->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(1.0f,0.75f),CCScaleTo::create(1.0f,1.0f),NULL)));
	guide_txt_0->setAnchorPoint(ccp(0.5f, 0.0f));
	guide_arrows_0->setPosition(ccp(dest.x + 100, dest.y));

#if Version_Free != 0
	guide_arrows_0->setPosition(ccp(dest.x + 100, dest.y + 20));
	mFinger->setPosition(ccpAdd(endPos,ccp(Finger_Pos_Offset_X - 10,Finger_Pos_Offset_Y + 50)));
#else
	mFinger->setPosition(ccpAdd(endPos,ccp(Finger_Pos_Offset_X - 10,Finger_Pos_Offset_Y + 20)));
#endif

#if 0
	mFinger->runAction(CCSequence::create(
		CCDelayTime::create(delay),
		CCMoveTo::create(1.1f,ccpAdd(endPos,ccp(Finger_Pos_Offset_X - 10,Finger_Pos_Offset_Y + 20))),
		CCScaleTo::create(1.0f,0.75f),
		CCFadeOut::create(1.0f),
		CCScaleTo::create(1.0f,1.0f),
		CCCallFuncN::create(this,SEL_CallFuncN(&GuideLayer::fingerPosReady)),NULL));
#endif
	guide_txt_0->setPosition(ccp(guide_arrows_0->getPositionX() + guide_arrows_0->getContentSize().width/2 - 10,guide_arrows_0->getPositionY() + guide_arrows_0->getContentSize().height/2 + 10));

}
void GuideLayer::fingerPosReady(CCNode* finger)
{
}
void GuideLayer::oneGuideEnd(CCNode* fingerLight)
{
	needRepeatNum --;
	mFinger->setVisible(false);
	mFinger->setPosition(ccp(700,0));

	if (needRepeatNum > 0)
	{
		oneGuide(0.8f,pos);
	}else
	{
		setTouchEnabled(false);
		setVisible(false);
	}
}

int GuideLayer::getGuideIndex()
{
	// 总共元素的多少
	int itemNum = sizeof(guide_sections)/sizeof(int);
	int colNum = sizeof(guide_sections[0])/sizeof(int);
	int rowNum = itemNum / colNum;

	CCAssert(sizeof(fingerPos)/sizeof(CCPoint) >= rowNum,"手指的位置个数一定不能少于需要教学的关卡数");

	for (int i=0;i<rowNum;i++)
	{
		if (guide_sections[i][0] == mSeasonId && guide_sections[i][1] == mSectionId)
		{
			return i;
		}
	}
	return -1;
}