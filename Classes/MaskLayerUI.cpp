#include "MaskLayerUI.h"
#include "CircleSprite.h"
#include "ResManager.h"

MaskLayerUI::MaskLayerUI()
	:CCLayer()
	,m_nTouchPriority(-998)
{
}
MaskLayerUI::~MaskLayerUI()
{
}
bool MaskLayerUI::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	return true;
}
void MaskLayerUI::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,m_nTouchPriority - 1,true);
	CCSize size = this->getContentSize();
	CCSprite *spr = CCSprite::createWithTexture(CCSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"jiaoxue_yuanquan.png").c_str())->getTexture());
	spr->setPosition(m_pCircleCenterPos);
	addChild(spr);
	
	//m_lNodeList.push_back(spr);
	ccColor4B color = {0,0,0,255};
	CCLayerColor *colorLayer1 = CCLayerColor::create(color);
	colorLayer1->setPosition(CCPointZero);
	colorLayer1->setContentSize(CCSizeMake(m_pCircleCenterPos.x - spr->getContentSize().width / 2,size.height));

	CCLayerColor *colorLayer2 = CCLayerColor::create(color);
	colorLayer2->setPosition(ccp(m_pCircleCenterPos.x + spr->getContentSize().width / 2,0));
	colorLayer2->setContentSize(CCSizeMake(size.width - m_pCircleCenterPos.x - spr->getContentSize().width / 2,size.height));

	CCLayerColor *colorLayer3 = CCLayerColor::create(color);
	colorLayer3->setPosition(ccp(m_pCircleCenterPos.x - spr->getContentSize().width / 2,0));
	colorLayer3->setContentSize(CCSizeMake(spr->getContentSize().width,m_pCircleCenterPos.y - spr->getContentSize().height / 2));

	CCLayerColor *colorLayer4 = CCLayerColor::create(color);
	colorLayer4->setPosition(ccp(m_pCircleCenterPos.x - spr->getContentSize().width / 2,m_pCircleCenterPos.y + spr->getContentSize().height / 2));
	colorLayer4->setContentSize(CCSizeMake(spr->getContentSize().width,size.height - m_pCircleCenterPos.y - spr->getContentSize().height / 2));

	m_lNodeList.push_back(colorLayer1);
	m_lNodeList.push_back(colorLayer2);
	m_lNodeList.push_back(colorLayer3);
	m_lNodeList.push_back(colorLayer4);

	addChild(colorLayer1);
	addChild(colorLayer2);
	addChild(colorLayer3);
	addChild(colorLayer4);
	spr->setOpacity(125);
	for (vector<CCLayerColor*>::iterator iter = m_lNodeList.begin();iter != m_lNodeList.end();iter ++)
	{
		(*iter)->setOpacity(125);
	}
	m_nRadius = spr->getContentSize().width / 2;
}

void MaskLayerUI::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}
bool MaskLayerUI::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint pos = this->convertToNodeSpace(pTouch->getLocation());
	
	int r = ( pos.x - m_pCircleCenterPos.x ) * ( pos.x - m_pCircleCenterPos.x ) + ( pos.y - m_pCircleCenterPos.y ) * ( pos.y - m_pCircleCenterPos.y );
	if (r > m_nRadius * m_nRadius)
	{
		
		return true;
	}
	else
	{
		//this->removeFromParent();
		return false;
	}
}
/****************添加圆***************************/
void MaskLayerUI::addCircle(CCPoint centerPos)
{
	m_pCircleCenterPos = centerPos;
}