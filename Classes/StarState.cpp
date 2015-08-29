#include "StarState.h"
#include "font.h"
#include "LFAlerts.h"
#include "ResManager.h"
#include "LevelManager.h"

#define STAR_STATE_TAG_ONE 1
#define STAR_STATE_TAG_TWO 2
#define STAR_STATE_TAG_THREE 3
#define STAR_STATE_TAG_ONE_BG 4
#define STAR_STATE_TAG_TWO_BG 5
#define STAR_STATE_TAG_THREE_BG 6

#define STAR_STATE_TAG_NUM 7
#define STAR_STATE_TAG_FINGER 8

StarState::StarState()
	:m_bIsThreeStarRemove(false),
	m_bIsSecondStarRemove(false)
{
	_sanxingCount = 0;
	_clickCount = 0;
	_lastStars = 3;
	_erxingCount = 0;
	_dynamicLayer = NULL;
}
StarState::~StarState(){
	_dynamicLayer = NULL;
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

StarState* StarState::create(unsigned int sanxingCount, unsigned int erxingCount)
{
	StarState* s = StarState::create();
	s->_sanxingCount = sanxingCount;
	s->_erxingCount = erxingCount;
	return s;
}

bool StarState::init(){
	if(!CCNode::init())
		return false;
	addChildren();

	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(StarState::useSuperTool), MSG_USE_SUPERTOOL, NULL);

	return true;
}

#define CREATE_STAR(_point_,_tag_)\
	star = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_xiaogk+"YX_start.png").c_str());\
	star->setPosition(_point_);\
	star->setTag(_tag_);\
	_dynamicLayer->addChild(star)

#define CREATE_STAR_BG(_point_,_tag_)\
	star = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_xiaogk+"YX_start_kong.png").c_str());\
	star->setPosition(_point_);\
	star->setTag(_tag_);\
	_dynamicLayer->addChild(star)

void StarState::addChildren(){
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	
	//动态层
	_dynamicLayer = CCNode::create();
	addChild(_dynamicLayer);

	int xoffset = winsize.width/2 + 120;

	//星星背板
	CCSprite* star;

	CREATE_STAR_BG(ccp(xoffset - 3*star->getContentSize().width, star->getContentSize().height),STAR_STATE_TAG_ONE_BG);

	CREATE_STAR_BG(ccp(xoffset - 2 *star->getContentSize().width, star->getContentSize().height),STAR_STATE_TAG_TWO_BG);

	CREATE_STAR_BG(ccp(xoffset - star->getContentSize().width, star->getContentSize().height),STAR_STATE_TAG_THREE_BG);
	
	//星星
	CREATE_STAR(ccp(xoffset - 3*star->getContentSize().width, star->getContentSize().height),STAR_STATE_TAG_ONE);
	m_pointOne = star->getPosition();
	CREATE_STAR(ccp(xoffset - 2 * star->getContentSize().width, star->getContentSize().height),STAR_STATE_TAG_TWO);
	m_pointTwo = star->getPosition();
	CREATE_STAR(ccp(xoffset - star->getContentSize().width, star->getContentSize().height),STAR_STATE_TAG_THREE);
	m_pointThree = star->getPosition();

}

unsigned int StarState::getLastStars(){
	return _lastStars;
}

#define CHECK_CLICK(_count_,_tag_,_max_) \
	if(_count_ > _max_){\
		star = _dynamicLayer->getChildByTag(_tag_);\
		if(star){\
		point.x = star->getPosition().x;\
		point.y = star->getPosition().y;\
		_dynamicLayer->removeChildByTag(_max_);\
		}\
	}else if(_count_ == _max_){\
		star = _dynamicLayer->getChildByTag(_tag_);\
		if(star){\
			star->stopAllActions();\
			star->runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(0.5f,0x88),CCFadeTo::create(0.5f,0xFF),NULL)));\
		}\
	}
void StarState::teach_scaleChange(int type)
{
	if (type == 0)
	{
		CCLabelAtlas* label = (CCLabelAtlas*)_dynamicLayer->getChildByTag(STAR_STATE_TAG_NUM);
		if(label)
		{
			label->stopAllActions();
			label->setScale(0.75f);
			label->runAction(CCRepeat::create(CCSequence::create(CCScaleTo::create(0.5f, 1.2f), CCScaleBy::create(0.5f,  0.75f), NULL), 2));
		}
		CCSprite* spr = (CCSprite*)_dynamicLayer->getChildByTag(STAR_STATE_TAG_FINGER);
		if(spr)
		{
			spr->stopAllActions();
			spr->setScale(0.5f);
			spr->runAction(CCRepeat::create(CCSequence::create(CCScaleTo::create(0.5f, 0.65f), CCScaleTo::create(0.5f,  0.50f), NULL), 2));
		}
	}
	else if (type == 1)
	{
		CCSprite* spr = (CCSprite*)_dynamicLayer->getChildByTag(STAR_STATE_TAG_ONE);
		if(spr)
		{
			spr->stopAllActions();
			spr->setScale(1.0f);
			spr->runAction(CCRepeat::create(CCSequence::create(CCScaleTo::create(0.5f, 1.1f), CCScaleTo::create(0.5f,  1.0f), NULL), 2));
		}
		spr = (CCSprite*)_dynamicLayer->getChildByTag(STAR_STATE_TAG_TWO);
		if(spr)
		{
			spr->stopAllActions();
			spr->setScale(1.0f);
			spr->runAction(CCRepeat::create(CCSequence::create(CCScaleTo::create(0.5f, 1.1f), CCScaleTo::create(0.5f,  1.0f), NULL), 2));
		}
		spr = (CCSprite*)_dynamicLayer->getChildByTag(STAR_STATE_TAG_THREE);
		if(spr)
		{
			spr->stopAllActions();
			spr->setScale(1.0f);
			spr->runAction(CCRepeat::create(CCSequence::create(CCScaleTo::create(0.5f, 1.1f), CCScaleTo::create(0.5f,  1.0f), NULL), 2));
		}
	}
}
void StarState::onClick(int effectiveClickNum)
{
	CCLog("effectiveClickNum is %d", effectiveClickNum);
	if(!_dynamicLayer)
		return;
	if(effectiveClickNum >= 99)
		return;
	_clickCount  = effectiveClickNum;

	//更新数量
	CCLabelAtlas* label = (CCLabelAtlas*)_dynamicLayer->getChildByTag(STAR_STATE_TAG_NUM);
	if(label)
	{
		label->stopAllActions();
		label->setScale(0.75f);
		label->setString(CCString::createWithFormat("%d",_clickCount)->getCString());
		label->runAction(CCRepeat::create(CCSequence::create(CCScaleTo::create(0.5f, 1.2f), CCScaleBy::create(0.5f,  0.75f), NULL), 2));
	}
	//手指动画
	CCSprite* spr = (CCSprite*)_dynamicLayer->getChildByTag(STAR_STATE_TAG_FINGER);
	if(spr)
	{
		spr->stopAllActions();
		spr->setScale(0.5f);
		spr->runAction(CCRepeat::create(CCSequence::create(CCScaleTo::create(0.5f, 0.60f), CCScaleTo::create(0.5f,  0.50f), NULL), 2));
	}

	this->checkClick();

//	this->checkClick(_clickCount, STAR_STATE_TAG_THREE, _sanxingCount);
//	this->checkClick(_clickCount, STAR_STATE_TAG_TWO, _sanxingCount);
// 	//检查评星
// 	CCNode* star;
// 	CCNode* estar = NULL;
// 	CCPoint point;
// 	CHECK_CLICK(_clickCount,STAR_STATE_TAG_THREE, _sanxingCount);
// 	CHECK_CLICK(_clickCount,STAR_STATE_TAG_TWO, _erxingCount);
// 	if(point.x != 0){
// 		estar = CCSprite::create("youxizhong_xingxing_kong.png");
// 		estar->setPosition(point);
// 		_dynamicLayer->addChild(estar);
// 	}
}

void StarState::checkClick()
{
	CCNode* star = NULL;
	CCNode* estar = NULL;

	if (_sanxingCount == _clickCount && (!m_bIsSecondStarRemove) && (!m_bIsThreeStarRemove))
	{
		/*star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_THREE);
		if (star)
		{
			showStarShake(star);
		}
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_TWO);
		if (star)
		{
			showStarShake(star);
		}
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_ONE);
		if (star)
		{
			showStarShake(star);
		}*/
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_THREE);
		if (star)
		{
			showStarShake(star);
		}
		
	}
	else if (( _sanxingCount + 1 ) == _clickCount && (!m_bIsThreeStarRemove))
	{
		/*CCNode *star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_ONE);
		if (star)
		{
			showStarShakeEnd(star);
		}
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_TWO);
		if (star)
		{
			showStarShakeEnd(star);
		}*/
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_THREE);
		if (star)
		{
			m_bIsThreeStarRemove = true;
			star->runAction(CCSequence::create(
				CCCallFuncN::create(this, callfuncN_selector(StarState::removeSanxing)),
				NULL));
		}
	}
	if (_erxingCount == _clickCount && (!m_bIsSecondStarRemove))
	{
		/*star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_TWO);
		if (star)
		{
			showStarShake(star);
		}
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_ONE);
		if (star)
		{
			showStarShake(star);
		}*/
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_TWO);
		if (star)
		{
			showStarShake(star);
		}
	}
	else if ((_erxingCount + 1 ) == _clickCount && (!m_bIsSecondStarRemove))
	{
		/*CCNode *star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_ONE);
		if (star)
		{
			showStarShakeEnd(star);
		}*/
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_TWO);
		if (star)
		{
			m_bIsSecondStarRemove = true;
			removeSecondxing(star);
		}
	}

	/*
	//如果三星只有一步时，点击数等于三星显示动画，此时还是三星
	if (_sanxingCount - 1 == 0 && _clickCount == _sanxingCount)
	{
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_THREE);
		if (star)
		{
			showStarShake(star);
		}
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_TWO);
		if (star)
		{
			showStarShake(star);
		}
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_ONE);
		if (star)
		{
			showStarShake(star);
		}
	}
	//如果三星只有一步时，点击数小于三星时，显示动画，然后移除三星
	else if (_sanxingCount - 1 == 0 && _clickCount < _sanxingCount)
	{
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_THREE);
		if (star)
		{
			star->runAction(CCSequence::create(
				CCCallFuncN::create(this, callfuncN_selector(StarState::removeSanxing)),
				NULL));
		}
	}
	//检查是否是在三星内，显示动画
	if (_clickCount == _sanxingCount - 1)
	{
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_THREE);
		if (star)
		{
			showStarShake(star);
			//star->runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(0.5f,0x88),CCFadeTo::create(0.5f,0xFF),NULL)));
		}
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_TWO);
		if (star)
		{
			showStarShake(star);
		}
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_ONE);
		if (star)
		{
			showStarShake(star);
		}
	}
	//检查是否是在二星内，显示动画
	else if (_clickCount == _erxingCount - 1 && _clickCount > _sanxingCount)
	{
	//	star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_TWO);
		if (star)
		{
			//showStarShake(star);
			//star->runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(0.5f,0x88),CCFadeTo::create(0.5f,0xFF),NULL)));
		}
	//	star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_ONE);
		if (star)
		{
			//showStarShake(star);
			//star->runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(0.5f,0x88),CCFadeTo::create(0.5f,0xFF),NULL)));
		}
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_THREE);
		if (star)
		{
			this->removeStar(star);
			//star->removeFromParent();
		}
		//star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_TWO);
		//if (star)
		{
			//showStarShake(star);
			//star->runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(0.5f,0x88),CCFadeTo::create(0.5f,0xFF),NULL)));
		}
		//estar = CCSprite::create("youxizhong_xingxing_kong.png");
		//estar->setPosition(m_pointThree);
		//_dynamicLayer->addChild(estar);
		
	}
	
	//是二星时，播放三星的动画，然后移除三星，播放二星的动画
	if (_clickCount == _erxingCount)
	{
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_TWO);
		if (star)
		{
			showStarShake(star);
			//star->runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(0.5f,0x88),CCFadeTo::create(0.5f,0xFF),NULL)));
		}
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_ONE);
		if (star)
		{
			showStarShake(star);
			//star->runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(0.5f,0x88),CCFadeTo::create(0.5f,0xFF),NULL)));
		}
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_THREE);
		if (star)
		{
			star->runAction(CCSequence::create(
				CCCallFuncN::create(this, callfuncN_selector(StarState::removeSanxing)),
				NULL));
		}
	}

	//连二星都没有，就直接删除其他的星
	if (_clickCount > _erxingCount)
	{
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_ONE);
		if (star)
		{
			showStarShakeEnd(star);
		}
		star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_TWO);
		if (star)
		{
			removeSecondxing(star);
			//star->removeFromParent();
		}

		//estar = CCSprite::create("youxizhong_xingxing_kong.png");
		//estar->setPosition(m_pointTwo);
		//_dynamicLayer->addChild(estar);
	}*/
}
void StarState::removeSecondxing(CCNode* pNode)
{
	removeStar(pNode);
	CCNode *star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_ONE);
	if (star)
	{
		showStarShakeEnd(star);
	}
}
void StarState::removeSanxing(CCNode* pNode)
{
	removeStar(pNode);
	
	//pNode->removeFromParent();
	//CCSprite* estar = CCSprite::create("youxizhong_xingxing_kong.png");
	//estar->setPosition(m_pointThree);
	//_dynamicLayer->addChild(estar);
	
}

void StarState::useSuperTool(CCObject* pObj)
{
	//使用了超能道具后设置为1星
	CCNode* star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_TWO);
	if (star && (!m_bIsSecondStarRemove))
	{
		m_bIsSecondStarRemove = true;
		this->removeStar(star);
		//star->removeFromParent();
	}
	star = _dynamicLayer->getChildByTag(STAR_STATE_TAG_THREE);
	if (star && (!m_bIsThreeStarRemove))
	{
		m_bIsThreeStarRemove = true;
		this->removeStar(star);
		//star->removeFromParent();
	}
	//CCSprite* estar = CCSprite::create("youxizhong_xingxing_kong.png");
	//estar->setPosition(m_pointThree);
	//_dynamicLayer->addChild(estar);

	//estar = CCSprite::create("youxizhong_xingxing_kong.png");
	//estar->setPosition(m_pointTwo);
	//_dynamicLayer->addChild(estar);
}
void StarState::showStarShake(CCNode *node)
{
	const float ROTATE = 7.0f;
	const float TIME = 0.05f;
	node->runAction(CCRepeatForever::create(CCSequence::create(
		CCRotateBy::create(TIME, ROTATE),
		CCRotateBy::create(TIME, -ROTATE),
		CCRotateBy::create(TIME, -ROTATE),
		CCRotateBy::create(TIME, ROTATE),
		CCRotateBy::create(TIME, ROTATE),
		CCRotateBy::create(TIME, -ROTATE),
		CCDelayTime::create(1.0f),
		NULL)));
}
void StarState::showStarShakeEnd(CCNode *node)
{
	node->stopAllActions();
	node->setRotation(0);
}
void StarState::removeStar(CCNode *node)
{
	const float ROTATE = 7.0f;
	const float TIME = 0.05f;
	node->stopAllActions();
	node->runAction(CCSequence::create(CCSpawn::create(CCSequence::create(
		CCRotateBy::create(TIME, ROTATE),
		CCRotateBy::create(TIME, -ROTATE),
		CCRotateBy::create(TIME, -ROTATE),
		CCRotateBy::create(TIME, ROTATE),
		CCRotateBy::create(TIME, ROTATE),
		CCRotateBy::create(TIME, -ROTATE),
		NULL),
		CCScaleTo::create(0.3f,1.15f),
		NULL),
		CCCallFuncN::create(this,SEL_CallFuncN(&StarState::removeThisStar)),
		CCDelayTime::create(0.3f),
		NULL));
}
void StarState::removeStarEnd(CCNode *node)
{
	node->removeFromParent();
}
void StarState::removeThisStar(CCNode *node)
{
	
	node->runAction(CCRepeatForever::create(CCRotateBy::create(2.0f, 900.0f)));
	node->runAction(CCSequence::create(CCSpawn::create(CCFadeOut::create(0.5f),
		CCScaleTo::create(0.5f,0),
		NULL),
		CCCallFuncN::create(this,SEL_CallFuncN(&StarState::removeStarEnd)),
		NULL));
	
}