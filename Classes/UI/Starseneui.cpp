#include "Starseneui.h"
#include "Biguangka.h"
#include "Starmain.h"
#include "Acount.h"
#include "Xiaocheckpoint.h"
#include "Stopgame.h"
#include "Gameset.h"
#include "Guankamsg.h"
#include "LevelManager.h"
#include "Achive.h"
#include "Biguangka.h"
#include "Shieldingmanger.h"
#include "Music.h"
#include "VersionControl.h"
#include "GameLayerFactory.h"

const Layertag Starseneui::UI_LAYERS[UI_LAYER_NUM] = 
{
	Starmain_tag,
	Bigcheckpoint_tag,
	Smalllayer_tag
};

Starseneui::Starseneui(void):
	mCurLayerIndex(0),
	mLastTouchTime(0L),
	mLayerTouchMovingVertical(false),
	mLayerTouchMoving(false),
	mAngleDir(-1),
	mIsInVerticalLayer(true),
	mLayerActionMoving(false),
    mTouchID(-1),
	m_bMoveActionOver(true)
{
	memset(mLayerPosNext, 0, sizeof(mLayerPosNext));
	memset(mLayerPos, 0, sizeof(mLayerPos));
	
	Music::sharedMusicPlayer()->setLastPlaySeasondBgMusicId(0);
	Music::sharedMusicPlayer()->playMainMusic();

	//ui chnage 
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(Starseneui::switchStateChanged), MSG_UI_ChANGE, NULL);
}


Starseneui::~Starseneui(void)
{
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void Starseneui::GoBiggk(int tag){

	if(tag==Bigcheckpoint_tag){

		this->focusLayer(Smalllayer_tag);
#if 0
		Myobject *object=new Myobject();
		object->parent_id=Starmain_tag;
		object->current_id=Bigcheckpoint_tag;
		object->season=LevelManager::getManager()->getLastSeasonId();
		object->mark=true;
		object->time = 0.0f;
		object->pCurNode = this->getChildByTag(Starmain_tag);
		object->autorelease();
		switchStateChanged(object);
#endif
	}
}

void Starseneui::Goxiaogk(int tag){

	if(tag==Smalllayer_tag){

		this->focusLayer(Smalllayer_tag);
#if 0
		Myobject *object=new Myobject();
		object->parent_id=Bigcheckpoint_tag;
		object->current_id=Smalllayer_tag;
		object->season=LevelManager::getManager()->getLastSeasonId();
		object->mark=true;
		object->time = 0.0f;
		object->pCurNode = this->getChildByTag(Starmain_tag);
		object->autorelease();
		switchStateChanged(object);
#endif
	}
}

void Starseneui::doHorizentalTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	for(int i = 0; i < UI_LAYER_NUM; i++)
	{
		if(i == mCurLayerIndex)
		{
			Layertag layerId = UI_LAYERS[i];
			CCLayer* layer =(CCLayer*)this->getChildByTag(layerId);
			if(layer != NULL)
			{
				layer->ccTouchBegan(pTouch, pEvent);
			}
			break;
		}
	}
}
void Starseneui::doHorizentalTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	for(int i = 0; i < UI_LAYER_NUM; i++)
	{
		if(i == mCurLayerIndex)
		{
			Layertag layerId = UI_LAYERS[i];
			CCLayer* layer =(CCLayer*)this->getChildByTag(layerId);
			if(layer != NULL)
			{
				layer->ccTouchMoved(pTouch, pEvent);
			}
			break;
		}
	}
}
void Starseneui::doHorizentalTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	for(int i = 0; i < UI_LAYER_NUM; i++)
	{
		if(i == mCurLayerIndex)
		{
			Layertag layerId = UI_LAYERS[i];
			CCLayer* layer =(CCLayer*)this->getChildByTag(layerId);
			if(layer != NULL)
			{
				layer->ccTouchEnded(pTouch, pEvent);
			}
			break;
		}
	}
}

bool Starseneui::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){

    if(mTouchID >= 0 && pTouch->getID() != mTouchID)
    {
        return false;
    }

	mTouchID = pTouch->getID();
	mAngleDir = -1;
	doHorizentalTouchBegan(pTouch, pEvent);
	if(!mIsInVerticalLayer)
	{
		mTouchID = -1;
		return false;
	}

	mStartTouchPoint = pTouch->getLocation();
	mLayerTouchMoving = false;
	mLayerActionMoving = false;
	mLayerTouchMovingVertical = false;
	Shieldingmanger::getSielding()->setHorizontalActive(false);

	struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	mLastTouchTime = (double)now.tv_sec + (double)now.tv_usec / 1000000.0f;

	for(int i = 0;i<UI_LAYER_NUM;i++)
	{
		CCNode* layer = this->getChildByTag(UI_LAYERS[i]);
		CCAction* action = layer->getActionManager()->getActionByTag(UI_Action_Move_Tag, layer);
		if(action != NULL && !action->isDone())
		{
			layer->stopAllActions();
			mLayerActionMoving = true;
		}
		else
		{
			//需要等到移动完成了才记录位置,不然会导致在mainlayer没有移动完成时就记录了
			if (m_bMoveActionOver)
			{
				mLayerPos[i]	  = layer->getPosition();
				mLayerPosNext[i]  = mLayerPos[i];
			}
		}

	}

	

	return true;
}
void Starseneui::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

	CCPoint point = pTouch->getLocation();
	CCPoint deltaPoint = point - mStartTouchPoint;

	float dis = point.getDistance(mStartTouchPoint);
	//移动距离大于某值，才开始跟着移动

	const float DISTANCE_TO_MOVE = 10.0f; //指定像素距离后才判断是水平或垂直
	const float V_ANGLE_MIN = 45.0f; //判断是否垂直移动的起始角度
	float angle =  abs( CC_RADIANS_TO_DEGREES(deltaPoint.getAngle()) );

	bool angleVertical = (angle >V_ANGLE_MIN && angle < 180.0f - V_ANGLE_MIN);

#if Show_End_BigGuanQia != 0
	if (angleVertical && LevelManager::getManager()->getLastSeasonId() == (GameLayerFactory::getInstance()->getValidSeasonNum()))
	{
		if ((Bigcheckpoint_tag == Shieldingmanger::getSielding()->getCureentlayertag()) && deltaPoint.y >= 0)
		{
			return;
		}
	}
#endif

	if(!mLayerTouchMoving && !mLayerTouchMovingVertical)
	{
		mAngleDir = angleVertical?0:1;
	}

	if(mAngleDir == 0)
	{
		if(dis > DISTANCE_TO_MOVE && !mLayerTouchMovingVertical)
		{
			mLayerTouchMoving = true;
			mLayerTouchMovingVertical = false;
		}
	}
	else if(mAngleDir == 1)
	{
		if(dis > DISTANCE_TO_MOVE) // && !mLayerTouchMoving)
		{
			//mLayerTouchMoving = false;
			mLayerTouchMovingVertical = true;
			Shieldingmanger::getSielding()->setHorizontalActive(true);
			doHorizentalTouchMoved(pTouch, pEvent);
			return; //视为非垂直移动，不动
		}		
	}
	

	if(mLayerTouchMoving 
		&& !(mCurLayerIndex==0 && deltaPoint.y < 0 || (mCurLayerIndex == UI_LAYER_NUM - 1) && deltaPoint.y > 0 ))
	{
		float dy = deltaPoint.y;

		for(int i = 0; i < UI_LAYER_NUM; i++)
		{
			Layertag layerId = UI_LAYERS[i];
			CCNode* layer = this->getChildByTag(layerId);
			if(layer != NULL)
			{
				layer->setPositionY(mLayerPos[i].y + dy);
			}
		}
	}
}

void Starseneui::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

	doHorizentalTouchEnded(pTouch, pEvent);
    mTouchID = -1;
	if(!mLayerTouchMoving && !mLayerActionMoving)
	{
		return;
	}
	CCPoint point = pTouch->getLocation();

	CCPoint deltaPoint = point - mStartTouchPoint;
	float dis = point.getDistance(mStartTouchPoint);

	struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	double nowTime = (double)now.tv_sec + (double)now.tv_usec / 1000000.0f;
	double deltaTime = (double)(nowTime - mLastTouchTime);
	float vel = dis / (float)deltaTime * 1.0f; //TODO: 1.0f替换成（PPI / 标准PPI）
	const float MOVE_SPEED_MIN = 900.0f;
	//CCLog("vel=%f, dis=%f, deltaTime=%f nowTime=%f, mLastTouchTime=%f\n", vel, dis, deltaTime, nowTime, mLastTouchTime);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	const float MOVE_DIS_MIN = size.height/2;
	if(mAngleDir == 0 
		&& (vel > MOVE_SPEED_MIN || abs(deltaPoint.y) > MOVE_DIS_MIN))
	{
		setLayerNextAction(deltaPoint.y > 0);
	}

	actionMoveLayer();

	mLayerTouchMoving = false;
	mLayerTouchMovingVertical = false;
	mStartTouchPoint = point;
	Shieldingmanger::getSielding()->setHorizontalActive(false);
	

}

void Starseneui::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){

	CCPoint point = pTouch->getLocation();
	ccTouchEnded(pTouch, pEvent);

}

void Starseneui::resetLayerLocation()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	for(int i = 0;i<UI_LAYER_NUM;i++)
	{
		float posY = -( i * size.height);
		CCNode* layer = this->getChildByTag(UI_LAYERS[i]);
		layer->setPosition(0, posY);
		mLayerPos[i].y	   = posY;
		mLayerPosNext[i]   = mLayerPos[i];
	}
}

void Starseneui::focusLayer(Layertag tag)
{
	int index = -1;
	for(int i = 0;i < UI_LAYER_NUM;i++)
	{
		if(UI_LAYERS[i] == tag)
		{
			index = i;
			break;
		}
	}

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	if(index >= 0)
	{
		for(int i = 0;i < UI_LAYER_NUM;i++)
		{
			float posY = -( (i - index) * size.height);
			CCNode* layer = this->getChildByTag(UI_LAYERS[i]);
			layer->setPositionY(posY);

			mLayerPos[i].y	  = posY;
			mLayerPosNext[i] = mLayerPos[i];
		}
		mCurLayerIndex = index;
		Shieldingmanger::getSielding()->setCurrentlayertag(UI_LAYERS[mCurLayerIndex]);
	}
}

void Starseneui::setLayerNextAction(bool isUp)
{
	int sign = isUp ? 1 : -1;

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	if((sign > 0 && mCurLayerIndex == 0) || (sign < 0 && mCurLayerIndex == UI_LAYER_NUM - 1) || (mCurLayerIndex > 0 && mCurLayerIndex < UI_LAYER_NUM - 1) )
	{
		for(int i = 0;i<UI_LAYER_NUM;i++)
		{
			mLayerPosNext[i].y = mLayerPosNext[i].y + size.height * sign;
			mLayerPos[i]     = mLayerPosNext[i];
		}
		mCurLayerIndex += sign;
		Shieldingmanger::getSielding()->setCurrentlayertag(UI_LAYERS[mCurLayerIndex]);
	}
}
void Starseneui::actionMoveLayer()
{
	m_bMoveActionOver = false;
	const float MOVE_TIME = 0.4f;
	bool bCalled = false;
	for( int i = 0;i<UI_LAYER_NUM;i++)
	{
		CCNode* layer = this->getChildByTag(UI_LAYERS[i]);
		if(layer != NULL)
		{
			CCCallFuncN* pCallFuncN = NULL;
			if (!bCalled)
			{
				bCalled = true;
				pCallFuncN = CCCallFuncN::create(this, callfuncN_selector(Starseneui::mainLayerMoveActionOver));
			}
			CCAction* action = CCSequence::create(
				CCEaseExponentialOut::create(CCMoveTo::create(MOVE_TIME, mLayerPosNext[i])),
				pCallFuncN,
				NULL);
			action->setTag(UI_Action_Move_Tag);
			layer->runAction(action);
		}
	}
	if (!bCalled)
	{
		m_bMoveActionOver = true;
	}
}

void Starseneui::moveActionOver(CCNode* pNode)
{
	if (pNode->getTag() == Achievelayer_tag && pNode->getPositionX() >= CCDirector::sharedDirector()->getWinSize().width - 5.0f)
	{
		pNode->removeFromParent();
	}
}

void Starseneui::mainLayerMoveActionOver(CCNode* pNode)
{
	m_bMoveActionOver = true;
}

void Starseneui::registerWithTouchDispatcher(void){

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,maintouchtype,false);
}

void Starseneui::switchStateChanged(CCObject* obj)
{
	if (!m_bMoveActionOver)
	{
		return;
	}
	Myobject *fromLayerInfo=(Myobject*)obj;

	bool mark		= fromLayerInfo->mark;
	int currentId	= fromLayerInfo->current_id;
	//int parentId	= fromLayerInfo->parent_id;
	float time		= fromLayerInfo->time;
	//CCNode* curNode = fromLayerInfo->pCurNode;

	if(currentId != Gamesetlayer_tag && currentId != Achievelayer_tag)
	{
		setLayerNextAction(mark);
		actionMoveLayer();
		return;
	}
	m_bMoveActionOver = false;
	//CCAssert(curNode != NULL, "pCurNode should not be null!");

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	float moveDis = size.width;

	CCNode* layer = this->getChildByTag( currentId );
	//创建设置或成就界面
	if((currentId == Gamesetlayer_tag || currentId == Achievelayer_tag) 
		&& layer == NULL)
	{
		layer = (currentId == Gamesetlayer_tag) ? (CCNode*)Gameset::create() : (CCNode*)Achive::create();
		addChild(layer, 1, currentId);
	}
	//移动设置或成就
	layer->setPositionX(mark ? moveDis : 0);
	layer->runAction(
		CCSequence::create(
		CCEaseExponentialOut::create(CCMoveTo::create(time, ccp(mark ? 0 : moveDis, 0))),
		CCCallFuncN::create(this, callfuncN_selector(Starseneui::moveActionOver)),
		NULL));

	//移动主界面
	Starmain* pMainLayer = (Starmain*)this->getChildByTag(Starmain_tag);
	pMainLayer->setPositionX(mark ? 0 : -moveDis);
	pMainLayer->runAction(
		CCSequence::create(CCEaseExponentialOut::create(CCMoveTo::create(time, ccp(mark ? -moveDis : 0, 0))),
		CCCallFuncN::create(this, callfuncN_selector(Starseneui::mainLayerMoveActionOver)),
		NULL)
		);

	mIsInVerticalLayer = !mark;
}

void Starseneui::fadeInOrOut(CCNode* pParent, float time, bool bOut)
{
	if (!pParent)
	{
		return;
	}

	CCArray* pArray = pParent->getChildren();

	for (size_t i = 0; i < pArray->count(); i++)
	{
		CCNode* pNode = (CCNode*)pArray->objectAtIndex(i);
		if (bOut)
		{
			pNode->runAction(CCFadeOut::create(time));
		}
		else
		{
			pNode->runAction(CCFadeIn::create(time));
		}
	}
}

void Starseneui::removeOtherLayers(CCNode* pNode)
{
	// 	Starmain* pMainLayer = (Starmain*)this->getChildByTag(Starmain_tag);
	// 	if (pMainLayer)
	// 	{
	// 		pMainLayer->Cutout((CCSprite*)pMainLayer->getChildByTag(100));
	// 	}
	return;

	if(pNode != this->getChildByTag(Gamesetlayer_tag)){
		this->removeChildByTag(Gamesetlayer_tag);
	}

	if(pNode != this->getChildByTag(Bigcheckpoint_tag)){
		this->removeChildByTag(Bigcheckpoint_tag);
	}

	if(pNode != this->getChildByTag(Smalllayer_tag)){
		this->removeChildByTag(Smalllayer_tag);
	}
}

CCScene* Starseneui::scene(int tag/*=Starmain_tag*/){



	Shieldingmanger::getSielding()->setCurrentlayertag(Starmain_tag);
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	// 'layer' is an autorelease object
	Starseneui *layer = Starseneui::create();
	layer->setTag(Starseneui_tag);
	//add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	//	layer->Goxiaogk(tag);

	return scene;
}

bool Starseneui::init(){

	if ( !CCLayer::init() )
	{
		return false;
	}
	//UI消息处理
	layerStack.push(Starmain_tag);
	Starmain* p=Starmain::create();
	p->setTag(Starmain_tag);
	addChild(p);


	Biguangka* bigcheck = Biguangka::creteByseason(LevelManager::getManager()->getLastSeasonId());
	int lsatsenson=LevelManager::getManager()->getLastSeasonId();
	int lastsectionid=LevelManager::getManager()->getLastSectionId();
	addChild(bigcheck, 1, Bigcheckpoint_tag);

	Xiaocheckpoint* smallCheck = Xiaocheckpoint::create();
	addChild(smallCheck, 1, Smalllayer_tag);
	smallCheck->setcurrentId(bigcheck->getCurrentseason());


	resetLayerLocation();


	this->setTouchEnabled(true);

	this->scheduleOnce(schedule_selector(Starseneui::updateInitData), 0);
	return true;
}

void Starseneui::updateInitData(float dt)
{
	//为什么放在这里？
	//如果在loading界面showwaiting会被replaceScene干掉，放在这里是为了避免同步数据的时候没有转圈等待
	CmdHelper::getInstance()->initSyncServerData();
}

void Starseneui::removecurrentLayer(){

	if(this->getChildByTag(Weiyi_tag)){
		this->removeChildByTag(Weiyi_tag);
	}
}

void Starseneui::Fahuiparentlayer(Myobject* object){

	//removecurrentLayer();
	int tag=Starmain_tag;
	if(layerStack.size()==1){
		tag=layerStack.top();
	}else if(layerStack.size()>1){
		tag=layerStack.top();
		layerStack.pop();
	}
	//byTagloadlayer(tag,object);
}

//重定向
void Starseneui::ChongdingX(Myobject* object){

	if(layerStack.top()!=object->parent_id){
		layerStack.push(object->parent_id);
	}
	removecurrentLayer();
	byTagloadlayer(object->current_id,object);
}

void Starseneui::byTagloadlayer(int tag,Myobject* object){

	if(tag==Starmain_tag){
		Starmain* p=Starmain::create();
		p->setTag(Weiyi_tag);
		addChild(p);
	}else if(tag==Bigcheckpoint_tag){
		Biguangka* p=Biguangka::creteByseason(LevelManager::getManager()->getLastSeasonId());
		p->setTag(Weiyi_tag);
		addChild(p);
	}else if(tag==Smalllayer_tag){

		Xiaocheckpoint* p=Xiaocheckpoint::create();
		p->setcurrentId(object->season);
		p->setTag(Weiyi_tag);
		addChild(p);

	}else if(tag==Gamesetlayer_tag){

		Gameset* p=Gameset::create();
		p->setTag(Weiyi_tag);
		addChild(p);
	}else if(tag==Guangkalayer_tag){

		Guankamsg* p=Guankamsg::createByid(object->season,object->gk_id);
		p->setTag(Weiyi_tag);
		addChild(p);

	}
}

void Starseneui::changeBg(int seasonId, CCScale9Sprite* pSprite)
{
	if (!pSprite)
	{
		return;
	}
	CCLOG("-------> bg id is %d", seasonId);

	string back_str = CCString::createWithFormat("ZY_bg_%d.png", seasonId)->getCString();	
//	pSprite->initWithSpriteFrameName(back_str.c_str());
	back_str = ResManager::getManager()->getSharedFilePath(g_mainlayerPath+back_str);
	pSprite->initWithFile(back_str.c_str());
	pSprite->setContentSize(CCDirector::sharedDirector()->getWinSize());
}

bool Starseneui::checkIsCanTouched(CCLayer* pLayer)
{
	if (Starseneui* pLayer1 = dynamic_cast<Starseneui*>(CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Starseneui_tag)))
	{
		if (pLayer1->getTouchID() == -1 && pLayer1->getIsMoveActionOver())
		{
			return true;
		}
	}
	return false;
}

