#include "GuideLayer_JuqingDaoju.h"
#include "BaseScense.h"
#include "History.h"
#include "JuqingDaoJuManager.h"
#include "LevelManager.h"
// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_JuqingDaoju::GuideLayer_JuqingDaoju(int seasonId, int sectionId):BaseGuideLayer(seasonId,sectionId)
{
	m_pTipSpr = NULL;
	//如果本关卡已经获得剧情道具就不处理
	LevelManager* manager = LevelManager::getManager();
	JuqingDaoJu daoju;
	if(!JuqingDaoJuManager::getManager()->getItemInSection(manager->getLastSeasonId(),manager->getLastSectionId(),daoju))
		return;
	if(daoju.state != DJS_NotGetted)
		return;
	if (!CCUserDefault::sharedUserDefault()->getBoolForKey(Juqingdaoju_Teach_Is_Done, false))
	{
		m_pTipSpr = CCSprite::create(getTxtImageFile("teach_word_1_1_5.png").c_str());
		this->addChild(m_pTipSpr);
		m_pTipSpr->setScale(0.5f);
		m_pTipSpr->setPosition(ccp(650, 320));

		CCSprite* pArrow = CCSprite::create(getImageFile("teach_arrows.png").c_str());
		pArrow->setRotation(-30);
		m_pTipSpr->addChild(pArrow);
		pArrow->setPosition(ccp(m_pTipSpr->getContentSize().width, -pArrow->getContentSize().width/2));
	}
}

void GuideLayer_JuqingDaoju::fingerPosReady(CCNode* finger)
{
}

void GuideLayer_JuqingDaoju::onEnter()
{
 	BaseGuideLayer::onEnter();
// 
// 	// 下面的不要了
// 	return;
// 
// 	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-127,true);
}
void GuideLayer_JuqingDaoju::onExit()
{
	BaseGuideLayer::onExit();
//	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

// 必须重写，因为父类调用了setTouchEnabled
// 就会把现在的触摸禁掉了
void GuideLayer_JuqingDaoju::pause()
{
	//setTouchEnabled(false);
//	SpriteHelper::pauseNodeAndChildren(this);
}
void GuideLayer_JuqingDaoju::resume()
{
	//setTouchEnabled(true);
//	SpriteHelper::resumeNodeAndChildren(this);
}

bool GuideLayer_JuqingDaoju::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true; // 屏蔽
}
void GuideLayer_JuqingDaoju::doubleClickDown(CCPoint glPoint)
{
}
void GuideLayer_JuqingDaoju::lfClick(CCPoint glPoint)
{
}

void GuideLayer_JuqingDaoju::scaleBigRunning()
{
}

void GuideLayer_JuqingDaoju::showCanDrag(float time)
{
}

void GuideLayer_JuqingDaoju::baseContentLayerEffctiveClicked(CCNode* node)
{
}