#include "BaseShakeLayer.h"
#include "CommonFunc.h"
BaseShakeLayer::BaseShakeLayer()
	:CCLayer(),
	m_nNodeCount(0)
{
}
BaseShakeLayer::~BaseShakeLayer()
{
}
CCScene* BaseShakeLayer::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	BaseShakeLayer *layer = BaseShakeLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}
bool BaseShakeLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	this->ignoreAnchorPointForPosition(false);
	this->setAnchorPoint(ccp(0.5,0.5));
	this->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2,CCDirector::sharedDirector()->getWinSize().height / 2));
	return true;
}
void BaseShakeLayer::onEnter()
{
	CCLayer::onEnter();
	
	showShake();
}
void BaseShakeLayer::onExit()
{
	CCLayer::onExit();
	m_sColorLayer->runAction(CCSequence::create(CCFadeTo::create(0.3f,0),
		CCCallFuncN::create(this,SEL_CallFuncN(&BaseShakeLayer::removeLayerColor)),
		NULL));
}
void BaseShakeLayer::removeLayerColor(CCNode *node)
{
	node->getParent()->removeChild(node);
}
void BaseShakeLayer::closeMyself()
{

}
void BaseShakeLayer::showShake()
{
	int win_width =  CCDirector::sharedDirector()->getWinSize().width;
	int win_height =  CCDirector::sharedDirector()->getWinSize().height;
	ccColor4B color = {0, 0, 0, 255};
	CCLayerColor *colorLayer = CCLayerColor::create(color);
	colorLayer->ignoreAnchorPointForPosition(false);
	colorLayer->setAnchorPoint(ccp(0.5,0.5));
	colorLayer->setPosition(ccp(win_width / 2 , win_height / 2));
	colorLayer->setContentSize(CCSizeMake(win_width,win_height));
	colorLayer->setOpacity(0);
	this->getParent()->addChild(colorLayer,this->getZOrder() - 1);

	this->setScale(0.2f);
	this->runAction(CCSequence::create(
		CCFadeOut::create(0),
		CCSpawn::create(CCFadeIn::create(0.15f),
		CCScaleTo::create(0.3f,1.05f),
		NULL),
		//CCScaleTo::create(0.15f,0.9f),
		//CCScaleTo::create(0.15f,1.05f),
		CCScaleTo::create(0.1f,1.0f),
		NULL));
	colorLayer->runAction(CCFadeTo::create(0.3f,100));
	m_sColorLayer = colorLayer;
}
void BaseShakeLayer::replaceLayer(CCNode *outNode,CCNode *inNode,CCObject *obj,SEL_CallFuncN handler)
{
	/*
	FX::nodeAndAllChildRunAction(outNode, CCSequence::create(
		CCSpawn::create(CCFadeOut::create(0.3f), NULL),
		obj!=NULL ?  CCCallFuncN::create(obj,handler) : NULL,
		NULL));
	outNode->runAction(CCScaleTo::create(0.5f, 0.8f));
	outNode->runAction(CCSequence::create(CCDelayTime::create(0.3f), CCHide::create(), NULL));

	FX::setAllChildrenColorOrOpacity(inNode, ccWHITE, 0, 2);
	inNode->setScale(0.2f);
	inNode->runAction(CCScaleTo::create(0.5f, 1.0f));
	FX::nodeAndAllChildRunAction(inNode, CCSequence::create(
		CCDelayTime::create(0.3f),
		CCSpawn::create(CCFadeIn::create(0.3f),NULL),
		NULL));
	inNode->runAction(CCSequence::create(CCDelayTime::create(0.6f), CCShow::create(), NULL));*/

	nodeAndAllChildRunAction(outNode, CCSequence::create(
		CCSpawn::create(CCFadeOut::create(0.3f), NULL),
		NULL),obj,handler);
	//outNode->runAction(CCScaleTo::create(0.5f, 0.8f));
	outNode->runAction(CCSequence::create(CCDelayTime::create(0.3f), CCHide::create(), NULL));

	FX::setAllChildrenColorOrOpacity(inNode, ccWHITE, 0, 2);
	//inNode->setScale(0.2f);
	//inNode->runAction(CCScaleTo::create(0.5f, 1.0f));
	nodeAndAllChildRunAction(inNode, CCSequence::create(
		CCDelayTime::create(0.3f),
		CCSpawn::create(CCFadeIn::create(0.3f),NULL),
		NULL));
	inNode->runAction(CCSequence::create(CCDelayTime::create(0.6f), CCShow::create(), NULL));
	m_nNodeCountList.clear();
}
void BaseShakeLayer::nodeAndAllChildRunAction(CCNode* node, CCActionInterval* action,CCObject *obj,SEL_CallFuncN handler)
{
	m_nNodeCount++;
	if(node == NULL || action == NULL)
		return;
	//childe
	CCArray* childs = node->getChildren();
	if (childs != NULL)
	{
		for (int i=0; i!=childs->count(); ++i)
		{
			nodeAndAllChildRunAction((CCNode*)childs->objectAtIndex(i), (CCActionInterval*)action->copy()->autorelease());
		}
	}
	if (m_nNodeCount == 1 && obj)
	{
		node->runAction(CCSequence::create(action,
			CCCallFuncN::create(obj,handler),
			NULL));
	}
	else
	{
		//自己
		node->runAction(action);
	}
	m_nNodeCount--;
}