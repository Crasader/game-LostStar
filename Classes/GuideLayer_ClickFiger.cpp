#include "GuideLayer_ClickFinger.h"
#include "LFToast.h"
#include "LFStrings.h"
#include "font.h"
#include "MenuLayer.h"
#include "History.h"
#include "VersionControl.h"
// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_ClickFinger::GuideLayer_ClickFinger(int seasonId, int sectionId)
	:BaseGuideLayer(seasonId,sectionId)
{
    m_sGuide_arrowsSpr = NULL;
	m_tLabel = NULL;
	m_pBg = NULL;

}

void GuideLayer_ClickFinger::startGuide()
{
	if (!CCUserDefault::sharedUserDefault()->getBoolForKey(Show_Guide_1_2_Teach, true))
	{
		return;
	} 
	m_bIsTechEnd = false;
	//背景
	m_pBg = CCScale9Sprite::create(getImageFile("jiaoxue_wenzikuang.png").c_str());
	this->addChild(m_pBg, 1);

	m_sGuide_arrowsSpr = createShadowArrow(getImageFile("teach_arrows.png").c_str());
	this->addChild(m_sGuide_arrowsSpr);
#if Version_Free != 0
	m_sGuide_arrowsSpr->setPosition(ccp(512 + m_sGuide_arrowsSpr->getContentSize().width/2 + 85, m_sGuide_arrowsSpr->getContentSize().height/2 + 25));
#else	
	m_sGuide_arrowsSpr->setPosition(ccp(512 + m_sGuide_arrowsSpr->getContentSize().width/2 + 90, m_sGuide_arrowsSpr->getContentSize().height/2 + 60));
#endif

	m_tLabel = createShadowLabel(LFStrings::getValue("teach_word_1_2").c_str(), fontStr_KlavikaBold);
	m_tLabel->removeChildByTag(Tag_Shadow);
	m_pBg->addChild(m_tLabel, 1);
	m_tLabel->setAnchorPoint(ccp(0.5f, 0.5f));
	m_pBg->setContentSize(CCSizeMake(m_tLabel->getContentSize().width + 10, m_pBg->getContentSize().height));
	m_tLabel->setPosition(ccp(m_pBg->getContentSize().width/2, m_pBg->getContentSize().height/2));

	m_pBg->setPosition(ccp(m_sGuide_arrowsSpr->getPositionX() + m_sGuide_arrowsSpr->getContentSize().width/2, m_sGuide_arrowsSpr->getPositionY()  + m_sGuide_arrowsSpr->getContentSize().height/2 + m_pBg->getContentSize().height/2 + 10));

	schedule(SEL_SCHEDULE(&GuideLayer_ClickFinger::techUpdata), 3.0f);
	CCLayerColor* pColor = CCLayerColor::create(ccc4(0.0f, 0.0f, 0.0f, 125.0f));
    pColor->setScale(2.0f);
	this->addChild(pColor, -1);

	BaseScense::getCurrentScene()->setCollisionEnable(false);
	BaseScense::getCurrentScene()->getMenuLayer()->getGameMenu()->setEnabled(false);
	BaseScense::getCurrentScene()->getMenuLayer()->getGameMenu()->setVisible(false);
}

void GuideLayer_ClickFinger::techUpdata(float dt)
{
	m_bIsTechEnd = true;
	unschedule(SEL_SCHEDULE(&GuideLayer_ClickFinger::techUpdata));
}

void GuideLayer_ClickFinger::lfClick(CCPoint glPoint)
{
	if (!m_pBg)
	{
		return;
	}

    if (m_bIsTechEnd)
    {
        this->removeAllChildren();
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
        this->runAction(CCSequence::create(CCDelayTime::create(0.01f),
                                           CCCallFuncN::create(this, callfuncN_selector(GuideLayer_ClickFinger::openTouched)),
                                           NULL));
        CCUserDefault::sharedUserDefault()->setBoolForKey(Show_Guide_1_2_Teach, true);
        CCUserDefault::sharedUserDefault()->flush();
    }
    else
    {
        float scale = 1.0f;
        m_pBg->stopAllActions();
        m_pBg->runAction(CCSequence::create(
                                            CCScaleTo::create(0.1f,scale+0.1f),
                                            CCScaleTo::create(0.1f,scale-0.1f),
                                            CCScaleTo::create(0.05f,scale),
                                            NULL
                                            ));
    }

}

void GuideLayer_ClickFinger::openTouched(CCNode* pNode)
{
    BaseScense::getCurrentScene()->setCollisionEnable(true);


	BaseScense::getCurrentScene()->getMenuLayer()->getGameMenu()->setEnabled(true);
	BaseScense::getCurrentScene()->getMenuLayer()->getGameMenu()->setVisible(true);
}