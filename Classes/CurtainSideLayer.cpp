#include "CurtainSideLayer.h"
#include "ResManager.h"

CurtainSideLayer::CurtainSideLayer(void)
{
	init();

	CCSprite* left_side = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"curtain_bian_left.png").c_str());
	CCSprite* right_side = CCSprite::createWithTexture(left_side->getTexture());
	right_side->setFlipX(true);


	addChild(left_side);
	addChild(right_side);

	float x_offset = (getContentSize().width - 1024)/2.0f + 12;

	// 根据1024,768的外延，差多少就盖多少
	left_side->setPosition(ccp(-left_side->getContentSize().width/2 + x_offset,left_side->getContentSize().height/2));
	right_side->setPosition(ccp(getContentSize().width + right_side->getContentSize().width/2 - x_offset,right_side->getContentSize().height/2));
}


CurtainSideLayer::~CurtainSideLayer(void)
{
}
