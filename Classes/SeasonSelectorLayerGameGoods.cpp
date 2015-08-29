#include "SeasonSelectorLayerGameGoods.h"
#include "GameInforEditer.h"
#include "MarketLayer.h"
#include "JuqingDaoJuManager.h"
#include "SeasonSelectorLayer.h"
#include "History.h"
#include "CircleSprite.h"

SeasonSelectorLayerGameGoods* SeasonSelectorLayerGameGoods::create(CCNode* title,SeasonSelectorLayerMenu* menu){
	SeasonSelectorLayerGameGoods* g = SeasonSelectorLayerGameGoods::create();
	g->_title=title;
	g->_menusLayer = menu;
	return g;
}
SeasonSelectorLayerGameGoods::SeasonSelectorLayerGameGoods()
	:m_bIsTeaching(false),
	m_nNodeCount(0)
{
	_seasonId = 0;
	_title = NULL;
	_menusLayer = NULL;
	_canNotTouche = false;
	//setTouchEnabled(true);
}

SeasonSelectorLayerGameGoods::~SeasonSelectorLayerGameGoods(){
	_title = NULL;
	//setTouchEnabled(false);
}

bool SeasonSelectorLayerGameGoods::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return _canNotTouche;
}
void SeasonSelectorLayerGameGoods::setSeasonId(unsigned int seasonId){_seasonId=seasonId;updateChildren();}
void SeasonSelectorLayerGameGoods::onExit(){
	//CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
	CCLayer::onExit();
}
void SeasonSelectorLayerGameGoods::onEnter(){
	CCLayer::onEnter();
//	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(SeasonSelectorLayerGameGoods::onPayResult), MSG_PayEndRsp, NULL);
//	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(SeasonSelectorLayerGameGoods::onPayResult), MSG_openBoxRsp, NULL);
}

void SeasonSelectorLayerGameGoods::onPayResult(CCObject* object){
	updateChildren();
}

void SeasonSelectorLayerGameGoods::openUnlock(){
	size_t i;
	CCNode* node;
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	for(i=0;i<_goodsArray.count();++i){
		node = (CCNode*)_goodsArray.objectAtIndex(i);
		node->runAction(CCSpawn::create(CCMoveTo::create(0.5f,ccp(winsize.width/2,winsize.height/2)),CCFadeOut::create(0.5f),NULL));
		node->getChildByTag(1)->runAction(CCSequence::create(CCFadeOut::create(0.5f),CCCallFuncN::create(this,callfuncN_selector(SeasonSelectorLayerGameGoods::onRunActionComplete)),NULL));
	}
}

void SeasonSelectorLayerGameGoods::showOpenUnlock(){
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	_xuanzhuanLayer = CCSprite::create(CCString::createWithFormat(ResManager::getManager()->getSharedFilePath(g_collectlayerPath+"chatu_%d_biankuang.png").c_str(),_seasonId - 1)->getCString());
	if(_xuanzhuanLayer){
		CCUserDefault::sharedUserDefault()->getIntegerForKey(TheNewSelectSeason,_seasonId);
		CCUserDefault::sharedUserDefault()->flush();
		
		ccColor4B color = {0, 0, 0, 125};
		CCLayerColor *colorLayer = CCLayerColor::create(color);
		colorLayer->ignoreAnchorPointForPosition(false);
		colorLayer->setAnchorPoint(ccp(0.5,0.5));
		colorLayer->setPosition(ccp(winsize.width / 2 , winsize.height / 2));
		colorLayer->setContentSize(CCSizeMake(winsize.width,winsize.height));
		colorLayer->setOpacity(0);
		addChild(colorLayer);
		m_sLayerColor = colorLayer;
		colorLayer->runAction(CCFadeTo::create(0.3f,100));

		CCSprite *oldPic = CCSprite::create(CCString::createWithFormat(ResManager::getManager()->getSharedFilePath(g_collectlayerPath+"chatu_%d_biankuang.png").c_str(),_seasonId - 1)->getCString());
		oldPic->setPosition(ccp(winsize.width/2,winsize.height/2));
		addChild(oldPic);
		oldPic->setScale(0.2f);
		oldPic->runAction(CCScaleTo::create(0.5f,1.0f));
		m_sOldPicSpr = oldPic;
		_xuanzhuanLayer->setVisible(false);
		_xuanzhuanLayer->setPosition(ccp(winsize.width/2,winsize.height/2));
		addChild(_xuanzhuanLayer);
		m_sPhotoSpr = _xuanzhuanLayer;

		this->schedule(SEL_SCHEDULE(&SeasonSelectorLayerGameGoods::showLightEffects),1.0f);

		SeasonSelectorLayer::getCurrentLayer()->getSeasonItem()->setVisible(false);
		/*
		_xuanzhuanLayer->setScale(0);
		_xuanzhuanLayer->setPosition(ccp(winsize.width/2,winsize.height/2));
		CCSprite* bg = CCSprite::create("chatu_biankuang.png");
		bg->setAnchorPoint(CCPointZero);
		_xuanzhuanLayer->addChild(bg);
		addChild(_xuanzhuanLayer);
		_xuanzhuanLayer->runAction(CCSequence::create(CCScaleTo::create(0.15f, 0.9f), CCScaleTo::create(0.1f, 0.75f), NULL));*/
	}
	
}
void SeasonSelectorLayerGameGoods::showLightEffects(float dt)
{
	this->unschedule(SEL_SCHEDULE(&SeasonSelectorLayerGameGoods::showLightEffects));
	showFlashLight(m_sPhotoSpr);
	this->removeChild(m_sOldPicSpr);
}
void SeasonSelectorLayerGameGoods::showFlashLight(CCNode *photo)
{
	ccColor4B color = {255, 255, 255, 255};
	int win_width = CCDirector::sharedDirector()->getWinSize().width;
	int win_height = CCDirector::sharedDirector()->getWinSize().height;
	CCLayerColor *colorLayer = CCLayerColor::create(color);
	colorLayer->ignoreAnchorPointForPosition(false);
	colorLayer->setAnchorPoint(ccp(0.5,0.5));
	colorLayer->setPosition(ccp(win_width / 2 , win_height / 2));
	colorLayer->setContentSize(CCSizeMake(win_width,win_height));
	colorLayer->setOpacity(0);
	photo->getParent()->addChild(colorLayer,photo->getZOrder());
	Music::sharedMusicPlayer()->playEffectSound("kuaimen.wav");
	colorLayer->runAction(CCSequence::create(CCFadeIn::create(0.2f),
		CCCallFunc::create(this,SEL_CallFunc(&SeasonSelectorLayerGameGoods::showPhoto)),
		CCFadeOut::create(0.8f),
		CCCallFuncN::create(this,SEL_CallFuncN(&SeasonSelectorLayerGameGoods::showFlashLightEnd)),
		NULL));
}
void SeasonSelectorLayerGameGoods::showShake(CCNode *node)
{
	//node->setScale(0.8);
	/*this->nodeAndAllChildRunAction(node, CCSequence::create(
		CCFadeOut::create(0),
		CCFadeIn::create(0.15f),
		NULL));
*/
	node->runAction(CCSequence::create(CCScaleTo::create(0.1f,0.8f),
		CCScaleTo::create(0.15f,1.05f),
		CCScaleTo::create(0.1f,1.0f),
		NULL));
	/*node->runAction(CCSequence::create(
		CCFadeOut::create(0),
		CCSpawn::create(CCFadeIn::create(0.15f),
		CCScaleTo::create(0.15f,1.05f),
		NULL),
		//CCScaleTo::create(0.15f,0.9f),
		//CCScaleTo::create(0.15f,1.05f),
		CCScaleTo::create(0.1f,1.0f),
		NULL));*/
}
void SeasonSelectorLayerGameGoods::showPhoto()
{
	m_sPhotoSpr->setVisible(true);
	m_sPhotoSpr->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(0.8f),
		CCScaleTo::create(0.5f,0.75f),
		NULL),
		NULL));

	CCSprite *biankuang = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_collectlayerPath+"chatu_xiaoguo_baibian_guangxiao.png").c_str());
	//biankuang->setAnchorPoint(ccp());
	biankuang->setPosition(m_sPhotoSpr->getPosition());
	addChild(biankuang);
	biankuang->setOpacity(0);
	biankuang->runAction(CCSequence::create(CCDelayTime::create(0.5f),
		CCFadeIn::create(0),
		CCCallFuncN::create(this,SEL_CallFuncN(&SeasonSelectorLayerGameGoods::BlinkEnd)),
		NULL));
	m_sBianKuang = biankuang;
	int iStarCount = 10;
	int posIndex[10][2] = {{272,658},{264,453},{225,318},{435,57},{888,53},{1117,106},{1143,319},{1117,573},{1015,698},{550,702}};
	m_sStarList.clear();
	for (int i = 0; i < iStarCount; ++i)
	{
		CCSprite* pStar = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"zhangjie_xuanze_xiaoguo_xingmang.png").c_str());
		pStar->setScale(this->randScale());
		pStar->setRotation(this->randRotation());
		addChild(pStar);
		
		pStar->setPosition(ccp(posIndex[i][0],posIndex[i][1]));

		float time = 0;
		float minTime = 0.5f;
		float maxTime = 1.2f;
		time = CCRANDOM_0_1()*(maxTime - minTime) + minTime;
		Blink(pStar,time);
		m_sStarList.push_back(pStar);
		//pStar->runAction(CCRepeatForever::create(CCBlink::create(time,6)));
	}
}
void SeasonSelectorLayerGameGoods::Blink(CCNode *node,float dur)
{
	node->runAction(CCRepeatForever::create(CCSequence::create(CCFadeIn::create(dur / 2),CCFadeOut::create(dur / 2),NULL)));
}
void SeasonSelectorLayerGameGoods::showFlashLightEnd(CCNode *node)
{
	node->getParent()->removeChild(node);
	//分享
	CCSize winsize =  CCDirector::sharedDirector()->getWinSize();
	CCMenuItemImage* share = CCMenuItemImage::create(ResManager::getManager()->getFilePathByLanguageType(g_seasonlayerPath+"main_shoucang_anniu_shoucang.png").c_str(),
		ResManager::getManager()->getFilePathByLanguageType(g_seasonlayerPath+"main_shoucang_anniu_shoucang_select.png").c_str(),
		this,menu_selector(SeasonSelectorLayerGameGoods::onShared));
	share->setOpacity(200);
	_shareMenu = CCMenu::create(share,NULL);
	if(_xuanzhuanLayer)
		_shareMenu->setPosition(ccp(winsize.width/2,_xuanzhuanLayer->getPositionY()-_xuanzhuanLayer->getContentSize().height/2+78+25));
	_shareMenu->setTouchPriority(BgLayerTouchPriority-1);
	addChild(_shareMenu);
	showShake(share);
}
void SeasonSelectorLayerGameGoods::showWhiteLight()
{
	this->removeChild(m_sOldPicSpr);
}
void SeasonSelectorLayerGameGoods::shakeEnd(CCNode *node)
{
	node->removeAllChildren();
	
	if(_seasonId == 0){
		CCLog("seasonId is zero...");
		return;
	}
	if(_seasonId <= 1)
		return;
	if(_title)
	{
		_title->setVisible(false);
	}
	//检查是否已经解锁
	GameInforEditer* infor = GameInforEditer::getGameInfor();
	SeasonInfor* seasonInfor = infor->getSeasonInfor(_seasonId);
	if (!(seasonInfor == NULL || seasonInfor->locked) && !GameInforEditer::getGameInfor()->isIgnoreAllLock()){
		if(_title)
			_title->setVisible(true);

		return;
	}
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();


	//屏蔽剧情道具相关
/*
	//背景
	CCSprite* bg = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"guanqiaxuanze_daoju_shouji_2.png").c_str());
	bg->setPosition(ccp(winsize.width/2,winsize.height-bg->getContentSize().height/2));
	addChild(bg);
	bg->setOpacity(0);

	//道具
	CCArray menus;
	vector<JuqingDaoJu> goodsArray = JuqingDaoJuManager::getManager()->getAllItemsInSeason(_seasonId-1);
	int i;
	int span = 120;
	int maxWidth = span * 4;
	CCSprite* daojux1 = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_daojuPath+"daoju_xiaoguo_1.png").c_str());
	CCSprite* daojux2 = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_daojuPath+"daoju_xiaoguo_2.png").c_str());
	CCSprite* daojux3 = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_daojuPath+"daoju_xiaoguo_3.png").c_str());
	for(i=0;i<goodsArray.size();++i){
		if(goodsArray[i].state == DJS_NotGetted){
			//没有获得道具
			CCMenuItemImage* item = CCMenuItemImage::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"guanqiaxuanze_daoju_shouji_weizhi.png").c_str(),
				ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"guanqiaxuanze_daoju_shouji_weizhi_select.png").c_str(),
				this,menu_selector(SeasonSelectorLayerGameGoods::onClickGameGoods));
			item->setPosition(ccp(winsize.width/2 - maxWidth/2 + i * span+span/2,bg->getPositionY()+5));
			item->setTag(goodsArray[i].daojuId);
			item->setEnabled(false);
			menus.addObject(item);
		}else{
			CCSprite* icon = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_daojuPath+goodsArray[i].fileName).c_str());
			CCSprite* goods = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_seasonlayerPath+"guanqiaxuanze_daoju_shouji_yiyou.png").c_str());
			if(icon){
				//手动缩小icon
				icon->setScale(0.45);
				icon->setPosition(ccp(goods->getContentSize().width/2,goods->getContentSize().height/2));
				icon->setTag(1);
				goods->addChild(icon);
			}
			goods->setPosition(ccp(winsize.width/2 - maxWidth/2 + i * span+span/2,bg->getPositionY()+5));
			addChild(goods,1);
			
// 			CCSprite* daojux1Spr = CCSprite::createWithTexture(daojux1->getTexture());
// 			daojux1Spr->setScale(1.0f/0.42f);
// 			daojux1Spr->setPosition(ccp(goods->getContentSize().width/2,goods->getContentSize().height/2));
// 			daojux1Spr->runAction(CCRepeatForever::create(CCSequence::create(CCRotateTo::create(2.0f,-180),CCRotateTo::create(2.0f,-360),NULL)));
// 			goods->addChild(daojux1Spr);
// 			CCSprite* daojux2Spr = CCSprite::createWithTexture(daojux2->getTexture());
// 			daojux2Spr->setScale(1.0f/0.42f);
// 			daojux2Spr->setPosition(ccp(goods->getContentSize().width/2,goods->getContentSize().height/2));
// 			daojux2Spr->runAction(CCRepeatForever::create(CCSequence::create(CCRotateTo::create(1.5f,180),CCRotateTo::create(1.5f,360),NULL)));
// 			goods->addChild(daojux2Spr);
// 			CCSprite* daojux3Spr = CCSprite::createWithTexture(daojux3->getTexture());
// 			daojux3Spr->setScale(1.0f/0.42f);
// 			daojux3Spr->setPosition(ccp(goods->getContentSize().width/2,goods->getContentSize().height/2));
// 			goods->addChild(daojux3Spr);
			
			_goodsArray.addObject(goods);

			icon->setOpacity(0);
			goods->setOpacity(0);
			//daojux1Spr->setOpacity(0);
			//daojux2Spr->setOpacity(0);
			//daojux3Spr->setOpacity(0);
		}
	}
	CCMenu* menu = CCMenu::createWithArray(&menus);
	menu->setPosition(CCPointZero);
	addChild(menu);
	this->nodeAndAllChildRunAction(this,CCSequence::create(CCFadeIn::create(0.3f),NULL));
*/
}
void SeasonSelectorLayerGameGoods::nodeAndAllChildRunAction(CCNode* node, CCActionInterval* action,CCObject *obj,SEL_CallFuncN handler)
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
		node->stopAllActions();
		node->runAction(CCSequence::create(action,
			CCCallFuncN::create(obj,handler),
			NULL));
	}
	else
	{
		node->stopAllActions();
		//自己
		node->runAction(action);
	}
	m_nNodeCount--;
}
void SeasonSelectorLayerGameGoods::updateChildren(){
	if (this->getChildrenCount() == 0)
	{
		shakeEnd(this);
	}
	else
	{
		this->nodeAndAllChildRunAction(this,CCSequence::create(CCFadeOut::create(0.3f),NULL),this,SEL_CallFuncN(&SeasonSelectorLayerGameGoods::shakeEnd));
	}
	
	//removeAllChildren();	
}

void SeasonSelectorLayerGameGoods::onClickGameGoods(CCObject* object){
	MarketLayer* m = new MarketLayer(MS_SpecialItem);
	CCDirector::sharedDirector()->getRunningScene()->addChild(m,1024);
	m->release();
}
void SeasonSelectorLayerGameGoods::onShared(CCObject* objeect){
	if(!_shareMenu)
		return;

	SeasonSelectorLayer::getCurrentLayer()->getSeasonItem()->setVisible(true);
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	_shareMenu->setVisible(false);
	
// 	runAction(CCSequence::create(
// 		CCScaleTo::create(0.2f,0.6f),
// 		CCSpawn::create(CCScaleTo::create(0.2f,0.03f),CCMoveTo::create(0.4f,ccp(winsize.width/2-40,-winsize.height/2+170)),NULL),
// 		CCCallFuncN::create(this,callfuncN_selector(SeasonSelectorLayerGameGoods::onChoucangActionComplete)),
// 		NULL));
// 	_xuanzhuanLayer->runAction(CCRepeatForever::create(CCSequence::create(CCRotateTo::create(0.1f,180),CCRotateTo::create(0.1f,360),NULL)));
	BlinkEnd();

	_xuanzhuanLayer->runAction(CCSequence::create(
		CCSpawn::create(CCScaleTo::create(0.6f, 0), CCFadeOut::create(0.6f), CCRotateBy::create(0.5f, 180), CCMoveTo::create(0.5f,ccp(winsize.width-60, 120)), NULL),
		CCCallFuncN::create(this,callfuncN_selector(SeasonSelectorLayerGameGoods::onChoucangActionComplete)),
		NULL));
}
void SeasonSelectorLayerGameGoods::onRunActionComplete(CCNode* object){
	//CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,BgLayerTouchPriority,true);
	_canNotTouche = true;
	removeAllChildren();
	showOpenUnlock();
	_goodsArray.removeAllObjects();
}
void SeasonSelectorLayerGameGoods::onChoucangActionComplete(CCNode* object){

	if (CCUserDefault::sharedUserDefault()->getBoolForKey(IsFirstCollect,true))
	{
		beginTeach();
	}
	else
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	}
	_shareMenu = NULL;

	removeAllChildren();
	_canNotTouche = false;
	if(_title)
	{
			_title->setVisible(true);
	}
	//
	setScale(1.0f);
	setPosition(CCPointZero);
	if(_menusLayer){
		_menusLayer->onJoinCollect();
	}
}
void SeasonSelectorLayerGameGoods::beginTeach()
{
	m_bIsTeaching = true;
// 	MaskLayerUI *layer = MaskLayerUI::create();
// 	layer->addCircle(ccp(pos.x + m_iMenuItem->getPositionX() , pos.y + m_iMenuItem->getPositionY()));
// 	layer->ignoreAnchorPointForPosition(false);
// 	layer->setAnchorPoint(CCPointZero);
// 	layer->setPosition(CCPointZero);
// 	layer->setContentSize(size);
// 	layer->setTouchPriority(BgLayerTouchPriority);
// 	addChild(layer,10);
}