#include "GuideLayer_Scale.h"
#include "BaseContentWrapLayer.h"
#include "BaseScense.h"
#include "History.h"
#include "LFToast.h"
#include "LFStrings.h"
#include "font.h"
GuideLayer_Scale::GuideLayer_Scale(void)
{
}


GuideLayer_Scale::~GuideLayer_Scale(void)
{
}


void GuideLayer_Scale::onEnter()
{
	CCLayerColor::onEnter();
}

void GuideLayer_Scale::onExit()
{
	CCLayerColor::onExit();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

bool GuideLayer_Scale::init()
{
	//add by jj
	//屏蔽缩放教学
	return false;

	ccColor4B color = {0, 0, 0, 125};

	if (!CCLayerColor::initWithColor(color))
	{
		return false;
	}

// 	BaseScense::getCurrentScene()->setCollisionEnable(false);
// 
//     m_iCallbackTimes = 0;
// 
// 	const int sumLittle = 200;
// 	{
// 		CCLabelTTF *pTipStr = CCLabelTTF::create(LFStrings::getValue("FingerChangeBig").c_str(), fontStr_katong,30.0f);//, CCSize(270,0), kCCTextAlignmentCenter);
// 		pTipStr->setColor(ccBLACK);
// 		pTipStr->setHorizontalAlignment(kCCTextAlignmentLeft);
// 		addChild(pTipStr);
// 		pTipStr->setPosition(ccp(512 - sumLittle + 130, this->getContentSize().height - pTipStr->getContentSize().height - 240));
// 		m_tTipStr = pTipStr;
// 		//LFToast::makeToast(LFStrings::getValue("FingerChangeBig"),3.0f);
// 	}
// 	//,CCPointMake(512 - sumLittle, this->getContentSize().height - 220)
// 	//提示字符串图片
// 	/*CCSprite* pTipStr = CCSprite::create("teach_word_5_2_zh.png");
// 	this->addChild(pTipStr);
// 	pTipStr->setPosition(ccp(512 - sumLittle, this->getContentSize().height - pTipStr->getContentSize().height - 220));
// 	*/
// 	//手指放大
// 	m_pScaleBig = CCSprite::create("finger_kuoda_1.png");
// 	m_pScaleBig->setPosition(ccp(512 - 20, this->getContentSize().height/2 + 20));
// 	this->addChild(m_pScaleBig);
// 	m_pScaleBig->setOpacity(0);
//     //m_pScaleBig->setVisible(false);
//     //手指缩小
// 	m_pScaleSmall = CCSprite::create("finger_suoxiao_1.png");
// 	m_pScaleSmall->setPosition(ccp(m_pScaleBig->getPositionX() + sumLittle - 20, m_pScaleBig->getPositionY() + 20));
// 	this->addChild(m_pScaleSmall);
//     m_pScaleSmall->setVisible(false);
// 	m_pScaleSmall->setOpacity(0);
// 	CCAnimation* pAni = CCAnimation::create();
// 	for (int i = 0; i < 5; ++i)
// 	{
// 		pAni->addSpriteFrameWithFileName(CCString::createWithFormat("finger_kuoda_%d.png", i+1)->getCString());
// 	}
// 	pAni->setLoops(1);
// 	pAni->setDelayPerUnit(0.1f);
// 	m_pScaleBig->runAction(CCRepeatForever::create(
// 		CCSequence::create(CCSpawn::create(CCFadeIn::create(0.5f),
// 		CCAnimate::create(pAni),NULL),
// 		CCDelayTime::create(1.0f),
// 		CCFadeOut::create(0.5f),
// 		CCDelayTime::create(1.0f),
// 		NULL)));
// 
// 	pAni = CCAnimation::create();
// 	for (int i = 0; i < 5; ++i)
// 	{
// 		pAni->addSpriteFrameWithFileName(CCString::createWithFormat("finger_suoxiao_%d.png", i+1)->getCString());
// 	}
// 	pAni->setLoops(1);
// 	pAni->setDelayPerUnit(0.1f);
// 	m_pScaleSmall->runAction(CCRepeatForever::create(
// 		CCSequence::create(CCSpawn::create(CCFadeIn::create(0.5f),
// 		CCAnimate::create(pAni),NULL),
// 			CCDelayTime::create(1.0f),
// 			CCFadeOut::create(0.5f),
// 			CCDelayTime::create(1.0f),
// 			NULL)));
// 
//     CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GuideLayer_Scale::scaleEnd), MSG_FIXED_SCALE_END, NULL);

	return true;
}

void GuideLayer_Scale::scaleEnd(CCObject* pObj)
{
    m_iCallbackTimes++;
    if (m_iCallbackTimes == 1)
    {
        if (m_pScaleBig)
        {
            m_pScaleBig->removeFromParent();
            m_pScaleBig = NULL;
            m_pScaleSmall->setVisible(true);
			m_tTipStr->setString(LFStrings::getValue("FingerChangeSmall").c_str());
			//LFToast::makeToast(LFStrings::getValue("FingerChangeSmall"),3.0f);
        }
    }
    else if (m_iCallbackTimes == 2)
    {
		CCUserDefault::sharedUserDefault()->setBoolForKey(Scale_Teach_Is_Done, true);
		CCUserDefault::sharedUserDefault()->flush();
		BaseScense::getCurrentScene()->setCollisionEnable(true);
        this->removeFromParent();
    }
}